// Файл GOST-main.cpp
// Контрольная реализация алгоритма ЭЦП ГОСТ Р 34.10-2001 
// Применение параметров домена из RFC 4357

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <ctime>
#include "zzhex.h"
#include "QxyNTL.h"
#include "G3410-2001.h"

using namespace std;
using namespace NTL;

const long MAXP = 100;
long L;

ZZ p, a, b; 

struct DPar 
{
	long L_dec;
	char a_hex[MAXP];
	char b_hex[MAXP];
	char p_hex[MAXP];
	char q_hex[MAXP];
	char xp_hex[MAXP];
	char yp_hex[MAXP];
};

DPar dp[10] = {
	// id-GostR3410-2001-TestParamSet	(0)
	// a, b, p, q, xp, yp 
	{
	256,
	"7",
	"5FBFF498AA938CE739B8E022FBAFEF40563F6E6A3472FC2A514C0CE9DAE23B7E",
	"8000000000000000000000000000000000000000000000000000000000000431",
	"8000000000000000000000000000000150FE8A1892976154C59CFC193ACCF5B3",
	"2",
	"08E2A8A0E65147D4BD6316030E16D19C85C97F0A9CA267122B96ABBCEA7E8FC8"
	},
	// id-GostR3410-2001-CryptoPro-A-ParamSet	(1)
	// a, b, p, q, xp, yp 
	{
	256,
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD94",
	"166",
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD97",
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6C611070995AD10045841B09B761B893",
	"1",
	"8D91E471E0989CDA27DF505A453F2B7635294F2DDF23E3B122ACC99C9E9F1E14"
	},
	// id-GostR3410-2001-CryptoPro-B-ParamSet (2)
	// a, b, p, q, xp, yp 
	{
	256,
	"8000000000000000000000000000000000000000000000000000000000000C96",
	"3E1AF419A269A5F866A7D3C25C3DF80AE979259373FF2B182F49D4CE7E1BBC8B",
	"8000000000000000000000000000000000000000000000000000000000000C99",
	"800000000000000000000000000000015F700CFFF1A624E5E497161BCC8A198F",
	"1",
	"3FA8124359F96680B83D1C3EB2C070E5C545C9858D03ECFB744BF8D717717EFC"
	},
	// id-GostR3410-2001-CryptoPro-C-ParamSet (3)
	// a, b, p, q, xp, yp 
	{
	256,
	"9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D7598",
	"32858",
	"9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D759B",
	"9B9F605F5A858107AB1EC85E6B41C8AA582CA3511EDDFB74F02F3A6598980BB9",
	"0",
	"41ECE55743711A8C3CBF3783CD08C0EE4D4DC440D4641A8F366E550DFDB3BB67"
	},
	// id-GostR3410-2001-CryptoPro-XchA-ParamSet	(4)
	// a, b, p, q, xp, yp 
	{
	256,
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD94",
	"166",
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD97",
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6C611070995AD10045841B09B761B893",
	"1",
	"8D91E471E0989CDA27DF505A453F2B7635294F2DDF23E3B122ACC99C9E9F1E14"
	},
	// id-GostR3410-2001-CryptoPro-XchB-ParamSet	(5)
	//  a, b, p, q, xp, yp
	{
	256,
	"9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D7598",
	"32858",
	"9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D759B",
	"9B9F605F5A858107AB1EC85E6B41C8AA582CA3511EDDFB74F02F3A6598980BB9",
	"0",
	"41ECE55743711A8C3CBF3783CD08C0EE4D4DC440D4641A8F366E550DFDB3BB67"
	}

};	 
	 
//======================================
void line()
{
	cout << endl;	
	for (int i = 0; i < 79; i++) cout << "=";
	cout << endl;
}

int main(int argc, char* argv[])
{
	ZZ q;
	ZZ bpn;


	//======================================
	line();

	cout << "\nDomain parameters:\n";
	long id = 0;		// id of params set
	if (argc == 2)
		id = atoi(argv[1]);	
	L = dp[id].L_dec;

	p.SetSize(L);
	b.SetSize(L);
	
	// p = conv<ZZ> (p_dec);
	ZZFromHex (p, dp[id].p_hex);
	cout << "\np (dec) = \n" << p << endl;
	cout << "\np (hex) = \n"; ShowZZInHex (p);	cout << endl;

	// a = conv<ZZ> (a_dec);
	ZZFromHex (a, dp[id].a_hex);
	cout << "\na (dec) = \n" << a << endl;
	cout << "\na (hex) = \n"; ShowZZInHex (a);	cout << endl;

	// b = conv<ZZ> (b_dec);
	ZZFromHex (b, dp[id].b_hex);
	cout << "\nb (dec) = \n" << b << endl;
	cout << "\nb (hex) = \n"; ShowZZInHex (b);	cout << endl;

	// q = conv<ZZ> (q_dec);
	ZZFromHex (q, dp[id].q_hex);
	cout << "\nq (dec) = \n" << q << endl;		
	cout << "\nq (hex) = \n"; ShowZZInHex (q);	cout << endl;

	
	ZZ_p::init(p);

	Qxy P;
	ZZ xp;
	ZZ yp;
	ZZFromHex (xp, dp[id].xp_hex);
	ZZFromHex (yp, dp[id].yp_hex);

	P.getQxy(xp, yp);

	cout << "\nBase point P:\n";
	P.putQxy();
	cout << endl;

	//======================================
	line();


	// Тест параметров ЭК
	testparams ( P, q );

	//======================================
	line();

	// Генерация ключей
	cout << "\nSignature and verification keys.\n";
	ZZ d;				// Ключ	подписи 1 < d < q-1
	Qxy Q;				// Ключ проверки
	Qxy Qt;				// Проверка генерации открытого ключа
	ZZ xq, yq;

	genkeys ( d, Q, P, q );
	
	cout << endl;

	//======================================
	line();

	// Генерация ЭЦП
	
	cout << "\nSignature process.\n";	
	
	ZZ e;
	// e =conv<ZZ>(e_dec);
	RandomBnd(e, q-1);		// Хэш
	if ( e == 0 )
		e = 1;
	
	cout << "\nHash-code e:\n";
	cout << "\ne (dec) = \n" << e << endl;
	cout << "\ne (hex) = \n"; ShowZZInHex (e);	cout << endl;

	ZZ r, s, k;
	
	gensign ( r, s, k, e, d, P, q );

	//======================================
	line();

	// Проверка ЭЦП
	cout << "\nVerification process.\n";	
	ZZ R;
	verifysign ( R, r, s, e, Q, P, q  );

	//======================================
	line();

	return 0;
}