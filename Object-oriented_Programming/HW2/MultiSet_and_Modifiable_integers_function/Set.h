#pragma once
#include <exception>
#include <iostream>
namespace Consts
{
	constexpr short SHORT_MIN = 1 << 15;
	constexpr short SHORT_MAX = SHORT_MIN - 1;
	constexpr unsigned int COUNT_NUMBERS_SHORT = 1 << 16;
	const uint8_t BITS_IN_BYTE = 8;
	constexpr unsigned BUCKETS_ISVALID = COUNT_NUMBERS_SHORT / BITS_IN_BYTE;
}
using namespace Consts;
#define convert(x) (unsigned)(x-SHORT_MIN)

class Set
{
	unsigned char buckets[BUCKETS_ISVALID]{ 0 };

	unsigned getBucket(int number) const;
	unsigned getPosition(int number) const;
	unsigned char getMask(int number) const;
	void setAppear(int number, bool to);

	friend class ModifiableIntegersFunction;
public:
	bool isAppeared(int number) const;
	short lowestDefined() const;
	void add(int number);
	void remove(int number);
	bool isEmpty() const;
	void clear();

	friend bool operator==(const Set& lhs, const Set& rhs);
	friend bool operator!=(const Set& lhs, const Set& rhs);
};
