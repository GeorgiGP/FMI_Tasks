#pragma once
#include "PartFunc.h"
#include "CombinedMaxMin.h"

class FuncMax : public CombinedMaxMin
{
private:
	int32_t findDefinedMaxValue(int32_t arg) const;
public:
	FuncMax(unsigned size);

	bool addElement(PartFunc* newElement);
	PairFunc operator()(int32_t arg) const override;
	PartFunc* clone() const override;
};

