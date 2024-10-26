#include "_ModifiableIntegersFunction_.h"

#define MAX(x,y) (int)((x>=y)?(x):(y))
#define MIN(x,y) (int)((x<=y)?(x):(y))

int16_t defaultFunction(int16_t s)
{
	return 0;
}

namespace Functions
{
	short sumNums(short lhs, short rhs)
	{
		int result = lhs;
		result += rhs;
		if (result >= SHORT_MAX)
		{
			return SHORT_MAX;
		}
		else if (result <= SHORT_MIN)
		{
			return SHORT_MIN;
		}
		else
		{
			return result;
		}
	}
	short subtractNums(short lhs, short rhs)
	{
		if (rhs == SHORT_MIN)
		{
			if (lhs <= 0)
			{
				return lhs - rhs;
			}
			else
			{
				return SHORT_MAX;
			}
			return sumNums(lhs, SHORT_MAX);
		}
		return sumNums(lhs, -rhs);
	}
}
using namespace Functions;

ModifiableIntegersFunction::ModifiableIntegersFunction()
{

	setUp(defaultFunction);
}

ModifiableIntegersFunction::ModifiableIntegersFunction(int16_t(*newFunc)(int16_t))
{
	setUp(newFunc);
}

ModifiableIntegersFunction::ModifiableIntegersFunction(const ModifiableIntegersFunction & other)
{
	copyFrom(other);
}

