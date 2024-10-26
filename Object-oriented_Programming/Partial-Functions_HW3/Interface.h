#pragma once
#include <iostream>
#include "PartFunc.h"
#include "PartFuncFactory.h"

void calculateAndPrint(const PartFunc& function, int32_t value);
void calculateBetweenAndPrint(const PartFunc& function, int32_t lowerBorder, int32_t upperBorder);

void printOptions();

void mainMenu();

