#ifndef __SOUND__RIFF_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __SOUND__RIFF_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>


/* Macros */

#ifdef FOURCC_RIFF
 #undef FOURCC_RIFF
#endif

#ifdef FOURCC_LIST
 #undef FOURCC_LIST
#endif

#define MAKE_FOURCC(a, b, c, d) \
	(((uint32)(a) & 0xFF) | \
	(((uint32)(b) << 8) & 0xFF00) | \
	(((uint32)(c) << 16) & 0xFF0000) | \
	(((uint32)(d) << 24) & 0xFF000000))


/* Constants */

#define FOURCC_RIFF		MAKE_FOURCC('R','I','F','F')
#define FOURCC_CSET		MAKE_FOURCC('C','S','E','T')	/**< Character set information. */
#define FOURCC_JUNK		MAKE_FOURCC('J','U','N','K')	/**< Unused information. */
#define FOURCC_LIST 	MAKE_FOURCC('L','I','S','T')	/**< List of subchunks. */
#define FOURCC_IARL		MAKE_FOURCC('I','A','R','L')	/**< Archival location. */
#define FOURCC_IART		MAKE_FOURCC('I','A','R','T')	/**< Artist. */
#define FOURCC_ICMS		MAKE_FOURCC('I','C','M','S')	/**< Commissioned. */
#define FOURCC_ICMT		MAKE_FOURCC('I','C','M','T')	/**< Comments. */
#define FOURCC_ICOP		MAKE_FOURCC('I','C','O','P')	/**< Copyright. */
#define FOURCC_ICRD		MAKE_FOURCC('I','C','R','D')	/**< Creation date. */
#define FOURCC_ICRP		MAKE_FOURCC('I','C','R','P')	/**< Cropped. */
#define FOURCC_IDIM		MAKE_FOURCC('I','D','I','M')	/**< Dimensions. */
#define FOURCC_IDPI		MAKE_FOURCC('I','D','P','I')	/**< Dots per inch. */
#define FOURCC_IENG		MAKE_FOURCC('I','E','N','G')	/**< Engineer. */
#define FOURCC_IGNR		MAKE_FOURCC('I','G','N','R')	/**< Genre. */
#define FOURCC_IKEY		MAKE_FOURCC('I','K','E','Y')	/**< Keywords. */
#define FOURCC_ILGT		MAKE_FOURCC('I','L','G','T')	/**< Lightness. */
#define FOURCC_IMED		MAKE_FOURCC('I','M','E','D')	/**< Medium. */
#define FOURCC_INAM		MAKE_FOURCC('I','N','A','M')	/**< Name. */
#define FOURCC_IPLT		MAKE_FOURCC('I','P','L','T')	/**< Palette setting. */
#define FOURCC_IPRD		MAKE_FOURCC('I','P','R','D')	/**< Product. */
#define FOURCC_ISBJ		MAKE_FOURCC('I','S','B','J')	/**< Subject. */
#define FOURCC_ISFT		MAKE_FOURCC('I','S','F','T')	/**< Software. */
#define FOURCC_ISHP		MAKE_FOURCC('I','S','H','P')	/**< Sharpness. */
#define FOURCC_ISRC		MAKE_FOURCC('I','S','R','C')	/**< Source. */
#define FOURCC_ISRF		MAKE_FOURCC('I','S','R','F')	/**< Source form. */
#define FOURCC_ITCH		MAKE_FOURCC('I','T','C','H')	/**< Technician. */
#define FOURCC_CGRP		MAKE_FOURCC('C','G','R','P')	/**< Compound file element group. */
#define FOURCC_CTOC		MAKE_FOURCC('C','T','O','C')	/**< Compound file table of contents. */
#define FOURCC_XPLT		MAKE_FOURCC('X','P','L','T')	/**< External palette. */
#define FOURCC_bmhd		MAKE_FOURCC('b','m','h','d')	/**< Bitmap header. */
#define FOURCC_PLTH		MAKE_FOURCC('p','l','t','h')	/**< Palette header. */
#define FOURCC_data		MAKE_FOURCC('d','a','t','a')	/**< Data chunk. */
#define FOURCC_yuvp		MAKE_FOURCC('y','u','v','p')	/**< YUV palette. */
#define FOURCC_fmt		MAKE_FOURCC('f','m','t',' ')	/**< Format description. */
#define FOURCC_slnt		MAKE_FOURCC('s','l','n','t')	/**< Count of silent samples. */
#define FOURCC_fact		MAKE_FOURCC('f','a','c','t')	/**< Information about the contents of the wave file. */
#define FOURCC_cue		MAKE_FOURCC('c','u','e',' ')	/**< Series of positions in the waveform data stream. */
#define FOURCC_plst		MAKE_FOURCC('p','l','s','t')	/**< Play order for a series of cue points. */

