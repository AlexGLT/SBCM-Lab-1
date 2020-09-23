#include <iostream> 
#include <cmath>

unsigned int* LongAdd(const unsigned int *numberA, const unsigned int *numberB)
{
	return nullptr;
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
				bigNumber[(bigBitCount - (numberSize - (i - 1)) * bitForHexCount) - 1 + j] =
					(numberHexBit & static_cast<short>(pow(2, (bitForHexCount - j)))) >> (bitForHexCount - j);
			}				
		}
	}

	return bigNumber;
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
	
	/*std::cout << "Input number B: ";
	std::cin >> number;
	unsigned int* numberB = toBigIntConverting(number, bitRate);*/

	
	for (int i = 0; i < (2048 / bitRate); i++)
	{
		std::cout << numberA[i];
	}

	return 0;
}