ModifiableIntegersFunction & ModifiableIntegersFunction::operator=(const ModifiableIntegersFunction & other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

ModifiableIntegersFunction::~ModifiableIntegersFunction()
{
	free();
}

int16_t ModifiableIntegersFunction::operator()(int number) const
{
	if (number < SHORT_MIN || number > SHORT_MAX)
	{
		throw std::invalid_argument("input should be an argument between SHORT_MIN and SHORT_MAX");
	}
	if (!this->isValidSet.isAppeared(number))
	{
		throw std::invalid_argument("function should be defined in a point to return value");
	}
	return this->range[convert(number)];
}

ModifiableIntegersFunction& ModifiableIntegersFunction::operator+=(const ModifiableIntegersFunction & rhs)
{
	operatorsEqualAndElse(rhs, sumNums);
	return *this;
}

ModifiableIntegersFunction & ModifiableIntegersFunction::operator-=(const ModifiableIntegersFunction & rhs)
{
	operatorsEqualAndElse(rhs, subtractNums);
	return *this;
}

ModifiableIntegersFunction operator^(const ModifiableIntegersFunction& lhs, int16_t power)
{
	if (power > 1)
	{
		ModifiableIntegersFunction resultMIF = lhs;
		for (size_t i = 1; i < power; i++)
		{
			resultMIF = lhs * resultMIF;
		}
		return resultMIF;
	}
	else if (power == -1)
	{
		if (!lhs.isInjection())
		{
			throw std::exception("Function must be Inective to has inverse (f^-1)!");
		}
		ModifiableIntegersFunction result = lhs;
		result.isValidSet.clear();

		for (int i = SHORT_MIN; i <= SHORT_MAX; i++)
		{
			if (lhs.isValidSet.isAppeared(i))
			{
				result.setCustomResult(lhs.range[convert(i)], i);
			}
		}
		return result;
	}
	else
	{
		return lhs;
	}

}

void ModifiableIntegersFunction::setCustomResult(short forNumber, short customValue)
{
	if (range[convert(forNumber)] == customValue && isValidSet.isAppeared(forNumber))
	{
		return;
	}
	isValidSet.add(forNumber);
	range[convert(forNumber)] = customValue;
	isInjectionChanged = true;
	isSurrectionChanged = true;
}

void ModifiableIntegersFunction::excludePoint(short number)
{
	if (!isValidSet.isAppeared(number))
	{
		return;
	}
	this->isValidSet.remove(number);
	isInjectionChanged = true;
	surrectionValue = false;
	isSurrectionChanged = false;
}

bool ModifiableIntegersFunction::isInjection() const
{
	if (!isInjectionChanged)
	{
		return injectionValue;
	}
	Set isOccurred;
	for (int i = SHORT_MIN; i <= SHORT_MAX; i++)
	{
		if (!this->isValidSet.isAppeared(i))
		{
			continue;
		}
		if (isOccurred.isAppeared(range[convert(i)])) //we have x1 = y1 and x2 = y1 => no inection
		{
			isInjectionChanged = false;
			return injectionValue = false;
		}
		isOccurred.add(range[convert(i)]);
	}
	isInjectionChanged = false;
	return injectionValue = true;
}

bool ModifiableIntegersFunction::isSurrection() const
{
	if (!isSurrectionChanged)
	{
		return surrectionValue;
	}
	Set isOccurred;
	for (int i = SHORT_MIN; i <= SHORT_MAX; i++)
	{
		if (!this->isValidSet.isAppeared(i)) //in order to be surrection, all x should be valid to access all y. All y from SHORT_MIN to SHORT_MAX =>surrection
		{
			isSurrectionChanged = false;
			return surrectionValue = false;
		}
		if (isOccurred.isAppeared(range[convert(i)])) //if x1=y1 and x2=y1 => two X-es point to 1 y, so all y can't be achieved
		{
			isSurrectionChanged = false;
			return surrectionValue = false;
		}
		isOccurred.add(range[convert(i)]);
	}
	isSurrectionChanged = false;
	return surrectionValue = true;
}

bool ModifiableIntegersFunction::isBijection() const
{
	return (this->isInjection() && this->isSurrection());
}

void ModifiableIntegersFunction::serialize(const char * fileName) const
{
	if (!fileName)
	{
		throw std::exception("Nullptr");
	}
	std::ofstream ofs(fileName, std::ios::out | std::ios::binary);
	ofs.write((const char*)&function, sizeof(function));
	ofs.write((const char*)range, sizeof(int16_t)*COUNT_NUMBERS_SHORT);
	ofs.write((const char*)&isValidSet, sizeof(isValidSet));
	ofs.write((const char*)&injectionValue, sizeof(bool));
	ofs.write((const char*)&isInjectionChanged, sizeof(bool));
	ofs.write((const char*)&surrectionValue, sizeof(bool));
	ofs.write((const char*)&isSurrectionChanged, sizeof(bool));
	ofs.close();
}

void ModifiableIntegersFunction::deserialize(const char * fileName)
{
	if (!fileName)
	{
		throw std::exception("Nullptr");
	}
	std::ifstream ifs(fileName, std::ios::in | std::ios::binary);
	ifs.read((char*)&function, sizeof(function));
	ifs.read((char*)range, sizeof(int16_t)*COUNT_NUMBERS_SHORT);
	ifs.read((char*)&isValidSet, sizeof(isValidSet));
	ifs.read((char*)&injectionValue, sizeof(bool));
	ifs.read((char*)&isInjectionChanged, sizeof(bool));
	ifs.read((char*)&surrectionValue, sizeof(bool));
	ifs.read((char*)&isSurrectionChanged, sizeof(bool));
	ifs.close();
}

void ModifiableIntegersFunction::printField(int fromX, int fromY, int _SizeField) const
{
	if (fromX<SHORT_MIN || fromX>SHORT_MAX || fromY<SHORT_MIN || fromY>SHORT_MAX)
	{
		throw std::invalid_argument("Arguments must be between SHORT_MIN and SHORT_MAX");
	}
	if (_SizeField <= 0 || _SizeField > MAX_FIELD_SIZE)
	{
		throw std::invalid_argument("Size of the field must be between 1 and MAX_FIELD_SIZE");
	}
	uint8_t sizeField = _SizeField;
	if (fromX + _SizeField > SHORT_MAX || fromY + _SizeField > SHORT_MAX)
	{
		sizeField = SHORT_MAX - MAX(fromX, fromY);
	}
	bool** matrix = new bool*[sizeField];
	for (size_t i = 0; i < sizeField; i++)
	{
		matrix[i] = new bool[sizeField] {};
	}
	for (size_t i = 0; i < sizeField; i++)
	{
		if (isValidSet.isAppeared(fromX) && range[convert(fromX)] >= fromY && range[convert(fromX)] < fromY + sizeField)
		{
			matrix[i][range[convert(fromX)] - fromY] = true;
		}
		fromX++;
	}

	std::cout << std::endl;
	for (int i = sizeField - 1; i >= 0; i--)
	{
		std::cout << '|';
		for (size_t j = 0; j < sizeField; j++)
		{
			if (matrix[j][i])
			{
				std::cout << 'x';
			}
			else
			{
				std::cout << ' ';
			}
		}
		std::cout << std::endl;
	}
	std::cout << ' ';
	for (size_t i = 1; i < sizeField; i++)
	{
		std::cout << '-';
	}
	std::cout << std::endl;

	for (size_t i = 0; i < sizeField; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

void ModifiableIntegersFunction::printField(int fromX, int fromY) const
{
	printField(fromX, fromY, FIELD_SIZE);
}

void ModifiableIntegersFunction::setUp(int16_t(*newFunc)(int16_t))
{
	if (newFunc == nullptr)
	{
		throw std::exception("Nullptr");
	}
	this->function = newFunc;
	range = new int16_t[COUNT_NUMBERS_SHORT]{};

	for (int i = SHORT_MIN; i <= SHORT_MAX; i++)
	{
		range[convert(i)] = function(i);
	}
	for (size_t i = 0; i < BUCKETS_ISVALID; i++)
	{
		this->isValidSet.buckets[i]--; //11111111 from 0 to 255 filled with 1 for valid
	}
}

void ModifiableIntegersFunction::operatorsEqualAndElse(const ModifiableIntegersFunction& rhs, int16_t(*func)(int16_t, int16_t))
{
	for (size_t i = 0; i < BUCKETS_ISVALID; i++)
	{
		this->isValidSet.buckets[i] &= rhs.isValidSet.buckets[i];
	}
	for (int i = SHORT_MIN; i <= SHORT_MAX; i++)
	{
		if (!isValidSet.isAppeared(i))
		{
			continue;
		}
		this->range[convert(i)] = func(this->range[convert(i)], rhs.range[convert(i)]);
	}
	isInjectionChanged = true;
	isSurrectionChanged = true;
}

void ModifiableIntegersFunction::copyFrom(const ModifiableIntegersFunction & other)
{
	function = other.function;
	isValidSet = other.isValidSet;
	injectionValue = other.injectionValue;
	isInjectionChanged = other.isInjectionChanged;
	surrectionValue = other.surrectionValue;
	isSurrectionChanged = other.isSurrectionChanged;

	range = new int16_t[COUNT_NUMBERS_SHORT]{};
	for (int i = SHORT_MIN; i <= SHORT_MAX; i++)
	{
		if (isValidSet.isAppeared(i))
		{
			range[convert(i)] = other.range[convert(i)];
		}
	}

}

void ModifiableIntegersFunction::free()
{
	function = nullptr;
	delete[] range;
	range = nullptr;
	injectionValue = false;
	isInjectionChanged = true;
	surrectionValue = false;
	isSurrectionChanged = true;
}

ModifiableIntegersFunction operator+(const ModifiableIntegersFunction & lhs, const ModifiableIntegersFunction & rhs)
{
	ModifiableIntegersFunction result = lhs;
	result += rhs;
	return result;
}

ModifiableIntegersFunction operator-(const ModifiableIntegersFunction & lhs, const ModifiableIntegersFunction & rhs)
{
	ModifiableIntegersFunction result = lhs;
	result -= rhs;
	return result;
}

bool operator<(const ModifiableIntegersFunction & lhs, const ModifiableIntegersFunction & rhs)
{
	for (int i = SHORT_MIN; i <= SHORT_MAX; i++)
	{//if right is the SHORT_MIN or (left is SHORT_MIN and right is also SHORT_MIN) => invalid
		if (!rhs.isValidSet.isAppeared(i) || (!lhs.isValidSet.isAppeared(i) && rhs.range[convert(i)] == SHORT_MIN))
		{
			return false;
		}
		if (lhs.range[convert(i)] >= rhs.range[convert(i)])
		{
			return false;
		}
	}
	return true;
}

bool operator>(const ModifiableIntegersFunction & lhs, const ModifiableIntegersFunction & rhs)
{
	return (rhs < lhs);
}

bool operator==(const ModifiableIntegersFunction & lhs, const ModifiableIntegersFunction & rhs)
{
	for (int i = SHORT_MIN; i <= SHORT_MAX; i++)
	{
		if (!rhs.isValidSet.isAppeared(i) && !lhs.isValidSet.isAppeared(i))
		{
			continue;
		}
		if (!rhs.isValidSet.isAppeared(i))
		{
			if (lhs.range[convert(i)] == SHORT_MIN)
			{
				continue;
			}
			else
			{
				return false;
			}
		}
		if (!lhs.isValidSet.isAppeared(i))
		{
			if (rhs.range[convert(i)] == SHORT_MIN)
			{
				continue;
			}
			else
			{
				return false;
			}
		}
		if (lhs.range[convert(i)] != rhs.range[convert(i)])
		{
			return false;
		}
	}
	return true;
}

bool operator!=(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs)
{
	return !(lhs == rhs);
}

ModifiableIntegersFunction operator*(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs)
{
	ModifiableIntegersFunction result;
	result.isValidSet.clear();

	for (int i = SHORT_MIN; i <= SHORT_MAX; i++)
	{
		if (rhs.isValidSet.isAppeared(i) && lhs.isValidSet.isAppeared(rhs(i)))
		{
			result.setCustomResult(i, lhs(rhs(i)));
		}
	}
	result.isInjectionChanged = true;
	result.isSurrectionChanged = true;
	return result;
}

bool areParallel(const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs)
{
	if (lhs.isValidSet != rhs.isValidSet)
	{
		return false;
	}
	if (lhs.isValidSet.isEmpty())
	{
		return false;
	}
	short compareIndex = lhs.isValidSet.lowestDefined();
	int diff = (int)MAX(lhs.range[convert(compareIndex)], rhs.range[convert(compareIndex)]) - (int)MIN(lhs.range[convert(compareIndex)], rhs.range[convert(compareIndex)]);
	for (int i = SHORT_MIN; i <= SHORT_MAX; i++)
	{
		if (lhs.isValidSet.isAppeared(i) && ((int)MAX(lhs.range[convert(i)], rhs.range[convert(i)]) - (int)MIN(lhs.range[convert(i)], rhs.range[convert(i)]) != diff))
		{
			return false;
		}
	}
	return true;
}
