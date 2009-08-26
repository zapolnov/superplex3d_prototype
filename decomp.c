#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#ifndef __GNUC__
 #include <alloc.h>
 #include <dos.h>
 #include <mem.h>
#endif

#ifdef __GNUC__
 #define far
 #define farcalloc calloc
 #define farfree free
 #define MK_FP(seg, off) ((void *)((unsigned long)(((seg) << 4) + (off)) + (byte *)psp))
 #define FP_SEG(ptr) ((unsigned long)((byte *)(ptr) - (byte *)psp) >> 4)
 #define FP_OFF(ptr) ((unsigned long)((byte *)(ptr) - (byte *)psp) & 0xF)
#endif

#ifdef __GNUC__
 #define ADVANCE_PTR(ptr, step) \
	ptr = (void *)((byte *)ptr + step);
#else
 #define ADVANCE_PTR(ptr, step) \
	{ \
		word seg = FP_SEG(ptr); \
		word off = FP_OFF(ptr); \
		\
		unsigned long address = ((unsigned long)seg << 4) + (unsigned long)off; \
		address += step; \
		(ptr) = MK_FP((address >> 4), address & 0xF); \
	}
#endif



/*

FIXME:

Default segment prefix for [BP], [BP + SI] and [BP + DI] should be SS, but it is DS currently.

*/

/* #define _DEBUG_OUTPUT */
/* #define _LISTING_OUTPUT */

#define MAX_OPCODES 5

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

#pragma pack(push, 1)

typedef union tagFARPTR
{
	struct
	{
		word off;
		word seg;
	}
	addr;
	void far * value;
}
FARPTR;

typedef struct tagDosPSP
{
	word int20;				/* Int 0x20. */
}
DosPSP;

typedef struct tagDosHeader
{
	char mz[2];
	word lastPageSize;		/* Number of bytes in last 512-byte page of executable */
	word totalPages;		/* Total number of 512-byte pages in executable (including the last page) */
	word numRelocEntries;	/* Number of relocation entries */
	word headerSize;		/* Header size in paragraphs (one paragraph = 10h) */
	word minMemory;			/* Minimum paragraphs of memory allocated in addition to the code size */
	word maxMemory;			/* Maximum paragraphs of memory allocated in addition to the code size */
	word relativeSS;		/* Initial SS relative to start of executable */
	word initialSP;			/* Initial SP */
	word checksum;			/* Checksum */ 
	word initialIP;			/* Offset of the entry point */
	word relativeCS;		/* CS relative to start of executable */
	word relocTable;		/* Offset of relocation table */
	word overlayNumber;		/* Overlay number (0 = main program) */
}
DosHeader;

#pragma pack(pop)

static long total_memory_to_alloc;
static DosHeader far * file;
static DosPSP far * psp;

#define FLAG_CARRY 0x0001
#define FLAG_PARITY 0x0004
#define FLAG_AUX 0x0010
#define FLAG_ZERO 0x0040
#define FLAG_SIGN 0x0080
#define FLAG_TRAP 0x0100
#define FLAG_INTR 0x0200
#define FLAG_DIR 0x0400
#define FLAG_OVERFLOW 0x0800
#define FLAG_IOPL 0x3000
#define FLAG_NESTED 0x4000

static word regCS;
static word regSS;
static word regDS;
static word regES;
static word regFS;
static word regGS;

static word regAX;
static word regBX;
static word regCX;
static word regDX;
static word regSP;
static word regBP;
static word regSI;
static word regDI;

static word regIP;
static word regFlags;

static
byte readByte(word seg, word off)
{
	byte far * p = MK_FP(seg, off);
	return *p;
}

static
word readWord(word seg, word off)
{
	word far * p = MK_FP(seg, off);
	return *p;
}

typedef enum
{
	OpAL = 0,		/* AL */
	OpCL,			/* CL */
	OpDL,			/* DL */
	OpBL,			/* BL */
	OpAH,			/* AH */
	OpCH,			/* CH */
	OpDH,			/* DH */
	OpBH,			/* BH */
	OpAX,			/* AX */
	OpCX,			/* CX */
	OpDX,			/* DX */
	OpBX,			/* BX */
	OpSP,			/* SP */
	OpBP,			/* BP */
	OpSI,			/* SI */
	OpDI,			/* DI */
	OpMemBXSI,		/* [BX + SI] */
	OpMemBXDI,		/* [BX + DI] */
	OpMemBPSI,		/* [BP + SI] */
	OpMemBPDI,		/* [BP + DI] */
	OpMemSI,		/* [SI] */
	OpMemDI,		/* [DI] */
	OpMemAddr,		/* Memory address. */
	OpMemBX,		/* [BX] */
	OpSegES,		/* ES */
	OpSegCS,		/* CS */
	OpSegSS,		/* SS */
	OpSegDS,		/* DS */
	OpSegFS,		/* FS (386+) */
	OpSegGS,		/* GS (386+) */
	OpMemUnused1,	/* Unused */
	OpMemUnused2,	/* Unused */
	OpMemBP,		/* [BP] */
}
ModRM_Operand;

typedef enum
{
	PrNone = 0,		/* No prefix */
	PrRepe,			/* REPE */
	PrRepne			/* REPNE */
}
CPU_RepPrefix;

static
void setMemoryByte(void far * ptr, byte value)
{
#ifdef _DEBUG_OUTPUT
	fprintf(stderr, "set [%04X:%04X] = %02X\n", FP_SEG(ptr), FP_OFF(ptr), value);
#endif
	*(byte far *)ptr = value;
}

static
byte getMemoryByte(void far * ptr)
{
	byte value = *(byte far *)ptr;
#ifdef _DEBUG_OUTPUT
	fprintf(stderr, "read [%04X:%04X]: %02X\n", FP_SEG(ptr), FP_OFF(ptr), value);
#endif
	return value;
}

static
void setMemoryWord(void far * ptr, word value)
{
#ifdef _DEBUG_OUTPUT
	fprintf(stderr, "set [%04X:%04X] = %04X\n", FP_SEG(ptr), FP_OFF(ptr), value);
#endif
	*(word far *)ptr = value;
}

static
word getMemoryWord(void far * ptr)
{
	word value = *(word far *)ptr;
#ifdef _DEBUG_OUTPUT
	fprintf(stderr, "read [%04X:%04X]: %04X\n", FP_SEG(ptr), FP_OFF(ptr), value);
#endif
	return value;
}

static
ModRM_Operand modrm_GetRegOperand(int isword, byte modrm)
{
	return (ModRM_Operand)(((modrm >> 3) & 7) + (isword ? 8 : 0));
}

static
ModRM_Operand modrm_GetSegRegOperand(byte modrm)
{
	return (ModRM_Operand)(((modrm >> 3) & 7) + 24);
}

static
ModRM_Operand modrm_GetRegMemOperand(int isword, byte modrm)
{
	if ((modrm & 0xC0) == 0xC0)
		return (ModRM_Operand)((modrm & 7) + (isword ? 8 : 0));
	if ((modrm & 0xC0) != 0 && (modrm & 7) == 6)
		return OpMemBP;
	return (ModRM_Operand)((modrm & 7) + 16);
}

static
const char * modrm_OperandToString(ModRM_Operand op, ModRM_Operand seg, word addr)
{
	static char buf[256];

	switch (op)
	{
	case OpAL:
		return "AL";
	case OpCL:
		return "CL";
	case OpDL:
		return "DL";
	case OpBL:
		return "BL";
	case OpAH:
		return "AH";
	case OpCH:
		return "CH";
	case OpDH:
		return "DH";
	case OpBH:
		return "BH";
	case OpAX:
		return "AX";
	case OpCX:
		return "CX";
	case OpDX:
		return "DX";
	case OpBX:
		return "BX";
	case OpSP:
		return "SP";
	case OpBP:
		return "BP";
	case OpSI:
		return "SI";
	case OpDI:
		return "DI";
	case OpSegES:
		return "ES";
	case OpSegCS:
		return "CS";
	case OpSegSS:
		return "SS";
	case OpSegDS:
		return "DS";
	case OpSegFS:
		return "FS";
	case OpSegGS:
		return "GS";
	case OpMemUnused1:
	case OpMemUnused2:
		return "?";

	case OpMemBXSI:
		sprintf(buf, "%s:[BX + SI + %04X]", modrm_OperandToString(seg, seg, 0), addr);
		return buf;

	case OpMemBXDI:
		sprintf(buf, "%s:[BX + DI + %04X]", modrm_OperandToString(seg, seg, 0), addr);
		return buf;

	case OpMemBPSI:
		sprintf(buf, "%s:[BP + SI + %04X]", modrm_OperandToString(seg, seg, 0), addr);
		return buf;

	case OpMemBPDI:
		sprintf(buf, "%s:[BP + DI + %04X]", modrm_OperandToString(seg, seg, 0), addr);
		return buf;

	case OpMemSI:
		sprintf(buf, "%s:[SI + %04X]", modrm_OperandToString(seg, seg, 0), addr);
		return buf;

	case OpMemDI:
		sprintf(buf, "%s:[DI + %04X]", modrm_OperandToString(seg, seg, 0), addr);
		return buf;

	case OpMemAddr:
		sprintf(buf, "%s:[%04X]", modrm_OperandToString(seg, seg, 0), addr);
		return buf;

	case OpMemBX:
		sprintf(buf, "%s:[BX + %04X]", modrm_OperandToString(seg, seg, 0), addr);
		return buf;

	case OpMemBP:
		sprintf(buf, "%s:[BP + %04X]", modrm_OperandToString(seg, seg, 0), addr);
		return buf;

	default:
		return "<unknown>";
	}
}

#define SET_LOW(var, val) \
	(var) = ((var) & 0xFF00) | ((val) & 0xFF);

#define SET_HIGH(var, val) \
	(var) = ((var) & 0x00FF) | (((word)(val) & 0xFF) << 8);

