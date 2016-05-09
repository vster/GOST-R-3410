#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <ctime>
#include "convhex.h"
#include "QxyNTL.h"
#include "G3410-2001.h"

extern ZZ p;
extern ZZ a;
extern ZZ b;

// Генерация начальных параметров
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
    cout << "Parameters of elliptic curve" << endl;
    cout << "\np = " << p;
    cout << "\na = " << a;
    cout << "\nb = " << b;
    cout << "\nbpn = " << bpn << endl;
    cout << "\nq = " << q << endl;
    cout << "\nP = ";
    P.putQxy();
    cout << endl;
}

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
    // simpdiv( q );
    long NumTrials = 10;
    if ( !ProbPrime( q, NumTrials) )
        cout << "\nq is not prime! Please, change params (p,a,b)." << endl;

    cout << "\np = " << p;
    cout << "\na = " << a;
    cout << "\nb = " << b;
    cout << "\nq = " << q << endl;
}


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
out:
    {}

}
// Тест параметров ЭК
void testparams ( Qxy &P, ZZ &q)
{
    Qxy Q0;
    Q0.getQxy(conv<ZZ_p>(0),conv<ZZ_p>(0));
    // Тест: некая точка * порядок кривой = нулевая точка
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
    RandomBnd(d, q-1);				// Закрытый ключ	1 < d < q-1
    // d = 15;
    Q = P*d;			// Открытый ключ
    cout << "\nPrivate key d = " << d << endl;
    cout << "Public key Q = ";
    Q.putQxy();
    cout << endl;
}

// Генерация ЭЦП
void gensign (ZZ &r, ZZ &s, ZZ &e, ZZ &d, Qxy &P, ZZ &q)
{
    ZZ k;
    Qxy C;

    // srand (time(NULL));
    while ( true )
    {
        RandomBnd(k, q-1);
        if ( GCD(k,q) > 1 )
            continue;
        // k = 5;
        C = P*k;
        r = (conv<ZZ>(C.putx()))%q;
        if ( r == 0)
            continue;

        // ZZ k1;
        // if (InvModStatus( k1, k, q ))
        // 	continue;
        // k1 = InvMod(k,q);
        // s = (k1*((H+r*d)%q))%q;
        s = (r*d + k*e)%q;
        if ((r>0)&(s>0))
            break;
    }

    cout << "k = " << k << endl
         << "(r,s) = ("
         << r << "," << s << ")" << endl;
}

// Проверка ЭЦП
void verifysign (ZZ &R, ZZ &r, ZZ &s, ZZ &e, Qxy &Q, Qxy &P, ZZ &q  )
{
    ZZ v, z1, z2;
    Qxy C2;
    // ZZ s1;
    ZZ e1;
    if (InvModStatus( v, e, q ))
    {
        cout << "\nError in sign\n";
        goto end;
    }
    v = InvMod(e,q);
    z1 = (s*v)%q;
    z2 = (-r*v)%q;
    C2 = P*z1 + Q*z2;
    R = (conv<ZZ>(C2.putx()))%q;

    cout << "\nv = " << v << endl
         << "z1 = " << z1 << endl
         << "z2 = " << z2 << endl
         << "C2 = ";
    C2.putQxy();
    cout << endl;
    cout << "R = " << R << endl;
end:
    if ( r == R )
        cout << "\nr = R\nSign is OK";
    else
        cout << "Sign is FAILD";
    cout << endl;

}
