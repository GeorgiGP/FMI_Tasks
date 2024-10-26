#pragma once
#include <iostream>
#include "FuncsManip.hpp"
namespace
{
	const uint8_t DEFAULT_CAPACITY = 8;
}
template<class T>
class Set //for primitive types
{
	T* cells = nullptr;
	unsigned current = 0;
	unsigned capacity = 0;

	bool resize();

	void free();
	void copyFrom(const Set& other);
	void moveFrom(Set&& other);
public:
	Set();
	Set(unsigned count);

	Set(const Set& other);
	Set(Set&& other) noexcept;

	Set& operator=(const Set& other);
	Set& operator=(Set&& other) noexcept;

	~Set();

	unsigned size() const;
	bool addElement(const T& add);

	const T& operator[](unsigned index) const;
};


template<class T>
inline bool Set<T>::resize()
{
	if (capacity>=INT_MAX)
	{
		return false;
	}
	T* newCells = new T[capacity *= 2];
	for (size_t i = 0; i < current; i++)
	{
		newCells[i] = cells[i];
	}
	delete[] cells;
	cells = newCells;
	return true;
}


template<class T>
inline void Set<T>::free()
{
	delete[] cells;
	cells = nullptr;
	capacity = current = 0;
}


template<class T>
inline void Set<T>::copyFrom(const Set& other)
{
	capacity = other.capacity;
	current = other.current;
	cells = new T[capacity];
	for (size_t i = 0; i < current; i++)
	{
		cells[i] = other.cells[i];
	}
}


template<class T>
inline void Set<T>::moveFrom(Set && other)
{
	cells = other.cells;
	capacity = other.capacity;
	current = other.current;

	other.cells = nullptr;
	other.capacity = other.current = 0;
}


template<class T>
inline Set<T>::Set()
{
	current = 0;
	capacity = DEFAULT_CAPACITY;
	cells = new T[capacity];
}


template<class T>
inline Set<T>::Set(unsigned capacity)
{
	if (capacity<=8)
	{
		capacity = 8;
	}
	else if (capacity >= INT_MAX)
	{
		capacity = INT_MAX;
	}
	else
	{
		roundToPowerOf2<unsigned>(capacity);
	}
	this->capacity = capacity;
	current = 0;
	cells = new T[capacity]; //abstract 0
}


template<class T>
inline Set<T>::Set(const Set & other)
{
	copyFrom(other);
}

template<class T>
inline Set<T>::Set(Set&& other) noexcept
{
	moveFrom(std::move(other));
}


template<class T>
inline Set<T>& Set<T>::operator=(const Set& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}


template<class T>
inline Set<T> & Set<T>::operator=(Set && other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}


template<class T>
inline Set<T>::~Set()
{
	free();
}


template<class T>
inline unsigned Set<T>::size() const
{
	return current;
}


template<class T>
inline bool Set<T>::addElement(const T& add)
{
	if (current == capacity && !resize())
	{
		return false;
	}
	cells[current++] = add;
	return true;
}


template<class T>
inline const T& Set<T>::operator[](unsigned index) const
{
	return cells[index];
}
