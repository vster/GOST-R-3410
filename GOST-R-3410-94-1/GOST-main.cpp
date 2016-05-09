// Реализация криптоалгоритма ГОСТ Р 34.10-94

#include <NTL/ZZ.h>
#include <ctime>
#include "convhex.h"
#include "G3410-94.h"

using namespace std;
using namespace NTL;

long N = 256;			// q
long L = 1024;		// p


int main()
{
	time_t begin, end;
	double seconds;
	ZZ p;	// Модуль
	ZZ q;	// Простое число, делитель (p-1)
	ZZ a;

	time_t now = time(0);
	ZZ seed = conv<ZZ>(difftime(now,0));
	SetSeed( seed ); 

	begin = time(NULL);
	genparams( p, q, a );
	end = time(NULL);
	
	seconds = difftime(end, begin);
	cout << "\nGeneration time = " << seconds << " sec" << endl;

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