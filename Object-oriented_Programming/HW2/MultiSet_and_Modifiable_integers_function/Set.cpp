#include "Set.h"

void Set::add(int number)
{
	if (SHORT_MIN > number || number > SHORT_MAX)
	{
		throw std::invalid_argument("Number should be between SHORT_MIN and SHORT_MAX");
	}
	setAppear(number, true);
}

void Set::remove(int number)
{
	if (SHORT_MIN > number || number > SHORT_MAX)
	{
		throw std::invalid_argument("Number should be between SHORT_MIN and SHORT_MAX");
	}
	setAppear(number, false);
}

bool Set::isEmpty() const
{
	for (size_t i = 0; i < BUCKETS_ISVALID; i++)
	{
		if (this->buckets[i])
		{
			return false;
		}
	}
	return true;
}

void Set::clear()
{
	for (size_t i = 0; i < BUCKETS_ISVALID; i++)
	{
		buckets[i] = 0;
	}
}

unsigned Set::getBucket(int number) const
{
	return convert(number) / BITS_IN_BYTE;
}

unsigned Set::getPosition(int number) const
{
	return convert(number) % BITS_IN_BYTE;
}

unsigned char Set::getMask(int number) const
{
	if (number < SHORT_MIN || number > SHORT_MAX)
	{
		return 0;
	}
	return (1 << (7 - getPosition(number)));
}

bool Set::isAppeared(int number) const
{
	if (SHORT_MIN > number || number > SHORT_MAX)
	{
		throw std::invalid_argument("Number should be between SHORT_MIN and SHORT_MAX");
	}
	return buckets[getBucket(number)] & getMask(number);
}

short Set::lowestDefined() const
{
	for (int i = SHORT_MIN; i <= SHORT_MAX; i++)
	{
		if (this->isAppeared(i))
		{
			return i;
		}
	}
	return SHORT_MAX;
}

void Set::setAppear(int number, bool to)
{
	if (SHORT_MIN > number || number > SHORT_MAX)
	{
		throw std::invalid_argument("Number should be between SHORT_MIN and SHORT_MAX");
	}
	if (isAppeared(number) == to)
	{
		return;
	}
	buckets[getBucket(number)] ^= getMask(number);
}

bool operator==(const Set& lhs, const Set& rhs)
{
	for (size_t i = 0; i < BUCKETS_ISVALID; i++)
	{
		if (lhs.buckets[i] != rhs.buckets[i])
		{
			return false;
		}
	}
	return true;
}

bool operator!=(const Set& lhs, const Set& rhs)
{
	return !(lhs == rhs);
}
