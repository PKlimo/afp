#include "ebcdic.h"

#define UNUSED(x) (void)(x)
void extractTle (uint8_t *arrP,unsigned long pos,unsigned int end);
void extractImm (uint8_t *arrP,unsigned long pos,unsigned int end);
void extractBpg (uint8_t *arrP,unsigned long pos,unsigned int end);
void extractEpg (uint8_t *arrP,unsigned long pos,unsigned int end);
void extractBrs (uint8_t *arrP,unsigned long pos,unsigned int end);
void extractBmm (uint8_t *arrP,unsigned long pos,unsigned int end);
void extractMmc (uint8_t *arrP,unsigned long pos,unsigned int end);
void extractNop (uint8_t *arrP,unsigned long pos,unsigned int end);
void extractEdt (uint8_t *arrP,unsigned long pos, unsigned int end);
void extractBrg (uint8_t *arrP,unsigned long pos, unsigned int end);

void extractTle (uint8_t *arrP,unsigned long pos,unsigned int end){
	UNUSED(end);
	uint8_t *arr;
	arr=arrP;
	int i;

	printf("TLE       ItemName=");

	if (arr[pos+1] != 0x02) printf("Wrong detecion at position %lu",pos);
	else {
		for (i=0; i<arr[pos]-4 ; i++)
			EBC2ASCII(arr[pos+4+i]);//name of attribut
		printf("  ItemValue=");

		for (i=0; i<arr[pos+arr[pos]]-4 ; i++)
			EBC2ASCII(arr[pos+arr[pos]+4+i]);//value of attribute

		printf("  PgName=  PgGroupName=\n");
	}
}

void extractImm (uint8_t *arrP,unsigned long pos,unsigned int end){
	UNUSED(end);
	uint8_t *arr;
	arr=arrP;
	int i;

	printf("IMM       ");

	for (i=0; i<8 ; i++)
		EBC2ASCII(arr[pos+i]);//name of mediatype
	printf("\n");
}

void extractBpg (uint8_t *arrP,unsigned long pos,unsigned int end){
    //void extractBpg (uint8_t *arrP,int pos)
	UNUSED(end);
	UNUSED(pos);
	UNUSED(arrP);

	printf("BPG       ");
	printf("MM=  MMpageNum=0  PGPrepeatgroup=0\n");
}

void extractEpg (uint8_t *arrP,unsigned long pos,unsigned int end){
	UNUSED(end);
	UNUSED(pos);
	UNUSED(arrP);

	printf("EPG       \n");
}

void extractBrs (uint8_t *arrP,unsigned long pos,unsigned int end){
	UNUSED(end);
	uint8_t *arr;
	arr=arrP;
	int i;

	printf("BR        ");

	for (i=0; i<8 ; i++)
		EBC2ASCII(arr[pos+i]);//name of resource
	printf("  ");
	//X21 triplet - Resource object type
	if (arr[pos+10] == 0x0a && arr[pos+11] == 0x21 && arr[pos+13] == 0 && arr[pos+14] == 0 && arr[pos+15] == 0 && arr[pos+16] == 0 && arr[pos+17] == 0 && arr[pos+18] == 0) {
		printf("Type=");
		switch (arr[pos+12]){
			case 0xfe: printf("FormDef Object");break;
			case 0x40: printf("Font Character Set Object");break;
		}
	}

	printf("\n");
}

void extractBmm (uint8_t *arrP,unsigned long pos,unsigned int end){
	UNUSED(end);
	uint8_t *arr;
	arr=arrP;
	int i;

	printf("BMM       ");

	for (i=0; i<8 ; i++)
		EBC2ASCII(arr[pos+i]);//name of resource
	printf("\n");
}

void extractMmc (uint8_t *arrP,unsigned long pos,unsigned int end){
	uint8_t *arr;
	arr=arrP;
	unsigned int i;

	printf("MMC       Jog=False  ");
	for (i=0;i<end-8;i++){
		if (arr[pos+i]==0xf4 && arr[pos+i+1]==0x01) printf("Duplex=No  ");
		if (arr[pos+i]==0xf4 && arr[pos+i+1]==0x02) printf("Duplex=Yes ");//Normal duplex. The media is turned around the Ym axis. 
		if (arr[pos+i]==0xf4 && arr[pos+i+1]==0x03) printf("Duplex=Yes ");//Tumble duplex. The media is turned around the Xm axis.
	}
	for (i=0;i<end-8;i++){
		if (arr[pos+i]==0xfc && arr[pos+i+1]==0x01) printf("nUP=1  ");//pos+i+1 number of partitioning
	}

	for (i=0;i<end-8;i++){
		if (arr[pos+i]==0xe0 && arr[pos+i+1]==0x01 && arr[pos+i+2]==0xe1) printf("TrayNum=%i  ",arr[pos+i+3]);
	}

	printf("Quality=255\n");
}

void extractNop (uint8_t *arrP,unsigned long pos,unsigned int end){
	UNUSED(end);
	uint8_t *arr;
	arr=arrP;
	int i;

	printf("NOP       ");

	for (i=0; i<arr[pos-7]-8 ; i++)
		EBC2ASCII(arr[pos+0+i]);//value of NOP
	printf("\n");
}

void extractEdt (uint8_t *arrP,unsigned long pos, unsigned int end){
	UNUSED(end);
	UNUSED(pos);
	UNUSED(arrP);

	return;
}

void extractBrg (uint8_t *arrP,unsigned long pos, unsigned int end){
	UNUSED(end);
	UNUSED(pos);
	UNUSED(arrP);

	return;
}