/** RIFF file formats. */
enum RiffFormat
{
	RiffBundle = MAKE_FOURCC('B','N','D',' '),		/**< RIFF Bundle. */
	RiffPalette = MAKE_FOURCC('P','A','L',' '),		/**< RIFF Palette format. */
	RiffBitmap = MAKE_FOURCC('R','D','I','B'),		/**< RIFF Device independent bitmap format. */
	RiffMidi = MAKE_FOURCC('R','M','I','D'),		/**< RIFF MIDI format. */
	RiffMovie = MAKE_FOURCC('R','M','M','P'),		/**< RIFF Multimedia movie file format. */
	RiffWave = MAKE_FOURCC('W','A','V','E'),		/**< RIFF Waveform audio format. */
	RiffAvi = MAKE_FOURCC('A','V','I',' '),			/**< RIFF Audio-video interleaved format. */
};

/** Wave format categories */
enum RiffWaveFormatTag
{
	RiffWavePCM = 0x0001,							/**< Pulse Code Modulation (PCM) format. */
	RiffWaveMulaw = 0x0101,							/**< IBM mu-law format. */
	RiffWaveAlaw = 0x0102,							/**< IBM a-law format. */
	RiffWaveADPCM = 0x0103,							/**< IBM AVC Adaptive Differential Pulse Code Modulation format. */
};


/* Structures */

#pragma pack(push, 1)

struct RiffChunk
{
	uint32 id;				/**< Chunk type identifier. */
	uint32 size;			/**< Chunk size in bytes. */
};

/** Character set chunk (@see FOURCC_CSET). */
struct RiffCharacterSet
{
	word wCodePage;			/**< Code page number (default ISO 8859/1). */
	word wCountryCode;		/**< Country code (default USA). */
	word wLanguageCode;		/**< Language code (default US English). */
	word wDialectCode;		/**< Language dialect code (default US English). */
};

/** Wave format. */
struct RiffWaveFormat
{
	word wFormatTag;		/**< Format category (@sa RiffWaveFormatTag). */
	word wChannels;			/**< Number of channels. */
	dword dwSamplesPerSec;	/**< Number of samples per second. */
	dword dwAvgBytesPerSec;	/**< Average number of bytes per second (for buffer estimation). */
	word wBlockAlign;		/**< Data block size. */
};

/** PCM-specific fields. */
struct RiffWavePcmSpecific
{
	word wBitsPerSample;	/**< Number of bits per sample. */
};

#pragma pack(pop)


/* Classes */

/** Helper class for parsing of Microsoft RIFF files. */
class RiffFile
{
public:
	/** LIST chunk types. */
	enum ListType
	{
		List_Info = MAKE_FOURCC('I','N','F','O'),		/**< Information chunk. */
		List_WaveList = MAKE_FOURCC('w','a','v','l'),	/**< List of wave samples. */
	};	

	/**
	 * Constructor.
	 * @param name Name of the file.
	 * @param device I/O device.
	 */
	RiffFile(const QString & name, QIODevice * device);

	/** Destructor. */
	~RiffFile();

	/** Returns RIFF format. */
	inline RiffFormat format() const
	{
		return m_Format;
	}

	/** Moves file pointer to the beginning of the RIFF chunk. */
	void seekToStart();

	/**
	 * Moves file pointer to the next chunk with the specified ID.
	 * @returns true on success, false if chunk was not found.
	 */
	bool seekToChunk(uint32 fourCC);

	/** Returns size of the current chunk. */
	inline uint32 currentChunkSize() const
	{
		return m_CurrentChunk.size;
	}

private:
	QString m_FileName;			/**< Name of the file. */
	QIODevice * m_Device;		/**< I/O device. */
	RiffFormat m_Format;		/**< RIFF format. */
	uint64 m_RiffChunkBegin;	/**< Offset of the beginning of the RIFF chunk. */
	uint32 m_RiffChunkSize;		/**< Size of the RIFF chunk. */
	RiffChunk m_CurrentChunk;	/**< Current chunk. */
};

#endif // __SOUND__RIFF_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
