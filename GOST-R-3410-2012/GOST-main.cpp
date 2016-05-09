// ���� GOST-main.cpp
// ����������� ���������� ��������� ��� ���� � 34.10-2012
// ���������� ������������ �������� �� ���� � 34.10-2012

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <ctime>
#include "convhex.h"
#include "QxyNTL.h"
#include "G3410-2012.h"

using namespace std;
using namespace NTL;

long L;
const long MAXP = 200;

struct DPar
{
    long L_dec;
    char p_dec[MAXP];
    char p_hex[MAXP];
    char a_dec[MAXP];
    char a_hex[MAXP];
    char b_dec[MAXP];
    char b_hex[MAXP];
    char q_dec[MAXP];
    char q_hex[MAXP];
    char xp_dec[MAXP];
    char xp_hex[MAXP];
    char yp_dec[MAXP];
    char yp_hex[MAXP];
    char d_dec[MAXP];
    char d_hex[MAXP];
    char xq_dec[MAXP];
    char xq_hex[MAXP];
    char yq_dec[MAXP];
    char yq_hex[MAXP];
    char e_dec[MAXP];
    char e_hex[MAXP];
    char k_dec[MAXP];
    char k_hex[MAXP];
};

DPar dp[2] =
{
    // ����������� ������ 1
    // Curve P-256
    {
        256,
        "57896044618658097711785492504343953926634992332820282019728792003956564821041",
        "8000000000000000000000000000000000000000000000000000000000000431",
        "7",
        "7",
        "43308876546767276905765904595650931995942111794451039583252968842033849580414",
        "5FBFF498AA938CE739B8E022FBAFEF40563F6E6A3472FC2A514C0CE9DAE23B7E",
        "57896044618658097711785492504343953927082934583725450622380973592137631069619",
        "8000000000000000000000000000000150FE8A1892976154C59CFC193ACCF5B3",
        "2",
        "2",
        "4018974056539037503335449422937059775635739389905545080690979365213431566280",
        "8E2A8A0E65147D4BD6316030E16D19C85C97F0A9CA267122B96ABBCEA7E8FC8",
        "55441196065363246126355624130324183196576709222340016572108097750006097525544",
        "7A929ADE789BB9BE10ED359DD39A72C11B60961F49397EEE1D19CE9891EC3B28",
        "57520216126176808443631405023338071176630104906313632182896741342206604859403",
        "7F2B49E270DB6D90D8595BEC458B50C58585BAlD4E9B788F6689DBD8E56FD80B",
        "17614944419213781543809391949654080031942662045363639260709847859438286763994",
        "26F1B489D6701DD185C8413A977B3CBBAF64D1C593D26627DFFB101A87FF77DA",
        "20798893674476452017134061561508270130637142515379653289952617252661468872421",
        "2DFBC1B372D89A1188C09C52E0EEC61FCE52032AB1022E8E67ECE6672B043EE5",
        "53854137677348463731403841147996619241504003434302020712960838528893196233395",
        "77105C9B20BCD3122823C8CF6FCC7B956DE33814E95B7FE64FED924594DCEAB3"
    },
    // ����������� ������ 2
    // Curve P-512
    {
        512,
        "36239861022290036359077887536838743060213209255346786050865461504508561666240"		// p
        "02482588482022271496854025090823603058735163734263822371964987228582907372403",
        "4531ACD1FE0023C7550D267B6B2FEE80922B14B2FFB90F04D4EB7C09B5D2D15D"					// p
        "F1D852741AF4704A0458047E80E4546D35B8336FAC224DD81664BBF528BE6373",
        "7",		//a
        "7",
        "15186550692108285345089500347140431549287475277402064361940188233528099824437"		// b
        "93732829756914785974674866041605397883677596626326413990136959047435811826396",
        "1CFF0806A31116DA29D8CFA54E57EB748BC5F377E49400FDD788B649ECA1AC43"
        "61834013B2AD7322480A89CA58E0CF74BC9E540C2ADD6897FAD0A3084F302ADC",
        "36239861022290036359077887536838743060213209255346786050865461504508561666239"		// q
        "69164898305032863068499961404079437936585455865192212970734808812618120619743",
        "4531ACD1FE0023C7550D267B6B2FEE80922B14B2FFB90F04D4EB7C09B5D2D15D"
        "A82F2D7ECB1DBAC719905C5EECC423F1D86E25EDBE23C595D644AAF187E6E6DF",
        "19283569440670228493993094012431375989977866354595079743570754913077665926858"		// xp
        "35441065557681003184874819658004903212332884252335830250729527632383493573274",
        "24D19CC64572EE30F396BF6EBBFD7A6C5213B3B3D7057CC825F91093A68CD762"
        "FD60611262CD838DC6B60AA7EEE804E28BC849977FAC33B4B530F1B120248A9A",
        "22887286933719728599700121555294784163535623273295061803144974259311028603015"		// yp
        "72814141997072271708807066593850650334152381857347798885864807605098724013854",
        "2BB312A43BD2CE6E0D020613C857ACDDCFBF061E91E5F2C3F32447C259F39B2C"
        "83AB156D77F1496BF7EB3351E1EE4E43DC1A18B91B24640B6DBB92CB1ADD371E",
        "61008180413637309821953815323984758300684551906953156298238813535489060630178"		// d
        "2255383608393423372379057665527595116827307025046458837440766121180466875860",
        "BA6048AADAE241BA40936D47756D7C93091A0E8514669700EE7508E508B10207"
        "2E8123B2200A0563322DAD2827E2714A2636B7BFD18AADFC62967821FA18DD4",
        "90954685300253659655669076866983031000692927254655628159637296537031249856318"		// xq
        "2320436892870052842808608262832456858223580713780290717986855863433431150561",
        "115DC5BC96760C7B48598D8AB9E740D4C4A85A65BE33C1815B5C320C854621DD"
        "5A515856D13314AF69BC5B924C8B4DDFF75C45415C1D9DD9DD33612CD530EFE1",
        "29214572033744256206324497342484154556407008235594887051648958375095391342973"		// yq
        "27397380287741428246088626609329139441895016863758984106326600572476822372076",
        "37C7C90CD40B0F5621DC3AC1B751CFA0E2634FA0503B3D52639F5D7FB72AFD61"
        "EA199441D943FFE7F0C70A2759A3CDB84C114E1F9339FDF27F35ECA93677BEEC",
        "28979638816828685755628272785538650491737451978718251995629474190413889509705"		// e
        "36661109553499954248733088719748844538964641281654463513296973827706272045964",
        "3754F3CFACC9E0615C4F4A7C4D8DAB531B09B6F9C170C533A71D147035B0C591"
        "7184EE536593F4414339976C647C5D5A407ADEDB1D560C4FC6777D2972075B8C",
        "17551635602585049954062827992112528033345103174773779165020814424318205707503"		// k
        "4446102986750962508909227235866126872473516807810541747529710309879958632945",
        "359E7F4B1410FEACC570456C6801496946312120B39D019D455986E364F36588"
        "6748ED7A44B3E794434006011842286212273A6D14CF70EA3AF71BB1AE679F1",
    }
};

