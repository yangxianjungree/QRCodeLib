#pragma once 

#ifndef _SHOWBITS_H_
#define _SHOWBITS_H_

#include <iostream>
#include <iomanip>


//void showBits(const char *data);
//void showunBits(const unsigned char *data);

/*const char *source="11111111111111111111111111111110000000000000000000000000111111111111111111111111111111100000000000000000";
showunBits((unsigned char *)source);*/

void showBits(const char *data)
{
	using namespace std;

	cout << "bits:     ";
	for (int i = 0; i < 16; i++)
	{
		cout << setw(3) << hex << i;
	}
	
	int iiiii = 0;
	const char *source = data;
	while (*source != '\0')
	{
		if (iiiii % 16 == 0)
		{
			cout<< endl;
			cout << "OX" << setw(6) << setfill('0') << hex << iiiii / 16 <<"  ";
			
			cout << setfill(' ');	
		}
		else
		{
			cout << setfill(' ');
		}

		if ((*source)&1)
		{
			cout << setw(3) << '1';
		}
		else
		{
			cout << setw(3) << '0';
		}
		source++;
		iiiii++;
	}

}

void showunBits(const unsigned char *data)	
{
	using namespace std;

	cout << "bits:     ";
	for (int i = 0; i < 16; i++)
	{
		cout << setw(3) << hex << i;
	}
	
	int iiiii = 0;
	const unsigned char *source = data;
	while (*source != '\0')
	{
		if (iiiii % 16 == 0)
		{
			cout<< endl;
			cout << "OX" << setw(6) << setfill('0') << hex << iiiii / 16 <<"  ";
			
			cout << setfill(' ');	
		}
		else
		{
			cout << setfill(' ');
		}

		if ((*source)&1)
		{
			cout << setw(3) << '1';
		}
		else
		{
			cout << setw(3) << '0';
		}
		source++;
		iiiii++;
	}

}


#endif