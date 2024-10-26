#include "PartFuncFactory.h"
#include "FuncByCriteria.hpp"
#include <fstream>
namespace
{
	const uint16_t MAX_FILENAME_LEN = 1024;
	const uint16_t MAX_N_VALUE = 32;
	const uint16_t COUNT_OF_FUNC_TYPES = 5;
}
char * FileFunctions::createCharStrFromBinFile(std::ifstream & ifs)
{
	char* result = new char[MAX_FILENAME_LEN];

	uint16_t index = 0;
	char ch = '\0';
	while (true)
	{
		if (!ifs.good())
		{
			throw std::invalid_argument("file does not contain string");
		}

		ifs.read((char*)&ch, sizeof(char));
		result[index++] = ch;
		
		if (ch == '\0' || index == MAX_FILENAME_LEN)
		{
			return result;
		}
	}
}
Func0* createFunc0(std::ifstream& ifs, uint16_t N)
{
	Func0* result = new Func0(N);
	int32_t* Domain = new int32_t[N];

	for (size_t i = 0; i < N; i++)
	{
		ifs.read((char*)&Domain[i], sizeof(int32_t));
	}
	for (size_t i = 0; i < N; i++)
	{
		int32_t y;
		ifs.read((char*)&y, sizeof(y));
		result->addElement(Domain[i], y);
	}

	delete[] Domain;
	Domain = nullptr;
	ifs.close();
	return result;

}

Func1* createFunc1(std::ifstream & ifs, uint16_t N)
{
	Func1* result = new Func1(N);
	for (size_t i = 0; i < N; i++)
	{
		int32_t x;
		ifs.read((char*)&x, sizeof(x));
		result->addElement(x);
	}

	ifs.close();
	return result;
}

Func2* createFunc2(std::ifstream & ifs, uint16_t N)
{
	Func2* result = new Func2(N);
	for (size_t i = 0; i < N; i++)
	{
		int32_t x;
		ifs.read((char*)&x, sizeof(x));
		result->addElement(x);
	}

	ifs.close();
	return result;
}

PartFunc* createFuncByCriteria(std::ifstream & ifs, uint16_t N, uint16_t Mode)
{
	switch (Mode)
	{
	case 0:
	{
		Func0* f = createFunc0(ifs, N);
		PartFunc* result = new FuncByCriteria<Func0>(*f);
		delete f;
		return result;
	}
	case 1:
	{
		Func1* f = createFunc1(ifs, N);
		PartFunc* result = new FuncByCriteria<Func1>(*f);
		delete f;
		return result;
	}
	case 2:
	{
		Func2* f = createFunc2(ifs, N);
		PartFunc* result = new FuncByCriteria<Func2>(*f);
		delete f;
		return result;
	}
	default:
		throw std::invalid_argument("No such function.");
	}
}

FuncMax* createFuncMax(std::ifstream & ifs, uint16_t N)
{
	FuncMax* result = new FuncMax(N);
	for (size_t i = 0; i < N; i++)
	{
		char* fileName = FileFunctions::createCharStrFromBinFile(ifs);
		PartFunc* add = createFromFile(fileName);
		delete fileName;

		result->addElement(add);
	}

	ifs.close();
	return result;
}

FuncMin* createFuncMin(std::ifstream & ifs, uint16_t N)
{
	FuncMin* result = new FuncMin(N);
	for (size_t i = 0; i < N; i++)
	{
		char* fileName = FileFunctions::createCharStrFromBinFile(ifs);
		PartFunc* add = createFromFile(fileName);
		delete fileName;

		result->addElement(add);
	}

	ifs.close();
	return result;
}

PartFunc* createFromBinFile(std::ifstream& ifs)
{
	uint16_t N, Type;

	ifs.read((char*)&N, sizeof(N));
	if (N > MAX_N_VALUE)
	{
		ifs.close();
		throw std::invalid_argument("Binary file is invalid!");
	}

	ifs.read((char*)&Type, sizeof(Type));
	if (Type >= COUNT_OF_FUNC_TYPES)
	{
		ifs.close();
		throw std::invalid_argument("Binary file is invalid! Unknown Function Type");
	}

	switch (Type)
	{
	case 0:
		return createFuncByCriteria(ifs, N, FUNC_MODE0);
	case 1:
		return createFuncByCriteria(ifs, N, FUNC_MODE1);
	case 2:
		return createFuncByCriteria(ifs, N, FUNC_MODE2);
	case 3:
		return createFuncMax(ifs, N);
	case 4:
		return createFuncMin(ifs, N);
	default:
		ifs.close();
		throw std::invalid_argument("Binary file is invalid! Unknown Function Type");
	}
}
PartFunc* createFromFile(const char* fileName)
{
	if (!fileName)
	{
		throw  std::nullptr_t();
	}
	std::ifstream ifs(fileName, std::ios::binary);
	if (!ifs.is_open())
	{
		std::cout << "No such file with this name";
	}

	return createFromBinFile(ifs);
}
PartFunc* partFuncFactory()
{
	char fileName[MAX_FILENAME_LEN]{};
	std::cout << "Choose file to serialize the function:" << std::endl;
	std::cin >> fileName;

	return createFromFile(fileName);
}