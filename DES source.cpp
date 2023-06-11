#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
설명 읽는 법
초기 설정 변수 이름 : 어떤 변수인지 설명
DES에서 무슨 역할 및 구조를 하는지 설명

IP : 초기치환표 (암호학적 의미 X)

IP_1 : 최종 치환표 (암호학적 의미 X)

F_EXP : 확장 P-박스(Expansion P-box)
입력값 RI-1 는 32비트 값이고, 라운드 키 KI 는 48비트이기 때문에, 32비트 RI-1 값을 48비트 값으로 확장해야 한다.

MAT_PERMU : 단순 치환(Straight Permutation)표
DES 함수에서 마지막 연산은 32비트 입력과 32비트 출력을 갖는 단순 치환이다.

KEY_PC_1 : 초기 KEY 비트 줄이기를 위한 표
Key 와 Parity bit 를 포함한 64비트 값을 입력받아 ParityDrop(PC1) 과정에서 PC1 테이블에 의해 8의 배수가 버려진다. 
따라서 이 연산 결과 값으로 56비트 암호키를 얻게 된다.(64bit -> 56bit)

KEY_PC_2 : 마지막 KEY 비트 줄이기를 위한 표
각 28비트 블록은 PC2 연산 - Compression(압축) 전치를 통해 합쳐진 56비트가 라운드함수에서 사용될 48비트 서브키가 된다.
이때도 1단계와 마찬가지로 입력 값의 8배수 비트는 버려진다.(56bit -> 48bit)

SBOX : SB0X 표
S-박스는 실제로 섞어주는 역할을 수행한다. 즉 혼돈 역할을 수행한다. 
DES 는 각각 6비트 입력값과 4비트 출력값을 갖는 8개의 S-박스를 사용한다. 
48비트의 데이터는 8개의 6비트 값으로 나누어지고, 각 6비트 값은 하나의 S-박스로 들어간다. 각 S-박스의 결과는 4비트 값이 된다.



*/

char IP[64] = { 58, 50, 42, 34, 26 ,18 ,10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17, 9, 1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7 }; 

char IP_1[64] = { 40, 8, 48, 16, 56, 24, 64, 32,
		  39, 7, 47, 15, 55, 23, 63, 31,
		  38, 6, 46, 14, 54, 22, 62, 30,
		  37, 5, 45, 13, 53, 21, 61, 29,
		  36, 4, 44, 12, 52, 20, 60, 28,
		  35, 3, 43, 11, 51, 19, 59, 27,
		  34, 2, 42, 10, 50, 18, 58, 26,
		  33, 1, 41, 9, 49, 17, 57, 25 };

char F_EXP[48] = { 32, 1, 2, 3, 4, 5,
		 4, 5, 6, 7, 8, 9,
		 8, 9, 10, 11, 12, 13,
		 12, 13, 14, 15, 16, 17,
		 16, 17, 18, 19, 20, 21,
		 20, 21, 22, 23, 24, 25,
		 24, 25, 26, 27, 28, 29,
		 28, 29, 30, 31, 32, 1 };

char MAT_PERMU[32] = { 16, 7, 20, 21, 29, 12, 28, 17,
		   1, 15, 23, 26, 5, 18, 31, 10,
		   2, 8, 24, 14, 32, 27, 3, 9,
		   19, 13, 30, 6, 22, 11, 4, 25 };

char KEY_PC_1[56] = { 57, 49, 41, 33, 25, 17, 9,
		  1, 58, 50, 42, 34, 26, 18,
		  10, 2, 59, 51, 43, 35, 27,
		  19, 11, 3, 60, 52, 44, 36,
		  63, 55, 47, 39, 31, 23, 15,
		  7, 62, 54, 46, 38, 30, 22,
		  14, 6, 61, 53, 45, 37, 29,
		  21, 13, 5, 28, 20, 12, 4 };

