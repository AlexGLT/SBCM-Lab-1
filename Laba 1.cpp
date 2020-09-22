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

		for (int i = 16 - numberSize; i > 0; i--)
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
		unsigned long long temp = 0;

		for (int j = 16; j > 0; j--)
		{
			short grade = static_cast<short>(number[(i * j) - 1]);
			std::cout << i << "; " << j << std::endl;
			if (grade < 58)
			{
				unsigned long long step = powl(16, (16 - j));
				temp += (grade - 48) * step;
			}
			else
			{
				unsigned long long step = powl(16, (16 - j));
				temp += (grade - 55) * powl(16, (16 - j));
			}

			std::cout << temp << std::endl;
		}

		bigNumber[31 - (bigNumberSize - i)] = temp;;
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