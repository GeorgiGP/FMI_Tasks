#pragma once
#include <iostream>
#include "PairFunc.h"
#include "Set.hpp"
namespace
{
	const uint16_t FUNC_MODE0 = 0;
}
class Func0
{
	Set<int32_t> domain;
	Set<int32_t> range;

public:
	Func0() = default;
	Func0(const int32_t * domain, const int32_t * range, unsigned size);
	Func0(unsigned size);

	bool addElement(int32_t x, int32_t y);
	bool isDefined(int32_t arg) const;

	void deserialize(const char* sourceFileName) const;
	
	PairFunc operator()(int32_t arg) const;

};


