#include <iostream> 
#include <algorithm>
#include <cmath>
#include <string>

unsigned int* toBigIntConverting(std::string& number, short bitRate)
{
	unsigned long long bigBitCount = 2048 / bitRate;
	auto* bigNumber = new unsigned int[bigBitCount];
	std::fill(&bigNumber[0], &bigNumber[bigBitCount], 0);

	short numberSize = number.length();

	if (bitRate >= 4)
	{
		short hexBitCount = (bitRate / 4);

		if (numberSize % hexBitCount != 0)
		{
			std::string nullStr;

			for (int i = bitRate - (numberSize % hexBitCount); i > 0; i--)
			{
				nullStr += "0";
				numberSize++;
			}

			number = nullStr + number;
		}

		int numberBigBitCount = numberSize / hexBitCount;

		for (int i = numberBigBitCount; i > 0; i--)
		{
			bigNumber[bigBitCount - 1 - (numberBigBitCount - i)] =
				strtoul(number.substr((i * hexBitCount) - hexBitCount, hexBitCount).c_str(), nullptr, 16);
		}
	}
	else
	{
		for (int i = numberSize; i > 0; i--)
		{
			short numberHexBit = strtol(number.substr(i - 1, 1).c_str(), nullptr, 16);

			//count of big bits for one hex cipher
			short bitForHexCount = 4 >> (bitRate - 1);

			for (short j = bitForHexCount; j > 0; j--)
			{
				bigNumber[(bigBitCount - (numberSize - (i - 1)) * bitForHexCount) - 1 + j] = (numberHexBit >> (4 - j)) & 1;
			}
		}
	}

	return bigNumber;
}

std::string* toHexConverting(unsigned int* bigNumber, short bitRate, unsigned long long bitLength = 2048)
{
	auto* hexNumber = new std::string;
	unsigned long long bigBitCount = bitLength / bitRate;

	bool headNull = true;

	if (bitRate >= 4)
	{
		for (long long i = 0; i < bigBitCount; i++)
		{
			if (bigNumber[i] != 0 || !headNull)
			{
				headNull == false;

				for (short j = 0; j < bitRate; j += 4)
				{
					short cipher = (bigNumber[i] >> ((bitRate - 4) - j)) & 15;

					if (cipher > 9)
					{
						cipher += 55;
					}
					else
					{
						cipher += 48;
					}

					*hexNumber += static_cast<char>(cipher);
				}
			}
		}
	}
	else
	{
		short summ = 0;

		for (long long i = 0; i < bigBitCount; i++)
		{
			summ += bigNumber[i] << 3 - (i % 4); //рахуємо hex число

			if ((i + 1) % 4 == 0)
			{
				if (summ != 0 || !headNull)
				{
					headNull = false;

					if (summ > 9)
					{
						summ += 55;
					}
					else
					{
						summ += 48;
					}

					*hexNumber += static_cast<char>(summ);
				}

				summ = 0;
			}
		}
	}

	short zeroCount = 0;

	for (short i = 0; i < bitRate / 4; i++)
	{
		if ((*hexNumber)[i] == '0')
		{
			zeroCount++;
		}
		else
		{
			break;
		}
	}

	(*hexNumber).erase(0, zeroCount);

	if (*hexNumber == "")
	{
		*hexNumber = '0';
	}

	return hexNumber;
}