ZZ p, a, b, q;

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

    // p.SetSize(L);
    // b.SetSize(L);

    //======================================
    line();

    cout << "\nDomain parameters:\n";
    long i = 1;		// ����� ������������ ������� � ���������� ������

    L = dp[i].L_dec;

    p = conv<ZZ> (dp[i].p_dec);
    cout << "\np (dec) = \n" << p << endl;
    cout << "\np (hex) = \n";
    show_dec_in_hex (p, L);
    cout << endl;

    a = conv<ZZ> (dp[i].a_dec);
    cout << "\na (dec) = \n" << a << endl;
    cout << "\na (hex) = \n";
    show_dec_in_hex (a, L);
    cout << endl;

    b = conv<ZZ> (dp[i].b_dec);
    cout << "\nb (dec) = \n" << b << endl;
    cout << "\nb (hex) = \n";
    show_dec_in_hex (b, L);
    cout << endl;

    q = conv<ZZ> (dp[i].q_dec);
    cout << "\nq (dec) = \n" << q << endl;
    cout << "\nq (hex) = \n";
    show_dec_in_hex (q, L);
    cout << endl;


    ZZ_p::init(p);

    Qxy P;
    ZZ xp;
    ZZ yp;
    xp = conv<ZZ>(dp[i].xp_dec);
    yp = conv<ZZ>(dp[i].yp_dec);

    P.getQxy(xp, yp);

    cout << "\nBase point P:\n";
    P.putQxy();
    cout << endl;

    //======================================
    line();


    // ���� ���������� ��
    // testparams ( P, q );

    //======================================
    //line();

    // ��������� ������
    cout << "\nSignature and verification keys.\n";
    ZZ d;				// ����	������� 1 < d < q-1
    Qxy Q;				// ���� ��������
    Qxy Qt;				// �������� ��������� ��������� �����
    ZZ xq, yq;
    d = conv<ZZ>(dp[i].d_dec);
    xq = conv<ZZ>(dp[i].xq_dec);
    yq = conv<ZZ>(dp[i].yq_dec);
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

    // ��������� ���

    cout << "\nSignature process.\n";

    ZZ e;
    e =conv<ZZ>(dp[i].e_dec);
    if ( e == 0 )
        e = 1;

    cout << "\nHash-code e:\n";
    cout << "\ne (dec) = \n" << e << endl;
    cout << "\ne (hex) = \n";
    show_dec_in_hex (e, L);
    cout << endl;

    ZZ r, s, k;
    k =conv<ZZ>(dp[i].k_dec);

    gensign ( r, s, k, e, d, P, q );

    //======================================
    line();

    // �������� ���
    cout << "\nVerification process.\n";
    ZZ R;
    verifysign ( R, r, s, e, Q, P, q  );

    //======================================
    line();

    return 0;
}
