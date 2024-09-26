/**
*
* Solution to homework assignment 2
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Georgi Hristov Lazov
* @idnumber 0MI0600299
* @task 1
*
*/
#include <iostream>
const int MAX_SIZE = 64;
const int LENGTH_BINARY = 8;

void inputStr(char *str);

char creatingNewSymbolInBinary(const char oldSymbol, const int startPositionBinary);

char firstHalfSymbol(char* const str, const int currentSymbolStr);

char secondHalfSymbol(char* const str, const int currentSymbolStr);

int main()
{
	char str[MAX_SIZE];
	inputStr(str);

	unsigned index = 0;
	while (str[index] != '\0')
	{
		std::cout << firstHalfSymbol(str, index);
		std::cout << secondHalfSymbol(str, index);

		index++;
	}
}

void inputStr(char *str)
{
	std::cin >> str;
}

char creatingNewSymbolInBinary(const char oldSymbol, const int startPositionBinary)
{
	int newSymbol = 0;
	for (int i = startPositionBinary - 1, leftShifting = 7; i >= (startPositionBinary - 4); i--)
	{
		if (oldSymbol & (1 << i)) //if 1: 10
		{
			newSymbol |= (1 << leftShifting);
			leftShifting -= 2;
		}
		else //if 0: 01
		{
			leftShifting--;
			newSymbol |= (1 << leftShifting);
			leftShifting--;
		}
	}
	return newSymbol;
}

char firstHalfSymbol(char* const str, const int currentSymbolStr)
{
	char symbol = str[currentSymbolStr];
	return creatingNewSymbolInBinary(symbol, LENGTH_BINARY);
}
char secondHalfSymbol(char* const str, const int currentSymbolStr)
{
	char symbol = str[currentSymbolStr];
	return creatingNewSymbolInBinary(symbol, (LENGTH_BINARY / 2));
}

 