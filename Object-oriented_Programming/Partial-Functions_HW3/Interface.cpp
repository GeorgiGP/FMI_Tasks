#include "Interface.h"

void calculateAndPrint(const PartFunc& function, int32_t value)
{
	//std::cout << "f(" << value << ") = " << function(value) << std::endl; // Option 1 (if not defined at point PairFunc has printing NOT_DEFINED)


	if (function(value).isDefined())                                          // Option 2 (not printing at undefined points)
	{                                                                         //
		std::cout << "f(" << value << ") = " << function(value) << std::endl; //
	}                                                                         //
}

void calculateBetweenAndPrint(const PartFunc& function, int32_t lowerBorder, int32_t upperBorder)
{
	if (upperBorder < lowerBorder)
	{
		mySwap<int32_t>(lowerBorder, upperBorder);
	}
	for (int32_t i = lowerBorder; i <= upperBorder; i++)
	{
		calculateAndPrint(function, i);
	}
}

void printOptions()
{
	using std::cout;
	using std::cin;
	using std::endl;

	cout << "Choose working option:" << endl;
	cout << "Option 1: print f(x), where x is between [a,b]." << endl;
	cout << "Option 2: print f(x), where x is one value" << endl;
	cout << "Option 3: exit" << endl;
}

void mainMenu()
{
	PartFunc* function = partFuncFactory(); //not nullptr, because 5 types defined returns partFuncDactory(), else throw exception
	bool flag = true;
	while (flag)
	{
		int option = 0;
		printOptions();
		std::cin >> option;
		std::cin.clear(); //not to break when typing big nums or char
		std::cin.ignore(); //
		
		switch (option)
		{
		case 1:
			int32_t a, b;
			std::cout << "Type a and b:" << std::endl;
			std::cin >> a >> b;
			calculateBetweenAndPrint(*function, a, b); //we can deref, not nullptr
			break;
		case 2:
			int32_t value;
			std::cout << "Type value:" << std::endl;
			std::cin >> value;
			calculateAndPrint(*function, value);  //we can deref, not nullptr
			break;
		case 3:
			flag = false;
			break;
		default:
			std::cout << "No such option, try again." << std::endl;
			break;
		}
	}
	delete function;
}
