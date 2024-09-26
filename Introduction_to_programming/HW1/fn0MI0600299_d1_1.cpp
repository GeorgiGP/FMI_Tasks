/**
*
* Solution to homework assignment 1
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

bool isWrongEntry(int n);

int dfunction(int num);

int countOfPairsXY(int num, int k);

int main()
{
	int num, coefficient = 0;
	std::cin >> num >> coefficient;
	if (isWrongEntry(num)||isWrongEntry(coefficient))
	{
		std::cout << "Incorrect input";
		return -1;
	}
	std::cout << countOfPairsXY(num, coefficient);
	return 0;
}

bool isWrongEntry(int n)
{
	if (n > 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int dfunction(int num) //Our func d(x) which represents how many divisors has a number
{
	int countOfDiv = 0;
	for (int i = 1; i <= num; i++)
	{
		if (num%i == 0)
		{
			countOfDiv += 1;
		}
	}
	return countOfDiv;
}

int countOfPairsXY(int num, int k) //our condition is "k.d(x).d(y)=x.y" and "1 <= x <= y <= n"
{
	int countTrueCondit = 0;
	for (int y = 1; y <= num; y++)
	{
		for (int x = 1; x <= y; x++)
		{
			if (k*dfunction(x)*dfunction(y) == x * y)
			{
				countTrueCondit += 1;
			}
		}
	}
	return countTrueCondit;
}



