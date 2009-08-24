
#include <idc.idc>

static main()
{
	auto special_port_struc, map_elements_enum;

	SetLongPrm(INF_STRTYPE, ASCSTR_TERMCHR);
	SetCharPrm(INF_LPREFIX, "@@");

	// --------------------------------------------
	// Segments

	//SegRename(0, "");
	SegRename(0x7E00, "MENU_DAT_SEG");
	SegRename(0xFB00, "GFX_DAT_SEG");
	SegRename(0x17800, "CONTROLS_DAT_SEG");
	SegRename(0x1F500, "BACK_DAT_SEG");
	//SegRename(0x27200, "");
	SegRename(0x36C20, "CODE_SEG");
	SegRename(0x3F600, "DATA_SEG");
	SegRename(0x4D460, "DOS_ERROR_MSG_SEG");
	SegRename(0x4D570, "INT80_SEG");
	SegRename(0x4EB60, "INT81_SEG");

	SegBounds(0x587B0, 0x587B0, 0x58BB0, 0);

	// --------------------------------------------
	// Structures

	special_port_struc = AddStrucEx(-1, "SPECIAL_PORT", 0);
	AddStrucMember(special_port_struc, "coords", -1, FF_WORD | FF_DATA, -1, 2);
	AddStrucMember(special_port_struc, "gravity", -1, FF_BYTE | FF_DATA, -1, 1);
	AddStrucMember(special_port_struc, "freeze_zonks", -1, FF_BYTE | FF_DATA, -1, 1);
	AddStrucMember(special_port_struc, "freeze_enemies", -1, FF_BYTE | FF_DATA, -1, 1);
	AddStrucMember(special_port_struc, "unused", -1, FF_BYTE | FF_DATA, -1, 1);

	// --------------------------------------------
	// Enums

	map_elements_enum = AddEnum(GetEnumQty(), "MAP_ELEMENT", 0);
	AddConstEx(map_elements_enum, "MAP_SPACE", 0, -1);
	AddConstEx(map_elements_enum, "MAP_ZONK", 1, -1);
	AddConstEx(map_elements_enum, "MAP_BASE", 2, -1);
	AddConstEx(map_elements_enum, "MAP_MURPHY", 3, -1);
	AddConstEx(map_elements_enum, "MAP_INFOTRON", 4, -1);
	AddConstEx(map_elements_enum, "MAP_RAM_CHIP_SQUARE", 5, -1);
	AddConstEx(map_elements_enum, "MAP_GRAY_DENTED_PYRAMID", 6, -1);
	AddConstEx(map_elements_enum, "MAP_EXIT", 7, -1);
	AddConstEx(map_elements_enum, "MAP_DISK_ORANGE", 8, -1);
	AddConstEx(map_elements_enum, "MAP_PORT_LEFT_TO_RIGHT", 9, -1);
	AddConstEx(map_elements_enum, "MAP_PORT_UP_TO_DOWN", 10, -1);
	AddConstEx(map_elements_enum, "MAP_PORT_RIGHT_TO_LEFT", 11, -1);
	AddConstEx(map_elements_enum, "MAP_PORT_DOWN_TO_UP", 12, -1);
	AddConstEx(map_elements_enum, "MAP_PORT_SPECIAL_LEFT_TO_RIGHT", 13, -1);
	AddConstEx(map_elements_enum, "MAP_PORT_SPECIAL_UP_TO_DOWN", 14, -1);
	AddConstEx(map_elements_enum, "MAP_PORT_SPECIAL_RIGHT_TO_LEFT", 15, -1);
	AddConstEx(map_elements_enum, "MAP_PORT_SPECIAL_DOWN_TO_UP", 16, -1);
	AddConstEx(map_elements_enum, "MAP_SNIK_SNAK", 17, -1);
	AddConstEx(map_elements_enum, "MAP_DISK_YELLOW", 18, -1);
	AddConstEx(map_elements_enum, "MAP_TERMINAL", 19, -1);
	AddConstEx(map_elements_enum, "MAP_DISK_RED", 20, -1);
	AddConstEx(map_elements_enum, "MAP_PORT_VERTICAL", 21, -1);
	AddConstEx(map_elements_enum, "MAP_PORT_HORIZONTAL", 22, -1);
	AddConstEx(map_elements_enum, "MAP_PORT_CROSS", 23, -1);
	AddConstEx(map_elements_enum, "MAP_ELECTRON", 24, -1);
	AddConstEx(map_elements_enum, "MAP_BUG", 25, -1);
	AddConstEx(map_elements_enum, "MAP_RAM_CHIP_LEFT", 26, -1);
	AddConstEx(map_elements_enum, "MAP_RAM_CHIP_RIGHT", 27, -1);
	AddConstEx(map_elements_enum, "MAP_HARDWARE_1", 28, -1);
	AddConstEx(map_elements_enum, "MAP_GREEN_LAMP", 29, -1);
	AddConstEx(map_elements_enum, "MAP_BLUE_LAMP", 30, -1);
	AddConstEx(map_elements_enum, "MAP_RED_LAMP", 31, -1);
	AddConstEx(map_elements_enum, "MAP_YELLOW_BLACK_DIAGONAL", 32, -1);
	AddConstEx(map_elements_enum, "MAP_HARDWARE_2", 33, -1);
	AddConstEx(map_elements_enum, "MAP_HARDWARE_3", 34, -1);
	AddConstEx(map_elements_enum, "MAP_HARDWARE_4", 35, -1);
	AddConstEx(map_elements_enum, "MAP_HARDWARE_5", 36, -1);
	AddConstEx(map_elements_enum, "MAP_HARDWARE_6", 37, -1);
	AddConstEx(map_elements_enum, "MAP_RAM_CHIP_TOP", 38, -1);
	AddConstEx(map_elements_enum, "MAP_RAM_CHIP_BOTTOM", 39, -1);
	AddConstEx(map_elements_enum, "MAP_INVISIBLE_WALL", 40, -1);

	map_elements_enum = AddEnum(GetEnumQty(), "MAP_DIMENSIONS", 0);
	AddConstEx(map_elements_enum, "MAP_WIDTH", 60, -1);
	AddConstEx(map_elements_enum, "MAP_HEIGHT", 24, -1);
	AddConstEx(map_elements_enum, "MAP_NUM_CELLS", 60 * 24, -1);

	// --------------------------------------------
	// Functions

	MakeUnkn(0x36C20, 0);
	MakeCode(0x36C20);
	MakeUnkn(0x36C25, 0);
	MakeFunction(0x36C20, 0x36CFC);
	MakeName(0x36C20, "main");
//	MakeNameEx(0x36C25, "@@1", 0);
//	MakeName(0x36C82, "@@2");
	OpOff(0x36C50, 1, 0x3F600);
	OpOff(0x36C59, 1, 0x3F600);
	OpOff(0x36C87, 1, 0x3F600);
	OpOff(0x36C98, 1, 0x3F600);
	OpOff(0x36CAD, 1, 0x3F600);
	OpOff(0x36CE0, 1, 0x3F600);

	MakeName(0x36CFD, "DisplayErrorAndExit");

	MakeUnkn(0x36D0F, 0);
	MakeUnkn(0x36D17, 0);
	MakeCode(0x36D0F);
	MakeFunction(0x36D0F, BADADDR);
	//MakeName(0x36D0F, "");
	OpOff(0x36D0F, 1, 0x3F600);
	OpOff(0x36D18, 1, 0x3F600);
	OpDecimal(0x36D38, 1);
	OpOff(0x36D3B, 1, 0x3F600);
	OpDecimal(0x36D46, 1);
	OpOff(0x36D49, 1, 0x3F600);
	OpOff(0x36D52, 1, 0x3F600);

	MakeFunction(0x36EA2, BADADDR);
	MakeName(0x36EA2, "InitKeyboard");
	OpOff(0x36EB2, 1, 0x36C20);

	MakeFunction(0x36EC4, BADADDR);
	MakeName(0x36EC4, "UninitKeyboard");

	MakeFunction(0x36ED8, BADADDR);
	MakeName(0x36ED8, "Int9_Handler");
	MakeComm(0x36F09, "Alt+SysRq");

	MakeUnkn(0x36F2A, 0);
	MakeUnkn(0x36F2E, 0);
	MakeUnkn(0x36F30, 0);
	MakeUnkn(0x36F37, 0);
	MakeFunction(0x36F2A, BADADDR);
	MakeName(0x36F2A, "Int8_Handler");
	//MakeNameEx(0x36F41, "@@1", SN_LOCAL);

	MakeFunction(0x36FB5, BADADDR);
	MakeName(0x36FB5, "InitTimer");
	OpOff(0x36FC5, 1, 0x36C20);

	MakeFunction(0x36FE3, BADADDR);
	MakeName(0x36FE3, "UninitTimer");

	MakeFunction(0x37003, BADADDR);
	MakeName(0x37003, "InitDosErrorHandler");
	OpOff(0x37013, 1, 0x36C20);

	MakeFunction(0x37024, BADADDR);
	MakeName(0x37024, "UninitDosErrorHandler");

	MakeUnkn(0x37038, 0);
	MakeFunction(0x37038, BADADDR);
	MakeName(0x37038, "DosErrorHandler");

	MakeFunction(0x3703B, BADADDR);
	MakeName(0x3703B, "ReadConfigFile");
	OpOff(0x3703E, 1, 0x3F600);
	OpChr(0x37080, 1);
	OpChr(0x37088, 1);
	OpChr(0x37090, 1);
	OpChr(0x37098, 1);
	OpChr(0x370A0, 1);
	OpChr(0x370A8, 1);
	OpChr(0x370B1, 1);
	OpChr(0x370BB, 1);
	OpChr(0x370C9, 1);
	OpChr(0x370D3, 1);
	OpChr(0x370DE, 1);
	OpChr(0x370E8, 1);

	MakeUnkn(0x370FC, 0);
	MakeFunction(0x370FC, BADADDR);
	MakeName(0x370FC, "WriteConfigFile");
	OpOff(0x37102, 1, 0x3F600);
	OpOff(0x3710F, 1, 0x3F600);
	OpChr(0x37119, 1);
	OpChr(0x37125, 1);
	OpChr(0x37131, 1);
	OpChr(0x3713D, 1);
	OpChr(0x37149, 1);
	OpChr(0x3714E, 1);
	OpChr(0x37159, 1);
	OpChr(0x3715E, 1);
	OpChr(0x37169, 1);
	OpChr(0x3716E, 1);
	OpChr(0x37179, 1);
	OpChr(0x3717E, 1);
	OpOff(0x3718B, 1, 0x3F600);

	MakeFunction(0x371A4, BADADDR);
	MakeName(0x371A4, "DisableFloppyMotor");

	MakeFunction(0x371AB, BADADDR);
	MakeName(0x371AB, "LoadDemoFile");
	OpOff(0x371B1, 1, 0x27200);
	OpOff(0x371BF, 1, 0x3F600);
	OpChr(0x371C2, 1);

	MakeFunction(0x3721A, BADADDR);
	MakeName(0x3721A, "LoadPalettes");
	OpOff(0x3721D, 1, 0x3F600);
	OpDecimal(0x3723B, 1);
	OpOff(0x3723E, 1, 0x3F600);

	MakeFunction(0x37345, BADADDR);
	MakeName(0x37345, "DisplayTitleScreen");
	OpOff(0x37348, 1, 0x3F600);
	OpDecimal(0x37381, 1);
	OpOff(0x37384, 1, 0x3F600);
	OpOff(0x3738E, 1, 0x3F600);
	OpDecimal(0x3739D, 1);
	OpDecimal(0x373A2, 1);
	OpDecimal(0x373AC, 1);
	OpOff(0x37400, 1, 0x3F600);
	OpDecimal(0x3742F, 1);
	OpOff(0x37432, 1, 0x3F600);
	OpOff(0x3743C, 1, 0x3F600);
	OpDecimal(0x3744B, 1);
	OpDecimal(0x37450, 1);

	MakeFunction(0x37479, BADADDR);
	MakeName(0x37479, "LoadFixedAndMoving");
	OpOff(0x37482, 1, 0x3F600);
	OpDecimal(0x374D1, 1);
	OpOff(0x374D4, 1, 0x3F600);
	OpOff(0x374DE, 1, 0x3F600);
	OpDecimal(0x3750B, 1);
	OpOff(0x37540, 1, 0x3F600);
	OpDecimal(0x37554, 1);
	OpOff(0x37557, 1, 0x3F600);

	MakeFunction(0x3757D, BADADDR);
	MakeName(0x3757D, "LoadPanelImage");
	OpOff(0x37580, 1, 0x3F600);
	OpDecimal(0x375A0, 1);
	OpOff(0x375A3, 1, 0x3F600);

	MakeFunction(0x375BC, BADADDR);
	MakeName(0x375BC, "LoadBackDat");
	OpOff(0x375BF, 1, 0x3F600);
	OpDecimal(0x375E5, 1);
	OpOff(0x375E8, 1, 0x1F500);

	MakeFunction(0x37603, BADADDR);
	MakeName(0x37603, "LoadFonts");
	OpOff(0x37606, 1, 0x3F600);
	OpDecimal(0x37626, 1);
	OpOff(0x37629, 1, 0x3F600);
	OpOff(0x37644, 1, 0x3F600);
	OpDecimal(0x37658, 1);
	OpOff(0x3765B, 1, 0x3F600);

	MakeFunction(0x37727, BADADDR);
	MakeName(0x37727, "LoadLevelsList");
	OpOff(0x3772A, 1, 0x3F600);
	OpDecimal(0x3774A, 1);
	OpOff(0x3774D, 1, 0x3F600);

	MakeFunction(0x37766, BADADDR);
	MakeName(0x37766, "LoadGfxDat");
	OpOff(0x37769, 1, 0x3F600);
	OpDecimal(0x3778F, 1);
	OpOff(0x37792, 1, 0xFB00);

	MakeFunction(0x377AD, BADADDR);
	MakeName(0x377AD, "LoadControlsDat");
	OpOff(0x377B0, 1, 0x3F600);
	OpDecimal(0x377D6, 1);
	OpOff(0x377D9, 1, 0x17800);

	MakeFunction(0x377F4, BADADDR);
	MakeName(0x377F4, "LoadPlayerLst");
	OpOff(0x377F7, 1, 0x3F600);
	OpDecimal(0x37803, 1);
	OpOff(0x37806, 1, 0x3F600);

	MakeFunction(0x37813, BADADDR);
	MakeName(0x37813, "LoadHallfameLst");
	OpOff(0x37816, 1, 0x3F600);
	OpDecimal(0x37822, 1);
	OpOff(0x37825, 1, 0x3F600);

	MakeFunction(0x37832, BADADDR);
	MakeName(0x37832, "LoadDataFiles");

	MakeFunction(0x382EA, BADADDR);
	//MakeName(0x382EA, "");
	OpOff(0x382EA, 0, 0x3F600);
	OpEnumEx(0x382EA, 1, GetEnum("MAP_ELEMENT"), 0);
	OpOff(0x382F1, 0, 0x3F600);
	OpEnumEx(0x382F1, 1, GetEnum("MAP_ELEMENT"), 0);
	OpOff(0x382F7, 0, 0x3F600);
	OpOff(0x382FE, 0, 0x3F600);
//	OpOff(0x38307, 0, 0x3F600);
//	OpOff(0x3830F, 0, 0x3F600);
//	OpOff(0x38316, 0, 0x3F600);
//	OpOff(0x3831E, 0, 0x3F600);
//	OpOff(0x38325, 0, 0x3F600);

	MakeFunction(0x38370, BADADDR);
	MakeName(0x38370, "DisplayDosErrorMessage");
	OpOff(0x38378, 1, 0x4D460);
	//MakeNameEx(0x3837D, "@@1", SN_LOCAL);
	OpDecimal(0x3837D, 1);
	OpDecimal(0x38397, 1);
	OpDecimal(0x3839F, 1);

	MakeFunction(0x383B0, BADADDR);
	MakeName(0x383B0, "InitPlayerState");

	MakeFunction(0x385DD, BADADDR);
	MakeName(0x385DD, "PollJoystick");

	MakeFunction(0x3890E, BADADDR);
	MakeName(0x3890E, "CalibrateJoystick");

	MakeFunction(0x38A7F, BADADDR);
	MakeName(0x38A7F, "WriteDemoFile");
	OpOff(0x38A7F, 1, 0x3F600);
	OpChr(0x38A82, 1);
	OpOff(0x38AA6, 1, 0x3F600);

	MakeFunction(0x38FBA, BADADDR);
	MakeName(0x38FBA, "JoystickRawRead");

	MakeFunction(0x3916F, BADADDR);
	MakeName(0x3916F, "InitRandomNumberGenerator");

	MakeFunction(0x3917B, BADADDR);
	MakeName(0x3917B, "GenerateRandomNumber");
	OpDecimal(0x3917E, 1);
	OpDecimal(0x39183, 1);

	MakeFunction(0x39295, BADADDR);
	MakeName(0x39295, "FindPlayerPositionOnLevelMap");
	OpOff(0x392A2, 1, 0x3F600);
	OpEnumEx(0x392A5, 1, GetEnum("MAP_DIMENSIONS"), 0);
	OpEnumEx(0x392A8, 1, GetEnum("MAP_ELEMENT"), 0);
	OpDecimal(0x392AF, 1);
	OpEnumEx(0x392BC, 1, GetEnum("MAP_DIMENSIONS"), 0);

	OpDecimal(0x392E3, 1);
	OpOff(0x392E6, 1, 0x3F600);
	OpEnumEx(0x392EB, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x392F0, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x392F5, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x392FA, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x392FF, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x39304, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x39309, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3930E, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x39313, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x39309, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x39318, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3931D, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x39327, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3932D, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x39333, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x39339, 1, GetEnum("MAP_ELEMENT"), 0);

	MakeFunction(0x393B9, BADADDR);
	MakeName(0x393B9, "StartLevel");

	MakeFunction(0x39A20, BADADDR);
	MakeName(0x39A20, "LoadMenu");
	OpOff(0x39A23, 1, 0x3F600);
	OpDecimal(0x39A3D, 1);
	OpOff(0x39A40, 1, 0x7E00);

	MakeFunction(0x3BCAC, BADADDR);
	MakeName(0x3BCAC, "WritePlayerLst");
	OpOff(0x3BCB2, 1, 0x3F600);
	OpDecimal(0x3BCBE, 1);
	OpOff(0x3BCC1, 1, 0x3F600);

	MakeFunction(0x3BCCE, BADADDR);
	MakeName(0x3BCCE, "WriteHallfameLst");
	OpOff(0x3BCD4, 1, 0x3F600);
	OpDecimal(0x3BCE0, 1);
	OpOff(0x3BCE3, 1, 0x3F600);

	MakeFunction(0x3BD99, BADADDR);
	//MakeName(0x3BD99, "");
	OpOff(0x3BDAD, 1, 0x3F600);
	OpOff(0x3BDC0, 1, 0x3F600);
	OpOff(0x3BDD3, 1, 0x3F600);
	OpOff(0x3BDE6, 1, 0x3F600);
	OpOff(0x3BE05, 1, 0x3F600);
	OpOff(0x3BE18, 1, 0x3F600);
	OpOff(0x3BE2B, 1, 0x3F600);
	OpOff(0x3BE3E, 1, 0x3F600);
	OpOff(0x3BE5D, 1, 0x3F600);
	OpOff(0x3BE70, 1, 0x3F600);
	OpOff(0x3BE83, 1, 0x3F600);
	OpOff(0x3BE96, 1, 0x3F600);

	MakeFunction(0x3BF60, BADADDR);
	MakeName(0x3BF60, "InitVideoMode");

	MakeFunction(0x3BF9F, BADADDR);
	MakeName(0x3BF9F, "SetPalette");	// ?
	OpDecimal(0x3BFA6, 1);
	OpOff(0x3BFBA, 1, 0x3F600);

	MakeFunction(0x3BFC1, BADADDR);
	MakeName(0x3BFC1, "UninitVideoMode");

	MakeFunction(0x3BFC9, BADADDR);
	MakeName(0x3BFC9, "InitMouse");
	MakeComm(0x3BFDB, "iret");
	OpDecimal(0x3BFFD, 1);
	OpDecimal(0x3C000, 1);
	OpDecimal(0x3C008, 1);
	OpDecimal(0x3C00B, 1);
	OpDecimal(0x3C013, 1);
	OpDecimal(0x3C016, 1);
	OpDecimal(0x3C01B, 1);
	OpDecimal(0x3C021, 1);

	MakeFunction(0x3C02F, BADADDR);
	MakeName(0x3C02F, "UninitMouse");

	MakeFunction(0x3C039, BADADDR);
	MakeName(0x3C039, "PollMouse");
	OpDecimal(0x3C0C6, 1);
	OpDecimal(0x3C0CB, 1);
	OpDecimal(0x3C0D2, 1);
	OpDecimal(0x3C0D8, 1);
	OpDecimal(0x3C0EB, 1);
	OpDecimal(0x3C0F1, 1);

	MakeFunction(0x3C11E, BADADDR);
	MakeName(0x3C11E, "WaitVerticalSyncEnd");

	MakeFunction(0x3C20A, BADADDR);
	MakeName(0x3C20A, "LoadLevelMap");
	OpOff(0x3C20D, 1, 0x3F600);
	OpDecimal(0x3C251, 1);
	OpOff(0x3C254, 1, 0x3F600);
	OpDecimal(0x3C263, 1);
	OpOff(0x3C266, 1, 0x3F600);
	OpOff(0x3C269, 1, 0x3F600);
	OpOff(0x3C272, 1, 0x3F600);
	OpDecimal(0x3C277, 1);

	MakeFunction(0x3C440, BADADDR);
	MakeName(0x3C440, "SetupInt80andInt81");

	
	OpOff(0x3C5F1, 1, 0x4D570);

	MakeFunction(0x3C641, BADADDR);
	MakeName(0x3C641, "SND_xxx");

	MakeFunction(0x3C69B, BADADDR);
	MakeName(0x3C69B, "SND_yyy");

	MakeFunction(0x3C6DE, BADADDR);
	MakeName(0x3C6DE, "SND_zzz");

	MakeFunction(0x3C71E, BADADDR);
	MakeName(0x3C71E, "SND_qqq");

	MakeFunction(0x3C747, BADADDR);
	MakeName(0x3C747, "SND_ttt");

	MakeFunction(0x3C7B0, BADADDR);
	MakeName(0x3C7B0, "SND_ppp");

	MakeFunction(0x3C819, BADADDR);
	MakeName(0x3C819, "SND_mmm");

	MakeFunction(0x3C882, BADADDR);
	MakeName(0x3C882, "SND_nnn");

	MakeFunction(0x3C8EB, BADADDR);
	MakeName(0x3C8EB, "SND_aaa");

	MakeFunction(0x3C955, BADADDR);
	MakeName(0x3C955, "SND_bbb");

	MakeFunction(0x3C9BE, BADADDR);
	MakeName(0x3C9BE, "SND_ccc");

	MakeFunction(0x3CA30, BADADDR);
	MakeName(0x3CA30, "SND_ddd");

	MakeFunction(0x3CA60, BADADDR);
	//MakeName(0x3CA60, "");
	OpEnumEx(0x3CA66, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3CA76, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3CA8A, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3CA91, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3CA98, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3CA9F, 1, GetEnum("MAP_ELEMENT"), 0);

	OpEnumEx(0x3CCAC, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3CCB3, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3CCBB, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3CCC3, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3CCCA, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3CCD1, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3CCD8, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3CCDF, 1, GetEnum("MAP_ELEMENT"), 0);
	OpEnumEx(0x3CCE6, 1, GetEnum("MAP_ELEMENT"), 0);

	
	OpOff(0x3EFD7, 1, 0x00000);
	OpOff(0x3EFDA, 1, 0x00000);

	
	OpOff(0x3F018, 1, 0x00000);
	OpOff(0x3F01B, 1, 0x00000);

	
	OpOff(0x3F06A, 1, 0x00000);
	OpOff(0x3F072, 1, 0x00000);
	OpDecimal(0x3F079, 1);

	
	OpOff(0x3F098, 1, 0x00000);
	OpOff(0x3F09B, 1, 0x3F600);
	OpDecimal(0x3F0A1, 1);
	OpOff(0x3F0B5, 1, 0x3F600);
	OpOff(0x3F0B8, 1, 0x00000);

	MakeFunction(0x3F1A0, BADADDR);
	MakeName(0x3F1A0, "GenerateRandomNumber2");
	OpDecimal(0x3F1A3, 1);
	OpDecimal(0x3F1A8, 1);

	MakeFunction(0x3F409, BADADDR);
	MakeName(0x3F409, "InputValidationCode");
	OpOff(0x3F40B, 1, 0x36C20);
	OpOff(0x3F40E, 1, 0x36C20);
	OpDecimal(0x3F417, 1);
	OpDecimal(0x3F44C, 1);
	OpDecimal(0x3F44F, 1);
	OpDecimal(0x3F45E, 1);
	OpOff(0x3F461, 1, 0x36C20);
	OpDecimal(0x3F478, 1);
	OpOff(0x3F47B, 1, 0x36C20);
	OpDecimal(0x3F4BD, 1);
	OpDecimal(0x3F4C4, 1);
	OpDecimal(0x3F4CA, 1);
	OpDecimal(0x3F4D1, 1);
	OpOff(0x3F4E3, 1, 0x3F600);
	OpDecimal(0x3F4E6, 1);
	OpDecimal(0x3F4ED, 1);
	OpDecimal(0x3F4F3, 1);
	OpDecimal(0x3F4FA, 1);
	OpOff(0x3F502, 1, 0x3F600);
	OpOff(0x3F505, 1, 0x3F600);
	OpOff(0x3F50D, 1, 0x3F600);
	OpOff(0x3F510, 1, 0x3F600);
	OpOff(0x3F518, 1, 0x3F600);
	OpDecimal(0x3F52D, 1);
	OpChr(0x3F531, 1);
	OpChr(0x3F539, 1);
	OpChr(0x3F541, 1);
	OpChr(0x3F549, 1);
	OpChr(0x3F551, 1);
	OpChr(0x3F559, 1);
	OpChr(0x3F561, 1);
	OpChr(0x3F569, 1);
	OpChr(0x3F571, 1);
	OpChr(0x3F579, 1);
	OpDecimal(0x3F57D, 1);
	OpChr(0x3F581, 1);
	OpChr(0x3F58D, 1);
	OpOff(0x3F5C2, 1, 0x3F600);
	OpDecimal(0x3F5C5, 1);
	OpChr(0x3F5C9, 1);
	OpChr(0x3F5D4, 1);
	OpChr(0x3F5DD, 1);

	// --------------------------------------------
	// Variables

	MakeUnkn(0x7E00, 0);
	MakeByte(0x7E00);
	MakeArray(0x7E00, 32000);
	MakeName(0x7E00, "MenuDat");
	OpDecimal(0x7E00, -1);

	MakeUnkn(0xFB00, 0);
	MakeByte(0xFB00);
	MakeArray(0xFB00, 32000);
	MakeName(0xFB00, "GfxDat");
	OpDecimal(0xFB00, -1);

	MakeUnkn(0x17800, 0);
	MakeByte(0x17800);
	MakeArray(0x17800, 32000);
	MakeName(0x17800, "ControlsDat");
	OpDecimal(0x17800, -1);

	MakeUnkn(0x1F500, 0);
	MakeByte(0x1F500);
	MakeArray(0x1F500, 32000);
	MakeName(0x1F500, "BackDat");
	OpDecimal(0x1F500, -1);

	MakeByte(0x3FC10);
	MakeName(0x3FC10, "SavedVideoMode");

	MakeByte(0x3FC30);
	MakeName(0x3FC30, "EnableJoystick");

	MakeName(0x3FC36, "JoystickCalibrated");
	MakeName(0x3FC37, "JoystickMinX");
	MakeName(0x3FC39, "JoystickMinY");
	MakeName(0x3FC3B, "JoystickMaxX");	// StepX ?
	MakeName(0x3FC3D, "JoystickMaxY");	// StepY ?
	MakeName(0x3FC3F, "JoystickX");
	MakeName(0x3FC41, "JoystickY");

	MakeWord(0x3FD66);
	MakeName(0x3FD66, "FileHandle");

	MakeUnkn(0x4038C, 0);
	MakeUnkn(0x4038E, 0);
	MakeDword(0x4038C);
	MakeName(0x4038C, "SavedInt8");

	MakeName(0x403B3, "PlayerPosition_MapX");
	MakeName(0x403B5, "PlayerPosition_MapY");

	MakeName(0x403B9, "PlayerPosition_Ofs");

	MakeUnkn(0x40397, 0);
	MakeUnkn(0x40399, 0);
	MakeDword(0x40397);
	MakeName(0x40397, "SavedInt24");

	MakeName(0x403AC, "PlayerPosition2_MapX");
	MakeName(0x403AE, "PlayerPosition2_MapY");

	MakeWord(0x403D4);
	MakeName(0x403D4, "FileHandle2");

	MakeName(0x403D8, "PlayerPosition_PixelsX");
	MakeName(0x403DA, "PlayerPosition_PixelsY");

	MakeUnkn(0x40CED, 0);
	MakeUnkn(0x40CEF, 0);
	MakeDword(0x40CED);
	MakeName(0x40CED, "SavedInt9");

	MakeName(0x40CF9, "KeyboardScanCode");

	MakeName(0x40DAF, "LEVELS_DAT");

	MakeUnkn(0x40DBA, 0);
	MakeByte(0x40DBA);
	MakeArray(0x40DBA, 1440);
	MakeName(0x40DBA, "LevelMap");
	OpDecimal(0x40DBA, -1);

	MakeDword(0x4135A);
	MakeName(0x4135A, "LevelMap_Unused");

	MakeByte(0x4135E);
	MakeName(0x4135E, "LevelMap_Gravitation");
	MakeComm(0x4135E, "0=off, 1=on");

	MakeByte(0x4135F);
	MakeName(0x4135F, "LevelMap_Version");
	MakeComm(0x4135F, "0x20 = original, 0x74 = v5.4, 0x80 = v6.0");

	MakeUnkn(0x41360, 0);
	MakeByte(0x41360);
	MakeArray(0x41360, 23);
	MakeName(0x41360, "LevelMap_Name");
	OpDecimal(0x41360, -1);

	MakeByte(0x41377);
	MakeName(0x41377, "LevelMap_FreezeZonks");
	MakeComm(0x41377, "Freeze zonks start value (0=off, 2=on) (yes 2=on, no mistake!)");

	MakeByte(0x41378);
	MakeName(0x41378, "LevelMap_InfotronsNeeded");
	MakeComm(0x41378, "0 means Supaplex will count the total number of infotrons in the level at the start and use that");

	MakeByte(0x41379);
	MakeName(0x41379, "LevelMap_NumSpecialPorts");
	MakeComm(0x41379, "Maximum 10");

	MakeStructEx(0x4137A, 6, "SPECIAL_PORT");
	MakeArray(0x4137A, 10);
	MakeName(0x4137A, "LevelMap_SpecialPorts");

	MakeDword(0x413B6);
	MakeName(0x413B6, "LevelMap_Unused2");

	MakeUnkn(0x419BA, 0);
	MakeByte(0x419BA);
	MakeArray(0x419BA, 1536);
	MakeName(0x419BA, "SomeLevelData");
	OpDecimal(0x419BA, -1);

	MakeName(0x42CA3, "SUPAPLEX_CFG");
	MakeName(0x42CB0, "MOVING_DAT");
	MakeName(0x42CBB, "FIXED_DAT");
	MakeName(0x42CC5, "PANEL_DAT");
	MakeName(0x42CCF, "MENU_DAT");
	MakeName(0x42CD8, "TITLE_DAT");
	MakeName(0x42CE2, "TITLE1_DAT");
	MakeName(0x42CED, "TITLE2_DAT");
	MakeName(0x42CF8, "BACK_DAT");
	MakeName(0x42D01, "GFX_DAT");
	MakeName(0x42D09, "CHARS6_DAT");
	MakeName(0x42D14, "CHARS8_DAT");
	MakeName(0x42D1F, "LEVEL_LST");
	MakeName(0x42D29, "CONTROLS_DAT");
	MakeName(0x42D36, "BEEP_SND");
	MakeName(0x42D3F, "SAMPLE_SND");
	MakeName(0x42D4A, "ADLIB_SND");
	MakeName(0x42D54, "ROLAND_SND");
	MakeName(0x42D5F, "BLASTER_SND");
	MakeName(0x42D6B, "PLAYER_LST");
	MakeName(0x42D76, "PALETTES_DAT");
	MakeName(0x42D83, "HALLFAME_LST");
	MakeName(0x42D90, "DEMO0_BIN");

	MakeUnkn(0x42D9B, 0);
	MakeByte(0x42D9B);
	MakeArray(0x42D9B, 5120);
	MakeName(0x42D9B, "Fixed");
	OpDecimal(0x42D9B, -1);

	MakeUnkn(0x4419B, 0);
	MakeByte(0x4419B);
	MakeArray(0x4419B, 3840);
	MakeName(0x4419B, "PanelImage");
	OpDecimal(0x4419B, -1);

	MakeUnkn(0x4509B, 0);
	MakeByte(0x4509B);
	MakeArray(0x4509B, 512);
	MakeName(0x4509B, "Chars6");
	OpDecimal(0x4509B, -1);

	MakeUnkn(0x4529B, 0);
	MakeByte(0x4529B);
	MakeArray(0x4529B, 512);
	MakeName(0x4529B, "Chars8");
	OpDecimal(0x4529B, -1);

	MakeUnkn(0x4555B, 0);
	MakeByte(0x4555B);
	MakeArray(0x4555B, 256);
	MakeName(0x4555B, "Palettes");
	OpDecimal(0x4555B, -1);

	MakeWord(0x4622F);
	MakeName(0x4622F, "RandomSeed");

	MakeName(0x4B06F, "MousePosX");

	MakeName(0x4B073, "MousePosY");
	MakeName(0x4B075, "HaveMouse");

	MakeName(0x4B99A, "PlayerLst");

	MakeName(0x4C412, "HallfameLst");

	MakeName(0x4D430, "RandomSeed2");

	MakeName(0x4D434, "ExpectedValidationCode");

	MakeUnkn(0x4D448, 0);
	MakeStr(0x4D454, BADADDR);
	MakeStr(0x4D448, 0x4D450);
	MakeStr(0x4D450, 0x4D454);

	MakeStr(0x4D460, BADADDR);
	MakeName(0x4D460, "DosErrorMessages");

	MakeStr(0x4D161, BADADDR);
	MakeName(0x4D461, "MSG_DosError1");
	MakeStr(0x4D484, BADADDR);
	MakeName(0x4D484, "MSG_DosError2");
	MakeStr(0x4D49E, BADADDR);
	MakeName(0x4D49E, "MSG_DosError3");
	MakeStr(0x4D4B8, BADADDR);
	MakeName(0x4D4B8, "MSG_DosError4");
	MakeStr(0x4D4D7, BADADDR);
	MakeName(0x4D4D7, "MSG_DosError5");
	MakeStr(0x4D4F0, BADADDR);
	MakeName(0x4D4F0, "MSG_DosError6");

	MakeUnkn(0x4D50A, 0);
	MakeByte(0x4D50A);
	MakeArray(0x4D50A, 102);
	OpDecimal(0x4D50A, -1);
}
