// Файл GOST-main.cpp
// Контрольная реализация алгоритма ЭЦП ГОСТ Р 34.10-2012

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <ctime>
#include "zzhex.h"
#include "QxyNTL.h"
#include "G3410-2012.h"

using namespace std;
using namespace NTL;

const long MAXP = 200;
long L;

ZZ a, b, p, q;
ZZ xp, yp;

struct DPar
{
    char oid[20];
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
    // OID:1.2.643.2.2.35.0
    // a, b, p, q, xp, yp
    {
        "1.2.643.2.2.35.0",
        256,
        "7",
        "5FBFF498AA938CE739B8E022FBAFEF40563F6E6A3472FC2A514C0CE9DAE23B7E",
        "8000000000000000000000000000000000000000000000000000000000000431",
        "8000000000000000000000000000000150FE8A1892976154C59CFC193ACCF5B3",
        "2",
        "08E2A8A0E65147D4BD6316030E16D19C85C97F0A9CA267122B96ABBCEA7E8FC8"
    },
    // id-GostR3410-2001-CryptoPro-A-ParamSet	(1) - ViPNet main
    // OID:1.2.643.2.2.35.1
    // a, b, p, q, xp, yp
    {
        "1.2.643.2.2.35.1",
        256,
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD94",
        "166",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD97",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6C611070995AD10045841B09B761B893",
        "1",
        "8D91E471E0989CDA27DF505A453F2B7635294F2DDF23E3B122ACC99C9E9F1E14"
    },
    // id-GostR3410-2001-CryptoPro-B-ParamSet (2)
    // OID:1.2.643.2.2.35.2
    // a, b, p, q, xp, yp
    {
        "1.2.643.2.2.35.2",
        256,
        "8000000000000000000000000000000000000000000000000000000000000C96",
        "3E1AF419A269A5F866A7D3C25C3DF80AE979259373FF2B182F49D4CE7E1BBC8B",
        "8000000000000000000000000000000000000000000000000000000000000C99",
        "800000000000000000000000000000015F700CFFF1A624E5E497161BCC8A198F",
        "1",
        "3FA8124359F96680B83D1C3EB2C070E5C545C9858D03ECFB744BF8D717717EFC"
    },
    // id-GostR3410-2001-CryptoPro-C-ParamSet (3)
    // OID:1.2.643.2.2.35.3
    // a, b, p, q, xp, yp
    {
        "1.2.643.2.2.35.3",
        256,
        "9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D7598",
        "32858",
        "9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D759B",
        "9B9F605F5A858107AB1EC85E6B41C8AA582CA3511EDDFB74F02F3A6598980BB9",
        "0",
        "41ECE55743711A8C3CBF3783CD08C0EE4D4DC440D4641A8F366E550DFDB3BB67"
    },
    // id-GostR3410-2001-CryptoPro-XchA-ParamSet	(4)
    // OID:1.2.643.2.2.35.4
    // a, b, p, q, xp, yp
    {
        "1.2.643.2.2.35.4",
        256,
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD94",
        "166",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD97",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6C611070995AD10045841B09B761B893",
        "1",
        "8D91E471E0989CDA27DF505A453F2B7635294F2DDF23E3B122ACC99C9E9F1E14"
    },
    // id-GostR3410-2001-CryptoPro-XchB-ParamSet	(5)
    // OID:1.2.643.2.2.35.5
    //  a, b, p, q, xp, yp
    {
        "1.2.643.2.2.35.5",
        256,
        "9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D7598",
        "32858",
        "9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D759B",
        "9B9F605F5A858107AB1EC85E6B41C8AA582CA3511EDDFB74F02F3A6598980BB9",
        "0",
        "41ECE55743711A8C3CBF3783CD08C0EE4D4DC440D4641A8F366E550DFDB3BB67"
    },
    // Параметры из конртрольных примеров ГОСТ Р 34.10-2012
    // 512 bit			(6)
    // OID:1.2.643.7.1.2.1.2.0
    // a, b, p, q, xp, yp
    {
        "1.2.643.7.1.2.1.2.0",
        512,
        "7",
        "1CFF0806A31116DA29D8CFA54E57EB748BC5F377E49400FDD788B649ECA1AC4361834013B2AD7322480A89CA58E0CF74BC9E540C2ADD6897FAD0A3084F302ADC",
        "4531ACD1FE0023C7550D267B6B2FEE80922B14B2FFB90F04D4EB7C09B5D2D15DF1D852741AF4704A0458047E80E4546D35B8336FAC224DD81664BBF528BE6373",
        "4531ACD1FE0023C7550D267B6B2FEE80922B14B2FFB90F04D4EB7C09B5D2D15DA82F2D7ECB1DBAC719905C5EECC423F1D86E25EDBE23C595D644AAF187E6E6DF",
        "24D19CC64572EE30F396BF6EBBFD7A6C5213B3B3D7057CC825F91093A68CD762FD60611262CD838DC6B60AA7EEE804E28BC849977FAC33B4B530F1B120248A9A",
        "2BB312A43BD2CE6E0D020613C857ACDDCFBF061E91E5F2C3F32447C259F39B2C83AB156D77F1496BF7EB3351E1EE4E43DC1A18B91B24640B6DBB92CB1ADD371E"
    },
    // Параметры рабочей группы
    // 512 bit - A		(7)
    // OID:1.2.643.7.1.2.1.2.1
    // a, b, p, q, xp, yp
    {
        "1.2.643.7.1.2.1.2.1",
        512,
        // a = -3
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC4",
        "E8C2505DEDFC86DDC1BD0B2B6667F1DA34B82574761CB0E879BD081CFD0B6265EE3CB090F30D27614CB4574010DA90DD862EF9D4EBEE4761503190785A71C760",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC7",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF27E69532F48D89116FF22B8D4E0560609B4B38ABFAD2B85DCACDB1411F10B275",
        "3",
        "7503CFE87A836AE3A61B8816E25450E6CE5E1C93ACF1ABC1778064FDCBEFA921DF1626BE4FD036E93D75E6A50E3A41E98028FE5FC235F5B889A589CB5215F2A4"
    },
    // 512 bit - B		(8) - ViPNet main
    // OID:1.2.643.7.1.2.1.2.2
    // a, b, p, q, xp, yp
    {
        "1.2.643.7.1.2.1.2.2",
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

// Тесты
struct VerifyTest
{
    long dpid;			// ID of Domain Params
    char h_hex[MAXP];	// Hash-code
    char xq_hex[MAXP];	// Open key x
    char yq_hex[MAXP];	// Open key y
    char r_hex[MAXP];	// Signature r
    char s_hex[MAXP];	// Signature s
};

VerifyTest vt[14] =
{
    // Test 1	256 bit		tid = 0
    // id, h, xq, yq, r, s
    {
        0,		// OID:1.2.643.2.2.35.0
        "2dfbc1b372d89a1188c09c52e0eec61fce52032ab1022e8e67ece6672b043ee5",
        "7f2b49e270db6d90d8595bec458b50c58585ba1d4e9b788f6689dbd8e56fd80b",
        "26f1b489d6701dd185c8413a977b3cbbaf64d1c593d26627dffb101a87ff77da",
        "41aa28d2f1ab148280cd9ed56feda41974053554a42767b83ad043fd39dc0493",
        "01456c64ba4642a1653c235a98a60249bcd6d3f746b631df928014f6c5bf9c40"
    },
    // Test 2	256 bit		tid = 1
    // id, h, xq, yq, r, s
    {
        0,		// OID:1.2.643.2.2.35.0
        "d3f04d9ff1694c1cc890e3febb7702342cb787f1b3f01a4cf2cae3c488ba7a0f",
        "7f2b49e270db6d90d8595bec458b50c58585ba1d4e9b788f6689dbd8e56fd80b",
        "26f1b489d6701dd185c8413a977b3cbbaf64d1c593d26627dffb101a87ff77da",
        "4b5142aa086c4f440b30ab15ae6c26b6100b5ef77ba5f1471586249fe10ea6f7",
        "23549d1f7c71265d4845daf1870fd4b92a8eaae2124e8b537a3fbc7bbd769113"
    },
    // Test 3	256 bit		tid = 2
    // id, h, xq, yq, r, s
    {
        1,		// OID:1.2.643.2.2.35.1
        "2dfbc1b372d89a1188c09c52e0eec61fce52032ab1022e8e67ece6672b043ee5",
        "fd21c21ab0dc84c154f3d218e9040bee64fff48bdff814b232295b09d0df72e4",
        "5026dec9ac4f07061a2a01d7a2307e0659239a82a95862df86041d1458e45049",
        "aad8d625369d14bd59c9b893b46ffa990b51e0638e2df79219b615115af2f3bc",
        "7b01941ed70dea0409ef11d387fb427e4f6186e4a7ef79d948c937c7c3284b8d"
    },
    // Тест 4	256 bit		tid = 3
    // id, h, xq, yq, r, s
    {
        1,		// OID:1.2.643.2.2.35.1
        "d3f04d9ff1694c1cc890e3febb7702342cb787f1b3f01a4cf2cae3c488ba7a0f",
        "fd21c21ab0dc84c154f3d218e9040bee64fff48bdff814b232295b09d0df72e4",
        "5026dec9ac4f07061a2a01d7a2307e0659239a82a95862df86041d1458e45049",
        "bc11a50c329cb3a767a922df5fb13e790b62510cf2337bd6904eb0189671b2e5",
        "3d7efa9963589973879261d9577bdcda1bd915a3a8893a5fa4f48084b79769b9"
    },
    // Тест 5	256 bit		tid = 4
    // id, h, xq, yq, r, s
    {
        2,		// OID:1.2.643.2.2.35.2
        "2dfbc1b372d89a1188c09c52e0eec61fce52032ab1022e8e67ece6672b043ee5",
        "740a4dc25fe6b805da88dcfb0dfffbdabb9645ad90f4311d405681c6f0ddc7bc",
        "580c28f4ae42cfea7ac953ea31db47e6b58a107149ba4158f6f16f3b7958a136",
        "42fe34edce46cb09109d77e02cac64845b54a65102ad3edf15768bf31d6980f2",
        "28e8ba2021fcfb173f60694c7e0db33d40f555c30ad994895a51d0311150ca04"
    },
    // Тест 6	256 bit		tid = 5
    // id, h, xq, yq, r, s
    {
        2,		// OID:1.2.643.2.2.35.2
        "d3f04d9ff1694c1cc890e3febb7702342cb787f1b3f01a4cf2cae3c488ba7a0f",
        "740a4dc25fe6b805da88dcfb0dfffbdabb9645ad90f4311d405681c6f0ddc7bc",
        "580c28f4ae42cfea7ac953ea31db47e6b58a107149ba4158f6f16f3b7958a136",
        "66821a831eb13981d94b4872eb23c1e3980243ab6e837774d72e8226065a8070",
        "79bb85348a3c56fdd1cab88e7d90b2bb28a256e7e56d47f3bc38d8455e262dfa"
    },
    // Тест 7	256 bit		tid = 6
    // id, h, xq, yq, r, s
    {
        3, 		// OID:1.2.643.2.2.35.3
        "2dfbc1b372d89a1188c09c52e0eec61fce52032ab1022e8e67ece6672b043ee5",
        "74259e68bd9e935fdfe55970a552e37c024bbc4e2b66e515ae0f7b2514014d60",
        "1e4a1f109b4dfde7e5ea18e1b904a0fb0a4bd462b64b8073e0e8ec735a91c206",
        "52c5eb6fa42f1176a1bacd9a1a05aa443fbef4277af857fd8ac2054049338d8f",
        "4c3be01ae6a11bfc4453d15982f1565c4532c78bc4dbbd8d4cfe94c31c4137c1"
    },
    // Тест 8	256 bit		tid = 7
    // id, h, xq, yq, r, s
    {
        3, 		// OID:1.2.643.2.2.35.3
        "d3f04d9ff1694c1cc890e3febb7702342cb787f1b3f01a4cf2cae3c488ba7a0f",
        "74259e68bd9e935fdfe55970a552e37c024bbc4e2b66e515ae0f7b2514014d60",
        "1e4a1f109b4dfde7e5ea18e1b904a0fb0a4bd462b64b8073e0e8ec735a91c206",
        "3a3de337de0fdf489e2b3fbbe4fb1e2ba73da37a156fa4e22926733083da1891",
        "4501fc5a2ed2fe60226f56e07ee17ee22910321565b665dfd92367df7760fada"
    },
    // Тест 1	512 bit		tid = 8
    // id, h, xq, yq, r, s
    {
        6,		// OID:1.2.643.7.1.2.1.2.0
        "3754f3cfacc9e0615c4f4a7c4d8dab531b09b6f9c170c533a71d147035b0c5917184ee536593f4414339976c647c5d5a407adedb1d560c4fc6777d2972075b8c",
        "115dc5bc96760c7b48598d8ab9e740d4c4a85a65be33c1815b5c320c854621dd5a515856d13314af69bc5b924c8b4ddff75c45415c1d9dd9dd33612cd530efe1",
        "37c7c90cd40b0f5621dc3ac1b751cfa0e2634fa0503b3d52639f5d7fb72afd61ea199441d943ffe7f0c70a2759a3cdb84c114e1f9339fdf27f35eca93677beec",
        "2f86fa60a081091a23dd795e1e3c689ee512a3c82ee0dcc2643c78eea8fcacd35492558486b20f1c9ec197c90699850260c93bcbcd9c5c3317e19344e173ae36",
        "1081b394696ffe8e6585e7a9362d26b6325f56778aadbc081c0bfbe933d52ff5823ce288e8c4f362526080df7f70ce406a6eeb1f56919cb92a9853bde73e5b4a"
    },
    // Тест 2	512 bit		tid = 9
    // id, h, xq, yq, r, s
    {
        6,		// OID:1.2.643.7.1.2.1.2.0
        "78c76890bee8011b9472a281db95196881c1e63d8b06e8b674bca8080870e622b6308a502f4ffba6f3ad9da28bdcc4002bac667ce756b1a10bfcf23674370e24",
        "115dc5bc96760c7b48598d8ab9e740d4c4a85a65be33c1815b5c320c854621dd5a515856d13314af69bc5b924c8b4ddff75c45415c1d9dd9dd33612cd530efe1",
        "37c7c90cd40b0f5621dc3ac1b751cfa0e2634fa0503b3d52639f5d7fb72afd61ea199441d943ffe7f0c70a2759a3cdb84c114e1f9339fdf27f35eca93677beec",
        "2e159416a12d6884db2e880183e6e105caee19e5699d31a66ef0be548cc516a48e7e750e412fa22ce6338a29063e3c6f5f7f101723f2c947577a3c6992eddd19",
        "16a7e2b7bf8b795f1c1c09c2950fe70927a52ffacd567d1841607d4c9826e0e92e3ac525b7eb41cf928624038eeb7c8e1b8b1034ae678581522e05b5c29f0bd2"
    },
    // Тест 3	512 bit		tid = 10
    // id, h, xq, yq, r, s
    {
        7,		// OID:1.2.643.7.1.2.1.2.1
        "3754f3cfacc9e0615c4f4a7c4d8dab531b09b6f9c170c533a71d147035b0c5917184ee536593f4414339976c647c5d5a407adedb1d560c4fc6777d2972075b8c",
        "dcef181eef936ebf949f4e966976548125067ef9b6dc5940144ec99f7610063cfeea097a6f5064093794957863a240e048882a25a9d0db185b8175cfe61c66aa",
        "670f9bed03d6ae001e9637b68154861ac4f68717acbbfc8a78f528ce7cb001447fe9e975eacc805ebf708da139d2d7490eaaf6ab2476c5e3d79bb63d0f8047d6",
        "e8cd8af9c2d8f16354e1405ba2effed5aebea7d61d9ef767419520d72c22538e61afdfc8aa646c8767fc85af80d88121c0e24135d68738ec0260b3f83c679234",
        "ec81f951285e41587bed827873f4bce3cd2894c03485bb9cc7d9545d7b5c0f71322b1b2d516cec8cb3fff2a57761c1b22050175e68f0f7ed9451d6ee805a70ae"
    },
    // Тест 4	512 bit		tid = 11
    // id, h, xq, yq, r, s
    {
        7,		// OID:1.2.643.7.1.2.1.2.1
        "78c76890bee8011b9472a281db95196881c1e63d8b06e8b674bca8080870e622b6308a502f4ffba6f3ad9da28bdcc4002bac667ce756b1a10bfcf23674370e24",
        "dcef181eef936ebf949f4e966976548125067ef9b6dc5940144ec99f7610063cfeea097a6f5064093794957863a240e048882a25a9d0db185b8175cfe61c66aa",
        "670f9bed03d6ae001e9637b68154861ac4f68717acbbfc8a78f528ce7cb001447fe9e975eacc805ebf708da139d2d7490eaaf6ab2476c5e3d79bb63d0f8047d6",
        "e0b8789a9b23ab584a9055757c015c386fc4242ae1d9f557fc725b651390d3aca6ad09dcb28680926c64f623cee8e60120333b549b1084bbe3a488a3e28ea4f2",
        "4a9cd0f7a4a369f6a634ad72421eda80d521d6908d64d3083d2cef702bb201521facb9adcfc2cd2181ba376c899a82fc59259ff8e9da93b4fa0fc21b2400fc3e",
    },
    // Тест 5	512 bit		tid = 12
    // id, h, xq, yq, r, s
    {
        8,		// OID:1.2.643.7.1.2.1.2.2
        "3754f3cfacc9e0615c4f4a7c4d8dab531b09b6f9c170c533a71d147035b0c5917184ee536593f4414339976c647c5d5a407adedb1d560c4fc6777d2972075b8c",
        "7d3301ff8eb22f04b94dd4973a137fe39a4b582277b3f061d02dec3e2bc457610a1a61b879934f54ffa189a20bf9a501279609274af2c47aabe826545fde0179",
        "30dab572313cef13f989cc0627fb1bc537378ca1ecc6c16bfe2d00ea933f96d3f157f01affc6711f7ed1e28aa465d74416e2ab3bbb5d1364682550fd2cea7d8f",
        "1fd1df11dc61c3c87e20fad01296729c2944a2ee6da613ef64b7509dcd7eadbe5a2683ee8d8fa5b57aca333fcefe2210eb2d9cefebbe1c6fa4a0cf0a9ddf3127",
        "7459ca46da32fb8c372fc50388f38ce5d757599cf07594b7b5255d1310890c49644e9a444d04a6beb11747d7866e456bac76b61c6627ee19a48053d6ee50c7e2"
    },
    // Тест 6	512 bit		tid = 13
    // id, h, xq, yq, r, s
    {
        8,		// OID:1.2.643.7.1.2.1.2.2
        "78c76890bee8011b9472a281db95196881c1e63d8b06e8b674bca8080870e622b6308a502f4ffba6f3ad9da28bdcc4002bac667ce756b1a10bfcf23674370e24",
        "7d3301ff8eb22f04b94dd4973a137fe39a4b582277b3f061d02dec3e2bc457610a1a61b879934f54ffa189a20bf9a501279609274af2c47aabe826545fde0179",
        "30dab572313cef13f989cc0627fb1bc537378ca1ecc6c16bfe2d00ea933f96d3f157f01affc6711f7ed1e28aa465d74416e2ab3bbb5d1364682550fd2cea7d8f",
        "25b5c01cc38d0aee973b05334455ff37354f845195a2c671c789e2b56a64a0e8882769a8b325915346ab09f22ff3e8f9ae9f2e2e8916301c0556007a675f678c",
        "0ff0748e1879cdbffd18a90f5991db694f2cad0d671a243858d900cb6a4ce54d3a4957e7e0fe0fe4fcedec0423f04fbd84e1355a1c634876f691ce42c0cb33cc"
    }

};

// Generation of keys test
struct KeyTest
{
    long dpid;			// ID of Domain Params
    char d_hex[MAXP];	// Secret Key
};

KeyTest kt[14] =
{
    // Test 1	256 bit		tid = 0
    {
        0,
        "7a929ade789bb9be10ed359dd39a72c11b60961f49397eee1d19ce9891ec3b28"
    },
    // Test 2	256 bit		tid = 1
    {
        0,
        "02951b1e41bddd16b2b549a64e0e676cf77b1cac3965194cf59e7c73fa680c73"
    },
    // Test 3	256 bit		tid = 2
    {
        1,
        "7a929ade789bb9be10ed359dd39a72c11b60961f49397eee1d19ce9891ec3b28"
    },
    // Test 4	256 bit		tid = 3
    {
        1,
        "02951b1e41bddd16b2b549a64e0e676cf77b1cac3965194cf59e7c73fa680c73"
    },
    // Test 5	256 bit		tid = 4
    {
        2,
        "7a929ade789bb9be10ed359dd39a72c11b60961f49397eee1d19ce9891ec3b28"
    },
    // Test 6	256 bit		tid = 5
    {
        2,
        "02951b1e41bddd16b2b549a64e0e676cf77b1cac3965194cf59e7c73fa680c73"
    },
    // Test 7	256 bit		tid = 6
    {
        3,
        "7a929ade789bb9be10ed359dd39a72c11b60961f49397eee1d19ce9891ec3b28"
    },
    // Test 8	256 bit		tid = 7
    {
        3,
        "02951b1e41bddd16b2b549a64e0e676cf77b1cac3965194cf59e7c73fa680c73"
    },
    // Test 1	512 bit		tid = 8
    {
        6,
        "0ba6048aadae241ba40936d47756d7c93091a0e8514669700ee7508e508b102072e8123b2200a0563322dad2827e2714a2636b7bfd18aadfc62967821fa18dd4"
    },
    // Test 2	512 bit		tid = 9
    {
        6,
        "f77c88c0dcac124eab3bcba3df79aa0f0e32e55dd8cb4370897fbeef2cae36e453be645855a9051e47dd91b6830dc422a19708449dc5dc300c88fd5cb2907bc1"
    },
    // Test 3	512 bit		tid = 10
    {
        7,
        "0ba6048aadae241ba40936d47756d7c93091a0e8514669700ee7508e508b102072e8123b2200a0563322dad2827e2714a2636b7bfd18aadfc62967821fa18dd4"
    },
    // Test 4	512 bit		tid = 11
    {
        7,
        "f77c88c0dcac124eab3bcba3df79aa0f0e32e55dd8cb4370897fbeef2cae36e453be645855a9051e47dd91b6830dc422a19708449dc5dc300c88fd5cb2907bc1"
    },
    // Test 5	512 bit		tid = 12
    {
        8,
        "0ba6048aadae241ba40936d47756d7c93091a0e8514669700ee7508e508b102072e8123b2200a0563322dad2827e2714a2636b7bfd18aadfc62967821fa18dd4"
    },
    // Test 6	512 bit		tid = 13
    {
        8,
        "f77c88c0dcac124eab3bcba3df79aa0f0e32e55dd8cb4370897fbeef2cae36e453be645855a9051e47dd91b6830dc422a19708449dc5dc300c88fd5cb2907bc1"
    }

};

//======================================
void line()
{
    cout << endl;
    for (int i = 0; i < 79; i++) cout << "=";
    cout << endl;
}
//======================================
void GetDP( long dpid )
{
    L = dp[dpid].L_dec;
    ZZFromHex (p, dp[dpid].p_hex);
    ZZFromHex (a, dp[dpid].a_hex);
    ZZFromHex (b, dp[dpid].b_hex);
    ZZFromHex (q, dp[dpid].q_hex);
    ZZFromHex (xp, dp[dpid].xp_hex);
    ZZFromHex (yp, dp[dpid].yp_hex);
}
//======================================
void ShowDP()
{
    // cout << "\np (dec) = \n" << p << endl;
    cout << "\np (hex) = \n";
    ShowZZInHex (p);
    cout << endl;

    // cout << "\na (dec) = \n" << a << endl;
    cout << "\na (hex) = \n";
    ShowZZInHex (a);
    cout << endl;

    // cout << "\nb (dec) = \n" << b << endl;
    cout << "\nb (hex) = \n";
    ShowZZInHex (b);
    cout << endl;

    // cout << "\nq (dec) = \n" << q << endl;
    cout << "\nq (hex) = \n";
    ShowZZInHex (q);
    cout << endl;

    // cout << "\nxp (dec) = \n" << xp << endl;
    cout << "\nxp (hex) = \n";
    ShowZZInHex (xp);
    cout << endl;

    // cout << "\nyp (dec) = \n" << yp << endl;
    cout << "\nyp (hex) = \n";
    ShowZZInHex (yp);
    cout << endl;
}
//======================================
int main(int argc, char* argv[])
{

    //======================================
    line();

    long tid = 3;

    for( tid = 0; tid < 14; tid++)
    {
        cout << "\nTest id:\n" << tid << endl;

        cout << "\nDomain parameters:\n\n";


        // long pid = kt[tid].pid;		// id of params set
        long dpid = vt[tid].dpid;		// id of params set
        // long pid = 0;
        if (argc == 2)
            dpid = atoi(argv[1]);

        GetDP( dpid );

        ShowDP();

        ZZ_p::init(p);

        Qxy P;

        P.getQxy(xp, yp);

        cout << "\nBase point P:\n";
        P.putQxy();
        cout << endl;

        //======================================
        line();

        // Тест параметров ЭК
        // testparams ( P, q );

        //======================================
        // line();

        // Генерация ключей
        cout << "\nSignature and verification keys.\n";
        ZZ d;				// Ключ	подписи 1 < d < q-1
        Qxy Q;				// Ключ проверки
        Qxy Qt;				// Проверка генерации открытого ключа
        ZZ xq, yq;

        ZZFromHex (xq, vt[tid].xq_hex);
        ZZFromHex (yq, vt[tid].yq_hex);

        Q.getQxy(xq, yq);
        Q.putQxy();
        // testparams ( Q, q );

        //ZZFromHex (d, kt[tid].d_hex );

        //genkeys ( d, Q, P, q );

        //======================================
        line();

        // Генерация ЭЦП

        cout << "\nSignature process.\n";

        ZZ h, e;

        // RandomBnd(e, q-1);		// Хэш
        ZZFromHex( h, vt[tid].h_hex );


        cout << "\nHash-code h:\n";
        // cout << "\nh (dec) = \n" << h << endl;
        cout << "\nh (hex) = \n";
        ShowZZInHex (h);
        cout << endl;

        e = h%q;
        if ( e == 0 )
            e = 1;

        cout << "\nHash-code e:\n";
        // cout << "\ne (dec) = \n" << e << endl;
        cout << "\ne (hex) = \n";
        ShowZZInHex (e);
        cout << endl;

        ZZ r, s, k;

        ZZFromHex (r, vt[tid].r_hex);
        ZZFromHex (s, vt[tid].s_hex);
        // gensign ( r, s, k, e, d, P, q );
        cout << "\nSignature:\n";

        // cout << "\nr (dec) = \n" << r << endl;
        cout << "\nr (hex) = \n";
        ShowZZInHex (r);
        cout << endl;

        // cout << "\ns (dec) = \n" << s << endl;
        cout << "\ns (hex) = \n";
        ShowZZInHex (s);
        cout << endl;

        //======================================
        line();

        // Проверка ЭЦП
        cout << "\nVerification process.\n";
        ZZ R;
        verifysign ( R, r, s, e, Q, P, q  );

        //======================================
        line();
    }
    return 0;
}
