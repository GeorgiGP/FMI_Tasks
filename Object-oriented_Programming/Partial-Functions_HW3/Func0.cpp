#include "Func0.h"
#include <fstream>

bool Func0::isDefined(int32_t arg) const
{
	for (size_t i = 0; i < domain.size(); i++)
	{
		if (domain[i] == arg)
		{
			return true;
		}
	}
	return false;
}

void Func0::deserialize(const char * destFileName) const
{
	if (!destFileName)
	{
		throw std::nullptr_t();
	}
	std::ofstream ofs(destFileName, std::ios::binary);
	if (!ofs.is_open())
	{
		throw std::exception("Creating file failed.");
	}
	uint16_t N = domain.size();
	ofs.write((const char*)&N, sizeof(N)); //N
	ofs.write((const char*)&FUNC_MODE0, sizeof(FUNC_MODE0)); //T

	for (size_t i = 0; i < N; i++)
	{
		ofs.write((const char*)&domain[i], sizeof(int32_t));
	}

	for (size_t i = 0; i < N; i++)
	{
		ofs.write((const char*)&range[i], sizeof(int32_t));
	}
	ofs.close();
}

bool Func0::addElement(int32_t x, int32_t y)
{
	if (!domain.addElement(x)) //full, not added
	{
		return false;
	}
	range.addElement(y);
	return true;
}


Func0::Func0(const int32_t* domain, const int32_t* range, unsigned size)
{
	if (!domain || !range)
	{
		throw std::nullptr_t();
	}

	for (size_t i = 0; i < size; i++)
	{
		this->domain.addElement(domain[i]);
	}

	for (size_t i = 0; i < size; i++)
	{
		this->range.addElement(range[i]);
	}
}

Func0::Func0(unsigned size) : domain(size), range(size) {}


PairFunc Func0::operator()(int32_t arg) const
{
	for (int8_t i = 0; i < domain.size(); i++)
	{
		if (domain[i] == arg)
		{
			return PairFunc(DEFINED, range[i]);
		}
	}
	return PairFunc();
}
