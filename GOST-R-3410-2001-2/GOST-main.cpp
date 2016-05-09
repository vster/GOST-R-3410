// Файл GOST-main.cpp
// Контрольная реализация алгоритма ЭЦП ГОСТ Р 34.10-2001
// Применение параметров домена из RFC 4357

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <ctime>
#include "convhex.h"
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

DPar dp[10] =
{
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

int main()
{
    ZZ q;
    ZZ bpn;


    //======================================
    line();

    cout << "\nDomain parameters:\n";
    long i = 5;		// id of params set

    L = dp[i].L_dec;

    p.SetSize(L);
    b.SetSize(L);

    // p = conv<ZZ> (p_dec);
    get_dec_from_hex (p, dp[i].p_hex, L);
    cout << "\np (dec) = \n" << p << endl;
    cout << "\np (hex) = \n";
    show_dec_in_hex (p, L);
    cout << endl;

    // a = conv<ZZ> (a_dec);
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

    // q = conv<ZZ> (q_dec);
    get_dec_from_hex (q, dp[i].q_hex, L);
    cout << "\nq (dec) = \n" << q << endl;
    cout << "\nq (hex) = \n";
    show_dec_in_hex (q, L);
    cout << endl;


    ZZ_p::init(p);

    Qxy P;
    ZZ xp;
    ZZ yp;
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

    cout << endl;
    /*
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
