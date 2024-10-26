#pragma once
#include "PartFunc.h"

template<class T>
class FuncByCriteria : public PartFunc
{
	T func;
public:

	FuncByCriteria(const T& newFunc);

	bool isDefined(int32_t arg) const override;
	PairFunc operator()(int32_t arg) const override;
	PartFunc* clone() const override;
};



template<class T>
inline FuncByCriteria<T>::FuncByCriteria(const T& newFunc) : func(newFunc) {}


template<class T>
bool FuncByCriteria<T>::isDefined(int32_t arg) const
{
	return func.isDefined(arg);
}

template<class T>
PairFunc FuncByCriteria<T>::operator()(int32_t arg) const
{
	return func(arg);
}

template<class T>
PartFunc * FuncByCriteria<T>::clone() const
{
	return new FuncByCriteria<T>(*this);
}


