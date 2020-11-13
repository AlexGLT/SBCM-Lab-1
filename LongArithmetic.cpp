#include <iostream>
#include <climits>
#include <cmath>
#include <string>
#include <vector>

#include "LongArithmetic.h"

bigInteger::bigInteger()
{
	size = 1;
	value = new unsigned int[1]{ 0 };
	hexString = "0";
}

bigInteger::bigInteger(const long long& size)
{
	this->size = size;
	hexString = "";
	value = new unsigned int[size];
}

bigInteger::bigInteger(const long long& size, const std::string& hexString)
{
	this->size = size;
	this->hexString = hexString;
	value = new unsigned int[size];
}

// For debugging
void showBigInteger(bigInteger* number, std::string numberName)
{
	std::cout << numberName << ": " << std::endl;

	std::cout << "|";

	for (int i = 0; i < number->size; i++)
	{
		std::cout << number->value[i] << "|";
	}

	std::cout << std::endl;
}

bigInteger* ZeroEraser(bigInteger* number)
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

	if (zeroCount == 0)
	{
		return number;
	}
	else
	{
		auto* optimizedNumber = new bigInteger(number->size - zeroCount);
		std::copy(number->value + zeroCount, number->value + number->size, optimizedNumber->value);

		return optimizedNumber;
	}
}

bigInteger* toBigIntConverting(std::string number, const int& bitRate)
{
	int numberSize = number.length();

	bigInteger* bigNumber = new bigInteger;

	if (bitRate >= 4)
	{
		std::string fixedNumber = number;
		int hexBitCount = (bitRate / 4);

		if (numberSize % hexBitCount != 0)
		{
			std::string nullStr;

			for (int i = hexBitCount - (numberSize % hexBitCount); i > 0; i--)
			{
				nullStr += "0";
				numberSize++;
			}

			fixedNumber = nullStr + number;
		}

		bigNumber = new bigInteger(numberSize / hexBitCount, number);

		for (long long i = bigNumber->size - 1; i >= 0; i--)
		{
			bigNumber->value[i] = strtoul(fixedNumber.substr(i * hexBitCount, hexBitCount).c_str(), nullptr, 16);
		}
	}
	else
	{
		//count of big bits for one hex cipher
		int bitForHexCount = (4 >> (bitRate - 1));

		bigNumber = new bigInteger(numberSize * bitForHexCount, number);

		for (int i = numberSize - 1; i >= 0; i--)
		{
			int numberHexBit = strtoul(number.substr(i, 1).c_str(), nullptr, 16);

			for (int j = bitForHexCount - 1; j >= 0; j--)
			{
				bigNumber->value[i * bitForHexCount + j] = (numberHexBit >> (3 - j)) & 1;
			}
		}

		bigNumber = ZeroEraser(bigNumber);
	}

	return bigNumber;
}

void toHexConverting(bigInteger* bigNumber, int bitRate)
{
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


			// Перескочити 4 значення масиву, але добавляємо 3, оскільки в ітераторі вже є i++
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

bool LongComp(bigInteger* numberA, bigInteger* numberB, bool severe)
{
	if (numberA->size < numberB->size) return false;
	if (numberA->size > numberB->size) return true;

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
	case false: return true;

	case true: return false;
	}
}

bigInteger* LongAdd(bigInteger* numberA, bigInteger* numberB, int bitRate, bool optimization)
{
	long long smallerNumberSize;
	bigInteger* smallerNumber;
	bigInteger* biggerNumber;

	bigInteger* numberC;

	if (numberA->size >= numberB->size)
	{
		numberC = new bigInteger(numberA->size + 1);
		smallerNumberSize = numberB->size;

		biggerNumber = numberA;
		smallerNumber = numberB;
	}
	else
	{
		numberC = new bigInteger(numberB->size + 1);
		smallerNumberSize = numberA->size;

		biggerNumber = numberB;
		smallerNumber = numberA;
	}

	unsigned long long module = ULLONG_MAX >> (64 - bitRate);

	unsigned int carry = 0;

	long long difference = abs(numberA->size - numberB->size);

	for (long long i = smallerNumberSize - 1; i >= 0; i--)
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

	auto* optimizedNumberC = ZeroEraser(numberC);

	if ((optimizedNumberC != numberC) && optimization)
	{
		delete[] numberC->value;
		numberC->value = NULL;
		
		delete numberC;
		numberC = NULL;
	}

	if (optimization)
	{
		toHexConverting(optimizedNumberC, bitRate);
	}

	return optimizedNumberC;
}

