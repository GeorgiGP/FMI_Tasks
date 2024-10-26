#include "FuncMax.h"

int32_t FuncMax::findDefinedMaxValue(int32_t arg) const
{
	int32_t maxResult = INT_MIN;
	for (unsigned i = 0; i < collection.size(); i++)
	{
		PairFunc curr = (*collection[i])(arg);
		if (curr.value() > maxResult)
		{
			maxResult = curr.value();
		}
	}
	return maxResult;
}

FuncMax::FuncMax(unsigned size) : CombinedMaxMin(size) {}


//void FuncMax::addElement(const PartFunc & newElement)
//{
//	collection.addPartFunc(&newElement);
//
//}

bool FuncMax::addElement(PartFunc* newElement)  //steal it
{
	if (!newElement)
	{
		throw std::nullptr_t();
	}
	return CombinedMaxMin::addElement(newElement);
}

//bool FuncMax::isDefined(int32_t arg) const
//{
//	for (unsigned i = 0; i < collection.size(); i++)
//	{
//		if (!collection[i]->isDefined(arg))
//		{
//			return false;
//		}
//	}
//	return true;
//}

PairFunc FuncMax::operator()(int32_t arg) const
{
	bool def = isDefined(arg);
	if (def)
	{
		return PairFunc(def, findDefinedMaxValue(arg));
	}
	else
	{
		return PairFunc(); // won't calc findDefinedMinValue(arg)
	}
}

PartFunc * FuncMax::clone() const
{
	return new FuncMax(*this);
}
