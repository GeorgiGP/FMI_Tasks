/**
*
* Solution to homework assignment 1
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

const short SIZE = 1000;

void swap(int& num1, int& num2);

void selectionSort(int array[], int SIZE);

bool isOutOfRange(int number, int max, int min);

bool isArrayOutOfRange(int array[], int arrLength, int max, int min);

void inputArray(int array[], int const SIZE, int const numberOfPeople);

int minSumOfAllPeople(int array[], int allQuestionedPeople);

int main()
{
	int numberOfQuestioned = 0;
	std::cin >> numberOfQuestioned;
	if (isOutOfRange(numberOfQuestioned, 1000, 1))
	{
		std::cout << "Incorrect input";
		return -1;
	}
	int arr[SIZE] = {};
	inputArray(arr, SIZE, numberOfQuestioned);
	if (isArrayOutOfRange(arr, numberOfQuestioned, 1000, 0))
	{
		std::cout << "Incorrect input";
		return -1;
	}
	selectionSort(arr, SIZE);
	std::cout << minSumOfAllPeople(arr, numberOfQuestioned);
}

void swap(int& num1, int& num2)
{
	int temp = num1;
	num1 = num2;
	num2 = temp;
}
void selectionSort(int array[], int SIZE)
{
	for (int j = 0; j < SIZE; j++)
	{
		int indexMin = j;
		for (int i = j + 1; i < SIZE; i++)
		{
			// the excess (unused) element form the array are by default -1, because 0 can be included in the range, we don't sort the excess numbers
			if (array[i] != -1 && (array[i] < array[indexMin]))
			{
				indexMin = i;
			}
		}
		swap(array[j], array[indexMin]);
	}
}

bool isOutOfRange(int number, int max, int min)
{
	if (number > max || number < min)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void inputArray(int array[], int const SIZE, int const lenght)
{
	for (int i = 0; i < SIZE; i++)
	{
		array[i] = -1;
		// number out of range, that cannot be written
	}
	for (int i = 0; i < lenght; i++)
	{
		std::cin >> array[i];
	}
}
bool isArrayOutOfRange(int array[], int arrLength, int max, int min)
{
	for (int i = 0; i < arrLength; i++)
	{
		if (array[i] > max || array[i] < min)
		{
			return true;
		}
	}
	return false;
}

int minSumOfAllPeople(int sortedArray[], int allQuestionedPeople)
{
	int result = 0;
	for (int i = 0; i < allQuestionedPeople;)
	{
		int countOfCurrentNum = 0;
		do //do-while for inluding 0 as and answer, if a person says 0, then he is the only person with this type of hair color.
		{
			countOfCurrentNum++;
			i++;
			if (sortedArray[i-1] == countOfCurrentNum - 1)
			{
				/*All people with this hair colour have been found.
				If there are more people form questioned with the same answer then they have different hair colour from those */
				break;
			}
		} while (sortedArray[i-1] == sortedArray[i]);

		result += sortedArray[i-1] + 1;
	}
	return result;
}
