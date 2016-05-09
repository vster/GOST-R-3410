// Файл GOST-main.cpp
// Контрольная реализация алгоритма ЭЦП ГОСТ Р 34.10-2012

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <ctime>
#include "zzhex.h"
#include "QxyNTL.h"
#include "G3410-2012.h"

using namespace std;
using namespace NTL;

const long MAXP = 200;
long L;

ZZ a, b, p, q; 

ZZ xp, yp;

struct DPar 
{
	char oid[20];
	long L_dec;
	char a_hex[MAXP];
	char b_hex[MAXP];
	char p_hex[MAXP];
	char q_hex[MAXP];
	char xp_hex[MAXP];
	char yp_hex[MAXP];
};

DPar dp[10] = {
	// RFC 4357
	// id-GostR3410-2001-TestParamSet	(0)
	// OID:1.2.643.2.2.35.0
	// a, b, p, q, xp, yp 
	{
	"1.2.643.2.2.35.0",
	256,
	"7",
	"5FBFF498AA938CE739B8E022FBAFEF40563F6E6A3472FC2A514C0CE9DAE23B7E",
	"8000000000000000000000000000000000000000000000000000000000000431",
	"8000000000000000000000000000000150FE8A1892976154C59CFC193ACCF5B3",
	"2",
	"08E2A8A0E65147D4BD6316030E16D19C85C97F0A9CA267122B96ABBCEA7E8FC8"
	},
	// id-GostR3410-2001-CryptoPro-A-ParamSet	(1) - ViPNet main
	// OID:1.2.643.2.2.35.1
	// a, b, p, q, xp, yp 
	{
	"1.2.643.2.2.35.1",
	256,
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD94",
	"166",
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD97",
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6C611070995AD10045841B09B761B893",
	"1",
	"8D91E471E0989CDA27DF505A453F2B7635294F2DDF23E3B122ACC99C9E9F1E14"
	},
	// id-GostR3410-2001-CryptoPro-B-ParamSet (2)
	// OID:1.2.643.2.2.35.2
	// a, b, p, q, xp, yp 
	{
	"1.2.643.2.2.35.2",
	256,
	"8000000000000000000000000000000000000000000000000000000000000C96",
	"3E1AF419A269A5F866A7D3C25C3DF80AE979259373FF2B182F49D4CE7E1BBC8B",
	"8000000000000000000000000000000000000000000000000000000000000C99",
	"800000000000000000000000000000015F700CFFF1A624E5E497161BCC8A198F",
	"1",
	"3FA8124359F96680B83D1C3EB2C070E5C545C9858D03ECFB744BF8D717717EFC"
	},
	// id-GostR3410-2001-CryptoPro-C-ParamSet (3)
	// OID:1.2.643.2.2.35.3
	// a, b, p, q, xp, yp 
	{
	"1.2.643.2.2.35.3",
	256,
	"9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D7598",
	"32858",
	"9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D759B",
	"9B9F605F5A858107AB1EC85E6B41C8AA582CA3511EDDFB74F02F3A6598980BB9",
	"0",
	"41ECE55743711A8C3CBF3783CD08C0EE4D4DC440D4641A8F366E550DFDB3BB67"
	},
	// id-GostR3410-2001-CryptoPro-XchA-ParamSet	(4)
	// OID:1.2.643.2.2.35.4
	// a, b, p, q, xp, yp 
	{
	"1.2.643.2.2.35.4",
	256,
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD94",
	"166",
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD97",
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6C611070995AD10045841B09B761B893",
	"1",
	"8D91E471E0989CDA27DF505A453F2B7635294F2DDF23E3B122ACC99C9E9F1E14"
	},
	// id-GostR3410-2001-CryptoPro-XchB-ParamSet	(5)
	// OID:1.2.643.2.2.35.5
	//  a, b, p, q, xp, yp
	{
	"1.2.643.2.2.35.5",
	256,
	"9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D7598",
	"32858",
	"9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D759B",
	"9B9F605F5A858107AB1EC85E6B41C8AA582CA3511EDDFB74F02F3A6598980BB9",
	"0",
	"41ECE55743711A8C3CBF3783CD08C0EE4D4DC440D4641A8F366E550DFDB3BB67"
	},
	// Параметры из контрольных примеров ГОСТ Р 34.10-2012
	// 512 bit			(6)
	// OID:1.2.643.7.1.2.1.2.0
	// a, b, p, q, xp, yp 
	{
	"1.2.643.7.1.2.1.2.0",
	512,
	"7",
	"1CFF0806A31116DA29D8CFA54E57EB748BC5F377E49400FDD788B649ECA1AC4361834013B2AD7322480A89CA58E0CF74BC9E540C2ADD6897FAD0A3084F302ADC",
	"4531ACD1FE0023C7550D267B6B2FEE80922B14B2FFB90F04D4EB7C09B5D2D15DF1D852741AF4704A0458047E80E4546D35B8336FAC224DD81664BBF528BE6373",
	"4531ACD1FE0023C7550D267B6B2FEE80922B14B2FFB90F04D4EB7C09B5D2D15DA82F2D7ECB1DBAC719905C5EECC423F1D86E25EDBE23C595D644AAF187E6E6DF",	
	"24D19CC64572EE30F396BF6EBBFD7A6C5213B3B3D7057CC825F91093A68CD762FD60611262CD838DC6B60AA7EEE804E28BC849977FAC33B4B530F1B120248A9A",
	"2BB312A43BD2CE6E0D020613C857ACDDCFBF061E91E5F2C3F32447C259F39B2C83AB156D77F1496BF7EB3351E1EE4E43DC1A18B91B24640B6DBB92CB1ADD371E"
	},
	// Параметры рабочей группы
	// 512 bit - A		(7)
	// OID:1.2.643.7.1.2.1.2.1
	// a, b, p, q, xp, yp 
	{
	"1.2.643.7.1.2.1.2.1",
	512,
	// a = -3
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC4",
	"E8C2505DEDFC86DDC1BD0B2B6667F1DA34B82574761CB0E879BD081CFD0B6265EE3CB090F30D27614CB4574010DA90DD862EF9D4EBEE4761503190785A71C760",
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC7",
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF27E69532F48D89116FF22B8D4E0560609B4B38ABFAD2B85DCACDB1411F10B275",
	"3",
	"7503CFE87A836AE3A61B8816E25450E6CE5E1C93ACF1ABC1778064FDCBEFA921DF1626BE4FD036E93D75E6A50E3A41E98028FE5FC235F5B889A589CB5215F2A4"
	},
	// 512 bit - B		(8) - ViPNet main
	// OID:1.2.643.7.1.2.1.2.2
	// a, b, p, q, xp, yp 
	{
	"1.2.643.7.1.2.1.2.2",
	512,
	//  a = -3
	"8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006C",
	"687D1B459DC841457E3E06CF6F5E2517B97C7D614AF138BCBF85DC806C4B289F3E965D2DB1416D217F8B276FAD1AB69C50F78BEE1FA3106EFB8CCBC7C5140116",
	"8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006F",
	"800000000000000000000000000000000000000000000000000000000000000149A1EC142565A545ACFDB77BD9D40CFA8B996712101BEA0EC6346C54374F25BD",
	"2",
	"1A8F7EDA389B094C2C071E3647A8940F3C123B697578C213BE6DD9E6C8EC7335DCB228FD1EDF4A39152CBCAAF8C0398828041055F94CEEEC7E21340780FE41BD"
	}

};

