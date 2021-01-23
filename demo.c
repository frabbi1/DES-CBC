#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>

#define ENC 1
#define DEC 0

DES_key_schedule key;

void strToHex(const_DES_cblock input, unsigned char *output) {
    int arSize = 8;
    unsigned int byte;
    for(int i=0; i<arSize; i++) {
        if(sscanf(input, "%2x", &byte) != 1) {
            break;
        }
        output[i] = byte;
        input += 2;
    }

}

void copyValue(const_DES_cblock val1, unsigned char *val2, int size) {
    
    for(int i=0; i<size; i++) {
        val2[i] = val1[i];
    }
}

void doBitwiseXor(DES_LONG *xorValue, DES_LONG* data, const_DES_cblock roundOutput) {
    DES_LONG temp[2];
    memcpy(temp, roundOutput, 8*sizeof(unsigned char));
    for(int i=0; i<2; i++) {
        xorValue[i] = temp[i] ^ data[i];
    }
}


void printvalueOfDesBlock(const_DES_cblock val) {
    for(int i=0; i<8; i++) {
        printf("%u\n", val[i]);
    }
    printf("\n");
}

void printvalueOfDES_LONG(DES_LONG* val) {
    for(int i=0; i<2; i++) {
        printf("%u\n", val[i]);
    }
    printf("\n");
}

void doCBCenc(DES_LONG *data, const_DES_cblock roundOutput, FILE *outFile) {
    DES_LONG in[2];
    doBitwiseXor(in, data, roundOutput);

    DES_encrypt1(in,&key,ENC);
    printf("ENCRYPTED\n");
    printvalueOfDES_LONG(in);
    fwrite(in, 8, 1, outFile);

    memcpy(roundOutput, in, 2*sizeof(DES_LONG));
}

void printChars(DES_LONG i) {
    unsigned char c[4];

    c[0] = i & 0xFF;
    c[1] = (i>>8) & 0xFF;
    c[2] = (i>>16) & 0xFF;
    c[3] = (i>>24) & 0xFF;

    printf("%c", c[0]);
    printf("%c", c[1]);
    printf("%c", c[2]);
    printf("%c", c[3]);

}

int main(int argc, char** argv)
{
    if(argc != 5) {
        printf("USAGE ERROR \nusage: ./exec_file IV key input_file out_file");
    }
	else {
        const_DES_cblock cbc_key = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
        const_DES_cblock IV = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
        
        int k;


        strToHex(argv[1], IV);
        strToHex(argv[2], cbc_key);

        // printvalueOfDesBlock(IV);
        // printvalueOfDesBlock(cbc_key);

        if ((k = DES_set_key_checked(&cbc_key,&key)) != 0)
		    printf("\nkey error\n");

        DES_LONG data[2] = {0, 0};
        FILE *inpFile;
        FILE *outFile;


        inpFile = fopen(argv[3], "r");
        outFile = fopen("demo.out", "wb");

        if(inpFile && outFile) {
            unsigned char ch;
            int count = 0;
            while(!feof(inpFile)) {
                ch = fgetc(inpFile);
                count++;
                if(count == 8) {
                    count = 0;
                }
            }
            // const_DES_cblock roundOutput;
            // // printvalueOfDesBlock(IV);
            // copyValue(IV, roundOutput, 8);
            // // printvalueOfDesBlock(roundOutput);
            // int succesfulBlockReadSize = fread(data, 1, 8, inpFile);
            // while (succesfulBlockReadSize == 8)
            // {
            //     printChars(data[0]);
            //     printChars(data[1]);
            //     printf("\n");
            //     doCBCenc(data, roundOutput, outFile);
            //     printf("\n");
            //     data[0] = 0; // for automatic padding by 0
            //     data[1] = 0; // for automatic padding by 0
            //     succesfulBlockReadSize = fread(data, 1, 8, inpFile);
            // }
            // if(succesfulBlockReadSize > 0) {
            //     printChars(data[0]);
            //     printChars(data[1]);
            //     printf("\n");
            //     doCBCenc(data, roundOutput, outFile); //for last block
            // }
            // printf("\n");
            
            
        } else {
            printf("Error in opening file\n");
        }

        fclose(inpFile);
        fclose(outFile);
        
    }
}

