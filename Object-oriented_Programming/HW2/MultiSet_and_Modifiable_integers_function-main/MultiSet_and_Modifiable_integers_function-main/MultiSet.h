#pragma once
#include <exception>
#include <iostream>
#include <fstream>
namespace GlobalConsts
{
	const unsigned short BITS_PER_BYTE = 8;
	const unsigned short ONE_BIT = 1;
}
using namespace GlobalConsts;

class MultiSet
{
	unsigned char* buckets = nullptr;
	unsigned countOfBuckets = 0;
	unsigned maxValueInSet = 0;
	uint8_t bitsPerNumber = 0;

	void setCountOfBuckets(int maxValue, uint8_t bitsPerNumber);
	unsigned getBucket(unsigned positionInBuckets) const;


	unsigned getMaxRepeat() const; //unsigned for k > 8 hypotheticly unsigned, otherwise uint8_t (255 max num for k==8)
	unsigned char getMask(unsigned positionInBuckets) const;

	bool getValueFromBit(unsigned positionInBuckets) const;
	void clearBit(unsigned positionInBuckets);

	unsigned mostRightBitPosition(int number) const;
	unsigned mostLeftBitPosition(int number) const;

	void setCountOfNumber(int number, int newCount);

	void copyFrom(const MultiSet& other);
	void free();
public:
	MultiSet() = delete;
	MultiSet(const MultiSet& other);
	MultiSet& operator=(const MultiSet& other);
	~MultiSet();
	explicit MultiSet(int maxValue, int bitsPerNumber); 

	friend MultiSet intersection(const MultiSet& lhs, const MultiSet& rhs);
	friend MultiSet subtraction(const MultiSet& lhs, const MultiSet& rhs);
	friend MultiSet complement(const MultiSet& mult);

	void add(int number);
	void add(int number, int addingCount);
	void remove(int number);
	void remove(int number, int removingCount);
	unsigned count(int number) const;
	void print() const;
	void printMemory() const;
	void serialize(const char* fileName) const;
	void deserialize(const char* fileName);
};


