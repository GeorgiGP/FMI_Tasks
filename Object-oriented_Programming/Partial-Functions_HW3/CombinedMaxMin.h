#pragma once
#include "CollectionPartFunc.h"
class CombinedMaxMin : public PartFunc
{
protected:
	CollectionPartFunc collection;

public:
	CombinedMaxMin(unsigned size);
	bool isDefined(int32_t arg) const override;

	bool addElement(PartFunc * newElement);
};

