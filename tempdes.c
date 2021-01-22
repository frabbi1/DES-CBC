#include<stdio.h>
#include <openssl/des.h>

#define ENC 1
#define DEC 0

int main()
{
	int k;
	DES_LONG	in[2];
	const_DES_cblock cbc_key = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
	DES_key_schedule key;

	if ((k = DES_set_key_checked(&cbc_key,&key)) != 0)
		printf("\nkey error\n");

	in[0] = 3212314;
	in[1] = 1231233;

	printf("DES Clear Text: %u%u\n",in[0],in[1]);
	DES_encrypt1(in,&key,ENC);

	printf("DES Encryption: %u%u\n",in[0],in[1]);

    DES_encrypt1(in,&key,DEC);
}
