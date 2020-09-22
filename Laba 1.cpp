#include <iostream> 
#include <cmath>

int* LongAdd()
{
	return nullptr;
}

unsigned long long* bigIntConverting(std::string& number)
{
	auto* bigNumber = new unsigned long long[32];

	int numberSize = number.length();

	if (numberSize % 16 != 0)
	{
		std::string nullStr;

		for (int i = 16 - (numberSize % 16); i > 0; i--)
		{
			nullStr += "0";
			numberSize++;
		}

		number = nullStr + number;
	}

	std::cout << number << std::endl;
	
	int bigNumberSize = numberSize / 16;

	for (int i = bigNumberSize; i > 0; i--)
	{
		unsigned int secondPart = strtoul(number.substr((i * 16) - 8, 8).c_str(), NULL, 16);
		
		unsigned long long firstPart = strtoul(number.substr((i * 16) - 16, 8).c_str(), NULL, 16);
		
		bigNumber[31 - (bigNumberSize - i)] = (firstPart << 32) + secondPart;
	}

	for (int i = 31 - bigNumberSize; i >= 0; i--)
	{
		bigNumber[i] = 0;
	}

	return bigNumber;
}

int main()
{
	std::string number;
	std::cin >> number;

	unsigned long long* numberA = bigIntConverting(number);

	for (int i = 0; i < 32; i++)
	{
		std::cout << numberA[i] << "|";
	}

	return 0;
}