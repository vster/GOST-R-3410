// Файл G3410-2001.h
// Контрольная реализация алгоритма ЭЦП ГОСТ Р 34.10-2001

void genparams ( Qxy &P, ZZ &q, ZZ &bpn );
void genparam_q ( Qxy &P, ZZ &q );
void genparam_P ( Qxy &P, ZZ &q, ZZ &bpn );
void testparams ( Qxy &P, ZZ &q);
void genkeys ( ZZ &d, Qxy &Q, Qxy &P, ZZ &q );
void gensign (ZZ &r, ZZ &s, ZZ &k, ZZ &e, ZZ &d, Qxy &P, ZZ &q);
void verifysign (ZZ &R, ZZ &r, ZZ &s, ZZ &e, Qxy &Q, Qxy &P, ZZ &q  );
