#include <NTL/ZZ.h>
#define byte unsigned char

using namespace std;
using namespace NTL;

// typedef unsigned char byte;
const long MAXB = 1000;

int IntFromChar(char x);
void BytesFromHex(byte bt[], char s[]);
void HexFromBytes( char s[], byte bt[], int num );
void ZZFromHex(ZZ &a, char s[]);
void ShowZZInHex ( const ZZ &a );
