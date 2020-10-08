#include <iostream> 
#include <cmath>
#include <string>

unsigned int* toBigIntConverting(std::string& number, short bitRate)
{
	short bigBitCount = 2048 / bitRate;
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

std::string* toHexConverting(unsigned int* bigNumber, short bitRate)
{
	auto* hexNumber = new std::string;
	short bigBitCount = 2048 / bitRate;

	bool headNull = true;

	if (bitRate >= 4)
	{
		for (short i = 0; i < bigBitCount; i++)
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

		for (short i = 0; i < bigBitCount; i++)
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

bool LongComp(const unsigned int* numberA, const unsigned int* numberB, bool severe, short bitRate)
{
	short bigBitCount = 2048 / bitRate;

	for (short i = 0; i < bigBitCount; i++)
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

unsigned int* LongAdd(const unsigned int *numberA, const unsigned int *numberB, short bitRate)
{
	unsigned int module = 0;
	module--;

	short bigBitCount = 2048 / bitRate;
	
	auto* numberC = new unsigned int[bigBitCount];

	short carry = 0;

	for (short i = bigBitCount - 1; i >= 0; i--)
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

		for (short i = bigBitCount - 1; i >= 0; i--)
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

	short bigBitCount = 2048 / bitRate;
	auto* numberC = new unsigned int[bigBitCount];

	short borrow = 0;

	for (short i = bigBitCount - 1; i >= 0; i--)
	{
		long long substraction = numberA[i];
		//if (i > 2040) std::cout << "lol kek:" << numberB[i] << std::endl;
		substraction -= numberB[i];
		substraction -= borrow;

		//std::cout << "i = " << i << ": "<< numberA[i] << " " << numberB[i] << " " << borrow << " " << std::endl;

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

unsigned int* LongMul(const unsigned int* numberA, const unsigned int* numberB, short bitRate)
{
	unsigned long long module = static_cast<unsigned long long>(1) << bitRate;

	short bigBitCount = 2048 / bitRate;
	auto* numberC = new unsigned int[bigBitCount];
	std::fill(&numberC[0], &numberC[bigBitCount], 0);

	for (short i = bigBitCount - 1; i >= 32; i--)
	{
		unsigned long long carry = 0;
		auto* intermediateMul = new unsigned int[bigBitCount];
		std::fill(&intermediateMul[0], &intermediateMul[bigBitCount], 0);

		for (short j = 63; j >= 32; j--)
		{
			unsigned long long temp = static_cast<unsigned long long>(numberA[j]) * numberB[i] + carry;
			intermediateMul[j - (63 - i)] = temp & (module - 1);
			carry = temp >> bitRate;

			//Закостилено для випадку, коли на вхід подається 1024 бітне число
			if (j == 32)
			{
				intermediateMul[j - (63 - i) - 1] = carry;
			}
		}

		numberC = LongAdd(numberC, intermediateMul, bitRate);

		delete[] intermediateMul;
	}

	return numberC;
}

unsigned short BitLength(const unsigned int* number, short bitRate)
{
	short bigBitCount = 2048 / bitRate;

	unsigned short bitLength = 0;

	for (short i = 0; i < bigBitCount; i++)
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
	short bigBitCount = 2048 / bitRate;

	unsigned short bitLength = 0;

	auto* highNumber = new unsigned int[bigBitCount];

	std::fill(&highNumber[0], &highNumber[bigBitCount], 0);

	for (short i = shift; i < bigBitCount; i++)
	{
		highNumber[i - shift] = number[i];
	}

	return highNumber;
}

unsigned int** LongDiv(std::string* dividend, std::string* divisor, short bitRate)
{
	short bigBitCount = 2048;

	unsigned int* numberA = toBigIntConverting(*dividend, 1);
	unsigned int* numberB = toBigIntConverting(*divisor, 1);

	unsigned short divisorBitLength = BitLength(numberB, 1);

	auto* divisionRemainder = numberA;

	auto* wholePart = new unsigned int[bigBitCount];
	std::fill(&wholePart[0], &wholePart[bigBitCount], 0);

	while (LongComp(divisionRemainder, numberB, false , 1))
	{
		unsigned short remainderBitLength = BitLength(divisionRemainder, 1);

		/*for (short i = 0; i < bigBitCount; i++)
		{
			std::cout << numberB[i];
		}

		std::cout << "|" << std::endl;*/

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

	/*std::cout << "Result of addition: ";
	unsigned int* addition = LongAdd(numberA, numberB, bitRate);
	std::cout << *toHexConverting(addition, bitRate) << std::endl;

	std::cout << "Result of substraction: ";
	unsigned int* substraction = LongSub(numberA, numberB, bitRate);
	std::cout << *toHexConverting(substraction, bitRate) << std::endl;

	std::cout << "Result of multiplication: ";
	unsigned int* multiplication = LongMul(numberA, numberB, bitRate);
	std::cout << *toHexConverting(multiplication, bitRate) << std::endl;

	std::cout << "Result of division: ";
	unsigned int* division = LongDiv(&dividend, &divisor, bitRate);
	std::cout << *toHexConverting(multiplication, bitRate) << std::endl;*/

	unsigned int** result = LongDiv(&dividend, &divisor, bitRate);
	unsigned int* division = result[0];
	unsigned int* remainder = result[1];
	
	std::cout << "Whole part from division: ";
	std::cout << *toHexConverting(division, 1) << std::endl;
	std::cout << "Remainder from division: ";
	std::cout << *toHexConverting(remainder, 1) << std::endl;

	return 0;
}