#include <iostream>
#include <climits>
#include <cmath>
#include <string>
#include <vector>


// Код для виводу вмісту масиву

// 	std::cout << "Whole part: " << std::endl;
// 
// 	std::cout << "|";
// 
// 	for (int i = 0; i < wholePartBitLength; i++)
// 	{
// 		std::cout << optimizedWholePart[i] << "|";
// 	}
// 
// 	std::cout << std::endl;


unsigned int* ZeroEraser(unsigned int* number, long long& numberSize)
{
	unsigned int zeroNumber = 0;

	for (int i = 0; i < numberSize; i++)
	{
		if (number[i] == 0)
		{
			zeroNumber++;
		}
		else
		{
			break;
		}
	}

	if (zeroNumber == 0)
	{
		return number;
	}
	else
	{
		auto* optimizedNumber = new unsigned int[numberSize - zeroNumber];
		std::copy(number + zeroNumber, number + numberSize, optimizedNumber);

		numberSize -= zeroNumber;

		return optimizedNumber;
	}
}

unsigned int* toBigIntConverting(std::string &number, const int &bitRate, long long &bigNumSize)
{
	unsigned int* bigNumber;
	int numberSize = number.length();

	if (bitRate >= 4)
	{
		int hexBitCount = (bitRate / 4);

		if (numberSize % hexBitCount != 0)
		{
			std::string nullStr;

			for (int i = hexBitCount - (numberSize % hexBitCount); i > 0; i--)
			{
				nullStr += "0";
				numberSize++;
			}

			number = nullStr + number;
		}

		int bigBitCount = numberSize / hexBitCount;

		bigNumber = new unsigned int[bigBitCount];

		for (int i = bigBitCount - 1; i >= 0; i--)
		{
			bigNumber[i] = strtoul(number.substr(i * hexBitCount, hexBitCount).c_str(), nullptr, 16);
		}

		bigNumSize = bigBitCount;
	}
	else
	{
		//count of big bits for one hex cipher
		int bitForHexCount = (4 >> (bitRate - 1));

		bigNumSize = numberSize * bitForHexCount;

		bigNumber = new unsigned int[bigNumSize];

		for (int i = numberSize - 1; i >= 0; i--)
		{
			int numberHexBit = strtoul(number.substr(i, 1).c_str(), nullptr, 16);

			for (int j = bitForHexCount - 1; j >= 0; j--)
			{
				bigNumber[i * bitForHexCount + j] = (numberHexBit >> (3 - j)) & 1;
			}
		}

		bigNumber = ZeroEraser(bigNumber, bigNumSize);
	}

	return bigNumber;
}

