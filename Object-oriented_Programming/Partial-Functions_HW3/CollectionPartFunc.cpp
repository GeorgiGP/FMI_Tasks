#include "CollectionPartFunc.h"
#include "FuncsManip.hpp"
namespace
{
	const uint16_t STARTING_CAPACITY = 8;
}


bool CollectionPartFunc::resize()
{
	if (capacity >= INT_MAX)
	{
		return false;
	}
	PartFunc** newList = new PartFunc*[capacity *= 2];
	for (size_t i = 0; i < countFuncs; i++)
	{
		newList[i] = listOfFuncs[i];
	}
	delete[] listOfFuncs;
	listOfFuncs = newList;

	return true;
}

void CollectionPartFunc::free()
{
	for (size_t i = 0; i < countFuncs; i++)
	{
		delete listOfFuncs[i];
	}
	delete[] listOfFuncs;
	listOfFuncs = nullptr;
	countFuncs = capacity = 0;
}

void CollectionPartFunc::copyFrom(const CollectionPartFunc & other)
{
	capacity = other.capacity;
	countFuncs = other.countFuncs;
	listOfFuncs = new PartFunc*[capacity];
	for (size_t i = 0; i < countFuncs; i++)
	{
		listOfFuncs[i] = other.listOfFuncs[i]->clone();
	}
}

void CollectionPartFunc::moveFrom(CollectionPartFunc && other)
{
	listOfFuncs = other.listOfFuncs;
	countFuncs = other.countFuncs;
	capacity = other.capacity;

	other.listOfFuncs = nullptr;
	other.capacity = other.countFuncs = 0;
}


CollectionPartFunc::CollectionPartFunc()
{
	capacity = STARTING_CAPACITY;
	countFuncs = 0;
	listOfFuncs = new PartFunc*[capacity];
}

CollectionPartFunc::CollectionPartFunc(unsigned startingCap)
{
	if (startingCap >= INT_MAX)
	{
		startingCap = INT_MAX;
	}
	else
	{
		roundToPowerOf2<unsigned>(startingCap);
	}
	capacity = startingCap;
	countFuncs = 0;
	listOfFuncs = new PartFunc*[capacity];
}


CollectionPartFunc::CollectionPartFunc(const CollectionPartFunc & other)
{
	copyFrom(other);
}

CollectionPartFunc::CollectionPartFunc(CollectionPartFunc && other) noexcept
{
	moveFrom(std::move(other));
}


CollectionPartFunc & CollectionPartFunc::operator=(const CollectionPartFunc& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

CollectionPartFunc & CollectionPartFunc::operator=(CollectionPartFunc && other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}


CollectionPartFunc::~CollectionPartFunc()
{
	free();
}


unsigned CollectionPartFunc::size() const
{
	return countFuncs;
}

bool CollectionPartFunc::addPartFunc(PartFunc * partFunc) //steal it
{
	if (!partFunc)
	{
		throw std::nullptr_t();
	}
	if (countFuncs >= capacity && !resize())
	{
		return false; //maxCapacity
	}
	listOfFuncs[countFuncs++] = partFunc;
	partFunc = nullptr;
	return true;
}

const PartFunc* CollectionPartFunc::operator[](unsigned index) const
{
	return listOfFuncs[index];
}