bool LongComp(const unsigned int* numberA, const unsigned int* numberB, bool severe, short bitRate, unsigned long long bitLength = 2048)
{
	unsigned long long bigBitCount = bitLength / bitRate;

	for (long long i = 0; i < bigBitCount; i++)
	{
		if (numberA[i] > numberB[i])
		{
			return true;
		}
		else if (numberA[i] < numberB[i])
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
	;
}

unsigned int* LongAdd(const unsigned int *numberA, const unsigned int *numberB, short bitRate, unsigned long long bitLength = 2048)
{
	unsigned int module = 0;
	module--;

	unsigned long long bigBitCount = bitLength / bitRate;
	
	auto* numberC = new unsigned int[bigBitCount];

	short carry = 0;

	for (long long i = bigBitCount - 1; i >= 0; i--)
	{
		unsigned long long summ = static_cast<unsigned long long>(numberA[i]) + numberB[i] + carry;

		numberC[i] = summ & module;

		carry = summ >> bitRate;
	}

	if (numberC[0] == 0)
	{
		numberC[0] = carry;
	}
	else if (numberC[0] != 0 && carry != 0)
	{
		auto* temp = new unsigned int[bigBitCount + 1];

		for (long long i = bigBitCount - 1; i >= 0; i--)
		{
			temp[i + 1] = numberC[i];
		}
		delete[] numberC;

		numberC = temp;
		numberC[0] = carry;
	}
	
	return numberC;
}

unsigned int* LongSub(const unsigned int* numberA, const unsigned int* numberB, short bitRate)
{
	unsigned long long module = static_cast<unsigned long long>(1) << bitRate;

	unsigned long long bigBitCount = 2048 / bitRate;
	auto* numberC = new unsigned int[bigBitCount];

	short borrow = 0;

	for (long long i = bigBitCount - 1; i >= 0; i--)
	{
		long long substraction = numberA[i];
		substraction -= numberB[i];
		substraction -= borrow;

		if (substraction >= 0)
		{
			numberC[i] = substraction;
			borrow = 0;
		}
		else
		{
			
			numberC[i] = module + substraction;
			borrow = 1;
		}
	}

	if (borrow != 0)
	{
		std::fill(&numberC[0], &numberC[bigBitCount], 0);
	}

	return numberC;
}

unsigned int* LongMul(const unsigned int* numberA, const unsigned int* numberB, short bitRate, unsigned long long bitLength = 2048)
{
	unsigned long long module = static_cast<unsigned long long>(1) << bitRate;

	unsigned long long bigBitCount = bitLength / bitRate;
	auto* numberC = new unsigned int[bigBitCount];
	std::fill(&numberC[0], &numberC[bigBitCount], 0);

	for (long long i = bigBitCount - 1; i >= bigBitCount / 2; i--)
	{
		unsigned long long carry = 0;
		auto* intermediateMul = new unsigned int[bigBitCount];
		std::fill(&intermediateMul[0], &intermediateMul[bigBitCount], 0);

		for (long long j = bigBitCount - 1; j >= bigBitCount / 2; j--)
		{
			unsigned long long temp = static_cast<unsigned long long>(numberA[j]) * numberB[i] + carry;
			intermediateMul[j - (bigBitCount - 1 - i)] = temp & (module - 1);
			carry = temp >> bitRate;

			if (j == bigBitCount / 2)
			{
				intermediateMul[j - (bigBitCount - 1 - i) - 1] = carry;
			}
		}

		numberC = LongAdd(numberC, intermediateMul, bitRate, bigBitCount * 32);

		delete[] intermediateMul;
	}

	return numberC;
}

unsigned short BitLength(const unsigned int* number, short bitRate)
{
	unsigned long long bigBitCount = 2048 / bitRate;

	unsigned short bitLength = 0;

	for (long long i = 0; i < bigBitCount; i++)
	{
		if (number[i] == 0)
		{
			continue;
		}
		else
		{
			bitLength = 2048 - i;
			break;
		}
	}

	return bitLength;
}

unsigned int* LongShiftBitsToHigh(const unsigned int* number, unsigned short shift, short bitRate)
{
	unsigned long long bigBitCount = 2048 / bitRate;

	unsigned short bitLength = 0;

	auto* highNumber = new unsigned int[bigBitCount];

	std::fill(&highNumber[0], &highNumber[bigBitCount], 0);

	for (long long i = shift; i < bigBitCount; i++)
	{
		highNumber[i - shift] = number[i];
	}

	return highNumber;
}

unsigned int** LongDiv(std::string* dividend, std::string* divisor, short bitRate)
{
	unsigned long long bigBitCount = 2048;

	unsigned int* numberA = toBigIntConverting(*dividend, 1);
	unsigned int* numberB = toBigIntConverting(*divisor, 1);

	unsigned short divisorBitLength = BitLength(numberB, 1);

	auto* divisionRemainder = numberA;

	auto* wholePart = new unsigned int[bigBitCount];
	std::fill(&wholePart[0], &wholePart[bigBitCount], 0);

	while (LongComp(divisionRemainder, numberB, false , 1))
	{
		unsigned short remainderBitLength = BitLength(divisionRemainder, 1);

		unsigned int* maxDivisor = LongShiftBitsToHigh(numberB, remainderBitLength - divisorBitLength, 1);

		if (LongComp(maxDivisor, divisionRemainder, true, 1))
		{
			remainderBitLength -= 1;

			maxDivisor = LongShiftBitsToHigh(numberB, (remainderBitLength - divisorBitLength), 1);
		}
		
		divisionRemainder = LongSub(divisionRemainder, maxDivisor, 1);

		wholePart[2047 - (remainderBitLength - divisorBitLength)] = 1;
	}

	unsigned int** answer = new unsigned int* [2]{ wholePart, divisionRemainder };

	return answer;
}

unsigned int* LongPower(const unsigned int* numberA, std::string* numberB, short bitRate, unsigned long long &bigBitCountPower)
{
	unsigned long long bigBitCount = 2048 / bitRate;

	auto* bitNumberB = toBigIntConverting(*numberB, 1);

	unsigned int numberBBitLength = BitLength(bitNumberB, 1);

	auto* numberC = new unsigned int[bigBitCount];
	std::fill(&numberC[0], &numberC[bigBitCount], 0);
	numberC[bigBitCount - 1] = 1;

	for (long long i = numberBBitLength - 1; i >= 0; i--)
	{
		unsigned short corrector = 1;
		unsigned short modifiedLength;

		if (bitNumberB[2047 - i] == 1 && i != numberBBitLength - 1)
		{
			corrector++;
		}

		bigBitCount *= (2 * corrector);

		auto* temp = new unsigned int[bigBitCount];
		
		switch (corrector)
		{
		case 1:
		{
			modifiedLength = bigBitCount / 2;
			std::fill(&temp[0], &temp[modifiedLength], 0);
			std::copy(numberC, numberC + modifiedLength, temp + modifiedLength);
		}
			break;
		case 2:
		{
			modifiedLength = bigBitCount * 3 / 4;
			std::fill(&temp[0], &temp[modifiedLength], 0);
			std::copy(numberC, numberC + bigBitCount / 4, temp + modifiedLength);
		}
			break;
		}

		//unsigned int* deleteTemp = temp;

		if (bitNumberB[2047 - i] == 1)
		{
			auto* bigNumberA = new unsigned int[bigBitCount];
			std::fill(&bigNumberA[0], &bigNumberA[bigBitCount - 2048 / bitRate], 0);
			std::copy(numberA, numberA + 2048 / bitRate, bigNumberA + bigBitCount - 2048 / bitRate);

			temp = LongMul(temp, bigNumberA, bitRate, bigBitCount * 32);

			delete[] bigNumberA;
		}

		if (i != 0)
		{
			temp = LongMul(temp, temp, bitRate, bigBitCount * 32);
		}

		//delete[] deleteTemp;

		numberC = temp;
	}

	bigBitCountPower = bigBitCount;

	return numberC;
}

int main()
{
	short bitRate;
	std::string number;

	std::cout << "Input numbers bit rate: ";
	std::cin >> bitRate;

	std::cout << "Input number A: ";
	std::cin >> number;
	unsigned int* numberA = toBigIntConverting(number, bitRate);

	std::string dividend(number);

	std::cout << "Input number B: ";
	std::cin >> number;
	unsigned int* numberB = toBigIntConverting(number, bitRate);

	std::string divisor(number);

	std::cout << "Result of addition: ";
	unsigned int* addition = LongAdd(numberA, numberB, bitRate);
	std::cout << *toHexConverting(addition, bitRate) << std::endl;

	std::cout << "Result of substraction: ";
	unsigned int* substraction = LongSub(numberA, numberB, bitRate);
	std::cout << *toHexConverting(substraction, bitRate) << std::endl;

	std::cout << "Result of multiplication: ";
	unsigned int* multiplication = LongMul(numberA, numberB, bitRate);
	std::cout << *toHexConverting(multiplication, bitRate) << std::endl;

	unsigned int** divisionResult = LongDiv(&dividend, &divisor, bitRate);
	unsigned int* division = divisionResult[0];
	unsigned int* remainder = divisionResult[1];

	std::cout << "Whole part from division: ";
	std::cout << *toHexConverting(division, 1) << std::endl;
	std::cout << "Remainder from division: ";
	std::cout << *toHexConverting(remainder, 1) << std::endl;

	unsigned long long bitBigBitCount;
	std::cout << "Do you want to A^B?" << std::endl;
	char choice;
	std::cin >> choice;

	if (choice == 'y')
	{
		std::cout << "\nResult of power: ";
		unsigned int* power = LongPower(numberA, &divisor, bitRate, bitBigBitCount);
		std::cout << *toHexConverting(power, bitRate, bitBigBitCount * 32) << std::endl;
	}

	return 0;
}