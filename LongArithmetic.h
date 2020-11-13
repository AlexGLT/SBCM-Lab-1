#include <iostream>

struct bigInteger
{
	long long size;
	std::string hexString;

	unsigned int* value;
	
	bigInteger();

	bigInteger(const long long& size);

	bigInteger(const long long& size, const std::string& hexString);
};

//unsigned int* ZeroEraser(unsigned int* number, long long& numberSize);
bigInteger* ZeroEraser(bigInteger* number);
//unsigned int* toBigIntConverting(std::string& number, const int& bitRate, long long& bigNumSize);
bigInteger* toBigIntConverting(std::string number, const int& bitRate);

//std::string* toHexConverting(unsigned int* bigNumber, int bitRate, long long& bigNumSize);
void toHexConverting(bigInteger* bigNumber, int bitRate);

//bool LongComp(const unsigned int* numberA, const unsigned int* numberB, const long long& numberASize, const long long& numberBSize, bool severe);
bool LongComp(bigInteger* numberA, bigInteger* numberB, bool severe);

//unsigned int* LongAdd(const unsigned int* numberA, const unsigned int* numberB, int bitRate, long long numberASize, long long numberBSize, long long& numberCSize, bool mul = false);
bigInteger* LongAdd(bigInteger* numberA, bigInteger* numberB, int bitRate, bool optimization = true);

//unsigned int* LongSub(const unsigned int* numberA, const unsigned int* numberB, int bitRate, long long numberASize, long long numberBSize, long long& numberCSize, bool div = false);
bigInteger* LongSub(bigInteger* numberA, bigInteger* numberB, int bitRate, bool optimization = true);

// unsigned int* LongMul(const unsigned int* numberA, const unsigned int* numberB, int bitRate, long long numberASize, long long numberBSize, long long& numberCSize);
bigInteger* LongMul(bigInteger* numberA, bigInteger* numberB, int bitRate);

// unsigned int* LongShiftBitsToHigh(const unsigned int* number, unsigned long long shift, const long long& numberSize, long long& shiftedNumberSize);
bigInteger* LongShiftBitsToHigh(bigInteger* number, unsigned long long shift);

// unsigned int* SmallFix(const unsigned int* number, long long& numberSize);
bigInteger* SmallFix(bigInteger* number);

// unsigned int** LongDiv(std::string& dividend, std::string& divisor, int bitRate, long long& wholePartBitLength, long long& remainderBitLength);
bigInteger** LongDiv(bigInteger* dividend, bigInteger* divisor, int bitRate);

// unsigned int* LongPower(const unsigned int* numberA, const unsigned int* numberB, int bitRate, long long numberASize, long long numberBBitSize, long long& numberCSize);
bigInteger* LongPower(bigInteger* numberA, bigInteger* numberB, int bitRate);