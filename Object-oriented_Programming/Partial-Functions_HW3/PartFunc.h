#pragma once
#include <iostream>
#include "PairFunc.h"

class PartFunc
{
private:
public:
	virtual bool isDefined(int32_t arg) const = 0;
	virtual PairFunc operator()(int32_t arg) const = 0;
	virtual PartFunc* clone() const = 0;
	virtual ~PartFunc() = default;
};

