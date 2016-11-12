//
//  Reform.c
//  Reform
//
//  Created by Robert Enderson on 10.11.16.
//  Copyright © 2016 Robert Enderson. All rights reserved.
//

#include "Reform.h"

//Pre-counted length values
#define BYTE            8
#define BYTE_SHIFT      7
#define HALF_BIT_SIZE   BIT_SIZE /2

#define CODE            0
#define DECODE          1

//Private functions
uint8_t binaryToDecimal(uint8_t *byte);
void Reform_private(uint8_t *in, uint8_t *out, uint8_t codeOrDecode);

//Global variables
uint8_t mask[8] =
{
    128,
    64,
    32,
    16,
    8,
    4,
    2,
    1
};

//Interface implemantation
void Reform(uint8_t *in, uint8_t *out)
{
    Reform_private(in, out, CODE);
}

void Reform_1(uint8_t *in, uint8_t *out)
{
    Reform_private(in, out, DECODE);
}

//Private functions implementation
void Reform_private(uint8_t *in, uint8_t *out, uint8_t codeOrDecode)
{
//    printf("\n");
    //Output bits
//    for (size_t i = 0; i < SIZE; i++)
//    {
//        for (uint8_t bitCount = 0; bitCount < BYTE; bitCount++)
//        {
//            printf("%u",temp[BYTE * i + bitCount]);
//        }
//        printf("\n");
//    }
//    printf("\n");
    //Reforming
    uint8_t *reform = (uint8_t*)calloc(BIT_SIZE, sizeof(uint8_t));
    if (codeOrDecode == CODE)
    {
        uint8_t *temp = (uint8_t*)calloc(BIT_SIZE, sizeof(uint8_t));
        //Translating to bit representation
        for (size_t i = 0; i < SIZE; i++)
        {
            for (uint8_t bitCount = 0; bitCount < BYTE; bitCount++)
            {
                if((in[i] & mask[bitCount]) == 0)
                    temp[BYTE * i + bitCount] = 0;
                else
                    temp[BYTE * i + bitCount] = 1;
            }
        }
        for (size_t i = 0, j = 0; i < BIT_SIZE; i+=2, j++)
        {
            out[i] = temp[j];
            out[i + 1] = temp[j + HALF_BIT_SIZE];
            printf("%u%u",reform[i], reform[i + 1]);
        }
        free(temp);
    }
    else
    {
        for (size_t i = 0, j = 0; i < BIT_SIZE; i+=2, j++)
        {
            reform[j] = in[i];
            reform[j + HALF_BIT_SIZE] = in[i + 1];
        }
        for (size_t j = 0; j < BIT_SIZE; j+=2)
        {
            printf("%u%u",reform[j], reform[j + 1]);
        }
        //Converting back to normal bytes
        for (size_t i = 0; i < SIZE; i++)
        {
            out[i] = binaryToDecimal(reform + BYTE * i);
        }

    }
    free(reform);
}

uint8_t binaryToDecimal(uint8_t *byte)
{
    uint8_t result = 0;
    for (int bitCount = 0; bitCount < BYTE; bitCount++)
    {
//        printf("%u",byte[bitCount]);
        result = result | byte[bitCount] << (BYTE_SHIFT - bitCount);
    }
//    printf(" = %u",result);
//    printf("\n");
    return result;
}
