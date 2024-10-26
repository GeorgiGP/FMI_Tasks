#pragma once
#include <iostream>
#include "PairFunc.h"
#include "Set.hpp"
namespace
{
	const uint16_t FUNC_MODE1 = 1;
}
class Func1
{
	Set<int32_t> deletedNums;

public:
	Func1() = default;
	Func1(unsigned size);
	Func1(const int32_t* domain, unsigned size);

	bool isDefined(int32_t arg) const;
	bool addElement(int32_t x);

	void deserialize(const char* sourceFileName) const;

	PairFunc operator()(int32_t arg) const;
};

