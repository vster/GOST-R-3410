// Файл QxyNTL.cpp
// Операции с эллиптическими кривыми

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <ctime>
#include "zzhex.h"
#include "QxyNTL.h"

using namespace std;
using namespace NTL;

extern ZZ a;

// Отобразить координаты точки Q
void Qxy::putQxy ( )
{
    // cout << "\nx (dec) = \n" << x << endl;
    cout << "\nx (hex) = \n";
    ShowZZInHex (conv<ZZ>(x));
    cout << endl;
    // cout << "\ny (dec) = \n" << y; cout << endl;
    cout << "\ny (hex) = \n";
    ShowZZInHex (conv<ZZ>(y));
    cout << endl;
}

// Сравнить две точки
bool Qxy::operator == ( Qxy q2 )
{
    return ((x == q2.x) & (y == q2.y)) ? true : false ;
}

// Сложение двух точек
Qxy Qxy::operator + ( Qxy q2 )
{
    Qxy tq, tq0;
    tq0.getQxy(conv<ZZ_p>(0),conv<ZZ_p>(0));
    ZZ_p dx, dy, lmb;
    tq.x = x;
    tq.y = y;
    if ( q2 == tq0 )
        return tq;
    if ( tq == tq0 )
        return q2;
    if ((x == q2.x) & ( y != q2.y))
    {
        return tq0;
    }
    if ((x == q2.x ) & (y == q2.y))
    {
        tq = q2.double_p();
        return tq;
    }
    dy = q2.y - y;
    dx = q2.x - x;
    lmb = dy / dx;
    tq.x = lmb * lmb - x - q2.x;
    tq.y = lmb * ( x - tq.x) - y;
    return tq;
}

// Умножение точки Q на 2
Qxy Qxy::double_p ( )
{
    Qxy tq2;
    ZZ_p lmb;
    lmb = ( 3*x*x + conv<ZZ_p>(a)) / (2*y);
    tq2.x = lmb * lmb - 2 * x;
    tq2.y = lmb * ( x - tq2.x ) - y;
    return tq2;
}

// Умножение точки Q на k
Qxy Qxy::operator * ( ZZ k )
{
    Qxy tq, tq2, tq0;
    tq0.getQxy(conv<ZZ_p>(0),conv<ZZ_p>(0));
    tq.x = x;
    tq.y = y;

    Qxy sum;
    Qxy dtq;
    long len = NumBits(k);

    dtq = tq;
    sum = tq0;
    for ( long i = 0; i < len; i++ )
    {
        if (bit(k,i))
            sum = sum + dtq;
        dtq = dtq.double_p();
    }
    return sum;
}
