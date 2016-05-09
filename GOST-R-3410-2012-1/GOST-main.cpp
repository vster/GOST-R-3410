// Файл GOST-main.cpp
// Контрольная реализация алгоритма ЭЦП ГОСТ Р 34.10-2012

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <ctime>
#include "zzhex.h"
#include "convhex.h"
#include "QxyNTL.h"
#include "G3410-2012.h"

using namespace std;
using namespace NTL;

const long MAXP = 200;
long L;

ZZ a, b, p, q;

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

DPar dp[10] =
{
    // RFC 4357
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
    // id-GostR3410-2001-CryptoPro-A-ParamSet	(1) - ViPNet main
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
    },
    // Параметры рабочей группы
    // 512 bit - A		(6)
    // a, b, p, q, xp, yp
    {
        512,
        // a = -3
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC4",
        "E8C2505DEDFC86DDC1BD0B2B6667F1DA34B82574761CB0E879BD081CFD0B6265EE3CB090F30D27614CB4574010DA90DD862EF9D4EBEE4761503190785A71C760",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC7",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF27E69532F48D89116FF22B8D4E0560609B4B38ABFAD2B85DCACDB1411F10B275",
        "3",
        "7503CFE87A836AE3A61B8816E25450E6CE5E1C93ACF1ABC1778064FDCBEFA921DF1626BE4FD036E93D75E6A50E3A41E98028FE5FC235F5B889A589CB5215F2A4"
    },
    // 512 bit - B		(7) - ViPNet main
    {
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
//======================================
void line()
{
    cout << endl;
    for (int i = 0; i < 79; i++) cout << "=";
    cout << endl;
}

int main()
{
    // ZZ q;
    // ZZ bpn;


    //======================================
    line();

    cout << "\nDomain parameters:\n";

    long i = 6;		// id of params set

    L = dp[i].L_dec;

    // p.SetSize(L);
    // b.SetSize(L);

    // p = conv<ZZ> (p_dec);
    get_dec_from_hex (p, dp[i].p_hex, L);
    cout << "\np (dec) = \n" << p << endl;
    cout << "\np (hex) = \n";
    show_dec_in_hex (p, L);
    cout << endl;

    // a = conv<ZZ> (dp[i].a_dec);
    get_dec_from_hex (a, dp[i].a_hex, L);
    cout << "\na (dec) = \n" << a << endl;
    cout << "\na (hex) = \n";
    show_dec_in_hex (a, L);
    cout << endl;

    // b = conv<ZZ> (b_dec);
    get_dec_from_hex (b, dp[i].b_hex, L);
    cout << "\nb (dec) = \n" << b << endl;
    cout << "\nb (hex) = \n";
    show_dec_in_hex (b, L);
    cout << endl;

    //q = conv<ZZ> (q_dec);
    get_dec_from_hex (q, dp[i].q_hex, L);
    cout << "\nq (dec) = \n" << q << endl;
    cout << "\nq (hex) = \n";
    show_dec_in_hex (q, L);
    cout << endl;


    ZZ_p::init(p);

    Qxy P;
    ZZ xp, yp;
    // xp = conv<ZZ>(xp_dec);
    // yp = conv<ZZ>(yp_dec);
    get_dec_from_hex (xp, dp[i].xp_hex, L);
    get_dec_from_hex (yp, dp[i].yp_hex, L);

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
    // d = conv<ZZ>(d_dec);
    // xq = conv<ZZ>(xq_dec);
    // yq = conv<ZZ>(yq_dec);
    // Qt.getQxy(xq, yq);

    genkeys ( d, Q, P, q );

    /*
    cout << endl;
    if ( Q == Qt )
    	cout << "Generation of verification key is OK.";
    else
    	cout << "Generation of verification key is FAILD.";
    cout << endl;
    */

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
    cout << "\ne (hex) = \n";
    show_dec_in_hex (e, L);
    cout << endl;

    ZZ r, s, k;
    // k =conv<ZZ>(k_dec);

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
