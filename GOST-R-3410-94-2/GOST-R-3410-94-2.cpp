// Реализация криптоалгоритма ГОСТ Р 34.10-94

#include <NTL/ZZ.h>
#include <ctime>
#include "convhex.h"

using namespace std;
using namespace NTL;

const long N = 256;			// q
const long L = 512;			// p


char p_hex_be[] = "EE8172AE8996608FB69359B89EB82A69854510E2977A4D63BC97322CE5DC3386EA0A12B343E9190F32177539845839786BB0C345D165976EF2195EC9B1C379E3";
char q_hex_be[] = "98915E7EC8265EDFCDA31E88F24809DDB064BDC7285DD50D7289F0AC6F49DD2D";
char a_hex_be[] = "9E96031500C8774A86958D4AFDE2127AFAD2538B4B6270A6F7C8837B50D50F206755984A49E509304D648BE2AB5AAB18EBE2CD46AC3D8495B142AA6CE23E21C";

void genparams( ZZ &p, ZZ &q, ZZ &a)
{

	long err = 80;	
	
	// GenPrime(q, N, err);
	
	get_dec_from_hex (q, q_hex_be, N);
	// get_dec_from_hex (p, p_hex_be, N);
	
	ZZ m;
	RandomLen(m, L-N);

	cout << "\nGenerating p, q and a...\n";

	long NumTrials = 20;
	for (long i = 0; i < 10000; i++, m++)
	{
		p = q * m + 1;
		
		if (ProbPrime(p, NumTrials))
		{
			cout << "\ni = " << i << endl;
			cout << "OK" << endl;
			break;
		}
	}
	// get_dec_from_hex (p, p_hex_be, L);

	cout << "\np = \n"; show_dec_in_hex (p, L);	cout << endl;
	cout << "\nq = \n"; show_dec_in_hex (q, N);	cout << endl;
	
	ZZ d, f;
//	ZZ f1 = ((p-1) * InvMod(q, p)) % p;
	ZZ f1 = m;	// = (p-1)/q
//	ZZ f1 = (p-1)/q;
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
	ZZ r = (p-1)%q;
	cout << "\nr = \n"; show_dec_in_hex (r, L);	cout << endl;

//	cout << "\nd = \n"; show_dec_in_hex (d, L-N);	cout << endl;		
//	cout << "\nf = \n"; show_dec_in_hex (f, L);	cout << endl;	
//	cout << "\nf1 = \n"; show_dec_in_hex (f1, L);	cout << endl;
//	cout << "\nf2 = \n"; show_dec_in_hex (f2, L);	cout << endl;
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

int main()
{
	ZZ p;	// Модуль
	ZZ q;	// Простое число, делитель (p-1)
	ZZ a;

	time_t now = time(0);
	ZZ seed = conv<ZZ>(difftime(now,0));
	SetSeed( seed ); 

	
	genparams( p, q, a );
	
	// Генерация ключей
	ZZ x, y;
	genkeys ( x, y, p, q, a );

	ZZ H;
	RandomBnd(H, q-1);
	cout << "\nH = \n"; show_dec_in_hex (H, N);	cout << endl;

	// Генерация ЭЦП
	ZZ r, s;					
	gensign(r, s, H, x, p, q, a);

	// Проверка ЭЦП	
	ZZ u;
	verifysign ( u, r, s, H, y, p, q, a );

	return 0;
}