static
word getOperandValue(ModRM_Operand op, ModRM_Operand seg, word addr)
{
	word far * ptr;

	switch (op)
	{
	case OpAL:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read AL: %02X\n", regAX & 0xFF);
	#endif
		return regAX & 0xFF;
	case OpCL:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read CL: %02X\n", regCX & 0xFF);
	#endif
		return regCX & 0xFF;
	case OpDL:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read DL: %02X\n", regDX & 0xFF);
	#endif
		return regDX & 0xFF;
	case OpBL:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read BL: %02X\n", regBX & 0xFF);
	#endif
		return regBX & 0xFF;
	case OpAH:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read AH: %02X\n", (regAX >> 8) & 0xFF);
	#endif
		return (regAX >> 8) & 0xFF;
	case OpCH:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read CH: %02X\n", (regCX >> 8) & 0xFF);
	#endif
		return (regCX >> 8) & 0xFF;
	case OpDH:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read DH: %02X\n", (regDX >> 8) & 0xFF);
	#endif
		return (regDX >> 8) & 0xFF;
	case OpBH:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read BH: %02X\n", (regBX >> 8) & 0xFF);
	#endif
		return (regBX >> 8) & 0xFF;
	case OpAX:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read AX: %04X\n", regAX);
	#endif
		return regAX;
	case OpCX:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read CX: %04X\n", regCX);
	#endif
		return regCX;
	case OpDX:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read DX: %04X\n", regDX);
	#endif
		return regDX;
	case OpBX:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read BX: %04X\n", regBX);
	#endif
		return regBX;
	case OpSP:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read SP: %04X\n", regSP);
	#endif
		return regSP;
	case OpBP:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read BP: %04X\n", regBP);
	#endif
		return regBP;
	case OpSI:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read SI: %04X\n", regSI);
	#endif
		return regSI;
	case OpDI:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read DI: %04X\n", regDI);
	#endif
		return regDI;
	case OpSegES:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read ES: %04X\n", regES);
	#endif
		return regES;
	case OpSegCS:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read CS: %04X\n", regCS);
	#endif
		return regCS;
	case OpSegSS:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read SS: %04X\n", regSS);
	#endif
		return regSS;
	case OpSegDS:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read DS: %04X\n", regDS);
	#endif
		return regDS;
	case OpSegFS:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read FS: %04X\n", regFS);
	#endif
		return regFS;
	case OpSegGS:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read GS: %04X\n", regGS);
	#endif
		return regGS;
	case OpMemBXSI:
		ptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
			getOperandValue(OpBX, seg, 0) + getOperandValue(OpSI, seg, 0) + addr);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read [%04X:%04X]: %04X\n", FP_SEG(ptr), FP_OFF(ptr), *ptr);
	#endif
		return *ptr;
	case OpMemBXDI:
		ptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
			getOperandValue(OpBX, seg, 0) + getOperandValue(OpDI, seg, 0) + addr);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read [%04X:%04X]: %04X\n", FP_SEG(ptr), FP_OFF(ptr), *ptr);
	#endif
		return *ptr;
	case OpMemBPSI:
		ptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
			getOperandValue(OpBP, seg, 0) + getOperandValue(OpSI, seg, 0) + addr);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read [%04X:%04X]: %04X\n", FP_SEG(ptr), FP_OFF(ptr), *ptr);
	#endif
		return *ptr;
	case OpMemBPDI:
		ptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
			getOperandValue(OpBP, seg, 0) + getOperandValue(OpDI, seg, 0) + addr);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read [%04X:%04X]: %04X\n", FP_SEG(ptr), FP_OFF(ptr), *ptr);
	#endif
		return *ptr;
	case OpMemSI:
		ptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
			getOperandValue(OpSI, seg, 0) + addr);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read [%04X:%04X]: %04X\n", FP_SEG(ptr), FP_OFF(ptr), *ptr);
	#endif
		return *ptr;
	case OpMemDI:
		ptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
			getOperandValue(OpDI, seg, 0) + addr);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read [%04X:%04X]: %04X\n", FP_SEG(ptr), FP_OFF(ptr), *ptr);
	#endif
		return *ptr;
	case OpMemBX:
		ptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
			getOperandValue(OpBX, seg, 0) + addr);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read [%04X:%04X]: %04X\n", FP_SEG(ptr), FP_OFF(ptr), *ptr);
	#endif
		return *ptr;
	case OpMemBP:
		ptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
			getOperandValue(OpBP, seg, 0) + addr);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read [%04X:%04X]: %04X\n", FP_SEG(ptr), FP_OFF(ptr), *ptr);
	#endif
		return *ptr;
	case OpMemAddr:
		ptr = (word far *)MK_FP(getOperandValue(seg, seg, 0), addr);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read [%04X:%04X]: %04X\n", FP_SEG(ptr), FP_OFF(ptr), *ptr);
	#endif
		return *ptr;
	}

	return 0xFFFF;
}