std::string* toHexConverting(unsigned int* bigNumber, int bitRate, long long &bigNumSize)
{
	auto* hexNumber = new std::string;

	for (long long i = 0; i < bigNumSize; i++)
	{
		int binSumm = 0;
		std::vector<int> smallNumber;

		if (bitRate >= 4)
		{
			for (int j = 0; j < bitRate; j += 4)
			{
				smallNumber.push_back((bigNumber[i] >> ((bitRate - 4) - j)) & 15);
			}
		}
		else
		{
			for (int j = 0; j < 4; j++)
			{
				binSumm += bigNumber[i + j] << (3 - j);
			}


			// Перескочити 4 значення масиву, але добавляємо 3, оскільки в ітераторі вже є i++
			i += 3;

			smallNumber.push_back(binSumm);
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

			*hexNumber += static_cast<char>(cipher);
		}

		smallNumber.clear();
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

bool LongComp(const unsigned int* numberA, const unsigned int* numberB, const long long &numberASize, const long long &numberBSize, bool severe)
{
	if (numberASize < numberBSize) return false;
	if (numberASize > numberBSize) return true;

	for (long long i = 0; i < numberASize; i++)
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
}

unsigned int* LongAdd(const unsigned int *numberA, const unsigned int *numberB, int bitRate, long long numberASize, long long numberBSize, long long& numberCSize, bool mul = false)
{
	long long smallerNumberSize;
	const unsigned int* smallerNumber;
	const unsigned int* biggerNumber;

	if (numberASize >= numberBSize)
	{
		numberCSize = numberASize + 1;
		smallerNumberSize = numberBSize;
		
		biggerNumber = numberA;
		smallerNumber = numberB;
	}
	else
	{
		numberCSize = numberBSize + 1;
		smallerNumberSize = numberASize;

		biggerNumber = numberB;
		smallerNumber = numberA;
	}

	unsigned long long module = (ULLONG_MAX >> (64 - bitRate));
	
	auto* numberC = new unsigned int[numberCSize];

	unsigned int carry = 0;

	int difference = abs(numberASize - numberBSize);

	for (long long i = smallerNumberSize - 1; i >= 0; i--)
	{
		unsigned long long summ = static_cast<unsigned long long>(smallerNumber[i]) + biggerNumber[i + difference] + carry;

		numberC[i + difference + 1] = summ & module;
		carry = summ >> bitRate;
	}
	for (long long i = difference - 1; i >= 0; i--)
	{
		unsigned long long summ = static_cast<unsigned long long>(biggerNumber[i]) +  carry;

		numberC[i + 1] = summ & module;

		carry = summ >> bitRate;
	}

	numberC[0] = carry;

	auto* optimizedNumberC = ZeroEraser(numberC, numberCSize);
	
	if (optimizedNumberC != numberC && !mul)
	{
		delete[] numberC;
	}

	return optimizedNumberC;
}

unsigned int* LongSub(const unsigned int* numberA, const unsigned int* numberB, int bitRate, long long numberASize, long long numberBSize, long long& numberCSize, bool div = false)
{
	long long module = static_cast<unsigned long long>(1) << bitRate;

	if (numberASize < numberBSize)
	{
		return new unsigned int[1]{ 0 };
	}

	numberCSize = numberASize;

	auto* numberC = new unsigned int[numberCSize];
	int borrow = 0;

	int difference = abs(numberASize - numberBSize);
	for (long long i = numberBSize - 1; i >= 0; i--)
	{
		long long substraction = static_cast<long long>(numberA[i + difference]) - static_cast<long long>(numberB[i]) - borrow;

		if (substraction >= 0)
		{
			numberC[i + difference] = substraction;
			borrow = 0;
		}
		else
		{
			numberC[i + difference] = module + substraction;
			borrow = 1;
		}
	}
	for (long long i = difference - 1; i >= 0; i--)
	{
		long long substraction = numberA[i] - borrow;

		if (substraction >= 0)
		{
			numberC[i] = substraction;
			borrow = 0;
		}
		else
		{
			std::cout << module + substraction << std::endl;
			numberC[i] = module + substraction;
			borrow = 1;
		}
	}

	if (borrow != 0)
	{
		return new unsigned int[1]{ 0 };
	}

	unsigned int* optimizedNumberC;

	if (bitRate >= 4)
	{
		optimizedNumberC = ZeroEraser(numberC, numberCSize);

		if (optimizedNumberC != numberC && !div)
		{
			delete[] numberC;
		}
	}
	else
	{
		optimizedNumberC = numberC;
	}

	return optimizedNumberC;
}

unsigned int* LongMul(const unsigned int* numberA, const unsigned int* numberB, int bitRate, long long numberASize, long long numberBSize, long long& numberCSize)
{
	unsigned long long module = (ULLONG_MAX >> (64 - bitRate));
	
	numberCSize = numberASize * numberBSize;
	auto* numberC = new unsigned int[numberCSize];
	std::fill(&numberC[0], &numberC[numberCSize], 0);

	for (long long i = numberBSize - 1; i >= 0; i--)
	{
		unsigned long long carry = 0;
		long long interMulNumberSize = numberASize + (numberBSize - 1 - i) + 1;
		auto* intermediateMul = new unsigned int[interMulNumberSize];
		std::fill(&intermediateMul[0], &intermediateMul[numberASize + (numberBSize - 1 - i) + 1], 0);

		for (long long j = numberASize - 1; j >= 0; j--)
		{
			unsigned long long temp = static_cast<unsigned long long>(numberA[j]) * numberB[i] + carry;
			intermediateMul[j + 1] = temp & module;
			carry = temp >> bitRate;

			if (j == 0)
			{
				intermediateMul[j] = carry;
			}
		}

		numberC = LongAdd(numberC, intermediateMul, bitRate, numberCSize, interMulNumberSize, numberCSize, true);
		
		if (numberC != intermediateMul)
		{
			delete[] intermediateMul;
		}
	}

	return numberC;
}

unsigned int* LongShiftBitsToHigh(const unsigned int* number, unsigned long long shift, const long long& numberSize, long long& shiftedNumberSize)
{
	shiftedNumberSize = numberSize + shift;

	auto* highNumber = new unsigned int[shiftedNumberSize];

	std::fill(&highNumber[0], &highNumber[shiftedNumberSize], 0);

	for (long long i = 0; i < numberSize; i++)
	{
		highNumber[i] = number[i];
	}

	auto* optimizedNumberC = ZeroEraser(highNumber, shiftedNumberSize);

	return optimizedNumberC;
}

unsigned int* SmallFix(const unsigned int* number, long long& numberSize)
{
	int fix = 4 - (numberSize % 4);

	auto* fixNumber = new unsigned int[numberSize + fix];
	std::fill(&fixNumber[0], &fixNumber[fix], 0);
	std::copy(number, number + numberSize, fixNumber + fix);

	numberSize += fix;

	return fixNumber;
}

unsigned int** LongDiv(std::string& dividend, std::string& divisor, int bitRate, long long& wholePartBitLength, long long& remainderBitLength)
{
	long long dividendBitLength, divisorBitLength;

	unsigned int* numberA = toBigIntConverting(dividend, 1, dividendBitLength);
	unsigned int* numberB = toBigIntConverting(divisor, 1, divisorBitLength);

	auto* divisionRemainder = numberA;
	remainderBitLength = dividendBitLength;

	wholePartBitLength = dividendBitLength;
	auto* wholePart = new unsigned int[wholePartBitLength];
	std::fill(&wholePart[0], &wholePart[dividendBitLength], 0);

	while (LongComp(divisionRemainder, numberB, remainderBitLength, divisorBitLength, false))
	{
		long long iterateLength = remainderBitLength;

		long long maxDivisorBitLength;
		unsigned int* maxDivisor = LongShiftBitsToHigh(numberB, (iterateLength - divisorBitLength), divisorBitLength, maxDivisorBitLength);

		if (LongComp(maxDivisor, divisionRemainder, maxDivisorBitLength, remainderBitLength, true))
		{
			iterateLength -= 1;

			maxDivisor = LongShiftBitsToHigh(numberB, (iterateLength - divisorBitLength), divisorBitLength, maxDivisorBitLength);
		}

		divisionRemainder = LongSub(divisionRemainder, maxDivisor, 1, remainderBitLength, maxDivisorBitLength, remainderBitLength, true);

 		divisionRemainder = ZeroEraser(divisionRemainder, remainderBitLength);

		wholePart[(wholePartBitLength - 1) - (iterateLength - divisorBitLength)] = 1;
	}

	auto* optimizedWholePart = ZeroEraser(wholePart, wholePartBitLength);

	if (wholePartBitLength % 4 != 0)
	{
		optimizedWholePart = SmallFix(optimizedWholePart, wholePartBitLength);
	}

	if (remainderBitLength % 4 != 0)
	{
		divisionRemainder = SmallFix(divisionRemainder, remainderBitLength);
	}

	unsigned int** answer = new unsigned int* [2]{ optimizedWholePart, divisionRemainder };

	return answer;
}

unsigned int* LongPower(const unsigned int* numberA, const unsigned int* numberB, int bitRate, long long numberASize, long long numberBBitSize, long long& numberCSize)
{
	numberCSize = 1;
	auto* numberC = new unsigned int[numberCSize]{1};

	for (long long i = 0; i < numberBBitSize; i++)
	{
		if (numberB[i] == 1)
		{
			numberC = LongMul(numberC, numberA, bitRate, numberCSize, numberASize, numberCSize);
		}

		if (i != numberBBitSize - 1)
		{
			numberC = LongMul(numberC, numberC, bitRate, numberCSize, numberCSize, numberCSize);
		}	
	}

	return numberC;
}
	
int main()
{
	int bitRate;
	std::string number;

	std::cout << "Input numbers bit rate: ";
	std::cin >> bitRate;

	std::cout << "Input number A: ";
	std::cin >> number;
	long long numberASize;
	std::string dividend(number);
	
	unsigned int* numberA = toBigIntConverting(number, bitRate, numberASize);

	std::cout << "Input number B: ";
	std::cin >> number;
	long long numberBSize;
	std::string divisor(number);
	
	unsigned int* numberB = toBigIntConverting(number, bitRate, numberBSize);

	std::cout << "Result of addition: ";
	long long additionSize;
	unsigned int* addition = LongAdd(numberA, numberB, bitRate, numberASize, numberBSize, additionSize);
	std::cout << *toHexConverting(addition, bitRate, additionSize) << std::endl;

	std::cout << "Result of substraction: ";
	long long substractionSize;
	unsigned int* substraction = LongSub(numberA, numberB, bitRate, numberASize, numberBSize, substractionSize);
	std::cout << *toHexConverting(substraction, bitRate, substractionSize) << std::endl;

	std::cout << "Result of multiplication: ";
	long long multiplicationSize;
	unsigned int* multiplication = LongMul(numberA, numberB, bitRate, numberASize, numberBSize, multiplicationSize);
	std::cout << *toHexConverting(multiplication, bitRate, multiplicationSize) << std::endl;

	long long wholePartSize, remainderSize;
	unsigned int** divisionResult = LongDiv(dividend, divisor, bitRate, wholePartSize, remainderSize);
	unsigned int* division = divisionResult[0];
	unsigned int* remainder = divisionResult[1];

	std::cout << "Whole part from division: ";
	std::cout << *toHexConverting(division, 1, wholePartSize) << std::endl;
	std::cout << "Remainder from division: ";
	std::cout << *toHexConverting(remainder, 1, remainderSize) << std::endl;
	
	std::cout << "Do you want to A^B?" << std::endl;
	char choice;
	std::cin >> choice;

	if (choice == 'y')
	{
		std::cout << "Result of power: ";
		long long powerSize;

		long long numberBBitSize;
		unsigned int* bitNumberB = toBigIntConverting(number, 1, numberBBitSize);

		unsigned int* power = LongPower(numberA, bitNumberB, bitRate, numberASize, numberBBitSize, powerSize);
		std::cout << *toHexConverting(power, bitRate, powerSize) << std::endl;
	}

	return 0;
}