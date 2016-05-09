// Файл GOST-main.cpp
// Контрольная реализация алгоритма ЭЦП ГОСТ Р 34.10-2001 
// Выполнение контрольного примера из ГОСТ Р 34.10-2001

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <ctime>
#include "convhex.h"
#include "QxyNTL.h"
#include "G3410-2001.h"

using namespace std;
using namespace NTL;

long L = 256;
ZZ p; 
ZZ a;
ZZ b;	

// Параметры эллиптической кривой из контрольного примера из ГОСТ Р 34.10-2001
char p_dec[] =	"57896044618658097711785492504343953926634992332820282019728792003956564821041";
char p_hex[] =	"8000000000000000000000000000000000000000000000000000000000000431";

char a_dec[] =	"7";
char a_hex[] =	"7";

char b_dec[] =	"43308876546767276905765904595650931995942111794451039583252968842033849580414";
char b_hex[] =	"5FBFF498AA938CE739B8E022FBAFEF40563F6E6A3472FC2A514C0CE9DAE23B7E";

char q_dec[] =	"57896044618658097711785492504343953927082934583725450622380973592137631069619"; 
char q_hex[] =	"8000000000000000000000000000000150FE8A1892976154C59CFC193ACCF5B3";

char xp_dec[] =	"2";
char xp_hex[] =	"2";

char yp_dec[] =	"4018974056539037503335449422937059775635739389905545080690979365213431566280";
char yp_hex[] =	"8E2A8A0E65147D4BD6316030E16D19C85C97F0A9CA267122B96ABBCEA7E8FC8";

char d_dec[] =	"55441196065363246126355624130324183196576709222340016572108097750006097525544";
char d_hex[] =	"7A929ADE789BB9BE10ED359DD39A72C11B60961F49397EEE1D19CE9891EC3B28";

char xq_dec[] =	"57520216126176808443631405023338071176630104906313632182896741342206604859403";
char xq_hex[] =	"7F2B49E270DB6D90D8595BEC458B50C58585BAlD4E9B788F6689DBD8E56FD80B";

char yq_dec[] =	"17614944419213781543809391949654080031942662045363639260709847859438286763994";
char yq_hex[] =	"26F1B489D6701DD185C8413A977B3CBBAF64D1C593D26627DFFB101A87FF77DA"; 

char e_dec[]  =	"20798893674476452017134061561508270130637142515379653289952617252661468872421";
char e_hex[]  =	"2DFBC1B372D89A1188C09C52E0EEC61FCE52032AB1022E8E67ECE6672B043EE5";

char k_dec[]  = "53854137677348463731403841147996619241504003434302020712960838528893196233395";
char k_hex[]  = "77105C9B20BCD3122823C8CF6FCC7B956DE33814E95B7FE64FED924594DCEAB3"; 

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

	p.SetSize(L);
	b.SetSize(L);
	
	//======================================
	line();

	cout << "\nDomain parameters:\n";
	p = conv<ZZ> (p_dec);
	cout << "\np (dec) = \n" << p << endl;
	cout << "\np (hex) = \n"; show_dec_in_hex (p, L);	cout << endl;

	a = conv<ZZ> (a_dec);
	cout << "\na (dec) = \n" << a << endl;
	cout << "\na (hex) = \n"; show_dec_in_hex (a, L);	cout << endl;

	b = conv<ZZ> (b_dec);
	cout << "\nb (dec) = \n" << b << endl;
	cout << "\nb (hex) = \n"; show_dec_in_hex (b, L);	cout << endl;

	q = conv<ZZ> (q_dec);	
	cout << "\nq (dec) = \n" << q << endl;		
	cout << "\nq (hex) = \n"; show_dec_in_hex (q, L);	cout << endl;

	
	ZZ_p::init(p);

	Qxy P;
	ZZ xp;
	ZZ yp;
	xp = conv<ZZ>(xp_dec);
	yp = conv<ZZ>(yp_dec);	

	P.getQxy(xp, yp);

	cout << "\nBase point P:\n";
	P.putQxy();
	cout << endl;

	//======================================
	line();


	// Тест параметров ЭК
	//testparams ( P, q );

	//======================================
	//line();

	// Генерация ключей
	cout << "\nSignature and verification keys.\n";
	ZZ d;				// Ключ	подписи 1 < d < q-1
	Qxy Q;				// Ключ проверки
	Qxy Qt;				// Проверка генерации открытого ключа
	ZZ xq, yq;
	d = conv<ZZ>(d_dec);	
	xq = conv<ZZ>(xq_dec);
	yq = conv<ZZ>(yq_dec);
	Qt.getQxy(xq, yq);

	genkeys ( d, Q, P, q );
	
	cout << endl;
	if ( Q == Qt )
		cout << "Generation of verification key is OK.";
	else
		cout << "Generation of verification key is FAILD.";
	cout << endl;

	//======================================
	line();

	// Генерация ЭЦП
	
	cout << "\nSignature process.\n";	
	
	ZZ e;
	e =conv<ZZ>(e_dec);
	if ( e == 0 )
		e = 1;
	
	cout << "\nHash-code e:\n";
	cout << "\ne (dec) = \n" << e << endl;
	cout << "\ne (hex) = \n"; show_dec_in_hex (e, L);	cout << endl;

	ZZ r, s, k;
	k =conv<ZZ>(k_dec);
	
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