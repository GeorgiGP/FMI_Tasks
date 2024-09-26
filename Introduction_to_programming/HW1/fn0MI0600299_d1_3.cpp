/**
*
* Solution to homework assignment 1
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
const int SIZE = 383; // n = (0, 256) , s = [0, 127] , if every new ball makes the array larger

bool isOutOfRange(int number, int max, int min);

void inputArray(char array[], int const lenght, bool& isArrayValid);

void printArray(char array[], int lenght);

bool miss(int lenghtArray, int index);

void addBallOnIndex(char array[], int& lenghtArray, int index, char symbol);

void distructionOnElements(char arr[], int& n, int& index, char& symbol);

bool moreThan3(char array[], int index);

bool moreThan3OnBothSides(char array[], int index);

int main()
{
	int n, s = 0;
	std::cin >> n;
	if (isOutOfRange(n, 255, 1) || isOutOfRange(s , 127, 0))
	{
		std::cout << "Incorrect input";
		return -1;
	}

	char arr[SIZE] = {};
	bool isValidArr = true;
	inputArray(arr, n, isValidArr);
	if (!(isValidArr))
	{
		std::cout << "Incorrect input";
		return -1;
	}
	
	std::cin >> s;
	for (int i = 0; i < s; i++)
	{
		int index = 0;
		char symbol = 0;
		std::cin >> index >> symbol;
		if (miss(n, index)) //if the array is empty, ball with index 0 will be added
			{
				continue;
			}
		if (isOutOfRange(symbol, 'z', 'a'))
		{
			std::cout << "Incorrect input";
			return -1;
		}

		addBallOnIndex(arr, n, index, symbol);

		if (moreThan3(arr, index))
		{
			distructionOnElements(arr, n, index, symbol);
			while (moreThan3OnBothSides(arr, index))
			{
				distructionOnElements(arr, n, index, symbol);
			}
		}
	}
	printArray(arr, n);
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
void inputArray(char array[], int const lenght, bool& isArrayValid)
{
	isArrayValid = true;
	for (int i = 0; i < lenght; i++)
	{
		std::cin >> array[i];
		if (isOutOfRange(array[i], 'z', 'a'))
		{
			isArrayValid = false;
			return;
		}
	}
}
void printArray(char array[], int lenght)
{
	if (lenght == 0)
	{
		std::cout << "-1"; //if it's empty print -1
		return;
	}
	for (int i = 0; i < lenght; i++)
	{
		std::cout << array[i] << " ";
	}
}

bool miss(int lenghtArray, int index)
{
	if (lenghtArray != 0 && (index >= lenghtArray || index < 0) || (lenghtArray == 0 && index != 0))
	{
		return true;
	}
	else
		return false;
}

void addBallOnIndex(char array[], int& lenghtArray, int index, char symbol)
{
	lenghtArray++;
	for (int i = lenghtArray; i > index + 1; i--)
	{
		array[i] = array[i - 1];
	}
	array[index + 1] = symbol;
}

void distructionOnElements(char arr[], int& lenghtArray, int& index, char& symbol)
{
	int countOfRemovables = 0;
	for (int i = 1; arr[index + i] == symbol ; i++, countOfRemovables++)
	{
		arr[index + i] = -1; //changing the value of the elements for deleting to unreachable value (out of 'a' to 'z')
	}
	for (int i = 0; arr[index + i] == symbol; i--, countOfRemovables++)
	{
		arr[index + i] = -1;
	}

	int mostLeftIndex = 0;
	for (int i = 0; i < lenghtArray; i++)
	{
		if (arr[i] == -1)
		{
			mostLeftIndex = i;
			break;
		}
	}
	int mostRightIndexOutOfDelete = 0;
	for (int i = lenghtArray - 1; i >= 0; i--)
	{
		if (arr[i] == -1)
		{
			mostRightIndexOutOfDelete = i + 1;
			break;
		}
	}

	int countAfterRightIndex = lenghtArray - mostRightIndexOutOfDelete;
	for (int j = 0; j < countAfterRightIndex; j++)
	{
		arr[mostLeftIndex + j] = arr[mostRightIndexOutOfDelete + j];
	}
	lenghtArray -= countOfRemovables;
	index = mostLeftIndex - 1; //set-up for cyclicality effect
	symbol = arr[mostLeftIndex]; //set-up 
}

bool moreThan3(char array[], int index)
{
	if (
		(array[index] == array[index + 1] && array[index + 1] == array[index + 2]) ||
		(array[index + 1] == array[index + 2] && array[index + 2] == array[index + 3]) ||
		(array[index - 1] == array[index] && array[index] == array[index + 1])
		)
	{
		return true;
	}
	else
		return false;
}
bool moreThan3OnBothSides(char array[], int index)
{
	if (
		(array[index + 1] == array[index]) && 
		( (array[index] == array[index - 1]) || (array[index + 1] == array[index + 2]) )
		)
	{
		return true;
	}
	else
		return false;
}