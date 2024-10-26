#pragma once
#include <iostream>
#include "Set.h"
#include <exception>
#include <fstream>

#define convert(x) (unsigned)(x-SHORT_MIN)

namespace GlobalConsts
{
	const uint8_t FIELD_SIZE = 20;
	const uint8_t MAX_FIELD_SIZE = 200;
}
using namespace GlobalConsts;

class ModifiableIntegersFunction
{
	int16_t(*function)(int16_t) = nullptr;
	int16_t* range = nullptr;
	Set isValidSet;

	mutable bool injectionValue = false;
	mutable bool isInjectionChanged = true;
	mutable bool surrectionValue = false;
	mutable bool isSurrectionChanged = true;

	void setUp(int16_t(*newFunc)(int16_t));
	void operatorsEqualAndElse(const ModifiableIntegersFunction& rhs, int16_t(*func)(int16_t, int16_t));

	void copyFrom(const ModifiableIntegersFunction& other);
	void free();
public:

	ModifiableIntegersFunction();
	ModifiableIntegersFunction(int16_t(*newFunc)(int16_t));
	ModifiableIntegersFunction(const ModifiableIntegersFunction& other);
	ModifiableIntegersFunction& operator=(const ModifiableIntegersFunction& other);
	~ModifiableIntegersFunction();

	int16_t operator()(int number) const;
	ModifiableIntegersFunction& operator+=(const ModifiableIntegersFunction& rhs);
	ModifiableIntegersFunction& operator-=(const ModifiableIntegersFunction& rhs);
	friend ModifiableIntegersFunction operator^(const ModifiableIntegersFunction& lhs, int16_t power);
	friend ModifiableIntegersFunction operator+(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);
	friend ModifiableIntegersFunction operator-(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);
	friend bool operator<(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);
	friend bool operator>(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);
	friend bool operator==(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);
	friend bool operator!=(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);
	friend ModifiableIntegersFunction operator*(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);//composition
	friend bool areParallel(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);

	void setCustomResult(short forNumber, short customValue);
	void excludePoint(short number);

	bool isInjection() const;
	bool isSurrection() const;
	bool isBijection() const;

	void serialize(const char* fileName) const;
	void deserialize(const char* fileName);

	void printField(int fromX, int fromY, int _SizeField) const;
	void printField(int fromX, int fromY) const;
};


