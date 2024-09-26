/**
*
* Solution to homework assignment 1
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Georgi Hristov Lazov
* @idnumber 0MI0600299
* @task 4
*
*/
#include <iostream>
const short maxIntervalPossible = 365;
const short minIntervalPossible = 1;
const short yearJulianToGrigorianCal = 1916;

bool isOutOfRange(int number, int max, int min);

bool isStartDateAfterEndDate(int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay);

bool isLeapYear(int year);

int daysInMonth(int year, int month);

bool areEntryDatesCorrect(int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay);

void increaseDayBy1(int& year, int& month, int& day);

void smallerDatesPerInterval(int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, int interval);

int main()
{
	int startYear, startMonth, startDay, endYear, endMonth, endDay, interval = 0;
	std::cin >> startYear >> startMonth >> startDay >> endYear >> endMonth >> endDay >> interval;
	if (
		isOutOfRange(interval, maxIntervalPossible, minIntervalPossible) ||
		!(areEntryDatesCorrect(startYear, startMonth, startDay, endYear, endMonth, endDay)) ||
		isStartDateAfterEndDate(startYear, startMonth, startDay, endYear, endMonth, endDay)
		)
	{
		std::cout << "Incorrect input";
		return -1;
	}
	smallerDatesPerInterval(startYear, startMonth, startDay, endYear, endMonth, endDay, interval);
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

bool isStartDateAfterEndDate(int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay)
{
	
	if (startYear > endYear)
	{
		return true;
	}
	else if (endYear == startYear)
	{
		if (startMonth > endMonth)
		{
			return true;
		}
		else if (startMonth == endMonth)
		{
			if (startDay > endDay)
			{
				return true;
			}
		}
	}
	return false;

}

bool isLeapYear(int year)
{
	if ( year%4==0 && (year <= yearJulianToGrigorianCal /*Julian*/||/*Grigorian*/ year%400 == 0|| year%100!=0)) //rules for leap year
	{
		return true;
	}
	else
	{
		return false;
	}
		
}
int daysInMonth(int year, int month)
{
	switch (month)
	{
	case 1: case 3: case 5: case 7: case 8: case 10: case 12: //jan march, may, juli, aug, oct, december have 31 days
		return 31;
	case 4: case 6: case 9: case 11: //april, june, september, november have 30 days
		return 30;
	case 2: //february has 29 days for leap year and 28 if year isn't leap
		if (isLeapYear(year))
		{
			return 29;
		}
		else
			return 28;
	}
}

bool areEntryDatesCorrect(int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay)
{
	if (
		(startYear== yearJulianToGrigorianCal && startMonth== 4 && !(isOutOfRange(startDay, 13, 1))) || //01.04.1916 - 13.04.1916 don't exist - can't be startDate
		(endYear == yearJulianToGrigorianCal && endMonth == 4 && !(isOutOfRange(endDay, 13, 1))) || //the same for endDate
		isOutOfRange(startYear, 2100, 1600) || //2100 max year and 1600 min year, limitation
		isOutOfRange(endYear, 2100, 1600) ||
		isOutOfRange(startMonth, 12, 1) || // max-december, min-january
		isOutOfRange(endMonth, 12, 1) ||
		isOutOfRange(startDay, daysInMonth(startYear, startMonth), 1) || //days in month can be 28, 29, 30 or 31 depends on the month
		isOutOfRange(endDay, daysInMonth(endYear, endMonth), 1)
		)
	{
		return false;
	}
	else
		return true;
}

void increaseDayBy1(int& year, int& month, int& day)
{
	if (year==1916 && month == 3 && day == 31) //if the date is 31.03.1916, the next day is 14.04.1916 due to the difference of the Julian and Grigorian calendar
	{
		month = 4;
		day = 14;
		return;
	}
	if (day < daysInMonth(year, month))
	{
		day++;
	}
	else
	{
		day = 1; //from the last day of the previous month
		if (month < 12)
		{
			month++;
			
		}
		else
		{
			year++;
			month = 1;  //from the last month of the previous year
		}
	}
}

void smallerDatesPerInterval(int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, int interval)
{
	while (!(isStartDateAfterEndDate(startYear, startMonth, startDay, endYear, endMonth, endDay)))
	{
		std::cout << startYear << "-" << startMonth << "-" << startDay; //leftDate
		for (int i = 1; i < interval; i++) 
		{
			if (startYear == endYear && startMonth == endMonth && startDay == endDay) //the last date
			{
				break;
			}
			increaseDayBy1(startYear, startMonth, startDay);


		}
		std::cout << " - " << startYear << "-" << startMonth << "-" << startDay << std::endl; //rightDate
		increaseDayBy1(startYear, startMonth, startDay);
	}
}