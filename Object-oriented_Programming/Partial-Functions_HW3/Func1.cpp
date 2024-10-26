#include "Func1.h"
#include <fstream>


Func1::Func1(unsigned size) : deletedNums(size) {}


Func1::Func1(const int32_t* domain, unsigned size)
{
	if (!domain)
	{
		throw std::nullptr_t();
	}
	for (size_t i = 0; i < size; i++)
	{
		this->deletedNums.addElement(domain[i]);
	}
}

bool Func1::isDefined(int32_t arg) const
{
	for (size_t i = 0; i < deletedNums.size(); i++)
	{
		if (deletedNums[i] == arg)
		{
			return false;
		}
	}
	return true;
}

bool Func1::addElement(int32_t x)
{
	return deletedNums.addElement(x);
}

void Func1::deserialize(const char* sourceFileName) const
{
	if (!sourceFileName)
	{
		throw std::nullptr_t();
	}

	std::ofstream ofs(sourceFileName, std::ios::binary);
	if (!ofs.is_open())
	{
		throw std::exception("Creating file failed.");
	}
	uint16_t N = deletedNums.size();
	ofs.write((const char*)&N, sizeof(N)); //N
	ofs.write((const char*)&FUNC_MODE1, sizeof(FUNC_MODE1)); //T

	for (size_t i = 0; i < N; i++)
	{
		ofs.write((const char*)&deletedNums[i], sizeof(int32_t));
	}

	ofs.close();
}

PairFunc Func1::operator()(int32_t arg) const
{
	for (uint8_t i = 0; i < deletedNums.size(); i++)
	{
		if (deletedNums[i] == arg)
		{
			return PairFunc();
		}
	}
	return PairFunc(DEFINED, arg);
}
