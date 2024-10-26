#pragma once
#include <iostream>
#include "PairFunc.h"
#include "Set.hpp"
namespace
{
	const uint16_t FUNC_MODE2 = 2;
}
class Func2
{
	Set<int32_t> targetNums;

public:
	Func2() = default;
	Func2(unsigned size);
	Func2(const int32_t* domain, unsigned size);

	bool isDefined(int32_t arg) const;
	bool addElement(int32_t x);
	void deserialize(const char* sourceFileName) const;

	PairFunc operator()(int32_t arg) const;
};

