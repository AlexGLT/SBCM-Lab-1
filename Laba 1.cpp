#include <iostream> 
#include <cmath>
#include <string>

unsigned int* LongAdd(const unsigned int *numberA, const unsigned int *numberB, short bitRate)
{
	unsigned int module = 0;
	module--;

	short bigBitCount = 2048 / bitRate;
	
	auto* numberC = new unsigned int[bigBitCount];

	short carry = 0;

	for (short i = bigBitCount - 1; i >= 0; i--)
	{
		unsigned long long summ = carry;
		summ += numberA[i];
		summ += numberB[i];

		numberC[i] = summ & module;
		
		carry = summ >> bitRate;
	}

	numberC[0] = carry;
	
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
		}

		/*std::cout << i << ": " << numberC[i] << std::endl;*/
	}

	if (borrow != 0)
	{
		std::fill(&numberC[0], &numberC[bigBitCount - 1], 0);
	}

	return numberC;
}

unsigned int* toBigIntConverting(std::string& number, short bitRate)
{
	short bigBitCount = 2048 / bitRate;
	auto* bigNumber = new unsigned int[bigBitCount];
	std::fill(&bigNumber[0], &bigNumber[bigBitCount - 1], 0);
		
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
	
	if (bitRate >= 4)
	{
		for (short i = 0; i < bigBitCount; i++)
		{
			if (bigNumber[i] != 0)
			{
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

	short zeroCount = 0;

	for (short i = 0; i < bitRate / 4; i++)
	{
		if ((*hexNumber)[i] == '0')
		{
			zeroCount++;
		}
	}
	
	(*hexNumber).erase(0, zeroCount);
	
	return hexNumber;
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

	std::cout << "Input number B: ";
	std::cin >> number;
	unsigned int* numberB = toBigIntConverting(number, bitRate);

	std::cout << "Result of addition: ";
	unsigned int* addition = LongAdd(numberA, numberB, bitRate);
	std::cout << *toHexConverting(addition, bitRate) << std::endl;

	std::cout << "Result of substraction: ";
	unsigned int* substraction = LongSub(numberA, numberB, bitRate);
	std::cout << *toHexConverting(substraction, bitRate) << std::endl;

	return 0;
}