static
void setOperandValue(ModRM_Operand op, ModRM_Operand seg, word addr, word value, int isword)
{
	word far * wptr;
	byte far * bptr;

	switch (op)
	{
	case OpAL:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read AX: %04X\n", regAX);
	#endif
		SET_LOW(regAX, value);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set AX = %04X\n", regAX);
	#endif
		break;
	case OpCL:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read CX: %04X\n", regCX);
	#endif
		SET_LOW(regCX, value);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set CX = %04X\n", regCX);
	#endif
		break;
	case OpDL:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read DX: %04X\n", regDX);
	#endif
		SET_LOW(regDX, value);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set DX = %04X\n", regDX);
	#endif
		break;
	case OpBL:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read BX: %04X\n", regBX);
	#endif
		SET_LOW(regBX, value);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set BX = %04X\n", regBX);
	#endif
		break;
	case OpAH:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read AX: %04X\n", regAX);
	#endif
		SET_HIGH(regAX, value);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set AX = %04X\n", regAX);
	#endif
		break;
	case OpCH:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read CX: %04X\n", regCX);
	#endif
		SET_HIGH(regCX, value);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set CX = %04X\n", regCX);
	#endif
		break;
	case OpDH:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read DX: %04X\n", regDX);
	#endif
		SET_HIGH(regDX, value);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set DX = %04X\n", regDX);
	#endif
		break;
	case OpBH:
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read BX: %04X\n", regBX);
	#endif
		SET_HIGH(regBX, value);
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set BX = %04X\n", regBX);
	#endif
		break;
	case OpAX:
		regAX = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set AX = %04X\n", regAX);
	#endif
		break;
	case OpCX:
		regCX = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set CX = %04X\n", regCX);
	#endif
		break;
	case OpDX:
		regDX = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set DX = %04X\n", regDX);
	#endif
		break;
	case OpBX:
		regBX = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set BX = %04X\n", regBX);
	#endif
		break;
	case OpSP:
		regSP = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set SP = %04X\n", regSP);
	#endif
		break;
	case OpBP:
		regBP = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set BP = %04X\n", regBP);
	#endif
		break;
	case OpSI:
		regSI = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set SI = %04X\n", regSI);
	#endif
		break;
	case OpDI:
		regDI = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set DI = %04X\n", regDI);
	#endif
		break;
	case OpSegES:
		regES = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set ES = %04X\n", regES);
	#endif
		break;
	case OpSegCS:
		regCS = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set CS = %04X\n", regCS);
	#endif
		break;
	case OpSegSS:
		regSS = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set SS = %04X\n", regSS);
	#endif
		break;
	case OpSegDS:
		regDS = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set DS = %04X\n", regDS);
	#endif
		break;
	case OpSegFS:
		regFS = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set FS = %04X\n", regFS);
	#endif
		break;
	case OpSegGS:
		regGS = value;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set GS = %04X\n", regGS);
	#endif
		break;
	case OpMemBXSI:
		if (isword)
		{
			wptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpBX, seg, 0) + getOperandValue(OpSI, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %04X\n", FP_SEG(wptr), FP_OFF(wptr), value);
		#endif
			*wptr = value;
		}
		else
		{
			bptr = (byte far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpBX, seg, 0) + getOperandValue(OpSI, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
    		fprintf(stderr, "set [%04X:%04X] = %02X\n", FP_SEG(bptr), FP_OFF(bptr), value & 0xFF);
		#endif
			*bptr = value & 0xFF;
		}
		break;
	case OpMemBXDI:
		if (isword)
		{
			wptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpBX, seg, 0) + getOperandValue(OpDI, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %04X\n", FP_SEG(wptr), FP_OFF(wptr), value);
		#endif
			*wptr = value;
		}
		else
		{
			bptr = (byte far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpBX, seg, 0) + getOperandValue(OpDI, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %02X\n", FP_SEG(bptr), FP_OFF(bptr), value & 0xFF);
		#endif
			*bptr = value & 0xFF;
		}
		break;
	case OpMemBPSI:
		if (isword)
		{
			wptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpBP, seg, 0) + getOperandValue(OpSI, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %04X\n", FP_SEG(wptr), FP_OFF(wptr), value);
		#endif
			*wptr = value;
		}
		else
		{
			bptr = (byte far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpBP, seg, 0) + getOperandValue(OpSI, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %02X\n", FP_SEG(bptr), FP_OFF(bptr), value & 0xFF);
		#endif
			*bptr = value & 0xFF;
		}
		break;
	case OpMemBPDI:
		if (isword)
		{
			wptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpBP, seg, 0) + getOperandValue(OpDI, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %04X\n", FP_SEG(wptr), FP_OFF(wptr), value);
		#endif
			*wptr = value;
		}
		else
		{
			bptr = (byte far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpBP, seg, 0) + getOperandValue(OpDI, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %02X\n", FP_SEG(bptr), FP_OFF(bptr), value & 0xFF);
		#endif
			*bptr = value & 0xFF;
		}
		break;
	case OpMemSI:
		if (isword)
		{
			wptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpSI, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %04X\n", FP_SEG(wptr), FP_OFF(wptr), value);
		#endif
			*wptr = value;
		}
		else
		{
			bptr = (byte far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpSI, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %02X\n", FP_SEG(bptr), FP_OFF(bptr), value & 0xFF);
		#endif
			*bptr = value & 0xFF;
		}
		break;
	case OpMemDI:
		if (isword)
		{
			wptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpDI, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %04X\n", FP_SEG(wptr), FP_OFF(wptr), value);
		#endif
			*wptr = value;
		}
		else
		{
			bptr = (byte far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpDI, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %02X\n", FP_SEG(bptr), FP_OFF(bptr), value & 0xFF);
		#endif
			*bptr = value & 0xFF;
		}
		break;
	case OpMemBX:
		if (isword)
		{
			wptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpBX, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %04X\n", FP_SEG(wptr), FP_OFF(wptr), value);
		#endif
			*wptr = value;
		}
		else
		{
			bptr = (byte far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpBX, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %02X\n", FP_SEG(bptr), FP_OFF(bptr), value & 0xFF);
		#endif
			*bptr = value & 0xFF;
		}
		break;
	case OpMemBP:
		if (isword)
		{
			wptr = (word far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpBP, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %04X\n", FP_SEG(wptr), FP_OFF(wptr), value);
		#endif
			*wptr = value;
		}
		else
		{
			bptr = (byte far *)MK_FP(getOperandValue(seg, seg, 0),
				getOperandValue(OpBP, seg, 0) + addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %02X\n", FP_SEG(bptr), FP_OFF(bptr), value & 0xFF);
		#endif
			*bptr = value & 0xFF;
		}
		break;
	case OpMemAddr:
		if (isword)
		{
			wptr = (word far *)MK_FP(getOperandValue(seg, seg, 0), addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %04X\n", FP_SEG(wptr), FP_OFF(wptr), value);
		#endif
			*wptr = value;
		}
		else
		{
			bptr = (byte far *)MK_FP(getOperandValue(seg, seg, 0), addr);
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set [%04X:%04X] = %02X\n", FP_SEG(bptr), FP_OFF(bptr), value & 0xFF);
		#endif
			*bptr = value & 0xFF;
		}
		break;
	}
}

static
void setFlag(word flag, int value)
{
#ifdef _DEBUG_OUTPUT
	const char * name;

	switch (flag)
	{
	case FLAG_CARRY:
		name = "CF";
		break;
	case FLAG_PARITY:
		name = "PF";
		break;
	case FLAG_AUX:
		name = "AF";
		break;
	case FLAG_ZERO:
		name = "ZF";
		break;
	case FLAG_SIGN:
		name = "SF";
		break;
	case FLAG_TRAP:
		name = "TF";
		break;
	case FLAG_INTR:
		name = "IF";
		break;
	case FLAG_DIR:
		name = "DF";
		break;
	case FLAG_OVERFLOW:
		name = "OF";
		break;
	default:
		assert(0);
		return;
	}	
#endif

	if (value)
	{
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set %s = 1\n", name);
	#endif
		regFlags |= flag;
	}
	else
	{
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set %s = 0\n", name);
	#endif
		regFlags &= ~flag;
	}
}

static
void calcParityFlag(word value)
{
	int i, num1 = 0;

	for (i = 0; i < 8; i++)
	{
		if ((value & (1 << i)) != 0)
			num1++;
	}

	setFlag(FLAG_PARITY, (num1 & 1) == 0);
}

static
word doAdd8bit(word op1, word op2)
{
	word result;

	assert(op1 <= 0xFF);
	assert(op2 <= 0xFF);

	result = op1 + op2;
	setFlag(FLAG_CARRY, result > 0xFF);
	calcParityFlag(result);
	/* FIXME: AF */
	setFlag(FLAG_ZERO, result == 0);
	setFlag(FLAG_SIGN, (result & 0x80) != 0);
	/* FIXME: OF */

	return result & 0xFF;
}

static
word doAdd16bit(word op1, word op2)
{
	unsigned long result = (unsigned long)op1 + (unsigned long)op2;
	word real_result = result & 0xFFFF;

	setFlag(FLAG_CARRY, result > 0xFFFF);
	calcParityFlag(real_result);
	/* FIXME: AF */
	setFlag(FLAG_ZERO, real_result == 0);
	setFlag(FLAG_SIGN, (real_result & 0x8000) != 0);
	/* FIXME: OF */

	return real_result;
}

static
word doAnd8bit(word op1, word op2)
{
	word result;

	assert(op1 <= 0xFF);
	assert(op2 <= 0xFF);

	result = op1 & op2;

	setFlag(FLAG_CARRY, 0);
	calcParityFlag(result);
	setFlag(FLAG_ZERO, result == 0);
	setFlag(FLAG_SIGN, (result & 0x80) != 0);
	setFlag(FLAG_OVERFLOW, 0);

	return result & 0xFF;
}

static
word doAnd16bit(word op1, word op2)
{
	word result;

	result = op1 & op2;

	setFlag(FLAG_CARRY, 0);
	calcParityFlag(result);
	setFlag(FLAG_ZERO, result == 0);
	setFlag(FLAG_SIGN, (result & 0x8000) != 0);
	setFlag(FLAG_OVERFLOW, 0);

	return result;
}

static
word doXor8bit(word op1, word op2)
{
	word result;

	assert(op1 <= 0xFF);
	assert(op2 <= 0xFF);

	result = op1 ^ op2;

	setFlag(FLAG_CARRY, 0);
	calcParityFlag(result);
	setFlag(FLAG_ZERO, result == 0);
	setFlag(FLAG_SIGN, (result & 0x80) != 0);
	setFlag(FLAG_OVERFLOW, 0);

	return result & 0xFF;
}

static
word doXor16bit(word op1, word op2)
{
	word result;

	result = op1 ^ op2;

	setFlag(FLAG_CARRY, 0);
	calcParityFlag(result);
	setFlag(FLAG_ZERO, result == 0);
	setFlag(FLAG_SIGN, (result & 0x8000) != 0);
	setFlag(FLAG_OVERFLOW, 0);

	return result;
}

static
word doDec16bit(word op1)
{
	unsigned long result = (unsigned long)op1 - 1UL;
	word real_result = result & 0xFFFF;

	/* DEC don't touch carry flag. */
	calcParityFlag(real_result);
	/* FIXME: AF */
	setFlag(FLAG_ZERO, real_result == 0);
	setFlag(FLAG_SIGN, (real_result & 0x8000) != 0);
	/* FIXME: OF */

	return real_result;
}

static
word doInc16bit(word op1)
{
	unsigned long result = (unsigned long)op1 + 1UL;
	word real_result = result & 0xFFFF;

	/* INC don't touch carry flag. */
	calcParityFlag(real_result);
	/* FIXME: AF */
	setFlag(FLAG_ZERO, real_result == 0);
	setFlag(FLAG_SIGN, (real_result & 0x8000) != 0);
	/* FIXME: OF */

	return real_result;
}

static
word doSub16bit(word op1, word op2)
{
	unsigned long result = (unsigned long)op1 - (unsigned long)op2;
	word real_result = result & 0xFFFF;

	setFlag(FLAG_CARRY, result > 0xFFFF);
	calcParityFlag(real_result);
	/* FIXME: AF */
	setFlag(FLAG_ZERO, real_result == 0);
	setFlag(FLAG_SIGN, (real_result & 0x8000) != 0);
	/* FIXME: OF */

	return real_result;
}

static
word doSub8bit(word op1, word op2)
{
	word result;

	assert(op1 <= 0xFF);
	assert(op2 <= 0xFF);

	result = op1 - op2;
	setFlag(FLAG_CARRY, result > 0xFF);
	calcParityFlag(result);
	/* FIXME: AF */
	setFlag(FLAG_ZERO, result == 0);
	setFlag(FLAG_SIGN, (result & 0x80) != 0);
	/* FIXME: OF */

	return result & 0xFF;
}

static
word doShr16bit(word value, byte count)
{
	word result;

	if (count == 0)
		return value;

	result = value >> count;

	setFlag(FLAG_CARRY, (value >> (count - 1)) & 0x01);
	calcParityFlag(result);
	setFlag(FLAG_ZERO, result == 0);
	setFlag(FLAG_SIGN, (result & 0x8000) != 0);
	/* FIXME: OF for single-bit shifts */

	return result;
}

static
byte doShr8bit(word value, byte count)
{
	byte result;

	assert(value <= 0xFF);

	if (count == 0)
		return value & 0xFF;

	result = (value & 0xFF) >> count;

	setFlag(FLAG_CARRY, (value >> (count - 1)) & 0x01);
	calcParityFlag(result);
	setFlag(FLAG_ZERO, result == 0);
	setFlag(FLAG_SIGN, (result & 0x80) != 0);
	/* FIXME: OF for single-bit shifts */

	return result;
}

static
word doShl16bit(word value, byte count)
{
	word result;

	if (count == 0)
		return value;

	result = value << count;

	setFlag(FLAG_CARRY, ((value << (count - 1)) & 0x8000) >> 15);
	calcParityFlag(result);
	setFlag(FLAG_ZERO, result == 0);
	setFlag(FLAG_SIGN, (result & 0x8000) != 0);
	/* FIXME: OF for single-bit shifts */

	return result;
}

static
byte doShl8bit(word value, byte count)
{
	byte result;

	assert(value <= 0xFF);

	if (count == 0)
		return value & 0xFF;

	result = (value & 0xFF) << count;

	setFlag(FLAG_CARRY, ((value << (count - 1)) & 0x80) >> 7);
	calcParityFlag(result);
	setFlag(FLAG_ZERO, result == 0);
	setFlag(FLAG_SIGN, (result & 0x80) != 0);
	/* FIXME: OF for single-bit shifts */

	return result;
}

static
void pushValue(word value)
{
	word far * ptr;

#ifdef _DEBUG_OUTPUT
	fprintf(stderr, "read SP: %04X\n", regSP);
#endif
	regSP -= 2;

#ifdef _DEBUG_OUTPUT
	fprintf(stderr, "set SP = %04X\n", regSP);
	fprintf(stderr, "set [%04X:%04X] = %04X\n", regSS, regSP, value);
#endif

	ptr = MK_FP(regSS, regSP);
	*ptr = value;
}

static
word popValue()
{
	word far * ptr = MK_FP(regSS, regSP);
	word result;

	result = *ptr;

#ifdef _DEBUG_OUTPUT
	fprintf(stderr, "read [%04X:%04X]: %04X\n", regSS, regSP, result);
	fprintf(stderr, "read SP: %04X\n", regSP);
#endif

	regSP += 2;

#ifdef _DEBUG_OUTPUT
	fprintf(stderr, "set SP = %04X\n", regSP);
#endif

	return result;
}

static
void doMovsb(ModRM_Operand seg)
{
	byte value;
	word step;

	step = ((regFlags & FLAG_DIR) != 0) ? 0xFFFF : 1;

	value = getMemoryByte(MK_FP(getOperandValue(seg, seg, 0), getOperandValue(OpSI, seg, 0)));
	setOperandValue(OpSI, seg, 0, getOperandValue(OpSI, seg, 0) + step, 1);
	setMemoryByte(MK_FP(getOperandValue(OpSegES, OpSegES, 0), getOperandValue(OpDI, OpSegES, 0)), value);
	setOperandValue(OpDI, seg, 0, getOperandValue(OpDI, OpSegES, 0) + step, 1);
}

static
void doStosb()
{
	byte value;
	word step;

	step = ((regFlags & FLAG_DIR) != 0) ? 0xFFFF : 1;

	value = getOperandValue(OpAL, OpSegES, 0);
	setMemoryByte(MK_FP(getOperandValue(OpSegES, OpSegES, 0), getOperandValue(OpDI, OpSegES, 0)), value);
	setOperandValue(OpDI, OpSegES, 0, getOperandValue(OpDI, OpSegES, 0) + step, 1);
}

static
void doStosw()
{
	word value;
	word step;

	step = ((regFlags & FLAG_DIR) != 0) ? 0xFFFE : 2;

	value = getOperandValue(OpAX, OpSegES, 0);
	setMemoryWord(MK_FP(getOperandValue(OpSegES, OpSegES, 0), getOperandValue(OpDI, OpSegES, 0)), value);
	setOperandValue(OpDI, OpSegES, 0, getOperandValue(OpDI, OpSegES, 0) + step, 1);
}

static
void doLodsb(ModRM_Operand seg)
{
	byte value;
	word step;

	step = ((regFlags & FLAG_DIR) != 0) ? 0xFFFF : 1;

	value = getMemoryByte(MK_FP(getOperandValue(seg, seg, 0), getOperandValue(OpSI, seg, 0)));
	setOperandValue(OpAL, seg, 0, value, 0);
	setOperandValue(OpSI, seg, 0, getOperandValue(OpSI, seg, 0) + step, 1);
}

static
void doLodsw(ModRM_Operand seg)
{
	word value;
	word step;

	step = ((regFlags & FLAG_DIR) != 0) ? 0xFFFE : 2;

	value = getMemoryWord(MK_FP(getOperandValue(seg, seg, 0), getOperandValue(OpSI, seg, 0)));
	setOperandValue(OpAX, seg, 0, value, 0);
	setOperandValue(OpSI, seg, 0, getOperandValue(OpSI, seg, 0) + step, 1);
}

static
void doScasb()
{
	word step, di;
	byte value;

	step = ((regFlags & FLAG_DIR) != 0) ? 0xFFFF : 1;

	di = getOperandValue(OpDI, OpSegES, 0);
	value = getMemoryByte(MK_FP(getOperandValue(OpSegES, OpSegES, 0), di));
	doSub8bit(getOperandValue(OpAL, OpSegES, 0), value);
	setOperandValue(OpDI, OpSegES, 0, di + step, 1);
}

static
void doRetf()
{
	word ip, cs;

	ip = popValue();
	regIP = ip;

#ifdef _DEBUG_OUTPUT
	fprintf(stderr, "set IP = %04X\n", regIP);
#endif

	cs = popValue();
	regCS = cs;

#ifdef _DEBUG_OUTPUT
	fprintf(stderr, "set CS = %04X\n", regCS);
#endif
}

#ifdef _LISTING_OUTPUT

static
void printOpcode(CPU_RepPrefix prefix, const char * name, byte * opcodes, int count,
	int haveSegPrefix, ModRM_Operand segPrefix, int haveLockPrefix)
{
	int i;

	fprintf(stderr, "%04X:%04X ", regCS, regIP);

	for (i = 0; i < count; i++)
		fprintf(stderr, "%02X ", opcodes[i]);
	for (; i < MAX_OPCODES; i++)
		fprintf(stderr, "   ");

	if (haveLockPrefix)
		fprintf(stderr, "LOCK ");

	switch (prefix)
	{
	case PrNone:
		break;
	case PrRepe:
		fprintf(stderr, "REPE ");
		break;
	case PrRepne:
		fprintf(stderr, "REPNE ");
		break;
	default:
		assert(0);
	}

	if (haveSegPrefix)
		fprintf(stderr, "%s: ", modrm_OperandToString(segPrefix, segPrefix, 0));

	fprintf(stderr, "%s\n", name);
}

 #define PRINT_OPCODE() \
	printOpcode(prefix, opcode_name, opcodes, count, haveSegPrefix, seg, haveLockPrefix)

#else

 #define PRINT_OPCODE() \
 	(void)haveLockPrefix;

#endif

#define READ_NEXT_OPCODE(dst_var) \
	{ \
		assert(count < MAX_OPCODES); \
		opcodes[count] = readByte(regCS, regIP + count); \
		(dst_var) = opcodes[count]; \
		count++; \
	}	

#define READ_NEXT_OPCODE_WORD(dst_var) \
	{ \
		byte bv1, bv2; \
		READ_NEXT_OPCODE(bv1); \
		READ_NEXT_OPCODE(bv2); \
		(dst_var) = (word)bv1 | ((word)bv2 << 8); \
	}	

#define GET_OPERAND_ADDR(dst_var, operand) \
	{ \
		byte mode = (modrm >> 6); \
		(dst_var) = 0; \
		if (mode == 2 || (mode == 0 && (operand) == OpMemAddr)) \
			READ_NEXT_OPCODE_WORD(dst_var) \
		else if (mode == 1) \
		{ \
			byte bv; \
			READ_NEXT_OPCODE(bv); \
			(dst_var) = bv; \
		} \
	}

static
int execCpuCommand()
{
	int count = 0, i, advanceIP = 1, haveSegPrefix = 0, haveLockPrefix = 0;
	ModRM_Operand op1, op2, seg = OpSegDS;
	byte opcode, modrm, arg1, arg2;
	CPU_RepPrefix prefix = PrNone;
	byte opcodes[MAX_OPCODES];
	char opcode_name[256];
	word val1, addr;

	for (;;)
	{
		READ_NEXT_OPCODE(opcode)
		if (opcode == 0xF3)
		{
			if (prefix == PrNone)
				prefix = PrRepe;
		}
		else if (opcode == 0xF2)
		{
			if (prefix == PrNone)
				prefix = PrRepne;
		}
		else if (opcode == 0x26)
		{
			if (!haveSegPrefix)
			{
				seg = OpSegES;
				haveSegPrefix = 1;
			}
		}
		else if (opcode == 0x3E)
		{
			if (!haveSegPrefix)
			{
				seg = OpSegDS;
				haveSegPrefix = 1;
			}
		}
		else if (opcode == 0x2E)
		{
			if (!haveSegPrefix)
			{
				seg = OpSegCS;
				haveSegPrefix = 1;
			}
		}
		else if (opcode == 0x64)
		{
			if (!haveSegPrefix)
			{
				seg = OpSegFS;
				haveSegPrefix = 1;
			}
		}
		else if (opcode == 0x65)
		{
			if (!haveSegPrefix)
			{
				seg = OpSegGS;
				haveSegPrefix = 1;
			}
		}
		else if (opcode == 0x36)
		{
			if (!haveSegPrefix)
			{
				seg = OpSegSS;
				haveSegPrefix = 1;
			}
		}
		else if (opcode == 0xF0)
			haveLockPrefix = 1;
		else
			break;
	}

	switch (opcode)
	{
	case 0x00:	/* ADD mem, regb */
	case 0x01:	/* ADD mem, regw */
		READ_NEXT_OPCODE(modrm)
		op1 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
		op2 = modrm_GetRegOperand(opcode & 0x01, modrm);
		GET_OPERAND_ADDR(addr, op1);
		sprintf(opcode_name, "ADD %s, %s", modrm_OperandToString(op1, seg, addr), modrm_OperandToString(op2, seg, 0));
		PRINT_OPCODE();
		if ((opcode & 0x01) != 0)
			setOperandValue(op1, seg, addr, doAdd16bit(getOperandValue(op1, seg, addr), getOperandValue(op2, seg, 0)), 1);
		else	
			setOperandValue(op1, seg, addr, doAdd8bit(getOperandValue(op1, seg, addr), getOperandValue(op2, seg, 0)), 0);
		break;

	case 0x02:	/* ADD regb, regb */
	case 0x03:	/* ADD regw, regw */
		READ_NEXT_OPCODE(modrm)
		op1 = modrm_GetRegOperand(opcode & 0x01, modrm);
		op2 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
		GET_OPERAND_ADDR(addr, op2);
		sprintf(opcode_name, "ADD %s, %s", modrm_OperandToString(op1, seg, 0), modrm_OperandToString(op2, seg, addr));
		PRINT_OPCODE();
		if ((opcode & 0x01) != 0)
			setOperandValue(op1, seg, 0, doAdd16bit(getOperandValue(op1, seg, 0), getOperandValue(op2, seg, addr)), 1);
		else	
			setOperandValue(op1, seg, 0, doAdd8bit(getOperandValue(op1, seg, 0), getOperandValue(op2, seg, addr)), 0);
		break;

	case 0x04:	/* ADD AL, imm */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "ADD AL, 0x%02X", arg1);
		PRINT_OPCODE();
		setOperandValue(OpAL, seg, 0, doAdd8bit(getOperandValue(OpAL, seg, 0), arg1), 0);
		break;

	case 0x05:	/* ADD AX, imm */
		READ_NEXT_OPCODE_WORD(val1);
		sprintf(opcode_name, "ADD AX, 0x%04X", val1);
		PRINT_OPCODE();
		setOperandValue(OpAX, seg, 0, doAdd16bit(getOperandValue(OpAX, seg, 0), val1), 1);
		break;

	case 0x0E:	/* PUSH CS */
		strcpy(opcode_name, "PUSH CS");
		PRINT_OPCODE();
		pushValue(getOperandValue(OpSegCS, seg, 0));
		break;

	case 0x1F:	/* POP DS */
		strcpy(opcode_name, "POP DS");
		PRINT_OPCODE();
		setOperandValue(OpSegDS, seg, 0, popValue(), 1);
		break;

	case 0x24:	/* AND AL, imm */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "AND AL, 0x%02X", arg1);
		PRINT_OPCODE();
		setOperandValue(OpAL, seg, 0, doAnd8bit(getOperandValue(OpAL, seg, 0), arg1), 0);
		break;

	case 0x25:	/* AND AX, imm */
		READ_NEXT_OPCODE_WORD(val1);
		sprintf(opcode_name, "AND AX, 0x%04X", val1);
		PRINT_OPCODE();
		setOperandValue(OpAX, seg, 0, doAnd16bit(getOperandValue(OpAX, seg, 0), val1), 1);
		break;

	case 0x2A:	/* SUB regb, regb */
	case 0x2B:	/* SUB regw, regw */
		READ_NEXT_OPCODE(modrm)
		op1 = modrm_GetRegOperand(opcode & 0x01, modrm);
		op2 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
		GET_OPERAND_ADDR(addr, op2);
		sprintf(opcode_name, "SUB %s, %s", modrm_OperandToString(op1, seg, 0), modrm_OperandToString(op2, seg, addr));
		PRINT_OPCODE();
		if ((opcode & 0x01) != 0)
			setOperandValue(op1, seg, 0, doSub16bit(getOperandValue(op1, seg, 0), getOperandValue(op2, seg, addr)), 1);
		else	
			setOperandValue(op1, seg, 0, doSub8bit(getOperandValue(op1, seg, 0), getOperandValue(op2, seg, addr)), 0);
		break;

	case 0x2C:	/* SUB AL, imm */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "SUB AL, 0x%02X", arg1);
		PRINT_OPCODE();
		setOperandValue(OpAL, seg, 0, doSub8bit(getOperandValue(OpAL, seg, 0), arg1), 0);
		break;

	case 0x2D:	/* SUB AX, imm */
		READ_NEXT_OPCODE_WORD(val1);
		sprintf(opcode_name, "SUB AX, 0x%04X", val1);
		PRINT_OPCODE();
		setOperandValue(OpAX, seg, 0, doSub16bit(getOperandValue(OpAX, seg, 0), val1), 1);
		break;

	case 0x32:	/* XOR regb, regb */
	case 0x33:	/* XOR regw, regw */
		READ_NEXT_OPCODE(modrm)
		op1 = modrm_GetRegOperand(opcode & 0x01, modrm);
		op2 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
		GET_OPERAND_ADDR(addr, op2);
		sprintf(opcode_name, "XOR %s, %s", modrm_OperandToString(op1, seg, 0), modrm_OperandToString(op2, seg, addr));
		PRINT_OPCODE();
		if ((opcode & 0x01) != 0)
			setOperandValue(op1, seg, 0, doXor16bit(getOperandValue(op1, seg, 0), getOperandValue(op2, seg, addr)), 1);
		else	
			setOperandValue(op1, seg, 0, doXor8bit(getOperandValue(op1, seg, 0), getOperandValue(op2, seg, addr)), 0);
		break;

	case 0x3C:	/* CMP AL, imm */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "CMP AL, 0x%02X", arg1);
		PRINT_OPCODE();
		doSub8bit(getOperandValue(OpAL, seg, 0), arg1);
		break;

	case 0x3D:	/* CMP AX, imm */
		READ_NEXT_OPCODE_WORD(val1);
		sprintf(opcode_name, "CMP AX, 0x%04X", val1);
		PRINT_OPCODE();
		doSub16bit(getOperandValue(OpAX, seg, 0), val1);
		break;

	case 0x40:	/* INC AX */
	case 0x41:	/* INC CX */
	case 0x42:	/* INC DX */
	case 0x43:	/* INC BX */
	case 0x44:	/* INC SP */
	case 0x45:	/* INC BP */
	case 0x46:	/* INC SI */
	case 0x47:	/* INC DI */
		op1 = (ModRM_Operand)((opcode & 7) + 8);
		sprintf(opcode_name, "INC %s", modrm_OperandToString(op1, seg, 0));
		PRINT_OPCODE();
		setOperandValue(op1, seg, 0, doInc16bit(getOperandValue(op1, seg, 0)), 1);
		break;

	case 0x48:	/* DEC AX */
	case 0x49:	/* DEC CX */
	case 0x4A:	/* DEC DX */
	case 0x4B:	/* DEC BX */
	case 0x4C:	/* DEC SP */
	case 0x4D:	/* DEC BP */
	case 0x4E:	/* DEC SI */
	case 0x4F:	/* DEC DI */
		op1 = (ModRM_Operand)((opcode & 7) + 8);
		sprintf(opcode_name, "DEC %s", modrm_OperandToString(op1, seg, 0));
		PRINT_OPCODE();
		setOperandValue(op1, seg, 0, doDec16bit(getOperandValue(op1, seg, 0)), 1);
		break;

	case 0x50:	/* PUSH AX */
	case 0x51:	/* PUSH CX */
	case 0x52:	/* PUSH DX */
	case 0x53:	/* PUSH BX */
	case 0x54:	/* PUSH SP */
	case 0x55:	/* PUSH BP */
	case 0x56:	/* PUSH SI */
	case 0x57:	/* PUSH DI */
		op1 = (ModRM_Operand)((opcode & 7) + 8);
		sprintf(opcode_name, "PUSH %s", modrm_OperandToString(op1, seg, 0));
		PRINT_OPCODE();
		pushValue(getOperandValue(op1, seg, 0));
		break;

	case 0x72:	/* JC / JB / JNAE short */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "JB SHORT %d", (char)arg1);
		PRINT_OPCODE();
		if ((regFlags & FLAG_CARRY) != 0)
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read CF: %d\n", 1);
			fprintf(stderr, "read IP: %04X\n", regIP);
		#endif
			regIP += (char)arg1;
			regIP += count;
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set IP = %04X\n", regIP);
		#endif
			advanceIP = 0;
		}
		else
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read CF: %d\n", 0);
		#endif
		}
		break;

	case 0x73:	/* JNC / JNB / JAE short */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "JNB SHORT %d", (char)arg1);
		PRINT_OPCODE();
		if ((regFlags & FLAG_CARRY) != 0)
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read CF: %d\n", 1);
		#endif
		}
		else
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read CF: %d\n", 0);
			fprintf(stderr, "read IP: %04X\n", regIP);
		#endif
			regIP += (char)arg1;
			regIP += count;
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set IP = %04X\n", regIP);
		#endif
			advanceIP = 0;
		}
		break;

	case 0x74:	/* JE / JZ short */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "JE SHORT %d", (char)arg1);
		PRINT_OPCODE();
		if ((regFlags & FLAG_ZERO) != 0)
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read ZF: %d\n", 1);
			fprintf(stderr, "read IP: %04X\n", regIP);
		#endif
			regIP += (char)arg1;
			regIP += count;
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set IP = %04X\n", regIP);
		#endif
			advanceIP = 0;
		}
		else
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read ZF: %d\n", 0);
		#endif
		}
		break;

	case 0x75:	/* JNE / JNZ short */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "JNE SHORT %d", (char)arg1);
		PRINT_OPCODE();
		if ((regFlags & FLAG_ZERO) != 0)
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read ZF: %d\n", 1);
		#endif
		}
		else
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read ZF: %d\n", 0);
			fprintf(stderr, "read IP: %04X\n", regIP);
		#endif
			regIP += (char)arg1;
			regIP += count;
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set IP = %04X\n", regIP);
		#endif
			advanceIP = 0;
		}
		break;

	case 0x78:	/* JS short */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "JS SHORT %d", (char)arg1);
		PRINT_OPCODE();
		if ((regFlags & FLAG_SIGN) != 0)
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read SF: %d\n", 1);
			fprintf(stderr, "read IP: %04X\n", regIP);
		#endif
			regIP += (char)arg1;
			regIP += count;
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set IP = %04X\n", regIP);
		#endif
			advanceIP = 0;
		}
		else
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read SF: %d\n", 0);
		#endif
		}
		break;

	case 0x79:	/* JNS short */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "JNS SHORT %d", (char)arg1);
		PRINT_OPCODE();
		if ((regFlags & FLAG_SIGN) != 0)
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read SF: %d\n", 1);
		#endif
		}
		else
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read SF: %d\n", 0);
			fprintf(stderr, "read IP: %04X\n", regIP);
		#endif
			regIP += (char)arg1;
			regIP += count;
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set IP = %04X\n", regIP);
		#endif
			advanceIP = 0;
		}
		break;

	case 0x7A:	/* JP / JPE short */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "JP SHORT %d", (char)arg1);
		PRINT_OPCODE();
		if ((regFlags & FLAG_PARITY) != 0)
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read PF: %d\n", 1);
			fprintf(stderr, "read IP: %04X\n", regIP);
		#endif
			regIP += (char)arg1;
			regIP += count;
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set IP = %04X\n", regIP);
		#endif
			advanceIP = 0;
		}
		else
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read PF: %d\n", 0);
		#endif
		}
		break;

	case 0x7B:	/* JNP / JPO short */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "JNP SHORT %d", (char)arg1);
		PRINT_OPCODE();
		if ((regFlags & FLAG_PARITY) != 0)
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read PF: %d\n", 1);
		#endif
		}
		else
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read PF: %d\n", 0);
			fprintf(stderr, "read IP: %04X\n", regIP);
		#endif
			regIP += (char)arg1;
			regIP += count;
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set IP = %04X\n", regIP);
		#endif
			advanceIP = 0;
		}
		break;

	case 0x80:	/* subset of commands (depending on MOD R/M) on bytes */
		READ_NEXT_OPCODE(modrm);
		switch ((modrm >> 3) & 7)
		{
		case 0: /* ADD regb, imm */
			READ_NEXT_OPCODE(arg1);
			op1 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
			GET_OPERAND_ADDR(addr, op1);
			sprintf(opcode_name, "ADD %s, 0x%02X", modrm_OperandToString(op1, seg, addr), arg1);
			PRINT_OPCODE();
			setOperandValue(op1, seg, addr, doAdd8bit(getOperandValue(op1, seg, addr), arg1), 0);
			break;

		case 7: /* CMP regb, imm */
			READ_NEXT_OPCODE(arg1);
			op1 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
			GET_OPERAND_ADDR(addr, op1);
			sprintf(opcode_name, "CMP %s, 0x%02X", modrm_OperandToString(op1, seg, addr), arg1);
			PRINT_OPCODE();
			doSub8bit(getOperandValue(op1, seg, addr), arg1);
			break;

		default:
			goto unknown_opcode;
		}	
		break;

	case 0x81:	/* subset of commands (depending on MOD R/M) on words */
		READ_NEXT_OPCODE(modrm);
		switch ((modrm >> 3) & 7)
		{
		case 0: /* ADD regw, imm */
			READ_NEXT_OPCODE_WORD(val1);
			op1 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
			GET_OPERAND_ADDR(addr, op1);
			sprintf(opcode_name, "ADD %s, 0x%04X", modrm_OperandToString(op1, seg, addr), val1);
			PRINT_OPCODE();
			setOperandValue(op1, seg, addr, doAdd16bit(getOperandValue(op1, seg, addr), val1), 1);
			break;

		case 7: /* CMP regw, imm */
			READ_NEXT_OPCODE_WORD(val1);
			op1 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
			GET_OPERAND_ADDR(addr, op1);
			sprintf(opcode_name, "CMP %s, 0x%04X", modrm_OperandToString(op1, seg, addr), val1);
			PRINT_OPCODE();
			doSub16bit(getOperandValue(op1, seg, addr), val1);
			break;

		default:
			goto unknown_opcode;
		}	
		break;

	case 0x82:	/* subset of commands (depending on MOD R/M) on bytes */
		READ_NEXT_OPCODE(modrm);
		switch ((modrm >> 3) & 7)
		{
		case 0: /* ADD regb, imm8 */
			READ_NEXT_OPCODE(arg1);
			op1 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
			GET_OPERAND_ADDR(addr, op1);
			sprintf(opcode_name, "ADD %s, 0x%02X", modrm_OperandToString(op1, seg, addr), arg1);
			PRINT_OPCODE();
			setOperandValue(op1, seg, addr, doAdd8bit(getOperandValue(op1, seg, addr), arg1), 0);
			break;

		case 7: /* CMP regb, imm8 */
			READ_NEXT_OPCODE(arg1);
			op1 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
			GET_OPERAND_ADDR(addr, op1);
			sprintf(opcode_name, "CMP %s, 0x%02X", modrm_OperandToString(op1, seg, addr), arg1);
			PRINT_OPCODE();
			doSub8bit(getOperandValue(op1, seg, addr), arg1);
			break;

		default:
			goto unknown_opcode;
		}	
		break;

	case 0x83:	/* subset of commands (depending on MOD R/M) on words */
		READ_NEXT_OPCODE(modrm);
		switch ((modrm >> 3) & 7)
		{
		case 0: /* ADD regw, imm8 */
			READ_NEXT_OPCODE(arg1);
			op1 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
			GET_OPERAND_ADDR(addr, op1);
			sprintf(opcode_name, "ADD %s, 0x%02X", modrm_OperandToString(op1, seg, addr), arg1);
			PRINT_OPCODE();
			setOperandValue(op1, seg, addr, doAdd16bit(getOperandValue(op1, seg, addr), arg1), 1);
			break;

		case 7: /* CMP regw, imm8 */
			READ_NEXT_OPCODE(arg1);
			op1 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
			GET_OPERAND_ADDR(addr, op1);
			sprintf(opcode_name, "CMP %s, 0x%02X", modrm_OperandToString(op1, seg, addr), arg1);
			PRINT_OPCODE();
			doSub16bit(getOperandValue(op1, seg, addr), arg1);
			break;

		default:
			goto unknown_opcode;
		}	
		break;

	case 0x8A:	/* MOV regb, regb */
	case 0x8B:	/* MOV regw, regw */
		READ_NEXT_OPCODE(modrm)
		op1 = modrm_GetRegOperand(opcode & 0x01, modrm);
		op2 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
		GET_OPERAND_ADDR(addr, op2);
		sprintf(opcode_name, "MOV %s, %s", modrm_OperandToString(op1, seg, 0), modrm_OperandToString(op2, seg, addr));
		PRINT_OPCODE();
		setOperandValue(op1, seg, 0, getOperandValue(op2, seg, addr), opcode & 0x01);
		break;

	case 0x8C:	/* MOV regw, seg */
		READ_NEXT_OPCODE(modrm);
		op2 = modrm_GetSegRegOperand(modrm);
		op1 = modrm_GetRegMemOperand(1, modrm);
		GET_OPERAND_ADDR(addr, op1);
		sprintf(opcode_name, "MOV %s, %s", modrm_OperandToString(op1, seg, addr), modrm_OperandToString(op2, seg, addr));
		PRINT_OPCODE();
		setOperandValue(op1, seg, 0, getOperandValue(op2, seg, addr), 1);
		break;

	case 0x8E:	/* MOV seg, regw */
		READ_NEXT_OPCODE(modrm);
		op1 = modrm_GetSegRegOperand(modrm);
		op2 = modrm_GetRegMemOperand(1, modrm);
		GET_OPERAND_ADDR(addr, op2);
		sprintf(opcode_name, "MOV %s, %s", modrm_OperandToString(op1, seg, addr), modrm_OperandToString(op2, seg, addr));
		PRINT_OPCODE();
		setOperandValue(op1, seg, 0, getOperandValue(op2, seg, addr), 1);
		break;

	case 0x90:	/* NOP */
		strcpy(opcode_name, "NOP");
		PRINT_OPCODE();
		break;

	case 0x9C:	/* PUSHF */
		strcpy(opcode_name, "PUSHF");
		PRINT_OPCODE();
		pushValue(regFlags);
		break;

	case 0x9D:	/* POPF */
		strcpy(opcode_name, "POPF");
		PRINT_OPCODE();
		regFlags = popValue();
		break;

	case 0x9E:	/* SAHF */
		strcpy(opcode_name, "SAHF");
		PRINT_OPCODE();
		regFlags &= 0xFF00;
		regFlags |= (getOperandValue(OpAH, seg, 0) & 0xD5) | 0x20;
		break;

	case 0x9F:	/* LAHF */
		strcpy(opcode_name, "LAHF");
		PRINT_OPCODE();
		setOperandValue(OpAH, seg, 0, regFlags & 0xFF, 1);
		break;

	case 0xA2:	/* MOV [addr], AL */
		READ_NEXT_OPCODE_WORD(val1);
		sprintf(opcode_name, "MOV %s:[%04X], AL", modrm_OperandToString(seg, seg, 0), val1);
		PRINT_OPCODE();
		setMemoryByte(MK_FP(getOperandValue(seg, seg, 0), val1), getOperandValue(OpAL, seg, 0));
		break;

	case 0xA3:	/* MOV [addr], AX */
		READ_NEXT_OPCODE_WORD(val1);
		sprintf(opcode_name, "MOV %s:[%04X], AX", modrm_OperandToString(seg, seg, 0), val1);
		PRINT_OPCODE();
		setMemoryWord(MK_FP(getOperandValue(seg, seg, 0), val1), getOperandValue(OpAX, seg, 0));
		break;

	case 0xA4:	/* MOVSB */
		sprintf(opcode_name, "MOVSB");
		PRINT_OPCODE();
		if (prefix == PrNone)
			doMovsb(seg);
		else
		{
			word value;

		#if 0	
			do
			{
				doMovsb(seg);
				value = getOperandValue(OpCX, seg, 0) - 1;
				setOperandValue(OpCX, seg, 0, value, 1);
			}
			while (value != 0);
		#else
			value = getOperandValue(OpCX, seg, 0);
		 #ifdef _DEBUG_OUTPUT
			fprintf(stderr, "Fast REP MOVSB [%04X:%04X] -> [%04X:%04X]\n", regDS, regSI, regES, regDI);
		 #endif
			do
			{
				*(byte far *)(MK_FP(regES, regDI)) = *(byte far *)(MK_FP(regDS, regSI));
				regDI += (((regFlags & FLAG_DIR) != 0) ? 0xFFFF : 1);
				regSI += (((regFlags & FLAG_DIR) != 0) ? 0xFFFF : 1);
				value--;
			}
			while (value != 0);
			setOperandValue(OpCX, seg, 0, value, 1);
		#endif
		}
		break;

	case 0xA8:	/* TEST AL, imm */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "TEST AL, 0x%02X", arg1);
		PRINT_OPCODE();
		doAnd8bit(getOperandValue(OpAL, seg, 0), arg1);
		break;

	case 0xA9:	/* TEST AX, imm */
		READ_NEXT_OPCODE_WORD(val1);
		sprintf(opcode_name, "TEST AX, 0x%04X", val1);
		PRINT_OPCODE();
		doAnd16bit(getOperandValue(OpAX, seg, 0), val1);
		break;

	case 0xAA:	/* STOSB */
		sprintf(opcode_name, "STOSB");
		PRINT_OPCODE();
		if (prefix == PrNone)
			doStosb();
		else
		{
			word value;

		#if 0
			do
			{
				doStosb();
				value = getOperandValue(OpCX, OpSegES, 0) - 1;
				setOperandValue(OpCX, OpSegES, 0, value, 1);
			}
			while (value != 0);
		#else
			value = getOperandValue(OpCX, OpSegES, 0);
		 #ifdef _DEBUG_OUTPUT
			fprintf(stderr, "Fast REP STOSB AL -> [%04X:%04X]\n", regES, regDI);
		 #endif
			do
			{
				*(byte far *)(MK_FP(regES, regDI)) = regAX & 0xFF;
				regDI += (((regFlags & FLAG_DIR) != 0) ? 0xFFFF : 1);
				value--;
			}
			while (value != 0);
			setOperandValue(OpCX, OpSegES, 0, value, 1);
		#endif
		}
		break;

	case 0xAB:	/* STOSW */
		sprintf(opcode_name, "STOSW");
		PRINT_OPCODE();
		if (prefix == PrNone)
			doStosw();
		else
		{
			word value;

			do
			{
				doStosw();
				value = getOperandValue(OpCX, OpSegES, 0) - 1;
				setOperandValue(OpCX, OpSegES, 0, value, 1);
			}
			while (value != 0);
		}
		break;

	case 0xAC:	/* LODSB */
		sprintf(opcode_name, "LODSB");
		PRINT_OPCODE();
		if (prefix == PrNone)
			doLodsb(seg);
		else
		{
			word value;

			do
			{
				doLodsb(seg);
				value = getOperandValue(OpCX, seg, 0) - 1;
				setOperandValue(OpCX, seg, 0, value, 1);
			}
			while (value != 0);
		}
		break;

	case 0xAD:	/* LODSW */
		sprintf(opcode_name, "LODSW");
		PRINT_OPCODE();
		if (prefix == PrNone)
			doLodsw(seg);
		else
		{
			word value;

			do
			{
				doLodsw(seg);
				value = getOperandValue(OpCX, seg, 0) - 1;
				setOperandValue(OpCX, seg, 0, value, 1);
			}
			while (value != 0);
		}
		break;

	case 0xAE:	/* SCASB */
		sprintf(opcode_name, "SCASB");
		PRINT_OPCODE();
		if (prefix == PrNone)
			doScasb(seg);
		else
		{
			word value, end_state;

			if (prefix == PrRepe)
				end_state = 0;
			else
				end_state = FLAG_ZERO;

			do
			{
				doScasb(seg);
				value = getOperandValue(OpCX, seg, 0) - 1;
				setOperandValue(OpCX, seg, 0, value, 1);
			}
			while (value != 0 && (regFlags & FLAG_ZERO) != end_state);
		}
		break;

	case 0xB0:	/* MOV AL, imm */
	case 0xB1:	/* MOV CL, imm */
	case 0xB2:	/* MOV DL, imm */
	case 0xB3:	/* MOV BL, imm */
	case 0xB4:	/* MOV AH, imm */
	case 0xB5:	/* MOV CH, imm */
	case 0xB6:	/* MOV DH, imm */
	case 0xB7:	/* MOV BH, imm */
		READ_NEXT_OPCODE(arg1);
		op1 = (ModRM_Operand)((opcode & 7));
		sprintf(opcode_name, "MOV %s, 0x%02X", modrm_OperandToString(op1, seg, 0), arg1);
		PRINT_OPCODE();
		setOperandValue(op1, seg, 0, arg1, 0);
		break;

	case 0xB8:	/* MOV AX, imm */
	case 0xB9:	/* MOV CX, imm */
	case 0xBA:	/* MOV DX, imm */
	case 0xBB:	/* MOV BX, imm */
	case 0xBC:	/* MOV SP, imm */
	case 0xBD:	/* MOV BP, imm */
	case 0xBE:	/* MOV SI, imm */
	case 0xBF:	/* MOV DI, imm */
		READ_NEXT_OPCODE_WORD(val1);
		op1 = (ModRM_Operand)((opcode & 7) + 8);
		sprintf(opcode_name, "MOV %s, 0x%04X", modrm_OperandToString(op1, seg, 0), val1);
		PRINT_OPCODE();
		setOperandValue(op1, seg, 0, val1, 1);
		break;

	case 0xC3:
		strcpy(opcode_name, "RET");
		PRINT_OPCODE();
		regIP = popValue();
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set IP = %04X\n", regIP);
	#endif
		advanceIP = 0;
		break;

	case 0xC9:
		strcpy(opcode_name, "LEAVE");
		PRINT_OPCODE();
		setOperandValue(OpSP, seg, 0, getOperandValue(OpBP, seg, 0), 1);
		setOperandValue(OpBP, seg, 0, popValue(), 1);
		break;

	case 0xCB:
		strcpy(opcode_name, "RETF");
		PRINT_OPCODE();
		doRetf();
		advanceIP = 0;
		break;

	case 0xD2:	/* subset of commands (depending on MOD R/M) on bytes */
	case 0xD3:	/* subset of commands (depending on MOD R/M) on words */
		READ_NEXT_OPCODE(modrm);
		switch ((modrm >> 3) & 7)
		{
		case 4: /* SHL op, CL */
			op1 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
			GET_OPERAND_ADDR(addr, op1);
			sprintf(opcode_name, "SHL %s, CL", modrm_OperandToString(op1, seg, addr));
			PRINT_OPCODE();
			arg1 = getOperandValue(OpCL, seg, 0) & 0xFF;
			if (opcode & 0x01)
				setOperandValue(op1, seg, addr, doShl16bit(getOperandValue(op1, seg, addr), arg1), 1);
			else
				setOperandValue(op1, seg, addr, doShl8bit(getOperandValue(op1, seg, addr), arg1), 0);
			break;

		case 5: /* SHR op, CL */
			op1 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
			GET_OPERAND_ADDR(addr, op1);
			sprintf(opcode_name, "SHR %s, CL", modrm_OperandToString(op1, seg, addr));
			PRINT_OPCODE();
			arg1 = getOperandValue(OpCL, seg, 0) & 0xFF;
			if (opcode & 0x01)
				setOperandValue(op1, seg, addr, doShr16bit(getOperandValue(op1, seg, addr), arg1), 1);
			else
				setOperandValue(op1, seg, addr, doShr8bit(getOperandValue(op1, seg, addr), arg1), 0);
			break;

		default:
			goto unknown_opcode;
		}
		break;

	case 0xE2:	/* LOOP short */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "LOOP SHORT %d", (char)arg1);
		PRINT_OPCODE();
		val1 = getOperandValue(OpCX, seg, 0) - 1;
		setOperandValue(OpCX, seg, 0, val1, 1);
		if (val1 != 0)
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read IP: %04X\n", regIP);
		#endif
			regIP += (char)arg1;
			regIP += count;
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set IP = %04X\n", regIP);
		#endif
			advanceIP = 0;
		}
		break;

	case 0xE3:	/* JCXZ / JCXE */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "JCXZ %d", (char)arg1);
		PRINT_OPCODE();
		val1 = getOperandValue(OpCX, seg, 0);
		if (val1 == 0)
		{
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "read IP: %04X\n", regIP);
		#endif
			regIP += (char)arg1;
			regIP += count;
		#ifdef _DEBUG_OUTPUT
			fprintf(stderr, "set IP = %04X\n", regIP);
		#endif
			advanceIP = 0;
		}	
		break;

	case 0xE9:	/* JMP near */
		READ_NEXT_OPCODE_WORD(val1);
		sprintf(opcode_name, "JMP NEAR %d", (short)val1);
		PRINT_OPCODE();
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read IP: %04X\n", regIP);
	#endif
		regIP += (short)val1;
		regIP += count;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set IP = %04X\n", regIP);
	#endif
		advanceIP = 0;
		break;

	case 0xEB:	/* JMP short */
		READ_NEXT_OPCODE(arg1);
		sprintf(opcode_name, "JMP SHORT %d", (char)arg1);
		PRINT_OPCODE();
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "read IP: %04X\n", regIP);
	#endif
		regIP += (char)arg1;
		regIP += count;
	#ifdef _DEBUG_OUTPUT
		fprintf(stderr, "set IP = %04X\n", regIP);
	#endif
		advanceIP = 0;
		break;

	case 0xF4:	/* HLT */
		sprintf(opcode_name, "HLT");
		PRINT_OPCODE();
		break;

	case 0xF5:	/* CMC */
		sprintf(opcode_name, "CMC");
		PRINT_OPCODE();
		setFlag(FLAG_CARRY, (regFlags & FLAG_CARRY) == 0);
		break;

	case 0xF6:	/* subset of commands (depending on MOD R/M) on bytes */
	case 0xF7:	/* subset of commands (depending on MOD R/M) on words */
		READ_NEXT_OPCODE(modrm);
		switch ((modrm >> 3) & 7)
		{
		case 2:	/* NOT op */
			op1 = modrm_GetRegMemOperand(opcode & 0x01, modrm);
			GET_OPERAND_ADDR(addr, op1);
			sprintf(opcode_name, "NOT %s", modrm_OperandToString(op1, seg, addr));
			PRINT_OPCODE();
			setOperandValue(op1, seg, addr, ~getOperandValue(op1, seg, addr), opcode & 0x01);
			break;

		default:
			goto unknown_opcode;
		}
		break;

	case 0xF8:	/* CLC */
		sprintf(opcode_name, "CLC");
		PRINT_OPCODE();
		setFlag(FLAG_CARRY, 0);
		break;

	case 0xF9:	/* STC */
		sprintf(opcode_name, "STC");
		PRINT_OPCODE();
		setFlag(FLAG_CARRY, 1);
		break;

	case 0xFA:	/* CLI */
		sprintf(opcode_name, "CLI");
		PRINT_OPCODE();
		setFlag(FLAG_INTR, 0);
		break;

	case 0xFB:	/* STI */
		sprintf(opcode_name, "STI");
		PRINT_OPCODE();
		setFlag(FLAG_INTR, 1);
		break;

	case 0xFC:	/* CLD */
		sprintf(opcode_name, "CLD");
		PRINT_OPCODE();
		setFlag(FLAG_DIR, 0);
		break;

	case 0xFD:	/* STD */
		sprintf(opcode_name, "STD");
		PRINT_OPCODE();
		setFlag(FLAG_DIR, 1);
		break;

	default:
		goto unknown_opcode;
	}

	if (advanceIP)
		regIP += count;

	return 1;

