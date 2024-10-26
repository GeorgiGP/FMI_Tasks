#include "PairFunc.h"

PairFunc::PairFunc() : Pair<bool, int32_t>(NOT_DEFINED, 0) {}


PairFunc::PairFunc(bool isDefined, int32_t value)
{
	setFirst(isDefined);
	setSecond(value);
}

bool PairFunc::isDefined() const
{
	return getFirst();
}

int32_t PairFunc::value() const
{
	return getSecond();
}

PairFunc & PairFunc::operator=(const PairFunc & other)
{
	setFirst(other.getFirst());
	setSecond(other.getSecond());
	return *this;
}

std::ostream & operator<<(std::ostream & os, const PairFunc & func)
{
	if (func.isDefined())
	{
		os << func.value();
	}
	else
	{
		os << "Not defined";
	}
	return os;
}
