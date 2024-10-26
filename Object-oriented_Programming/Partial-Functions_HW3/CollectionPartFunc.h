#pragma once
#include "PartFunc.h"
#include <iostream>

class CollectionPartFunc
{
	PartFunc** listOfFuncs = nullptr;
	unsigned countFuncs = 0;
	unsigned capacity = 0;

	bool resize();

	void free();
	void copyFrom(const CollectionPartFunc& other);
	void moveFrom(CollectionPartFunc&& other);

public:

	CollectionPartFunc();
	CollectionPartFunc(unsigned startingCap);

	CollectionPartFunc(const CollectionPartFunc& other);
	CollectionPartFunc(CollectionPartFunc&& other) noexcept;

	CollectionPartFunc& operator=(const CollectionPartFunc& other);
	CollectionPartFunc& operator=(CollectionPartFunc&& other) noexcept;

	~CollectionPartFunc();
	unsigned size() const;

	bool addPartFunc(PartFunc* partFunc);

	const PartFunc* operator[](unsigned index) const;

};

