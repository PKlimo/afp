#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

const unsigned char code[256]={ 0, 1, 2, 3, 156, 9, 134, 127, 151, 141,     142, 11, 12, 13, 14, 15, 16, 17, 18, 19, 157,     133, 8, 135, 24, 25, 146, 143, 28, 29, 30, 31,     128, 129, 130, 131, 132, 10, 23, 27, 136, 137, 138,     139, 140, 5, 6, 7, 144, 145, 22, 147, 148, 149,     150, 4, 152, 153, 154, 155, 20, 21, 158, 26, 32,     160, 161, 162, 163, 164, 165, 166, 167, 168, 91, 46,     60, 40, 43, 33, 38, 169, 170, 171, 172, 173, 174,     175, 176, 177, 93, 36, 42, 41, 59, 94, 45, 47,     178, 179, 180, 181, 182, 183, 184, 185, 124, 44, 37,     95, 62, 63, 186, 187, 188, 189, 190, 191, 192, 193,     194, 96, 58, 35, 64, 39, 61, 34, 195, 97, 98,     99, 100, 101, 102, 103, 104, 105, 196, 197, 198, 199,     200, 201, 202, 106, 107, 108, 109, 110, 111, 112, 113,     114, 203, 204, 205, 206, 207, 208, 209, 126, 115, 116,     117, 118, 119, 120, 121, 122, 210, 211, 212, 213, 214,     215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225,     226, 187, 228, 229, 230, 231, 123, 65, 66, 67, 68,     69, 70, 71, 72, 73, 232, 233, 234, 235, 236, 237,     125, 74, 75, 76, 77, 78, 79, 80, 81, 82, 238,     239, 240, 241, 242, 243, 92, 159, 83, 84, 85, 86,     87, 88, 89, 90, 244, 245, 246, 247, 248, 249, 48,     49, 50, 51, 52, 53, 54, 55, 56, 57, 250, 251,     252, 253, 254, 255 };

int readToMemory(char *file,uint8_t *arrP);
unsigned long getSize(char *file);
void EBC2ASCII (uint8_t co);

int readToMemory(char *file,uint8_t *arrP){
//copy file to memory
	uint8_t *arr;
	arr=arrP;

	FILE *fd;
	fd=fopen(file,"rb");

	if (fd==NULL) {
		printf("Can not open file");
		return -3;
	}

	int buf;
	uint8_t a,b;
	unsigned long i=0;
	while (!feof(fd)) {
		buf=0;
		fread(&buf, 2, 1, fd);
		a=buf;
		b=buf>>8;
		arr[i]=a;
		arr[i+1]=b;
		i=i+2;
	}

	fclose(fd);

	return i;
}

unsigned long getSize(char *file){
//return size of file
	int buf;
	int v=0;

	FILE *fd;
	fd=fopen(file,"rb");

	if (fd==NULL) {
		printf("Can not open file");
		exit(-3);
	}

	while (!feof(fd)) {
		buf=0;
		fread(&buf, 2, 1, fd);
		v=v+2;
	}

	fclose(fd);

	return v;
}

void EBC2ASCII (uint8_t co){
	//print converted character (from EBCIDIC to ASCII)
	//extern char code[256];
	printf("%c",code[co]);
}

#define UNUSED(x) (void)(x)
void extractTle (uint8_t *arrP,unsigned long pos,unsigned int end);
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

void extractImm (uint8_t *arrP,unsigned long pos,unsigned int end);
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

void extractBpg (uint8_t *arrP,unsigned long pos,unsigned int end);
void extractBpg (uint8_t *arrP,unsigned long pos,unsigned int end){
    //void extractBpg (uint8_t *arrP,int pos)
	UNUSED(end);
	UNUSED(pos);
	UNUSED(arrP);

	printf("BPG       ");
	printf("MM=  MMpageNum=0  PGPrepeatgroup=0\n");
}

void extractEpg (uint8_t *arrP,unsigned long pos,unsigned int end);
void extractEpg (uint8_t *arrP,unsigned long pos,unsigned int end){
	UNUSED(end);
	UNUSED(pos);
	UNUSED(arrP);

	printf("EPG       \n");
}

void extractBrs (uint8_t *arrP,unsigned long pos,unsigned int end);
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

void extractBmm (uint8_t *arrP,unsigned long pos,unsigned int end);
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

void extractMmc (uint8_t *arrP,unsigned long pos,unsigned int end);
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

void extractNop (uint8_t *arrP,unsigned long pos,unsigned int end);
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

void extractEdt (uint8_t *arrP,unsigned long pos, unsigned int end);
void extractEdt (uint8_t *arrP,unsigned long pos, unsigned int end){
	UNUSED(end);
	UNUSED(pos);
	UNUSED(arrP);

	return;
}

void extractBrg (uint8_t *arrP,unsigned long pos, unsigned int end);
void extractBrg (uint8_t *arrP,unsigned long pos, unsigned int end){
	UNUSED(end);
	UNUSED(pos);
	UNUSED(arrP);

	return;
}

#define SPOJ(d3,tt,cc) (d3<<16)+(tt<<8)+cc // d3-class code, tt-field type, cc-category code pg. 49
#define DESSIZE 32
typedef void (*extractSF) (uint8_t *arrP,unsigned long pos, unsigned int end); // pointer to extraction function
typedef struct {
		uint32_t id;
		char tag[4];
		char des[DESSIZE];
		int pg;
		extractSF extract;
} SFTypeT ;

