#include "MultiSet.h"
#define MIN(x,y) (x<=y)?(x):(y)
#define MAX(x,y) (x>=y)?(x):(y)
namespace Functions
{
	bool bitAtPosition(uint32_t number, uint16_t position) //position is backwards
	{
		return number & (1 << position);
	}
	void printIntInBinary(uint32_t number)
	{
		uint8_t bitsInInt = sizeof(int)*BITS_PER_BYTE;
		for (int8_t i = bitsInInt - 1; i >= 0; i--)
		{
			std::cout << bitAtPosition(number, i);
		}
	}
	void printCharInBinary(uint8_t number)
	{
		for (int8_t i = BITS_PER_BYTE - 1 ; i >= 0; i--)
		{
			std::cout << bitAtPosition(number, i);
		}
	}
}
using namespace Functions;
void MultiSet::setCountOfBuckets(int maxValue, uint8_t bitsPerNumber)
{
	this->countOfBuckets = (maxValue + 1)*bitsPerNumber / BITS_PER_BYTE + //+1 for Zero; BitsNeeded/8 == BytesNeeded;
		(((maxValue + 1)*bitsPerNumber % BITS_PER_BYTE)?(1):(0));//+ 1 or +0 for rounding up
}

unsigned MultiSet::getBucket(unsigned positionInBuckets) const
{
	return positionInBuckets / BITS_PER_BYTE;
}

unsigned MultiSet::getMaxRepeat() const
{
	return (1 << bitsPerNumber) - 1;
}

bool MultiSet::getValueFromBit(unsigned positionInBuckets) const
{
	unsigned char bucket = buckets[getBucket(positionInBuckets)];
	short LeftShift = positionInBuckets % BITS_PER_BYTE; // ---*---- 3 LeftShifts to clear bits from left
	short RightShift = BITS_PER_BYTE - 1; // *------- //to Right to return the Bit only, nothing else
	bucket <<= LeftShift;
	bucket >>= RightShift;
	return bucket; // returning the bit
}

void MultiSet::clearBit(unsigned positionInBuckets)
{
	if (getValueFromBit(positionInBuckets))
	{
		buckets[getBucket(positionInBuckets)] ^= getMask(positionInBuckets);
	}
}

unsigned MultiSet::mostRightBitPosition(int number) const
{
	return (number + 1)*bitsPerNumber - 1; // +1 including Zero Value; -1 because position starts from 0, not from 1
}

unsigned MultiSet::mostLeftBitPosition(int number) const
{
	return mostRightBitPosition(number) - (bitsPerNumber - 1);
}

unsigned char MultiSet::getMask(unsigned positionInBuckets) const
{
	return 1 << (7 - (positionInBuckets % BITS_PER_BYTE)); // 7 - x, shifting is backwards
}

void MultiSet::setCountOfNumber(int number, int newCount)
{
	if (number < 0 || number > maxValueInSet || newCount < 0 || newCount > getMaxRepeat())
	{
		return;
	}
	
	int mostLeftPosition = mostLeftBitPosition(number);
	int mostRightPosition = mostRightBitPosition(number);
	for (int i = mostRightPosition; i >= mostLeftPosition; --i)
	{
		clearBit(i);
	}

	for (int i = mostRightPosition; i >= mostLeftPosition; --i, newCount >>= 1)
	{
		if (newCount == 0)
		{
			break;
		}
		unsigned char mask = getMask(i);
		buckets[getBucket(i)] ^= mask*(newCount%2);
	}
}

void MultiSet::copyFrom(const MultiSet & other)
{
	free();
	countOfBuckets = other.countOfBuckets;
	bitsPerNumber = other.bitsPerNumber;
	maxValueInSet = other.maxValueInSet;
	buckets = new unsigned char[other.countOfBuckets]{};
	for (size_t i = 0; i < countOfBuckets; i++)
	{
		this->buckets[i] = other.buckets[i];
	}
}

void MultiSet::free()
{
	delete[] buckets;
	buckets = nullptr;
	countOfBuckets = 0;
	bitsPerNumber = 0;
	maxValueInSet = 0;
}

MultiSet::MultiSet(const MultiSet & other)
{
	copyFrom(other);
}