bigInteger* LongSub(bigInteger* numberA, bigInteger* numberB, int bitRate, bool optimization)
{
	unsigned long long module = static_cast<long long>(1) << bitRate;

	bigInteger* numberC;

	if (numberA->size < numberB->size)
	{
		numberC = new bigInteger;

		return numberC;
	}

	numberC = new bigInteger(numberA->size);

	long long borrow = 0;

	int difference = numberA->size - numberB->size;

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
			std::cout << module + subtraction << std::endl;
			numberC->value[i] = module + subtraction;
			borrow = 1;
		}
	}

	if (borrow != 0)
	{
		numberC = new bigInteger;

		return numberC;
	}

	if (bitRate >= 4)
	{
		auto* optimizedNumberC = new bigInteger;

		optimizedNumberC = ZeroEraser(numberC);

		if ((optimizedNumberC != numberC) && optimization)
		{
			delete[] numberC->value;
			numberC->value = NULL;

			delete numberC;
			numberC = NULL;
		}

		numberC = optimizedNumberC;
	}

	if (optimization)
	{
		toHexConverting(numberC, bitRate);
	}

	return numberC;
}

bigInteger* LongMul(bigInteger* numberA, bigInteger* numberB, int bitRate)
{
	unsigned long long module = ULLONG_MAX >> (64 - bitRate);

	auto* numberC = new bigInteger(numberA->size * numberB->size);
	std::fill(&numberC->value[0], &numberC->value[numberC->size], 0);

	for (long long i = numberB->size - 1; i >= 0; i--)
	{
		unsigned long long carry = 0;
		
		auto* intermediateMul = new bigInteger(numberA->size + (numberB->size - 1 - i) + 1);
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

		auto* newNumberC = LongAdd(numberC, intermediateMul, bitRate, false);

		if (newNumberC != numberC)
		{
			auto* temp = numberC;

			numberC = newNumberC;

			delete[] temp->value;
			temp->value = NULL;

			delete temp;
			temp = NULL;
		}

		delete[] intermediateMul->value;
		intermediateMul->value = NULL;

		delete intermediateMul;
		intermediateMul = NULL;
	}

	toHexConverting(numberC, bitRate);

	return numberC;
}

bigInteger* LongShiftBitsToHigh(bigInteger* number, unsigned long long shift)
{
	auto* highNumber = new bigInteger(number->size + shift);

	std::copy(number->value, number->value + number->size, highNumber->value);
	std::fill(&highNumber->value[number->size], &highNumber->value[highNumber->size], 0);

	return highNumber;
}

bigInteger* SmallFix(bigInteger* number)
{
	int fix = 4 - (number->size % 4);

	auto* fixNumber = new bigInteger(number->size + fix);

	std::fill(&fixNumber->value[0], &fixNumber->value[fix], 0);
	std::copy(number->value, number->value + number->size, fixNumber->value + fix);

	return fixNumber;
}

