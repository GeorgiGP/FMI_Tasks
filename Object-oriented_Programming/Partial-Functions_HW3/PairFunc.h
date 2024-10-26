#pragma once
#include "Pair.hpp"
#include <iostream>

namespace
{
	const bool NOT_DEFINED = false;
	const bool DEFINED = true;
}
class PairFunc : private Pair<bool, int32_t>
{
public:
	PairFunc();
	PairFunc(bool isDefined, int32_t value);
	bool isDefined() const;
	int32_t value() const;
	
	PairFunc& operator=(const PairFunc& other);
};
std::ostream& operator<<(std::ostream& os, const PairFunc& func);