char KEY_PC_2[48] = { 14, 17, 11, 24, 1, 5, 3, 28,
		  15, 6, 21, 10, 23, 19, 12, 4,
		  26, 8, 16, 7, 27, 20, 13, 2,
		  41, 52, 31, 37, 47, 55, 30, 40,
		  51, 45, 33, 48, 44, 49, 39, 56,
		  34, 53, 46, 42, 50, 36, 29, 32 };

unsigned char SBOX[8][4][16] = {
	{
		{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
		{0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
		{4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
		{15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}
	},
	{
		{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
		{3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
		{0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
		{13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}
	},
	{
		{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
		{13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
		{13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
		{1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}
	},
	{
		{7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
		{13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
		{10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
		{3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}
	},
	{
		{2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
		{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
		{4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
		{11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}
	},
	{
		{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
		{10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
		{9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
		{4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}
	},
	{
		{4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
		{13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
		{1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
		{6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}
	},
	{
		{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
		{1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
		{7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
		{2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
	} };
char Key[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1 };
int RKey[16][48];

void Keygenerator()
{
	char PCout[56];
	char t0[56];
	char C0[28], D0[28];
	char C1[28], D1[28];
	int i;

	// Permuted choice 1
	//Key 와 Parity bit 를 포함한 64비트 값을 입력받아 ParityDrop(PC1) 과정에서 PC1 테이블에 의해 8의 배수가 버려진다. 
	// 이 연산 결과 값으로 56비트 암호키를 얻게 된다.(64bit -> 56bit)

	for (i = 0; i < 56; i++)
	{
		PCout[i] = Key[KEY_PC_1[i] - 1];
	}


	//memcpy 
	//56비트 키를 Key swap 연산을 통해 두 개의 28비트 블록으로 나눈다.
	memcpy(C0, PCout, sizeof(char) * 28);
	memcpy(D0, PCout + 28, sizeof(char) * 28);

	//키 생성 부분

	//Round 1 Key
	for (i = 0; i < 27; i++)
	{
		C1[i] = C0[i + 1];
	}
	C1[27] = C0[0];

	for (i = 0; i < 27; i++)
	{
		D1[i] = D0[i + 1];
	}
	D1[27] = D0[0];

	memcpy(t0, C1, sizeof(char) * 28);
	memcpy(t0 + 28, D1, sizeof(char) * 28);

	//PC2->R1
	for (i = 0; i < 48; i++)
	{
		RKey[0][i] = t0[KEY_PC_2[i] - 1];
	}


	// Round 2 key
	//1. shift rotate C,D에 있는 비트를 밀어넘긴다.
	//2. PC2 => Roundkey1 =[R[0][0~47] C,D를 합여서 키 생성한다.

	//1. shift rotate
	for (i = 0; i < 27; i++)
	{
		C0[i] = C1[i + 1];
	}
	C0[27] = C1[0];

	for (i = 0; i < 27; i++)
	{
		D0[i] = D1[i + 1];
	}
	D0[27] = D1[0];

	memcpy(t0, C0, sizeof(char) * 28);
	memcpy(t0 + 28, D0, sizeof(char) * 28);

	//PC2->R1
	for (i = 0; i < 48; i++)
	{
		RKey[1][i] = t0[KEY_PC_2[i] - 1];
	}


	//Round 3 Key

	//1. shift rotate
	for (i = 0; i < 26; i++)
	{
		C1[i] = C0[i + 2];
	}
	C1[26] = C0[0];
	C1[27] = C0[1];

	for (i = 0; i < 26; i++)
	{
		D1[i] = D0[i + 2];
	}
	D1[26] = D0[0];
	D1[27] = D0[1];

	memcpy(t0, C1, sizeof(char) * 28);
	memcpy(t0 + 28, D1, sizeof(char) * 28);


	//2. PC2 -> Roundkeyi = R[0?][0~47]

	//PC2->R2
	for (i = 0; i < 48; i++)
	{
		RKey[2][i] = t0[KEY_PC_2[i] - 1];
	}


	//Round 4 Key
	//shift rotate
	for (i = 0; i < 26; i++)
	{
		C0[i] = C1[i + 2];
	}
	C0[26] = C1[0];
	C0[27] = C1[1];

	for (i = 0; i < 26; i++)
	{
		D0[i] = D1[i + 2];
	}
	D0[26] = D1[0];
	D0[27] = D1[1];

	memcpy(t0, C0, sizeof(char) * 28);
	memcpy(t0 + 28, D0, sizeof(char) * 28);

	//PC2-> R4
	for (i = 0; i < 48; i++)
	{
		RKey[3][i] = t0[KEY_PC_2[i] - 1];
	}


	//Round 5 Key

	for (i = 0; i < 26; i++)
	{
		C1[i] = C0[i + 2];
	}
	C1[26] = C0[0];
	C1[27] = C0[1];

	for (i = 0; i < 26; i++)
	{
		D1[i] = D0[i + 2];
	}
	D1[26] = D0[0];
	D1[27] = D0[1];

	memcpy(t0, C1, sizeof(char) * 28);
	memcpy(t0 + 28, D1, sizeof(char) * 28);

	//PC2-> R5
	for (i = 0; i < 48; i++)
	{
		RKey[4][i] = t0[KEY_PC_2[i] - 1];
	}


	//Round 6 Key
	for (i = 0; i < 26; i++)
	{
		C0[i] = C1[i + 2];
	}
	C0[27] = C1[1];
	C0[26] = C1[0];

	for (i = 0; i < 26; i++)
	{
		D0[i] = D1[i + 2];
	}
	D0[27] = D1[1];
	D0[26] = D1[0];

	memcpy(t0, C0, sizeof(char) * 28);
	memcpy(t0 + 28, D0, sizeof(char) * 28);

	//PC2->R6
	for (i = 0; i < 48; i++)
	{
		RKey[5][i] = t0[KEY_PC_2[i] - 1];
	}


	//Round 7 Key
	for (i = 0; i < 26; i++)
	{
		C1[i] = C0[i + 2];
	}
	C1[27] = C0[1];
	C1[26] = C0[0];
	for (i = 0; i < 26; i++)
	{
		D1[i] = D0[i + 2];
	}
	D1[27] = D0[1];
	D1[26] = D0[0];

	memcpy(t0, C1, sizeof(char) * 28);
	memcpy(t0 + 28, D1, sizeof(char) * 28);

	//PC2->R7
	for (i = 0; i < 48; i++)
	{
		RKey[6][i] = t0[KEY_PC_2[i] - 1];
	}

	//Round 8 Key
	for (i = 0; i < 26; i++)
	{
		C0[i] = C1[i + 2];
	}
	C0[26] = C1[0];
	C0[27] = C1[1];
	for (i = 0; i < 26; i++)
	{
		D0[i] = D1[i + 2];
	}
	D0[26] = D1[0];
	D0[27] = D1[1];

	memcpy(t0, C0, sizeof(char) * 28);
	memcpy(t0 + 28, D0, sizeof(char) * 28);

	//PC2->R8
	for (i = 0; i < 48; i++)
	{
		RKey[7][i] = t0[KEY_PC_2[i] - 1];
	}

	//Round 9 Key
	for (i = 0; i < 27; i++)
	{
		C1[i] = C0[i + 1];
	}
	C1[27] = C0[0];

	for (i = 0; i < 27; i++)
	{
		D1[i] = D0[i + 1];
	}
	D1[27] = D0[0];

	memcpy(t0, C1, sizeof(char) * 28);
	memcpy(t0 + 28, D1, sizeof(char) * 28);

	//PC2->R9
	for (i = 0; i < 48; i++)
	{
		RKey[8][i] = t0[KEY_PC_2[i] - 1];
	}

	//Round 10 Key
	for (i = 0; i < 26; i++)
	{
		C0[i] = C1[i + 2];
	}
	C0[26] = C1[0];
	C0[27] = C1[1];
	for (i = 0; i < 26; i++)
	{
		D0[i] = D1[i + 2];
	}
	D0[26] = D1[0];
	D0[27] = D1[1];

	memcpy(t0, C0, sizeof(char) * 28);
	memcpy(t0 + 28, D0, sizeof(char) * 28);

	//PC2->R10
	for (i = 0; i < 48; i++)
	{
		RKey[9][i] = t0[KEY_PC_2[i] - 1];
	}
	//Round 11 Key
	for (i = 0; i < 26; i++)
	{
		C1[i] = C0[i + 2];
	}
	C1[26] = C0[0];
	C1[27] = C0[1];
	for (i = 0; i < 26; i++)
	{
		D1[i] = D0[i + 2];
	}
	D1[27] = D0[1];
	D1[26] = D0[0];

	memcpy(t0, C1, sizeof(char) * 28);
	memcpy(t0 + 28, D1, sizeof(char) * 28);

	//PC2->R11
	for (i = 0; i < 48; i++)
	{
		RKey[10][i] = t0[KEY_PC_2[i] - 1];
	}

	//Round 12 Key
	for (i = 0; i < 26; i++)
	{
		C0[i] = C1[i + 2];
	}
	C0[26] = C1[0];
	C0[27] = C1[1];
	for (i = 0; i < 26; i++)
	{
		D0[i] = D1[i + 2];
	}
	D0[26] = D1[0];
	D0[27] = D1[1];

	memcpy(t0, C0, sizeof(char) * 28);
	memcpy(t0 + 28, D0, sizeof(char) * 28);

	//PC2->R12
	for (i = 0; i < 48; i++)
	{
		RKey[11][i] = t0[KEY_PC_2[i] - 1];
	}

	//Round 13 Key
	for (i = 0; i < 26; i++)
	{
		C1[i] = C0[i + 2];
	}
	C1[26] = C0[0];
	C1[27] = C0[1];
	for (i = 0; i < 26; i++)
	{
		D1[i] = D0[i + 2];
	}
	D1[26] = D0[0];
	D1[27] = D0[1];

	memcpy(t0, C1, sizeof(char) * 28);
	memcpy(t0 + 28, D1, sizeof(char) * 28);

	for (i = 0; i < 48; i++)
	{
		RKey[12][i] = t0[KEY_PC_2[i] - 1];


	}
	//Round 14 Key

	for (i = 0; i < 26; i++)
	{
		C0[i] = C1[i + 2];
	}
	C0[26] = C1[0];
	C0[27] = C1[1];
	for (i = 0; i < 26; i++)
	{
		D0[i] = D1[i + 2];
	}
	D0[27] = D1[1];
	D0[26] = D1[0];

	memcpy(t0, C0, sizeof(char) * 28);
	memcpy(t0 + 28, D0, sizeof(char) * 28);
	//PC2->R14
	for (i = 0; i < 48; i++)
	{
		RKey[13][i] = t0[KEY_PC_2[i] - 1];
	}


	//Round 15 Key

	for (i = 0; i < 26; i++)
	{
		C1[i] = C0[i + 2];
	}
	C1[26] = C0[0];
	C1[27] = C0[1];
	for (i = 0; i < 26; i++)
	{
		D1[i] = D0[i + 2];
	}
	D1[26] = D0[0];
	D1[27] = D0[1];

	memcpy(t0, C1, sizeof(char) * 28);
	memcpy(t0 + 28, D1, sizeof(char) * 28);
	//PC2->R15
	for (i = 0; i < 48; i++)
	{
		RKey[14][i] = t0[KEY_PC_2[i] - 1];
	}

	//Round 16 Key
	for (i = 0; i < 27; i++)
	{
		C0[i] = C1[i + 1];
	}
	C0[27] = C1[0];
	for (i = 0; i < 27; i++)
	{
		D0[i] = D1[i + 1];
	}
	D0[27] = D1[0];

	memcpy(t0, C0, sizeof(char) * 28);
	memcpy(t0 + 28, D0, sizeof(char) * 28);

	//PC2->R16
	for (i = 0; i < 48; i++)
	{
		RKey[15][i] = t0[KEY_PC_2[i] - 1];
	}


	// i=28 CD1[28]= (D[0]) = CD0[29]
}

void printvector()
{

	int i;
	unsigned char tmp[8];

	for (int j = 0; j < 16; j++)
	{
		memset(tmp, 0, sizeof(char) * 8);
		for (i = 0; i < 48; i++)
		{

			tmp[i >> 3] ^= (RKey[j][i] & 1) << (7 - (i % 8));
		}


		printf(" Round Key %d : ", j);
		for (i = 0; i < (48 >> 3); i++)
		{
			printf(" %.2X ", tmp[i]);
		}
		printf("\n");

	}

}

void printRound(char* L0, char* R0)
{

	//라운드를 돌리는 함수.

	int i;
	unsigned char tmp[8];

	memset(tmp, 0, sizeof(char) * 8);
	for (i = 0; i < 32; i++)
	{

		tmp[i >> 3] ^= (L0[i] & 1) << (7 - (i % 8));
	}


	printf(" OUT : ");
	for (i = 0; i < (32 >> 3); i++)
	{
		printf(" %.2X ", tmp[i]);
	}


	memset(tmp, 0, sizeof(char) * 8);
	for (i = 0; i < 32; i++)
	{

		tmp[i >> 3] ^= (R0[i] & 1) << (7 - (i % 8));
	}



	for (i = 0; i < (32 >> 3); i++)
	{
		printf(" %.2X ", tmp[i]);
	}

	printf("\n");
}

void SBOX_function(char out1, char out2, char out3, char out4, char s0, char s1, char s2, char s3, char s4, char s5, int num)
{
	//S-Box 연산 부분
	//S-박스는 실제로 섞어주는 역할을 수행한다. 즉 혼돈 역할을 수행한다. 
	//DES 는 각각 6비트 입력값과 4비트 출력값을 갖는 8개의 S-박스를 사용한다. 
	//48비트의 데이터는 8개의 6비트 값으로 나누어지고, 각 6비트 값은 하나의 S-박스로 들어간다. 
	//각 S-박스의 결과는 4비트 값이 된다.

	char r0, c0, a0, a1, a2, a3, sbox_out;

	r0 = (((s0 & 1) << 1) ^ (s5 & 1));
	a0 = (s1 & 1) << 3;
	a1 = (s2 & 1) << 2;
	a2 = (s3 & 1) << 1;
	a3 = (s4 & 1);
	c0 = a0 ^ a1 ^ a2 ^ a3;

	sbox_out = SBOX[num][r0][c0];
	out1 = ((sbox_out & 0x8) >> 3) & 1;
	out2 = ((sbox_out & 0x4) >> 2) & 1;
	out3 = ((sbox_out & 0x2) >> 1) & 1;
	out4 = ((sbox_out & 0x1)) & 1;


}

void SBOX_new(char* out, char* in, int num)
{
	char r0, c0, a0, a1, a2, a3, sbox_out;

	r0 = (((in[0] & 1) << 1) ^ (in[5] & 1));
	a0 = (in[1] & 1) << 3;
	a1 = (in[2] & 1) << 2;
	a2 = (in[3] & 1) << 1;
	a3 = (in[4] & 1);
	c0 = a0 ^ a1 ^ a2 ^ a3;

	sbox_out = SBOX[num][r0][c0];
	out[0] = ((sbox_out & 0x8) >> 3) & 1;
	out[1] = ((sbox_out & 0x4) >> 2) & 1;
	out[2] = ((sbox_out & 0x2) >> 1) & 1;
	out[3] = ((sbox_out & 0x1)) & 1;

}

void F_function(char* out, char* in, int num)
{
	//DES F-funcion
	
	char s0[48], t0[48];
	char i;
	char Sout[32];

	// 확장 P-BOX로 32bit -> 48bit

	for (i = 0; i < 48; i++)
	{
		t0[i] = in[F_EXP[i] - 1];
	}

	// Key와 input값 XOR연산 (48bit)

	for (i = 0; i < 48; i++)
	{
		s0[i] = t0[i] ^ RKey[num][i];
	}

	//연산 한 결과를 S-BOX에 넣어준다 (48bit -> 32bit)

	for (i = 0; i < 8; i++)
	{
		SBOX_new(Sout + 4 * i, s0 + 6 * i, i);
	}

	//단순 치환 P-BOX를 거친다 (32bit)

	for (i = 0; i < 32; i++)
	{
		out[i] = Sout[MAT_PERMU[i] - 1];
	}

}

void Round(char* out1, char* out2, char* in1, char* in2, int num)
{
	char v0[32];
	int i;

	//input이 들어오면 F-funcion을 거치고
	F_function(v0, in2, num);

	//L과 R 위치 바꾸기
	for (i = 0; i < 32; i++)
	{
		v0[i] ^= in1[i];
	}
	//다음 L과 R로 지정
	memcpy(out1, in2, sizeof(char) * 32);
	memcpy(out2, v0, sizeof(char) * 32);
}

void main()
{

	Keygenerator(); // 키 생성
	printvector(); // 출력을 위한 함수 (DES 알고리즘 X)
	char Msg[64] = { 0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,1,0,0,0,1,0,1,0,1,1,0,0,1,1,1,1,0,0,0,1,0,0,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,1,1,1,1,0,1,1,1,1 };
	char IPout[64];
	char L0[32], R0[32];
	int i;
	char v0[32], u0[32];

	//IP
	for (i = 0; i < 64; i++)
	{
		IPout[i] = Msg[IP[i] - 1];
	}
	memcpy(L0, IPout, sizeof(char) * 32);
	memcpy(R0, IPout + 32, sizeof(char) * 32);
	printRound(L0, R0);

	//총 16번의 라운드를 거쳐야 한다
	//라운드 2번
	Round(L0, R0, L0, R0, 0);
	printRound(L0, R0);
	//라운드 3번
	Round(L0, R0, L0, R0, 1);
	printRound(L0, R0);
	//라운드 4번
	Round(L0, R0, L0, R0, 2);
	printRound(L0, R0);
	//라운드 5번
	Round(L0, R0, L0, R0, 3);
	printRound(L0, R0);
	//라운드 6번
	Round(L0, R0, L0, R0, 4);
	printRound(L0, R0);
	//라운드 7번
	Round(L0, R0, L0, R0, 5);
	printRound(L0, R0);
	//라운드 8번
	Round(L0, R0, L0, R0, 6);
	printRound(L0, R0);
	//라운드 9번
	Round(L0, R0, L0, R0, 7);
	printRound(L0, R0);
	//라운드 10번
	Round(L0, R0, L0, R0, 8);
	printRound(L0, R0);
	//라운드 11번
	Round(L0, R0, L0, R0, 9);
	printRound(L0, R0);
	//라운드 12번
	Round(L0, R0, L0, R0, 10);
	printRound(L0, R0);
	//라운드 13번
	Round(L0, R0, L0, R0, 11);
	printRound(L0, R0);
	//라운드 14번
	Round(L0, R0, L0, R0, 12);
	printRound(L0, R0);
	//라운드 15번
	Round(L0, R0, L0, R0, 13);
	printRound(L0, R0);
	//라운드 16번
	Round(L0, R0, L0, R0, 14);
	printRound(L0, R0);

	F_function(v0, R0, 15);
	for (i = 0; i < 32; i++)
	{
		u0[i] = L0[i] ^ v0[i];
	}
	memcpy(L0, u0, sizeof(char) * 32);

	char w0[64];
\
	memcpy(w0, L0, sizeof(char) * 32);
	memcpy(w0 + 32, R0, sizeof(char) * 32);

	// 최종 치환
	for (i = 0; i < 32; i++)
	{
		L0[i] = w0[IP_1[i] - 1];
	}
	for (i = 0; i < 32; i++)
	{
		R0[i] = w0[IP_1[i + 32] - 1];
	}


	printRound(L0, R0);

	// IP-1

	getchar();
}
