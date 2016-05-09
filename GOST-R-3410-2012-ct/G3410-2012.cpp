// Файл G3410-2012.cpp
// Контрольная реализация алгоритма ЭЦП ГОСТ Р 34.10-2012

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <ctime>
#include "zzhex.h"
#include "QxyNTL.h"
#include "G3410-2012.h"

extern ZZ p; 
extern ZZ a;
extern ZZ b;	

// Генерация параметров ЭК
void genparams ( Qxy &P, ZZ &q, ZZ &bpn )
{
	ZZ i;
	ZZ x, y;
	ZZ f1, f2;

	for ( x = 0, i = 0; x < p; x++)
		for ( y = 0; y < p; y++)
		{
			f1 = PowerMod(y, 2, p);
			f2 = (PowerMod(x, 3, p) + a * x + b)%p;
			if ( f1 == f2 )
			{
				if ( i == bpn )
					P.getQxy(x,y);
				i++;
			}
		}
	q = i+1;
	long NumTrials = 10;
	if ( !ProbPrime( q, NumTrials) )
		cout << "\nq is not prime! Please, change params (p,a,b)." << endl;
	
	cout << "\nDomain parameters\n" << endl;
	cout << "\np = " << p;
	cout << "\na = " << a;
	cout << "\nb = " << b;
	cout << "\nbpn = " << bpn << endl;
	cout << "\nq = " << q << endl;
	cout << "\nP = "; P.putQxy(); cout << endl;
}

// Генерация параметров ЭК
// Порядок ЭК q
void genparam_q ( Qxy &P, ZZ &q )
{
	ZZ i;
	ZZ x, y;
	ZZ f1, f2;

	for ( x = 0, i = 0; x < p; x++)
		for ( y = 0; y < p; y++)
		{
			f1 = PowerMod(y, 2, p);
			f2 = (PowerMod(x, 3, p) + a * x + b)%p;
			if ( f1 == f2 ) i++;	
		}
	q = i+1;

	long NumTrials = 10;
	if ( !ProbPrime( q, NumTrials) )
		cout << "\nq is not prime! Please, change params (p,a,b)." << endl;
	
	cout << "\np = " << p;
	cout << "\na = " << a;
	cout << "\nb = " << b;
	cout << "\nq = " << q << endl;
}

// Генерация параметров ЭК
// Базовая точка P
void genparam_P ( Qxy &P, ZZ &q, ZZ &bpn )
{
	ZZ i;
	ZZ x, y;
	ZZ f1, f2;

	for ( x = 0, i = 0; x < p; x++)
		for ( y = 0; y < p; y++)
		{
			f1 = PowerMod(y, 2, p);
			f2 = (PowerMod(x, 3, p) + a * x + b)%p;
			if ( f1 == f2)
			{
				if (i == bpn)
				{ 
					P.getQxy(x,y);	
					goto out;
				}
				i++;
			}
		}
out:	{}
}

// Тест параметров ЭК
void testparams ( Qxy &P, ZZ &q)
{
	Qxy Q0;
	Q0.getQxy(conv<ZZ_p>(0),conv<ZZ_p>(0));
	// Тест: некая точка * порядок кривой = нулевая точка
	cout << "\nTesting of elliptic curve \n\n";
	Qxy TQ = P*q;		
	cout << "\nTest Q = ";
	TQ.putQxy();
	cout << "\nMust be (0,0)\n" << endl;
	if ( TQ == Q0 )
		cout << "Test of EC is OK\n";
	else
		cout << "Test of EC is FAILD\n";
}

// Генерация ключей 
void genkeys ( ZZ &d, Qxy &Q, Qxy &P, ZZ &q ) 
{
	// RandomBnd(d, q-1);				// Закрытый ключ	1 < d < q-1
	Q = P*d;			// Ключ проверки подписи
	cout << "\nSignature key d:";
	// cout <<	"\nd (dec) = \n" << d << endl;
	cout << "\nd (hex) = \n"; ShowZZInHex (d);	cout << endl;
	cout << "Verification key Q:\n";
	Q.putQxy();
	cout << endl;
}

// Процедура подписывания
void gensign (ZZ &r, ZZ &s, ZZ &k, ZZ &e, ZZ &d, Qxy &P, ZZ &q)
{
	//ZZ k;
	Qxy C;

	while ( true )
	{
		// RandomBnd(k, q-1);
		if ( GCD(k,q) > 1 )
			continue;
		C = P*k; 
		r = (conv<ZZ>(C.putx()))%q;
		if ( r == 0)
			continue;
		
		s = (r*d + k*e)%q;
		if ((r>0)&(s>0))
			break;
	}
	
	// cout << "\nk (dec) = \n" << k << endl;
	cout << "\nk (hex) = \n"; ShowZZInHex (k);	cout << endl;

	cout << "\nPoint C:\n";
	C.putQxy();

	cout << "\nSignature:\n";
	
	// cout << "\nr (dec) = \n" << r << endl;
	cout << "\nr (hex) = \n"; ShowZZInHex (r);	cout << endl;

	// cout << "\ns (dec) = \n" << s << endl;
	cout << "\ns (hex) = \n"; ShowZZInHex (s);	cout << endl;
}

// Процедура проверки подписи
void verifysign (ZZ &R, ZZ &r, ZZ &s, ZZ &e, Qxy &Q, Qxy &P, ZZ &q  )
{
	ZZ v, z1, z2;
	Qxy C;
	ZZ e1;
	if (InvModStatus( v, e, q ))
	{
		cout << "\nError in signature\n";
		goto end;
	}	
	v = InvMod(e,q);
	z1 = (s*v)%q;
	z2 = (-r*v)%q;
	C = P*z1 + Q*z2; 
	R = (conv<ZZ>(C.putx()))%q;

	// cout << "\nv (dec) = \n" << v << endl;
	cout << "\nv (hex) = \n"; ShowZZInHex (v);	cout << endl;

	// cout << "\nz1 (dec) = \n" << z1 << endl;
	cout << "\nz1 (hex) = \n"; ShowZZInHex (z1);	cout << endl;
	
	// cout << "\nz2 (dec) = \n" << z2 << endl;
	cout << "\nz2 (hex) = \n"; ShowZZInHex (z2);	cout << endl;

	cout << "\nPoint C:\n";
	C.putQxy();

	// cout << "\nR (dec) = \n" << R << endl;
	cout << "\nR (hex) = \n"; ShowZZInHex (R);	cout << endl;

end:
	if ( r == R )
		cout << "\nr = R\nSignature is OK";
	else
		cout << "\nSignature is FAILD";
	cout << endl;
}
