struct bigInteger
{
	long long size;
	std::string hexString;

	unsigned int *value;

	bigInteger();

	bigInteger(const long long &size);

	bigInteger(const long long &size, const std::string &hexString);

	~bigInteger();
};

void showBigInteger(std::shared_ptr<bigInteger> number, std::string numberName = "BigNumber: ");

void ZeroEraser(std::shared_ptr<bigInteger> number);
void SmallFix(std::shared_ptr<bigInteger> number);

std::shared_ptr<bigInteger> toBigIntConverting(const std::string &number, const int &bitRate);
void toHexConverting(std::shared_ptr<bigInteger> bigNumber, int bitRate);

bool LongComp(std::shared_ptr<bigInteger> numberA, std::shared_ptr<bigInteger> numberB, bool severe);
std::shared_ptr<bigInteger> LongShiftBitsToHigh(std::shared_ptr<bigInteger> number, unsigned long long shift);

std::shared_ptr<bigInteger> LongAdd(std::shared_ptr<bigInteger> numberA, std::shared_ptr<bigInteger> numberB, int bitRate, bool text = true);
std::shared_ptr<bigInteger> LongSub(std::shared_ptr<bigInteger> numberA, std::shared_ptr<bigInteger> numberB, int bitRate, bool text = true);
std::shared_ptr<bigInteger> LongMul(std::shared_ptr<bigInteger> numberA, std::shared_ptr<bigInteger> numberB, int bitRate, bool text = true);
std::shared_ptr<std::pair<std::shared_ptr<bigInteger>, std::shared_ptr<bigInteger>>> LongDiv(std::shared_ptr<bigInteger> dividend, std::shared_ptr<bigInteger> divisor, int bitRate, bool onlyWhole = false);
std::shared_ptr<bigInteger> LongPow(std::shared_ptr<bigInteger> numberA, std::shared_ptr<bigInteger> numberB, int bitRate);