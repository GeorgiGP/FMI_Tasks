#include "Func2.h"
#include <fstream>

Func2::Func2(unsigned size) : targetNums(size) {}


Func2::Func2(const int32_t* domain, unsigned size)
{
	if (!domain)
	{
		throw std::nullptr_t();
	}
	for (size_t i = 0; i < size; i++)
	{
		this->targetNums.addElement(domain[i]);
	}
}

bool Func2::isDefined(int32_t arg) const
{
	return true;
}

bool Func2::addElement(int32_t x)
{
	return targetNums.addElement(x);
}

void Func2::deserialize(const char* sourceFileName) const
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
	uint16_t N = targetNums.size();
	ofs.write((const char*)&N, sizeof(N)); //N
	ofs.write((const char*)&FUNC_MODE2, sizeof(FUNC_MODE2)); //T

	for (size_t i = 0; i < N; i++)
	{
		ofs.write((const char*)&targetNums[i], sizeof(int32_t));
	}

	ofs.close();
}

PairFunc Func2::operator()(int32_t arg) const
{
	for (uint8_t i = 0; i < targetNums.size(); i++)
	{
		if (targetNums[i] == arg)
		{
			return PairFunc(DEFINED, true);
		}
	}
	return PairFunc(DEFINED, false);
}
