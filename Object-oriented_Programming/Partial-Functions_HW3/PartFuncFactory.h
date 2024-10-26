#pragma once
#include "PartFunc.h"
#include "Func0.h"
#include "Func1.h"
#include "Func2.h"
#include "FuncMax.h"
#include "FuncMin.h"

namespace FileFunctions
{
	char* createCharStrFromBinFile(std::ifstream& ifs);
}

Func0* createFunc0(std::ifstream& ifs, uint16_t N);
Func1* createFunc1(std::ifstream& ifs, uint16_t N);
Func2* createFunc2(std::ifstream& ifs, uint16_t N);
PartFunc* createFuncByCriteria(std::ifstream& ifs, uint16_t N, uint16_t Mode);

FuncMax* createFuncMax(std::ifstream& ifs, uint16_t N);
FuncMin* createFuncMin(std::ifstream& ifs, uint16_t N);

PartFunc* partFuncFactory();
PartFunc* createFromFile(const char* fileName);
PartFunc* createFromBinFile(std::ifstream& ifs);