char d_hex[2][MAXP] = 
{
	"7a929ade789bb9be10ed359dd39a72c11b60961f49397eee1d19ce9891ec3b28",
	"02951b1e41bddd16b2b549a64e0e676cf77b1cac3965194cf59e7c73fa680c73"
};

char h_hex[2][MAXP] = 
{
	"2dfbc1b372d89a1188c09c52e0eec61fce52032ab1022e8e67ece6672b043ee5",
	"d3f04d9ff1694c1cc890e3febb7702342cb787f1b3f01a4cf2cae3c488ba7a0f"
};
	
char k_hex[MAXP] = "77105C9B20BCD3122823C8CF6FCC7B956DE33814E95B7FE64FED924594DCEAB3";

//======================================
void line()
{
	cout << endl;	
	for (int i = 0; i < 79; i++) cout << "=";
	cout << endl;
}
//======================================
void GetDP( long dpid )
{
	L = dp[dpid].L_dec;
	ZZFromHex (a, dp[dpid].a_hex);
	ZZFromHex (b, dp[dpid].b_hex);
	ZZFromHex (p, dp[dpid].p_hex);
	ZZFromHex (q, dp[dpid].q_hex);
	ZZFromHex (xp, dp[dpid].xp_hex);
	ZZFromHex (yp, dp[dpid].yp_hex);
}
//======================================
void ShowDP()
{
	// cout << "\na (dec) = \n" << a << endl;
	cout << "\na (hex) = \n"; ShowZZInHex (a);	cout << endl;

	// cout << "\nb (dec) = \n" << b << endl;
	cout << "\nb (hex) = \n"; ShowZZInHex (b);	cout << endl;

	// cout << "\np (dec) = \n" << p << endl;
	cout << "\np (hex) = \n"; ShowZZInHex (p);	cout << endl;

	// cout << "\nq (dec) = \n" << q << endl;		
	cout << "\nq (hex) = \n"; ShowZZInHex (q);	cout << endl;

	// cout << "\nxp (dec) = \n" << xp << endl;		
	cout << "\nxp (hex) = \n"; ShowZZInHex (xp);	cout << endl;

	// cout << "\nyp (dec) = \n" << yp << endl;		
	cout << "\nyp (hex) = \n"; ShowZZInHex (yp);	cout << endl;
}