SFTypeT SFType[]={
		{0,"XXX","Unknown type",0,NULL},
		{0xd3a9a8,"EDT","End Document",204,extractEdt},
		{0xd3a090,"TLE","Tag Logical Element",370,extractTle},
		{0xd3abcc,"IMM","Invoke Media Map",226,extractImm},
		{0xd3a8af,"BPG","Begin Page",179,extractBpg},
		{0xd3a9af,"EPG","End Page",214,extractEpg},
		{0xd3a8ce,"BRS","Begin Resource",188,extractBrs},
		{0xd3a8cc,"BMM","Begin Medium Map",164,extractBmm},
		{0xd3a788,"MMC","Medium Modification Control",302,extractMmc},
		{0xd3a8c6,"BRG","Begin Resource Group",186,extractBrg},
		{0xd3eeee,"NOP","No Operation",325,extractNop}
};

/*TODO do sekcie data dat vystup extracnej funkcie*/
/*TODO rozdelit zdrojak tle.c do viacerych suborov - afp specificke funkcie, pomocne funkcie na EBCIDIC, riadiace funkcie*/
/*TODO doplnit tabulku SFType*/
/*TODO zmenit funkciu ReadToMemory, aby nenacitala cely subor, ale iba jeden ramec (Structured Field)*/

void printSfType(uint32_t sf);
void printSfType(uint32_t sf)
//print the name of Structured Field Type
{
	#define SFTAG "SFType"
	size_t found=0;
	for (size_t i=0;i<sizeof(SFType)/sizeof(SFTypeT);i++)
			if (SFType[i].id == sf)
					found=i;
	printf("<" SFTAG " pg=\"%i\" des=\"%s\">%s</" SFTAG ">",SFType[found].pg,SFType[found].des,SFType[found].tag);
}


void process (uint8_t *arrP, unsigned long pos, unsigned int len);
void process (uint8_t *arrP, unsigned long pos, unsigned int len)
//function process field
//inputs: pos (beginning position in array - must be 0x5a) and len (length of field)
{
	uint8_t *arr;
	arr=arrP;
	unsigned int i;
	assert(arr[pos]==0x5a);//pos (beginning position in array - must be 0x5a)
//create XML file
	printf("<StructuredField position=\"%lu\" length=\"%u\">\n",pos,len);
	printf("<StructuredFieldIntroducer>");
	printf("<SFLength>%.2x %.2x</SFLength>\n",arr[pos+1],arr[pos+2]);
	printf("<SFTypeID>%.2x %.2x %.2x</SFTypeID>\n",arr[pos+3],arr[pos+4],arr[pos+5]);
	printSfType(SPOJ(arr[pos+3],arr[pos+4],arr[pos+5]));
	printf("<FlagByte>%.2x</FlagByte>",arr[pos+6]);
	printf("<Reserved>%.2x %.2x</Reserved>",arr[pos+7],arr[pos+8]);
	printf("</StructuredFieldIntroducer>");
	printf("<Data>");
	printf("</Data>");
	printf("<Padding>");
	printf("</Padding>");
	//hex
	printf("<Hex>");
	for (i=0;i<len;i++) printf("%.2x",arr[pos+i]);
	printf("</Hex>\n");
	printf("</StructuredField>\n");
//convert to hex:
//	for (i=0;i<len;i++) printf("%.2x",arr[pos+i]);
//	putchar('\n');

//debug:
//	printf("pos:%d, len:%d, field:",pos,len);
//	for (i=0;i<6;i++) printf("%.2x",arr[pos+i]);
//	putchar('\n');

//copy:
//	for (i=0;i<=len;i++) putchar(arr[pos+i]);

//remove tle:
//	i=pos+3;
//	if (!((arr[i]==0xd3) && (arr[i+1]==0xa0) && (arr[i+2]==0x90) && (arr[i+3]==0) && (arr[i+4]==0) && (arr[i+5]==0) )) //TLE detected, pg. 370, Tag Logical Element
//		for (i=0;i<=len;i++) putchar(arr[pos+i]);

	return;
}

uint8_t *arr;
unsigned long size;

int main(int argc, char **argv)
{
	unsigned long pos;//position in array
	unsigned int len;//length of field
	int i;//temporary counter for loop

	//do some checking of arguments - it is necessary exactly one argument - name of file to be processed
	if (argc == 1) {
		printf("Argument missing");
		return -1;
	}
	if (argc > 2) {
		printf("Too much arguments");
		return -2;
	}

	//get size of file (number of bytes)
	size=getSize(argv[1]);

	//allocate a memory
	arr=malloc(sizeof(uint8_t) * (size+1));
	if (arr==NULL) {
		printf("Can not alocate memory");
		return -4;
	}

	//read file into memory
	readToMemory(argv[1],arr);

	//process each field

	pos=0;
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	printf("<File>");
	printf("<Header>");
	printf("<FileName>%s</FileName>",argv[1]);
	printf("<FileSize>%lu</FileSize>",size);
	printf("<FileCheckSum>%d</FileCheckSum>",0);//TODO md5 checksup of file
	printf("</Header>");
	printf("<MoDca>");
	while (pos < size-1) {
		if (arr[pos]!=0x5a) {//checking prefix
			printf("Error in prefix on position %lu\n",pos);
			for (i=0;i<1000;i++) printf("%.2x",arr[pos+i]);
			return -5;
		}
		len=arr[pos+1]*256+arr[pos+2];//first 2 bytes are length
		process(arr,pos,len);//process the actual field
		pos=pos+len+1;//+1 because of prefix byte 5a
	}
	printf("</MoDca>");
	printf("</File>");

	free(arr);
	return 0;
}