unknown_opcode:
	strcpy(opcode_name, "<unknown opcode>");
	PRINT_OPCODE();
	return 0;
}

static
void traceDecompressor()
{
	int i;

	/* Fill the PSP. */
	psp->int20 = 0x20CD;

	/* Setup registers. */
	regCS = file->relativeCS;
	regIP = file->initialIP;
	regSS = file->relativeSS;
	regSP = file->initialSP;
	regDS = FP_SEG(psp);
	regES = FP_SEG(psp);
	regAX = 0;
	regFlags = 0x02 | FLAG_INTR;

#ifdef _DEBUG_OUTPUT
	fprintf(stderr, "set CS = %04X\n", regCS);
	fprintf(stderr, "set IP = %04X\n", regIP);
	fprintf(stderr, "set SS = %04X\n", regSS);
	fprintf(stderr, "set SP = %04X\n", regSP);
	fprintf(stderr, "set DS = %04X\n", regDS);
	fprintf(stderr, "set ES = %04X\n", regES);
	fprintf(stderr, "set AX = %04X\n", regAX);
	fprintf(stderr, "set IF = %d\n", 1);
#else
 #ifndef _LISTING_OUTPUT
	fprintf(stderr, "Decompressing ... \n");
 #endif
#endif

	/** Execute commands. */
	while (execCpuCommand())
		;
}

