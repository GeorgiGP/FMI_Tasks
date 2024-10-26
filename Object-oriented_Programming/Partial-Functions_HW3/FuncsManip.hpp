#pragma once
#include <iostream>
template<class T>
void roundToPowerOf2(T& num);

template<class T>
void mySwap(T& a, T& b);


template<class T>
inline void roundToPowerOf2(T& num)
{
	num--;
	unsigned bitsForT = sizeof(T) * 8;

	for (size_t i = 1; i < bitsForT; i *= 2)
	{
		num |= num >> i;
	}
	num++;
}

template<class T>
inline void mySwap(T& a, T& b)
{
	T temp = std::move(a);
	a = std::move(b);
	b = std::move(temp);
}

