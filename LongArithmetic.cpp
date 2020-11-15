#include <climits>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "LongArithmetic.h"

bigInteger::bigInteger()
{
	size = 1;
	value = new unsigned int[1]{0};
	hexString = "0";
}

bigInteger::bigInteger(const long long &size)
{
	this->size = size;
	hexString = "";
	value = new unsigned int[size];
}

bigInteger::bigInteger(const long long &size, const std::string &hexString)
{
	this->size = size;
	this->hexString = hexString;
	value = new unsigned int[size];
}

bigInteger::~bigInteger()
{
	delete[] value;
	value = nullptr;
}

// For debugging
void showBigInteger(std::shared_ptr<bigInteger> number, std::string numberName)
{
	std::cout << numberName << ": " << std::endl;

	std::cout << "|";

	for (int i = 0; i < number->size; i++)
	{
		std::cout << number->value[i] << "|";
	}

	std::cout << std::endl;
}

void SmallFix(std::shared_ptr<bigInteger> number)
{
	int fix = 4 - (number->size % 4);

	long long newSize = number->size + fix;
	auto* fixNumberValue = new unsigned int[newSize];

	std::fill(&fixNumberValue[0], &fixNumberValue[fix], 0);
	std::copy(number->value, number->value + number->size, fixNumberValue + fix);

	delete[] number->value;
	number->value = nullptr;

	number->size = newSize;
	number->value = fixNumberValue;
}

void ZeroEraser(std::shared_ptr<bigInteger> number)
{
	unsigned int zeroCount = 0;

	for (int i = 0; i < number->size; i++)
	{
		if (number->value[i] == 0)
		{
			zeroCount++;
		}
		else
		{
			break;
		}
	}

	if (number->size - zeroCount == 0)
	{
		zeroCount--;
	}

	if (zeroCount > 0)
	{
		long long newSize = number->size - zeroCount;

		auto *optimizedNumberValue = new unsigned int[newSize];
		std::fill(&optimizedNumberValue[0], &optimizedNumberValue[newSize], 0);
		std::copy(number->value + zeroCount, number->value + number->size, optimizedNumberValue);

		delete[] number->value;
		number->value = nullptr;

		number->size = newSize;
		number->value = optimizedNumberValue;
	}
}

std::shared_ptr<bigInteger> LongShiftBits(std::shared_ptr<bigInteger> number, long long shift)
{
	if (number->size == 1 && shift < 0)
	{
		return number;
	}

	auto highNumber = std::make_shared<bigInteger>(number->size + shift);

	if (shift > 0)
	{
		std::fill(&highNumber->value[number->size], &highNumber->value[highNumber->size], 0);
		shift = 0;
	}

	std::copy(number->value, number->value + number->size + shift, highNumber->value);

	return highNumber;
}

bool LongComp(std::shared_ptr<bigInteger> numberA, std::shared_ptr<bigInteger> numberB, bool severe)
{
	if (numberA->size < numberB->size)
		return false;
	if (numberA->size > numberB->size)
		return true;

	for (long long i = 0; i < numberA->size; i++)
	{
		if (numberA->value[i] > numberB->value[i])
		{
			return true;
		}
		else if (numberA->value[i] < numberB->value[i])
		{
			return false;
		}
		else
		{
			continue;
		}
	}

	switch (severe)
	{
	case false:
		return true;

	case true:
		return false;
	}
}

std::shared_ptr<bigInteger> toBigIntConverting(const std::string &number, const int &bitRate)
{
	long long numberSize = number.length();

	std::shared_ptr<bigInteger> bigNumber;

	if (bitRate >= 4)
	{
		std::string fixedNumber = number;
		int hexBitCount = (bitRate / 4);

		if (numberSize % hexBitCount != 0)
		{
			std::string nullptrStr;

			for (int i = hexBitCount - (numberSize % hexBitCount); i > 0; i--)
			{
				nullptrStr += "0";
				numberSize++;
			}

			fixedNumber = nullptrStr + number;
		}

		bigNumber = std::make_shared<bigInteger>(numberSize / hexBitCount, number);

		for (long long i = bigNumber->size - 1; i >= 0; i--)
		{
			bigNumber->value[i] = strtoul(fixedNumber.substr(i * hexBitCount, hexBitCount).c_str(), nullptr, 16);
		}
	}
	else
	{
		//count of big bits for one hex cipher
		int bitForHexCount = (4 >> (bitRate - 1));

		bigNumber = std::make_shared<bigInteger>(numberSize * bitForHexCount, number);

		for (long long i = numberSize - 1; i >= 0; i--)
		{
			int numberHexBit = strtoul(number.substr(i, 1).c_str(), nullptr, 16);

			for (int j = bitForHexCount - 1; j >= 0; j--)
			{
				bigNumber->value[i * bitForHexCount + j] = (numberHexBit >> (3 - j)) & 1;
			}
		}

		ZeroEraser(bigNumber);
	}

	return bigNumber;
}