static
void printExeHeader()
{
	fprintf(stderr, "------------------------------\n");
	fprintf(stderr, "Header size:    %d bytes\n", file->headerSize * 0x10);
	fprintf(stderr, "Total pages:    %d\n", file->totalPages);
	fprintf(stderr, "Last page size: %d bytes\n", file->lastPageSize);
	fprintf(stderr, "Min memory:     %ld bytes\n", (long)file->minMemory * 0x10L);
	fprintf(stderr, "Max memory:     %ld bytes\n", (long)file->maxMemory * 0x10L);
	fprintf(stderr, "Num relocs:     %d\n", file->numRelocEntries);
	fprintf(stderr, "Relocs ofs:     %d\n", file->relocTable);
	fprintf(stderr, "Initial stack:  %04X:%04X\n", file->relativeSS, file->initialSP);
	fprintf(stderr, "Entry point:    %04X:%04X\n", file->relativeCS, file->initialIP);
	fprintf(stderr, "------------------------------\n");
}

static
void dumpFile()
{
	word far * relocs, far * reldst, r_bx, r_dx, r_cx, r_ax, r_es, r_di;
	long bytes_left, file_size = total_memory_to_alloc;
	byte far * ptr;
	int i;

	FILE * f = fopen("unpacked.bin", "wb");
	if (!f)
	{
		fprintf(stderr, "Unable to create file: %s\n", strerror(errno));
		return;
	}

	/* Setup the header */
	file = MK_FP(FP_SEG(file) - 0x10, FP_OFF(file));
	file->mz[0] = 'M';
	file->mz[1] = 'Z';
	file->lastPageSize = file_size % 512;
	file->totalPages = (file_size + 511) / 512;
	file->numRelocEntries = 0;
	file->headerSize = 256 / 16;
	file->minMemory = 0;//(file_size + 15) / 16;
	file->maxMemory = 0xFFFF;
	file->relativeSS = regSS - FP_SEG(file) - file->headerSize;
	file->initialSP = regSP;
	file->checksum = 0;
	file->initialIP = getMemoryWord(MK_FP(regCS, regBX));
	file->relativeCS = getMemoryWord(MK_FP(regCS, regBX + 2)) - FP_SEG(file) - file->headerSize;
	file->relocTable = 30;
	file->overlayNumber = 0;

	/* Setup the relocations */
	relocs = (word far *)MK_FP(regCS, 0x0132);
	reldst = (word far *)file;
	ADVANCE_PTR(reldst, file->relocTable);
	r_bx = *(word far *)MK_FP(regCS, 0x0004);
	for (i = 0, r_dx = 0; i < 16; i++, r_dx += 0x1000)
	{
		r_cx = *relocs;
		ADVANCE_PTR(relocs, 2);
		if (r_cx == 0)
			continue;
		r_es = r_dx + r_bx;
		do
		{
			unsigned long relofs;

			r_ax = *relocs;
			ADVANCE_PTR(relocs, 2);
			r_di = r_ax;
			if (r_di != 0xFFFF)
				*(word far *)MK_FP(r_es, r_di) -= r_bx;
			else
				*(word far *)MK_FP(r_es + 1, r_di - 0x10) -= r_bx;
			relofs = ((unsigned long)(r_dx/* - file->headerSize*/) << 4) + r_di;
			reldst[0] = relofs & 0xF;
			reldst[1] = relofs >> 4;
			fprintf(stderr, "Relocation at %04X:%04X\n", reldst[1], reldst[0]);
			file->numRelocEntries++;
			ADVANCE_PTR(reldst, 4);
		}
		while (--r_cx != 0);
	}

	/* Write the file */
	ptr = (byte far *)file;
	bytes_left = file_size;
	while (bytes_left > 0)
	{
		char buf[1024];
		size_t bytes_to_write = (bytes_left > sizeof(buf) ? sizeof(buf) : (size_t)bytes_left);

	#ifdef __GNUC__
		memcpy(buf, ptr, bytes_to_write);
	#else
		movedata(FP_SEG(ptr), FP_OFF(ptr), FP_SEG(buf), FP_OFF(buf), bytes_to_write);
	#endif

		if (fwrite(buf, 1, bytes_to_write, f) != bytes_to_write)
		{
			fprintf(stderr, "Write failed\n");
			fclose(f);
			return;
		}

		ADVANCE_PTR(ptr, bytes_to_write);
		bytes_left -= bytes_to_write;
	}
	fprintf(stderr, "Executable file has been written.\n");
	printExeHeader();

	fclose(f);
}

