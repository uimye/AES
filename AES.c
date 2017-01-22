
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#include <malloc.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>
#include <sys/poll.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "IniFile.h"


#if 0
int candp(int a,int b,int c)
{
    int r = 1;
    b = b + 1;
    while (b != 1)
    {
        r = r * a;
        r = r % c;
        b--;
    }
    printf("%d\n",r);
    return r;
}
void main()
{
    int p,q,e,d,m,n,t,c,r;
    char s;
    printf("please input the p,q: ");
    scanf("%d,%d",&p,&q);
    n = p * q;
    printf("the n is %3d\n",n);
    t = (p - 1) * (q - 1);
    printf("the t is %3d\n",t);
    printf("begin ...\n");
    printf("please input the e: ");
    printf("end end end ...\n");
    scanf("%d",&e);
    printf("ok ok ok\n");
    if(e < 1 || e > t)
    {
         printf("e is error,please input again: ");
         scanf("%d",&e);
    }
    d = 1;
    while (((e * d) % t) != 1)   d++;
    printf("then caculate out that the d is %d\n",d);
    printf("the cipher please input 1\n");
    printf("the plain please input 2\n");
    scanf("%d",&r);
    switch(r)
    {
        case 1: printf("input the m: "); /*输入要加密的明文数字*/
                scanf("%d",&m);
                c = candp(m,e,n);
                printf("the cipher is %d\n",c);break;
        case 2: printf("input the c: "); /*输入要解密的密文数字*/
                scanf("%d",&c);
                m = candp(c,d,n);
                printf("the cipher is %d\n",m);break;
    }
}
#endif



/* symmetric bitwise swapping */
#define SYMM_SWAP(ch) (							\
	((ch & 1) << 7)                |			\
	((ch & (0b10)) << 5)           |			\
	((ch & (0b100)) << 3)          |			\
	((ch & (0b1000)) << 1)         |			\
	((ch & (0b10000)) >> 1)        |			\
	((ch & (0b100000)) >> 3)       |			\
	((ch & (0b1000000)) >> 5)      |			\
	((ch & (0b10000000)) >> 7))

/* define your own key value here */
#define KEY  0x35

int WzEncrypt(char *text, char *encrypt, int len, char key)
{
    int i;
	for(i = 0; i < len; i++)
	{
		text[i] = (SYMM_SWAP(text[i]) ^ key);
	}
    memcpy(encrypt, text, len);
    return 0;
}

int WzDecrypt(char *encrypt, char *text, int len, char key)
{
    int i = 0;
	for(i = 0; i < len; i++)
	{
		encrypt[i] ^= key;
		encrypt[i] = SYMM_SWAP(encrypt[i]);
	}
    memcpy(text, encrypt, len);

    return 0;
}


int main(int argc, char *argv[])
{
    if (3 != argc)
    {
        printf("Usage: \n");
        printf("    AES appkey appsecret\n");
        return -1;
    }
    //char text[1024] = "470c17efe693dd73";
    char appkey_text[256] = {0};
    char appsecret_text[256] = {0};
    char appkey_encrypt[256] = {0};
    char appsecret_encrypt[256] = {0};
    char key = 0x00;
    char hardware[64] = {0};
    int i = 0;
    IniFile ini;

    strncpy(appkey_text, argv[1], sizeof(appkey_text) - 1);
    strncpy(appsecret_text, argv[2], sizeof(appsecret_text) - 1);

    printf("appkey_text = %s\n", appkey_text);
    printf("appsecret_text = %s\n", appsecret_text);

	if (0 == ini.open("ver.ini"))
	{
        ini.read_string("base", "hardware", "ccccc", hardware, sizeof(hardware) - 1);
		ini.close();
	}

    int checksum = 0x00;
    for (i = 0; i < strlen(hardware); i++)
    {
        checksum += hardware[i];
    }
    key = (checksum & 0x000000ff);

    printf("hardware = %s, key = 0x%2x \n", hardware, key);
    WzEncrypt(appkey_text, appkey_encrypt, strlen(appkey_text), key);
    WzEncrypt(appsecret_text, appsecret_encrypt, strlen(appsecret_text), key);
	printf("after encrypt, appkey_encrypt : %s\n", appkey_encrypt);
    printf("after encrypt, appsecret_encrypt : %s\n", appsecret_encrypt);
    
	if (0 == ini.open("ver.ini"))
	{
        ini.write_string("base","app_key", appkey_encrypt);
        ini.write_string("base","app_secret", appsecret_encrypt);
		ini.close();
	}

    #if 0
    memset(encrypt, 0, sizeof(encrypt));
    int encrypted_len = strlen(encrypt);
    printf("strlen (encrypt) = %d\n", encrypted_len);
    memset(text, 0, sizeof(text));
    WzDecrypt(encrypt, text, encrypted_len, key);
    printf("after decrypt, text = %s\n", text);
    #endif

	return 0;
}

#if 0
#include <stdio.h>
char swapbit(char c) {
char i,num = 0,ch[8];
for(i = 0;i < 8;i++) {
ch[i] = c&1;
c = (c >> 1);
}
for(i = 0;i < 8;i++) {
num = 2*num + ch[i];
}
return num;
}
int main() {
char ch;
for(ch = 'A';ch <= 'Z';ch++) {
printf("%c = %X : %X\n",ch,ch,0XFF&swapbit(ch));
}
return 0;
}
#endif

#if 0
#define XOR 107

for(int i=0;str[i];i++) str[i]^=XOR
#endif



