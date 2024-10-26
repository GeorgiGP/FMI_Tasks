#include "FuncMin.h"

int32_t FuncMin::findDefinedMinValue(int32_t arg) const
{
	int32_t minResult = INT_MAX;
	for (unsigned i = 0; i < collection.size(); i++)
	{
		PairFunc curr = (*collection[i])(arg);
		if (curr.value() < minResult)
		{
			minResult = curr.value();
		}
	}
	return minResult;
}

FuncMin::FuncMin(unsigned size) : CombinedMaxMin(size) {}


bool FuncMin::addElement(PartFunc * newElement)  //steal it
{
	if (!newElement)
	{
		throw std::nullptr_t();
	}
	return CombinedMaxMin::addElement(newElement);
}



PairFunc FuncMin::operator()(int32_t arg) const
{
	bool def = isDefined(arg);
	if (def)
	{
		return PairFunc(def, findDefinedMinValue(arg));
	}
	else
	{
		return PairFunc(); // won't calc findDefinedMinValue(arg)
	}
}

PartFunc * FuncMin::clone() const
{
	return new FuncMin(*this);
}