void toHexConverting(std::shared_ptr<bigInteger> bigNumber, int bitRate)
{
	bigNumber->hexString = "";

	for (long long i = 0; i < bigNumber->size; i++)
	{
		int binSum = 0;
		std::vector<int> smallNumber;

		if (bitRate >= 4)
		{
			for (int j = 0; j < bitRate; j += 4)
			{
				smallNumber.push_back((bigNumber->value[i] >> ((bitRate - 4) - j)) & 15);
			}
		}
		else
		{
			for (int j = 0; j < 4; j++)
			{
				binSum += bigNumber->value[i + j] << (3 - j);
			}

			i += 3;

			smallNumber.push_back(binSum);
		}

		for (int j = 0; j < smallNumber.size(); j++)
		{
			int cipher = smallNumber[j];

			if (cipher > 9)
			{
				cipher += 55;
			}
			else
			{
				cipher += 48;
			}

			bigNumber->hexString += static_cast<char>(cipher);
		}

		smallNumber.clear();
	}

	short zeroCount = 0;

	for (short i = 0; i < bitRate / 4; i++)
	{
		if (bigNumber->hexString[i] == '0')
		{
			zeroCount++;
		}
		else
		{
			break;
		}
	}

	bigNumber->hexString.erase(0, zeroCount);

	if (bigNumber->hexString == "")
	{
		bigNumber->hexString = "0";
	}
}

std::shared_ptr<bigInteger> LongAdd(std::shared_ptr<bigInteger> numberA, std::shared_ptr<bigInteger> numberB, int bitRate, bool text)
{
	std::shared_ptr<bigInteger> smallerNumber;
	std::shared_ptr<bigInteger> biggerNumber;

	std::shared_ptr<bigInteger> numberC;

	if (numberA->size >= numberB->size)
	{
		numberC = std::make_shared<bigInteger>(numberA->size + 1);

		biggerNumber = numberA;
		smallerNumber = numberB;
	}
	else
	{
		numberC = std::make_shared<bigInteger>(numberB->size + 1);

		biggerNumber = numberB;
		smallerNumber = numberA;
	}

	unsigned long long module = ULLONG_MAX >> (64 - bitRate);

	unsigned long long carry = 0;

	long long difference = abs(numberA->size - numberB->size);

	for (long long i = smallerNumber->size - 1; i >= 0; i--)
	{
		unsigned long long sum = static_cast<unsigned long long>(biggerNumber->value[i + difference]) + static_cast<unsigned long long>(smallerNumber->value[i]) + carry;
		numberC->value[i + difference + 1] = sum & module;
		carry = sum >> bitRate;
	}
	for (long long i = difference - 1; i >= 0; i--)
	{
		unsigned long long sum = static_cast<unsigned long long>(biggerNumber->value[i]) + carry;
		numberC->value[i + 1] = sum & module;
		carry = sum >> bitRate;
	}

	numberC->value[0] = carry;

	ZeroEraser(numberC);

	if (text)
	{
		toHexConverting(numberC, bitRate);
	}

	return numberC;
}

std::shared_ptr<bigInteger> LongSub(std::shared_ptr<bigInteger> numberA, std::shared_ptr<bigInteger> numberB, int bitRate, bool text)
{
	unsigned long long module = static_cast<long long>(1) << bitRate;

	std::shared_ptr<bigInteger> numberC;

	if (numberA->size < numberB->size)
	{
		numberC = std::make_shared<bigInteger>();

		return numberC;
	}

	numberC = std::make_shared<bigInteger>(numberA->size);

	long long borrow = 0;

	long long difference = numberA->size - numberB->size;

	for (long long i = numberB->size - 1; i >= 0; i--)
	{
		long long subtraction = static_cast<long long>(numberA->value[i + difference]) - static_cast<long long>(numberB->value[i]) - borrow;

		if (subtraction >= 0)
		{
			numberC->value[i + difference] = subtraction;
			borrow = 0;
		}
		else
		{
			numberC->value[i + difference] = module + subtraction;
			borrow = 1;
		}
	}
	for (long long i = difference - 1; i >= 0; i--)
	{
		long long subtraction = static_cast<long long>(numberA->value[i]) - borrow;

		if (subtraction >= 0)
		{
			numberC->value[i] = subtraction;
			borrow = 0;
		}
		else
		{
			numberC->value[i] = module + subtraction;
			borrow = 1;
		}
	}

	if (borrow != 0)
	{
		numberC = std::make_shared<bigInteger>();

		return numberC;
	}

	if (bitRate >= 4)
	{
		ZeroEraser(numberC);
	}

	if (text)
	{
		toHexConverting(numberC, bitRate);
	}

	return numberC;
}

