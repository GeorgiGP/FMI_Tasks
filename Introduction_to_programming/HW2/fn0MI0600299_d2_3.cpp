/**
*
* Solution to homework assignment 2
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Georgi Hristov Lazov
* @idnumber 0MI0600299
* @task 3
*
*/
#include <iostream>
const short MAX_LENGTH = 1024;
const short COUNT_OF_LETTERS_ALPHABET = 26;

int strLength(char* str);

void swap(char& a, char& b);

void selectionSortStrIndex(char* str, int fromIndex);

bool sortingAlphabet(int length, char* str);

int main()
{
	char str[MAX_LENGTH];
	int number = 0;
	std::cin >> str;
	std::cin >> number;
	int length = strLength(str);
	
	selectionSortStrIndex(str, 0);
	for (size_t i = 0; i < number; i++)
	{
		std::cout << str << std::endl;
		if (!sortingAlphabet(length, str))
		{
			break;
		}
	}
	return 0;
}

int strLength(char* str)
{
	int count = 0;
	for (size_t i = 0; str[i] != '\0'; i++)
	{
		count++;
	}
	return count;
}

void swap(char& a, char& b)
{
	a = a ^ b;
	b = a ^ b;
	a = a ^ b;
}
void selectionSortStrIndex(char* str, int fromIndex)
{
	for (size_t j = fromIndex; str[j] != '\0'; j++)
	{
		char min = str[j];
		int minIndex = j;
		for (size_t i = j; str[i] != '\0'; i++)
		{
			if (str[i] < min)
			{
				min = str[i];
				minIndex = i;
			}
		}
		if (str[j] == min)
		{
			continue;
		}
		swap(str[j], str[minIndex]);
	}
}

bool sortingAlphabet(int length, char* str)
{
	char max = str[length - 1];
	for (int i = length - 2; i >= 0; i--)
	{
		if (str[i] < max) //if inreasing backwards breaks (breaking point)
		{
			selectionSortStrIndex(str, i + 1); //lowest alphabetical order
			for (size_t k = i + 1; str[k] != '\0'; k++)
			{
				if (str[i] < str[k]) //taking the smallest letter on aphlabetical perspective that is bigger than the breaking point
				{
					swap(str[i], str[k]);
					return true; //possible
				}
			}
		}
		else
			max = str[i]; //increasing backwards
	}
	return false; //the biggest alphabetical order, if all
}
