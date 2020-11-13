#include <iostream>
#include <string>
#include "LongArithmetic.h"

int main()
{
	int bitRate;
	std::string number;

	std::cout << "Input numbers bit rate: ";
	std::cin >> bitRate;

	std::cout << "Input number A: ";
	std::cin >> number;
	auto* numberA = toBigIntConverting(number, bitRate);

	std::cout << "Input number B: ";
	std::cin >> number;
	auto* numberB = toBigIntConverting(number, bitRate);

	std::cout << "Result of addition: ";
	auto* addition = LongAdd(numberA, numberB, bitRate);
	std::cout << addition->hexString << std::endl;

	std::cout << "Result of subtraction: ";
	auto* subtraction = LongSub(numberA, numberB, bitRate);
	std::cout << subtraction->hexString << std::endl;

	std::cout << "Result of multiplication: ";
	auto* multiplication = LongMul(numberA, numberB, bitRate);
	std::cout << multiplication->hexString << std::endl;

	auto** divisionResult = LongDiv(numberA, numberB, bitRate);

	std::cout << "Whole part from division: ";
	auto* wholePart = divisionResult[0];
	std::cout << wholePart->hexString << std::endl;

	std::cout << "Remainder from division: ";
	auto* remainder = divisionResult[1];
	std::cout << remainder->hexString << std::endl;

	std::cout << "Do you want to A^B?" << std::endl;
	char choice;
	std::cin >> choice;

	if (choice == 'y')
	{
		std::cout << "Result of power: ";
		auto* power = LongPower(numberA, numberB, bitRate);
		std::cout << power->hexString << std::endl;
	}

	std::cout << "-------TESTS------" << std::endl;

	std::cout << "1. Distributivity" << std::endl;

	std::cout << "1.1. (A + B) * A = ";
	auto* firstPropertie = LongMul(addition, numberA, bitRate);
	std::cout << firstPropertie->hexString << std::endl;

	std::cout << "1.2. A * (A + B) = ";
	auto* secondPropertie = LongMul(numberA, addition, bitRate);
	std::cout << secondPropertie->hexString << std::endl;

	std::cout << "1.3. A * A + A * B = ";
	auto* firstMultiplication = LongMul(numberA, numberA, bitRate);
	auto* thirdPropertie = LongAdd(firstMultiplication, multiplication, bitRate);
	std::cout << thirdPropertie->hexString << std::endl;
 
	std::cout << "2. Equivalence of multiplication by addition" << std::endl;

	int randomN = rand() % 100 + 2;
	std::cout << "n = " << randomN << std::endl;
	std::string s = std::to_string(randomN);
	randomN = strtoul(s.substr(0, s.length()).c_str(), nullptr, 16);


	long long randomNSize;
	auto* bigN = toBigIntConverting(s, bitRate);

	std::cout << "2.1. n * A = ";
	auto* nAMul = LongMul(numberA, bigN, bitRate);
	std::cout << nAMul->hexString << std::endl;

	auto* bigSum = numberA;

	for (int i = 1; i <= randomN - 1; i++)
	{
		bigSum = LongAdd(bigSum, numberA, bitRate, false);


	}

	std::cout << "2.2. A + A + ... + A = ";
	std::cout << bigSum->hexString << std::endl;
	return 0;
}