std::shared_ptr<bigInteger> LongMul(std::shared_ptr<bigInteger> numberA, std::shared_ptr<bigInteger> numberB, int bitRate, bool text)
{
	unsigned long long module = ULLONG_MAX >> (64 - bitRate);

	auto numberC = std::make_shared<bigInteger>(numberA->size + numberB->size);
	std::fill(&numberC->value[0], &numberC->value[numberC->size], 0);

	for (long long i = numberB->size - 1; i >= 0; i--)
	{
		unsigned long long carry = 0;

		auto intermediateMul = std::make_shared<bigInteger>(numberA->size + (numberB->size - 1 - i) + 1);
		std::fill(&intermediateMul->value[0], &intermediateMul->value[intermediateMul->size], 0);

		for (long long j = numberA->size - 1; j >= 0; j--)
		{
			unsigned long long temp = static_cast<unsigned long long>(numberA->value[j]) * static_cast<unsigned long long>(numberB->value[i]) + carry;
			intermediateMul->value[j + 1] = temp & module;

			carry = temp >> bitRate;

			if (j == 0)
			{
				intermediateMul->value[j] = carry;
			}
		}

		numberC = LongAdd(numberC, intermediateMul, bitRate, false);
	}

	if (text)
	{
		toHexConverting(numberC, bitRate);
	}

	return numberC;
}

std::shared_ptr<std::pair<std::shared_ptr<bigInteger>, std::shared_ptr<bigInteger>>> LongDiv(std::shared_ptr<bigInteger> dividend, std::shared_ptr<bigInteger> divisor, int bitRate, bool onlyWhole)
{
	auto bitDividend = toBigIntConverting(dividend->hexString, 1);
	auto bitDivisor = toBigIntConverting(divisor->hexString, 1);

	auto bitRemainder = std::make_shared<bigInteger>(bitDividend->size);
	std::copy(bitDividend->value, bitDividend->value + bitDividend->size, bitRemainder->value);

	auto bitWholePart = std::make_shared<bigInteger>(bitDividend->size);
	std::fill(&bitWholePart->value[0], &bitWholePart->value[bitDividend->size], 0);

	while (LongComp(bitRemainder, bitDivisor, false))
	{
		long long tempLength = bitRemainder->size;

		auto maxBitDivisor = LongShiftBits(bitDivisor, (tempLength - bitDivisor->size));

		if (LongComp(maxBitDivisor, bitRemainder, true))
		{
			tempLength--;

			maxBitDivisor = LongShiftBits(bitDivisor, (tempLength - bitDivisor->size));
		}

		bitRemainder = LongSub(bitRemainder, maxBitDivisor, 1, false);

		ZeroEraser(bitRemainder);

		bitWholePart->value[(bitWholePart->size - 1) - (tempLength - bitDivisor->size)] = 1;
	}

	ZeroEraser(bitWholePart);

	if (bitRemainder->size % 4 != 0)
	{
		SmallFix(bitRemainder);
	}

	if (bitWholePart->size % 4 != 0)
	{
		SmallFix(bitWholePart);
	}

	toHexConverting(bitWholePart, 1);
	auto wholePart = toBigIntConverting(bitWholePart->hexString, bitRate);

	std::shared_ptr<bigInteger> remainder = nullptr;

	if (!onlyWhole)
	{
		toHexConverting(bitRemainder, 1);
		remainder = toBigIntConverting(bitRemainder->hexString, bitRate);
	}

	return std::make_shared<std::pair<std::shared_ptr<bigInteger>, std::shared_ptr<bigInteger>>>(std::make_pair(wholePart, remainder));
}

std::shared_ptr<bigInteger> LongPow(std::shared_ptr<bigInteger> numberA, std::shared_ptr<bigInteger> numberB, int bitRate)
{
	auto bitNumberB = toBigIntConverting(numberB->hexString, 1);

	auto numberC = std::make_shared<bigInteger>();
	numberC->value[0] = static_cast<unsigned int>(1);

	for (long long i = 0; i < bitNumberB->size; i++)
	{
		if (bitNumberB->value[i] == 1)
		{
			numberC = LongMul(numberC, numberA, bitRate, false);
		}

		if (i != bitNumberB->size - 1)
		{
			numberC = LongMul(numberC, numberC, bitRate, false);
		}
	}

	toHexConverting(numberC, bitRate);

	return numberC;
}