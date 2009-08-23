
static main()
{
	// Functions

	MakeFunction(0x36F2A, 0xFFFFFFFF);
	MakeName(0x36F2A, "Int8_Handler");
//	MakeName(0x36F41, "@@1");

	MakeFunction(0x36FB5, 0xFFFFFFFF);
	MakeName(0x36FB5, "InitTimer");

	MakeFunction(0x3916F, 0xFFFFFFFF);
	MakeName(0x3916F, "InitRandomNumberGenerator");

	MakeFunction(0x3917B, 0xFFFFFFFF);
	MakeName(0x3917B, "GenerateRandomNumber");
	OpDecimal(0x3917E, 1);
	OpDecimal(0x39183, 1);

	// Variables

	MakeWord(0x4622F);
	MakeName(0x4622F, "RandomSeed");

	MakeUnkn(0x4038C, 0);
	MakeUnkn(0x4038E, 0);
	MakeDword(0x4038C);
	MakeName(0x4038C, "SavedInt8");

}
