#include <unistd.h>  // close
#include <sys/stat.h>  // fstat
#include <fcntl.h>  // open
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <openssl/md5.h>
#include "afp_extract_function.h"

int readToMemory(char *file,uint8_t *arrP);
unsigned long getSize(char *file);

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
    struct stat sb;
    int fd;

    fd = open (file, O_RDONLY);
    if (fd == -1) {
        perror ("open");
        return 0;
    }

    if (fstat (fd, &sb) == -1) {
        perror ("fstat");
        return 0;
    }

    if (close (fd) == -1) {
        perror ("close");
        return 0;
    }
    return sb.st_size;
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
		{0,"XXX","Unknown type",0,extractUnknown},
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

int getSFTypeid(uint32_t sf);
int getSFTypeid(uint32_t sf){
	for (size_t i=0;i<sizeof(SFType)/sizeof(SFTypeT);i++)
			if (SFType[i].id == sf)
					return i;
    return 0;
}

void printSfType(uint32_t sf);
void printSfType(uint32_t sf){
    //print the name of Structured Field Type
	#define SFTAG "SFType"
	int found= getSFTypeid(sf);
    printf("<" SFTAG " pg=\"%i\" des=\"%s\">%s</" SFTAG ">",SFType[found].pg,SFType[found].des,SFType[found].tag);
    #undef SFTAG
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
	printf("<Data><![CDATA[");
    // print data in AFP Explorer form - by using extraction functions
    int index = getSFTypeid(SPOJ(arr[pos+3],arr[pos+4],arr[pos+5]));
    SFType[index].extract(arr, pos+9, len);
	printf("]]></Data>");
	printf("<Padding>");
	printf("</Padding>");
	//hex
	printf("<Hex>");
	for (i=0;i<=len;i++) printf("%.2x",arr[pos+i]);
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

int md5(char *filename, unsigned char *c);
int md5(char *filename, unsigned char *c){
    FILE *inFile = fopen (filename, "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];

    if (inFile == NULL) {
        printf ("%s can't be opened.\n", filename);
        return -1;
    }

    MD5_Init (&mdContext);
    while ((bytes = fread (data, 1, 1024, inFile)) != 0)
        MD5_Update (&mdContext, data, bytes);
    MD5_Final (c,&mdContext);
    fclose (inFile);
    return 0;
}

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

    if (!strcmp(argv[1], "--version")){
        printf("afp2afpxml version %s\n", VERSION);
        return 0;
    }

	//get size of file (number of bytes)
	size=getSize(argv[1]);
    if (size == 0) {
        perror ("getSize");
        return -6;
    }


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
    // md5 checksum
    unsigned char *c = (unsigned char *) malloc (sizeof(char) * MD5_DIGEST_LENGTH);
    md5(argv[1], c);
	printf("<FileCheckSum hash=\"md5\">");
    for(i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", c[i]);
    printf("</FileCheckSum>");
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
