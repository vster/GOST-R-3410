// √Œ—“ – 34.10-94

void genparams( ZZ &p, ZZ &q, ZZ &a);
void genkeys ( ZZ &x, ZZ &y, ZZ &p, ZZ &q, ZZ &a);
void gensign ( ZZ &r, ZZ &s, ZZ &H, ZZ &x, ZZ &p, ZZ &q, ZZ &a);
void verifysign ( ZZ &u, ZZ &r, ZZ &s, ZZ &H, ZZ &y, ZZ &p, ZZ &q, ZZ &a);
