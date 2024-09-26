/**
*
* Solution to homework assignment 2
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Georgi Hristov Lazov
* @idnumber 0MI0600299
* @task 2
*
*/
#include <iostream>
const int MAX_SIZE = 1024;

bool isLetterOrDigitOrSpace(char symbol);

bool isStarPercentATSign(char symbol);

bool isValidText(char* str);

bool isValidTemplate(char* str);

int contains(char* text, char* destination, int destIndex, int textIndex = -1);

int main()
{
	char str[MAX_SIZE], templ[MAX_SIZE] = {};
	std::cin.get(str, MAX_SIZE, '\n');
	std::cin >> templ;
	if (!isValidText(str) || !isValidTemplate(templ) || str == nullptr || templ == nullptr)
	{
		std::cout << "Incorrect input";
		return -1;
	}
	int result = 0;
	for (int i = 0; str[i] != '\0'; i++)
	{
		result += contains(templ, str, i-1);
	}
	std::cout << result;
}

bool isLetterOrDigitOrSpace(char symbol)
{
	if (
		(symbol >= 'a' && symbol <= 'z') ||
		(symbol >= 'A' && symbol <= 'Z') ||
		(symbol >= '0' && symbol <= '9') ||
		(symbol == ' ')
		)
	{
		return true;
	}
	else
		return false;
}
bool isStarPercentATSign(char symbol)
{
	if (
		(symbol == '*') ||
		(symbol == '%') ||
		(symbol == '@')
		)
	{
		return true;
	}
	else
		return false;
}

bool isValidText(char* str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (!isLetterOrDigitOrSpace(str[i]))
		{
			return false;
		}
	}
	return true;
}
bool isValidTemplate(char* str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (!isLetterOrDigitOrSpace(str[i]) && !isStarPercentATSign(str[i]))
		{
			return false;
		}
	}
	return true;
}

int contains(char* text, char* destination, int destIndex, int textIndex)
{
	if (text[0] == '\0' ||destination[0] == '\0')
	{
		return 0;
	}
	int count = 0;
	do
	{
		textIndex++; destIndex++;
		if (text[textIndex] == '\0')
		{
			return ++count;
		}
		else if (destination[destIndex] == '\0' && text[textIndex] != '\0')
		{
			return count;
		}
		char symbol = text[textIndex];
		if (isStarPercentATSign(symbol))
		{
			if (symbol == '*')
			{
				continue;
			}

			else if (symbol == '%')
			{
				if (destination[destIndex]<'0' || destination[destIndex]>'9')
				{
					return count;
				}
				else // destination is number
				{
					if (destination[destIndex + 1] < '0' || destination[destIndex + 1] >'9')
					{
						continue;
					}
					else
					{
						count += contains(text, destination, destIndex, textIndex);
						if (destination[destIndex+1] != '\0')
						{
							destIndex++;
							count += contains(text, destination, destIndex, textIndex);
						}
						return count;
					}
				}
			}

			else //@
			{
				if ((destination[destIndex] >= 'a' && destination[destIndex] <= 'z') ||
					(destination[destIndex] >= 'A' && destination[destIndex] <= 'Z'))
				{
					continue;
				}
				else
					return count;
			}
		}
		else if (symbol == destination[destIndex])
		{
			continue;
		}
		else if (symbol != destination[destIndex])
		{
			return count;
		}
	} while (true);
}