MultiSet & MultiSet::operator=(const MultiSet & other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

MultiSet::~MultiSet()
{
	free();
}

MultiSet::MultiSet(int maxValue, int bitsPerNumber)
{//int bitsPerNumber, type int to catch invalid values and not overflowing
	if (maxValue < 0 || bitsPerNumber < ONE_BIT || bitsPerNumber > BITS_PER_BYTE)
	{
		throw std::invalid_argument("In constructor arguments must be: 0<=maxValue and 1<=bitsPerNumber<=8");
	}
	this->bitsPerNumber = bitsPerNumber;
	this->maxValueInSet = maxValue;
	setCountOfBuckets(maxValue, bitsPerNumber);
	buckets = new unsigned char[this->countOfBuckets]{};
}

MultiSet intersection(const MultiSet& lhs, const MultiSet& rhs)
{
	if (&lhs == &rhs)
	{
		return lhs;
	}
	unsigned resultMaxValue = MIN(lhs.maxValueInSet, rhs.maxValueInSet);
	uint8_t resultBitsPerNumber = MIN(lhs.bitsPerNumber, rhs.bitsPerNumber);
	MultiSet result(resultMaxValue, resultBitsPerNumber);

	for (size_t i = 0; i <= resultMaxValue; i++)
	{
		result.setCountOfNumber(i, MIN(lhs.count(i), rhs.count(i)));
	}
	return result;
}

MultiSet subtraction(const MultiSet & lhs, const MultiSet & rhs)
{
	MultiSet result = lhs;

	for (size_t i = 0; i <= result.maxValueInSet; i++)
	{
		result.setCountOfNumber(i, (lhs.count(i) > rhs.count(i))?(lhs.count(i) - rhs.count(i)):(0));
	}
	return result;
}

MultiSet complement(const MultiSet& mult)
{
	MultiSet result = mult;
	for (size_t i = 0; i < result.countOfBuckets; i++)
	{
		result.buckets[i] = ~(result.buckets[i]);
	}
	return result;
}

void MultiSet::add(int number)
{
	if (number > maxValueInSet || number < 0 || count(number) == getMaxRepeat())
	{
		return;
	}
	setCountOfNumber(number, count(number) + 1);
}

void MultiSet::add(int number, int addingCount)
{
	if (number > maxValueInSet || number < 0 || addingCount <= 0)
	{
		return;
	}
	if (count(number) + addingCount >= getMaxRepeat())
	{
		setCountOfNumber(number, getMaxRepeat());
	}
	else
	{
		setCountOfNumber(number, count(number) + addingCount);
	}
}

void MultiSet::remove(int number)
{
	if (number > maxValueInSet || number < 0 || count(number) == 0)
	{
		return;
	}
	setCountOfNumber(number, count(number) - 1);
}

void MultiSet::remove(int number, int removingCount)
{
	if (number > maxValueInSet || number < 0 || removingCount <= 0)
	{
		return;
	}
	if (count(number) <= removingCount)
	{
		setCountOfNumber(number, 0);
	}
	else
	{
		setCountOfNumber(number, count(number) - removingCount);
	}
}

unsigned MultiSet::count(int number) const
{
	if (number < 0 || number > maxValueInSet)
	{
		return 0;
	}
	unsigned countResult = 0;
	
	int mostLeftPosition = mostLeftBitPosition(number);
	int mostRightPosition = mostRightBitPosition(number);
	for (int i = mostRightPosition, multiplier = 1; i >= mostLeftPosition; --i, multiplier <<= 1)
	{
		countResult += getValueFromBit(i)*multiplier;
	}
	return countResult;
}

void MultiSet::print() const
{
	std::cout << "Current MultiSet has elements: ";
	for (size_t i = 0; i <= maxValueInSet; i++)
	{
		unsigned countOfNumber = count(i);
		for (size_t j = 0; j < countOfNumber; j++)
		{
			std::cout << i << ' ';
		}
	}
	std::cout << std::endl;
}

void MultiSet::printMemory() const
{
	std::cout << std::endl << "Current MultiSet in memory: " << std::endl << "Max Value: ";
	printIntInBinary(maxValueInSet);
	std::cout << std::endl << "Count of buckets in set: "; 
	printIntInBinary(countOfBuckets);
	std::cout << std::endl << "Count of bits needed for every number in set: ";
	printCharInBinary(bitsPerNumber);
	std::cout << std::endl << std::endl << "Set: " << std::endl;
	for (size_t i = 0; i < countOfBuckets; i++)
	{
		printCharInBinary(buckets[i]);
		std::cout << ' ';
	}
	std::cout << std::endl;
}

void MultiSet::serialize(const char * fileName) const
{
	if (!fileName)
	{
		throw std::exception("Nullptr");
	}
	std::ofstream ofs(fileName, std::ios::out | std::ios::binary);
	ofs.write((const char*)&maxValueInSet, sizeof(maxValueInSet));
	ofs.write((const char*)&bitsPerNumber, sizeof(bitsPerNumber));
	ofs.write((const char*)buckets, sizeof(unsigned char)* countOfBuckets);

	ofs.close();
}

void MultiSet::deserialize(const char * fileName) 
{
	if (!fileName)
	{
		throw std::exception("Nullptr");
	}
	std::ifstream ifs(fileName, std::ios::in | std::ios::binary);
	free();
	ifs.read((char*)&maxValueInSet, sizeof(maxValueInSet));
	ifs.read((char*)&bitsPerNumber, sizeof(bitsPerNumber));
	this->setCountOfBuckets(maxValueInSet, bitsPerNumber);
	buckets = new unsigned char[countOfBuckets];
	ifs.read((char*)buckets, sizeof(unsigned char)* countOfBuckets);
	ifs.close();
}

