#pragma once
#include "PartFunc.h"
#include "CombinedMaxMin.h"

class FuncMin : public CombinedMaxMin
{
private:
	int32_t findDefinedMinValue(int32_t arg) const;
public:
	FuncMin(unsigned size);

	bool addElement(PartFunc* newElement);
	PairFunc operator()(int32_t arg) const override;
	PartFunc* clone() const override;
};

