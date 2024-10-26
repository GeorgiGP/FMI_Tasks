#include "CombinedMaxMin.h"

CombinedMaxMin::CombinedMaxMin(unsigned size) : collection(size) {}


bool CombinedMaxMin::isDefined(int32_t arg) const
{
	for (unsigned i = 0; i < collection.size(); i++)
	{
		if (!collection[i]->isDefined(arg))
		{
			return false;
		}
	}
	return true;
}

bool CombinedMaxMin::addElement(PartFunc * newElement)
{
	if (!newElement)
	{
		throw std::nullptr_t();
	}
	return collection.addPartFunc(newElement);
}
