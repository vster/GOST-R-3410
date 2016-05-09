#include <NTL/ZZ.h>
#include <iomanip>
#include <sstream>
#include "zzhex.h"

int IntFromChar(char x)
{
	char chtab1[16] =				
		{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	char chtab2[16] =				
		{'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	
	for (int i = 0; i < 16; i++)
		if ( (x == chtab1[i]) || (x == chtab2[i]) )
			return i;
}

void BytesFromHex(byte bt[], char s[])
{
	long rs;
	rs = strlen(s);

	for (size_t i = 0; i < rs/2; i++)
		swap(s[i],s[rs-1-i]);
	
	for (long i = 0; i < rs; i++)
		if ( i%2 == 0 )
			bt[i/2] = IntFromChar(s[i]);
		else
			bt[i/2] += IntFromChar(s[i])*16;

	for (size_t i = 0; i < rs/2; i++)
		swap(s[i],s[rs-1-i]);
}

void HexFromBytes( char s[], byte bt[], int num )
{
	stringstream ss;
	for (long i = num-1; i >= 0; i--)
		ss << hex << setw(2) << setfill('0') 
			 << (int)bt[i];
	ss >> s;
}

void ZZFromHex(ZZ &a, char s[])
{
	byte bt[MAXB];
	long num;
	long slen = strlen(s);
	num = slen%2 ? slen/2+1 : slen/2;	

	BytesFromHex(bt, s);
	ZZFromBytes(a, bt, num);
}

void ShowZZInHex ( ZZ &a )
{
	byte bt[MAXB];
	
	long num = NumBytes(a);

	BytesFromZZ(bt, a, num);
	for (long i = num-1; i >= 0; i--)
		cout << hex << setw(2) << setfill('0') 
			 << (int)bt[i];
}