bigInteger** LongDiv(bigInteger* dividend, bigInteger* divisor, int bitRate)
{
	auto* bitDividend = toBigIntConverting(dividend->hexString, 1);
	auto* bitDivisor = toBigIntConverting(divisor->hexString, 1);

	auto* bitRemainder = new bigInteger(bitDividend->size);
	std::copy(bitDividend->value, bitDividend->value + bitDividend->size, bitRemainder->value);

	auto* wholePart = new bigInteger(bitDividend->size);
	std::fill(&wholePart->value[0], &wholePart->value[bitDividend->size], 0);

	while (LongComp(bitRemainder, bitDivisor, false))
	{
		long long tempLength = bitRemainder->size;

		auto* maxBitDivisor = LongShiftBitsToHigh(bitDivisor, (tempLength - bitDivisor->size));

		if (LongComp(maxBitDivisor, bitRemainder, true))
		{
			delete[] maxBitDivisor->value;
			maxBitDivisor->value = NULL;

			delete maxBitDivisor;
			maxBitDivisor = NULL;

			tempLength--;

			maxBitDivisor = LongShiftBitsToHigh(bitDivisor, (tempLength - bitDivisor->size));
		}

		auto* newBitRemainder = LongSub(bitRemainder, maxBitDivisor, 1, false);

		if (newBitRemainder != bitRemainder)
		{
			auto* temp = bitRemainder;

			bitRemainder = newBitRemainder;

			delete[] temp->value;
			temp->value = NULL;

			delete temp;
			temp = NULL;
		}

		auto* optimizedBitRemainder = ZeroEraser(bitRemainder);
		
		if (optimizedBitRemainder != bitRemainder)
		{
			auto* temp = bitRemainder;

			bitRemainder = optimizedBitRemainder;

			delete[] temp->value;
			temp->value = NULL;

			delete temp;
			temp = NULL;
		}

		wholePart->value[(wholePart->size - 1) - (tempLength - bitDivisor->size)] = 1;
	}

	auto* optimizedWholePart = ZeroEraser(wholePart);

	if (optimizedWholePart != wholePart)
	{
		auto* temp = wholePart;

		wholePart = optimizedWholePart;

		delete[] temp->value;
		temp->value = NULL;

		delete temp;
		temp = NULL;
	}

	if (bitRemainder->size % 4 != 0)
	{
		auto* newBitRemainder = SmallFix(bitRemainder);

		if (newBitRemainder != bitRemainder)
		{
			auto* temp = bitRemainder;

			bitRemainder = newBitRemainder;

			delete[] temp->value;
			temp->value = NULL;

			delete temp;
			temp = NULL;
		}
	}
	
	if (wholePart->size % 4 != 0)
	{
		auto* optimizedWholePart = SmallFix(wholePart);

		if (optimizedWholePart != wholePart)
		{
			auto* temp = wholePart;

			wholePart = optimizedWholePart;

			delete[] temp->value;
			temp->value = NULL;

			delete temp;
			temp = NULL;
		}
	}

	toHexConverting(wholePart, 1);
	toHexConverting(bitRemainder, 1);

	auto** answer = new bigInteger * [2]{ wholePart, bitRemainder };

	return answer;
}

//unsigned int* LongPower(const unsigned int* numberA, const unsigned int* numberB, int bitRate, long long numberASize, long long numberBBitSize, long long& numberCSize)
bigInteger* LongPower(bigInteger* numberA, bigInteger* numberB, int bitRate)
{
	auto* bitNumberB = toBigIntConverting(numberB->hexString, 1);

	auto* numberC = new bigInteger;
	numberC->value[0] = static_cast<unsigned int>(1);

	for (long long i = 0; i < bitNumberB->size; i++)
	{
		if (bitNumberB->value[i] == 1)
		{
//			numberC = LongMul(numberC, numberA, bitRate);

			auto* newNumberC = LongMul(numberC, numberA, bitRate);
			
			if (newNumberC != numberC)
			{
				auto* temp = numberC;

				numberC = newNumberC;

				delete[] temp->value;
				temp->value = NULL;

				delete temp;
				temp = NULL;
			}
		}

		if (i != bitNumberB->size - 1)
		{
//			numberC = LongMul(numberC, numberC, bitRate);

			auto* newNumberC = LongMul(numberC, numberC, bitRate);
			
			if (newNumberC != numberC)
			{
				auto* temp = numberC;

				numberC = newNumberC;

				delete[] temp->value;
				temp->value = NULL;

				delete temp;
				temp = NULL;
			}
		}
	}

	toHexConverting(numberC, bitRate);

	return numberC;
}