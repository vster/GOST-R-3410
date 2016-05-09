#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include <NTL/GF2.h>
#include <NTL/vec_GF2.h>
#include "convhex.h"

using namespace std;
using namespace NTL;

char chtab[16] =				
{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


// Convert dec (ZZ) to bin (GF2 vector)
void dec_to_bin (vec_GF2 &x_bin, ZZ &x, long len)
{
	for (long i = 0; i < len; i++)
		x_bin[i] = bit(x, i);
}

// Convert bin (GF2 vector) to dec (ZZ)
void bin_to_dec (ZZ &x, vec_GF2 &x_bin, long len)
{
	for (long i = 0; i < len; i++)
		if (x_bin[i] == 1) 
			SetBit(x, i);
}

// Convert bin (GF2 vector) to hex (char [])
void bin_to_hex ( char x_hex[], vec_GF2 &x_bin, long len)
{
	vec_GF2 btab[16];
	long i, j, k;
	long hexlen = len/4;
	vec_GF2 buf_bin;
	buf_bin.SetLength(4);

	for (k = 0; k < 16; k++)
	{
		btab[k].SetLength(4);
		for ( i = 0; i < 4; i++)
			btab[k][i] = bit(k,i);		
	}

	// char chtab[16] =				
	// {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	for (j = 0; j < hexlen; j++)
	{
		x_hex[j] = '0';
		
		for ( i = 0; i < 4; i++)
			buf_bin[i] = x_bin[4*j + i];

		for ( k = 0; k < 16; k++)
			if (buf_bin == btab[k])
			{
				x_hex[j] = chtab[k];
				break;
			}
	}
}

// Convert hex (char []) to bin (GF2 vector)
void hex_to_bin ( vec_GF2 &x_bin, char x_hex[], long len)
{
	vec_GF2 btab[16];
	long i, j, k;
	long hexlen = len/4;
	vec_GF2 buf_bin;

	for (k = 0; k < 16; k++)
	{
		btab[k].SetLength(4);
		for ( i = 0; i < 4; i++)
			btab[k][i] = bit(k,i);		
	}

	// char chtab[16] =				
	// {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	
	for (j = 0; j < hexlen; j++)
	{
		for ( k = 0; k < 16; k++)
			if (x_hex[j] == chtab[k])
			{
				append(buf_bin, btab[k]);
				break;
			}
	}
	x_bin = buf_bin;
}

// Convert dec (ZZ) to hex (char [])
void dec_to_hex(char x_hex[], ZZ &x, long len)
{
	vec_GF2 buf_bin;
	buf_bin.SetLength(len);
	
	dec_to_bin(buf_bin, x, len);
	bin_to_hex(x_hex, buf_bin, len);
}

// Convert hex (char []) to dec (ZZ)
void hex_to_dec(ZZ &x, char x_hex[], long len)
{
	vec_GF2 buf_bin;
	buf_bin.SetLength(len);
	
	hex_to_bin(buf_bin, x_hex, len);
	bin_to_dec(x, buf_bin, len);
}
// Show GF2 vector as binary number
void show_bin ( vec_GF2 &x_bin, long len )
{
	for (long i = len-1; i>=0; i--)
		cout << x_bin[i];
	cout << endl;
}
// Show string as hex number
// len - length in bits
void show_hex ( char x_hex[], long len )
{
	long hexlen = len/4;
	for (long i = hexlen-1;i >= 0; i--)
		cout << x_hex[i];
}
// Show big dec in hex 
void show_dec_in_hex ( ZZ &x, long len )
{
	char x_hex[maxhexlen];
	dec_to_hex( x_hex, x, len );
	show_hex (x_hex, len );
}
// Перевернуть массив из x_bend в x
// len - длина в битах
void reverse (char x[], char x_bend[], long len) 
{
	long hexlen = len/4;
	for (long i = 0; i < hexlen; i++)
		x[i] = x_bend[hexlen - 1 - i];

}
// Получить число ZZ из строки в формате big-endian
void get_dec_from_hex (ZZ &x, char x_bend[], long len )
{
	char x_hex[maxhexlen];
	reverse (x_hex, x_bend, len);
	hex_to_dec (x, x_hex, len);
}