int main()
{
	long size, bytes_left, extra_alloc, ii;
	void far * mem_alloced;
	DosHeader tmp_hdr;
	byte far * ptr;
	word seg, off;
	FARPTR p;
	FILE * f;

	/* Open the file */
	f = fopen("../supaplex.exe", "rb");
	if (!f)
	{
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return 1;
	}

	/* Get size of the file */
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	fprintf(stderr, "File size: %ld bytes\n", size);

	/* Read the dos header. */
	if (fread(&tmp_hdr, 1, sizeof(tmp_hdr), f) != sizeof(tmp_hdr))
	{
		fprintf(stderr, "Read error!\n");
		return 1;
	}
	if (tmp_hdr.mz[0] != 'M' || tmp_hdr.mz[1] != 'Z')
	{
		fprintf(stderr, "Not an MZ file\n");
		return 1;
	}
	extra_alloc = (long)tmp_hdr.minMemory * 0x10L;

	/* Allocate memory */
	total_memory_to_alloc = size + extra_alloc + 0x100L;
	mem_alloced = farcalloc(total_memory_to_alloc + 0x10L, 1);
	if (!mem_alloced)
	{
		fprintf(stderr, "Out of memory!\n");
		return 1;
	}

	/* Clear allocated memory */
#ifdef __GNUC__
	memset(mem_alloced, 0, total_memory_to_alloc + 0x10L);
#else
	seg = FP_SEG(mem_alloced);
	off = FP_OFF(mem_alloced);
	for (ii = 0; ii < total_memory_to_alloc + 0x10L; ii++)
	{
		*(byte far *)MK_FP(seg, off) = 0;
		if (off != 0xFFFF)
			off++;
		else
		{
			seg += 0x1000;
			off = 0;
		}
	}
#endif

	/* Align memory to the segment boundary */
#ifdef __GNUC__
	psp = mem_alloced;
	file = (DosHeader *)((byte *)mem_alloced + 0x100);
#else
	p.value = mem_alloced;
	assert(p.addr.off < 0x10);
	p.addr.off = 0;
	p.addr.seg++;
	psp = p.value;
	p.addr.seg += 0x10;
	file = p.value;
#endif

	/* Display info */
#ifdef __GNUC__
	fprintf(stderr, "Allocated %ld bytes at %04X:%04X - %04lX:%04X\n",
		total_memory_to_alloc, 0, 0,
		((total_memory_to_alloc + 0xFL) >> 4), 0);
	fprintf(stderr, "Program segment prefix at: %04X:%04X\n", 0, 0);
	fprintf(stderr, "Program start at: %04X:%04X\n", 0x10, 0);
#else
	fprintf(stderr, "Allocated %ld bytes at %04X:%04X - %04lX:%04X\n",
		total_memory_to_alloc, FP_SEG(psp), FP_OFF(psp),
		FP_SEG(psp) + ((total_memory_to_alloc + 0xFL) >> 4), 0);
	fprintf(stderr, "Program segment prefix at: %04X:%04X\n", FP_SEG(psp), FP_OFF(psp));
	fprintf(stderr, "Program start at: %04X:%04X\n", FP_SEG(file), FP_OFF(file));
#endif

	/* Read the file */
	rewind(f);
	ptr = (byte far *)file;
	bytes_left = size;
	while (bytes_left > 0)
	{
		char buf[1024];
		size_t bytes_to_read = (bytes_left > sizeof(buf) ? sizeof(buf) : (size_t)bytes_left);

		if (fread(buf, 1, bytes_to_read, f) != bytes_to_read)
		{
			fprintf(stderr, "Read failed\n");
			return 1;
		}

	#ifdef __GNUC__
		memcpy(ptr, buf, bytes_to_read);
	#else
		movedata(FP_SEG(buf), FP_OFF(buf), FP_SEG(ptr), FP_OFF(ptr), bytes_to_read);
	#endif
		ADVANCE_PTR(ptr, bytes_to_read);
		bytes_left -= bytes_to_read;
	}
	fprintf(stderr, "Executable file has been loaded.\n");

	/* Validate the header. */
	if (file->mz[0] != 'M' || file->mz[1] != 'Z')
	{
		fprintf(stderr, "Not an MZ file\n");
		return 1;
	}

	/* Fixup the header */
#ifdef __GNUC__
	file->relativeSS += 0x10 + file->headerSize;
	file->relativeCS += 0x10 + file->headerSize;
#else
	file->relativeSS += p.addr.seg + file->headerSize;
	file->relativeCS += p.addr.seg + file->headerSize;
#endif

	/* Dump header info */
	printExeHeader();

	/* Execute the decompressor */
	traceDecompressor();

	/* Dump memory into the binary file. */
	dumpFile();

	/* Free the memory */
	farfree(mem_alloced);

	return 0;
}
