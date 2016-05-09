#include <NTL/ZZ.h>
#include <ctime>
#include "convhex.h"
#include "G3410-94.h"

extern long N;			// q
extern long L;			// p

void genparams( ZZ &p, ZZ &q, ZZ &a)
{

	long err = 80;	
	GenPrime(q, N, err);

	ZZ m;
	RandomLen(m, L-N);

	cout << "\nGenerating p, q and a...\n";
	long NumTrials = 20;
	for (long i = 0; i < 10000; i++, m++)
	{
		p = q * m + 1;
		
		if (ProbPrime(p, NumTrials))
		{
			// cout << "\ni = " << i << endl;
			// cout << "OK" << endl;
			break;
		}
	}

	ZZ d, f;
//	ZZ f1 = ((p-1) * InvMod(q, p)) % p;
	ZZ f1 = m;	// = (p-1)/q
	for( d = 2; a == 0; d++ )
	{
		f = PowerMod(d%p, f1%p, p);
		if ( f >  1 )
		{	
			a = f;
			// cout << a << " ";
			break;
		}
	}
	cout << "\np = \n"; show_dec_in_hex (p, L);	cout << endl;
	cout << "\nq = \n"; show_dec_in_hex (q, N);	cout << endl;
	cout << "\na = \n"; show_dec_in_hex (a, L);	cout << endl;
}
// Генерация ключей
void genkeys ( ZZ &x, ZZ &y, ZZ &p, ZZ &q, ZZ &a)
{
	RandomBnd(x, q-1);			// Секретный ключ 
	PowerMod(y, a%p, x%p, p);	// Открытый ключ

	cout << "\nSecret key x = \n"; show_dec_in_hex (x, N);	cout << endl;
	cout << "\nPublic key y = \n"; show_dec_in_hex (y, L);	cout << endl;
}

//  Генерация ЭЦП
void gensign ( ZZ &r, ZZ &s, ZZ &H, ZZ &x, ZZ &p, ZZ &q, ZZ &a)
{
	ZZ k;
	while ( s == 0 )
	{
		RandomBnd(k, q);			// Случайное число k
		r = PowerMod(a%p, k%p, p) % q;
		if ( r == 0 )
			continue;
		s = (x * r + k * H ) % q;
		if ( s == 0 )
			continue;
	}
	cout << "\nGenerate sign\n";
	cout << "\nSession key k = \n"; show_dec_in_hex (H, N);	cout << endl;
	cout << "\nr = \n"; show_dec_in_hex (r, N);	cout << endl;
	cout << "\ns = \n"; show_dec_in_hex (s, N);	cout << endl;
}
// Проверка ЭЦП
void verifysign ( ZZ &u, ZZ &r, ZZ &s, ZZ &H, ZZ &y, ZZ &p, ZZ &q, ZZ &a)
{
	ZZ v, z1, z2;
	v = PowerMod(H, (q-2), q);
	z1 = (s * v) % q;
	z2 = (SubMod(q, r%q, q) * v) % q;
	u = ((PowerMod(a, z1, p) * PowerMod(y, z2, p)) % p) % q;
	
	cout << "\nCheck sign\n";
	cout << "\nv = \n"; show_dec_in_hex (v, N);	cout << endl;
	cout << "\nz1 = \n"; show_dec_in_hex (z1, N);	cout << endl;
	cout << "\nz2 = \n"; show_dec_in_hex (z2, N);	cout << endl;
	cout << "\nu = \n"; show_dec_in_hex (u, N);	cout << endl;
	
	cout << endl;
	if ( u == r )
		cout << "u = r Sign is OK" << endl;
	else
		cout << "Sign is FAILED" << endl;
}
