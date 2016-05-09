// File convhex.h
// Процедуры преобразования чисел в форматах dec, bin, hex


#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include <NTL/GF2.h>
#include <NTL/vec_GF2.h>

using namespace std;
using namespace NTL;

const long maxhexlen = 4096;

void dec_to_bin (vec_GF2 &x_bin, const ZZ &x, long len);
void bin_to_dec (ZZ &x, vec_GF2 &x_bin, long len);
void bin_to_hex ( char x_hex[], vec_GF2 &x_bin, long len);
void hex_to_bin ( vec_GF2 &x_bin, char x_hex[], long len);
void dec_to_hex(char x_hex[], const ZZ &x, long len);
void hex_to_dec(ZZ &x, char x_hex[], long len);
void show_bin ( vec_GF2 &x_bin, long len );
void show_hex ( char x_hex[], long len );
void show_dec_in_hex ( const ZZ &x, long len );
void reverse (char x[], char x_bend[], long len);
void get_dec_from_hex (ZZ &x, char x_bend[], long len );


