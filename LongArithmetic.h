#include <iostream>

struct bigInteger
{
	long long size;
	std::string hexString;

	unsigned int* value;
	
	bigInteger();

	bigInteger(const long long& size);

	bigInteger(const long long& size, const std::string& hexString);

	~bigInteger();
};


void freeMemory(int selfReculc, bigInteger* numberA, bigInteger* numberB = nullptr);
void ZeroEraser(bigInteger* number);
void SmallFix(bigInteger* number);

bigInteger* toBigIntConverting(std::string number, const int& bitRate);
void toHexConverting(bigInteger* bigNumber, int bitRate);

bool LongComp(bigInteger* numberA, bigInteger* numberB, bool severe);
bigInteger* LongShiftBitsToHigh(bigInteger* number, unsigned long long shift);

bigInteger* LongAdd(bigInteger* numberA, bigInteger* numberB, int bitRate, bool text = true, int selfReculc = 0);
bigInteger* LongSub(bigInteger* numberA, bigInteger* numberB, int bitRate, bool text = true, int selfReculc = 0);
bigInteger* LongMul(bigInteger* numberA, bigInteger* numberB, int bitRate, bool text = true, int selfReculc = 0);
bigInteger** LongDiv(bigInteger* dividend, bigInteger* divisor, int bitRate);
bigInteger* LongPower(bigInteger* numberA, bigInteger* numberB, int bitRate);