//======================================
int main(int argc, char* argv[])
{
		
	//======================================
	line();

	cout << "\nDomain parameters:\n";

	long dpid;		// id of params set
	
	dpid = 1;

	if (argc == 2)
		dpid = atoi(argv[1]);	
	
	cout << "\nOID : " << dp[dpid].oid << endl;

	// Set domain params
	GetDP( dpid );
	
	// Show domain params
	// ShowDP();

	ZZ_p::init(p);

	Qxy P;

	P.getQxy(xp, yp);

	// cout << "\nBase point P:\n";
	// P.putQxy();
	// cout << endl;

	//======================================
	// line();

	// Тест параметров ЭК
	// testparams ( P, q );

	//======================================
	// line();

	// Генерация ключей
	cout << "\nSignature and verification keys.\n";
	ZZ d;				// Ключ	подписи 1 < d < q-1
	Qxy Q;				// Ключ проверки
	// Qxy Qt;				// Проверка генерации открытого ключа

	ZZFromHex (d, d_hex[0]);	

	genkeys ( d, Q, P, q );
	
	//======================================
	line();

	// Генерация ЭЦП
	
	cout << "\nSignature process.\n";	
	
	ZZ h, e;

	// Hash-code
	ZZFromHex (h, h_hex[0]);
	
	cout << "\nHash-code h:\n";
	// cout << "\nh (dec) = \n" << h << endl;
	cout << "\nh (hex) = \n"; ShowZZInHex (h);	cout << endl;

	e = h%q;
	// RandomBnd(e, q-1);		// Хэш
	if ( e == 0 )
		e = 1;
	
	cout << "\nHash-code e:\n";
	// cout << "\ne (dec) = \n" << e << endl;
	cout << "\ne (hex) = \n"; ShowZZInHex (e);	cout << endl;

	ZZ r, s, k;
	
	ZZFromHex (k, k_hex);

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