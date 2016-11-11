//
//  main.cpp
//  ru.bmstu.www.gsm.grypt.a5
//
//  Created by Дмитрий  Загребаев on 10.11.16.
//  Copyright © 2016 Дмитрий  Загребаев. All rights reserved.
//

#include <iostream>
#include <stdint.h>
#include "SecretKey.h"

/* Masks for the three shift registers */
#define R1MASK  0x07FFFF /* 19 bits, numbered 0..18 */
#define R2MASK  0x3FFFFF /* 22 bits, numbered 0..21 */
#define R3MASK  0x7FFFFF /* 23 bits, numbered 0..22 */

using namespace std;

class Secret {

public:
    Secret() {};
    
public:
};

class SecretKey {
    
    private:
        uint64_t LFSR_1;
        uint64_t LFSR_2;
        uint64_t LFSR_3;
        uint64_t key;
        uint64_t logicBits[92];
    
    private:
//        static const uint64_t SIGNIFICANT_BITS = 0xFFFFE00000000000l;
//        static const uint64_t MEAN_BITS = 0x1FFFFF800000l;
//        static const uint64_t LEAST_SIGNIFICANT_BITS = 0x7FFFFFl;
    
    private:
        static const uint64_t LOGIC_ONE = 1;
//        static const uint64_t LOGIC_ONE_64_BIT = 0x8000000000000000l;
//        static const uint64_t LAST_28_BIT = 0xFFFFFFF000000000l;
//        static const uint64_t FIRST_22_BIT = 0x3FFFFF;
        static const uint64_t first_bit_null = 0xFFFFFFFFFFFFFFFEl;
    
    public:
        SecretKey() {
//            logicBits = (uint64_t*)malloc(sizeof(uint64_t) * 92);
            memset(logicBits, 0, sizeof(uint64_t) * 92);
        }
    
public:
    uint64_t* getLogicBits() {
        return this->logicBits;
    }
    
private: void iterations()
    {
        int j = 0;
        for (uint16_t i = 0; i < 192; i++)
        {
            if(i >= 100) {
                printf("_______________________\n");
                *(logicBits + j) =  logicOutput();
                j++;
            } else {
                logicOutput();
            }
            shiftLFSR();
        }
    }
    
private: void shiftLFSR()
    {
        uint8_t bit_10_LFSR_1 = ((LFSR_1 >> 9)  & LOGIC_ONE);
        uint8_t bit_11_LFSR_2 = ((LFSR_2 >> 10) & LOGIC_ONE);
        uint8_t bit_12_LFSR_3 = ((LFSR_3 >> 11) & LOGIC_ONE);
        
        uint8_t end_bit_LFSR_1 = ((LFSR_1 >> 18) & LOGIC_ONE);
        uint8_t end_bit_LFSR_2 = ((LFSR_2 >> 21) & LOGIC_ONE);
        uint8_t end_bit_LFSR_3 = ((LFSR_3 >> 22) & LOGIC_ONE);
        
        if (bit_10_LFSR_1 == bit_11_LFSR_2 && bit_10_LFSR_1 == bit_12_LFSR_3 && bit_11_LFSR_2 == bit_12_LFSR_3) {
            
            LFSR_1 <<= 1;
            LFSR_2 <<= 1;
            LFSR_3 <<= 1;
            
            uint64_t first_bit_LFSR_1 = (bit_10_LFSR_1 + end_bit_LFSR_1) % 2;
            
            if (first_bit_LFSR_1 == 1)
                LFSR_1 |= LOGIC_ONE;
            else
                LFSR_1 &= first_bit_null;
            
            uint64_t first_bit_LFSR_2 = (bit_11_LFSR_2 + end_bit_LFSR_2) % 2;
            
            if (first_bit_LFSR_2 == 1)
                LFSR_2 |= LOGIC_ONE;
            else
                LFSR_2 &= first_bit_null;

            uint64_t first_bit_LFSR_3 = (bit_12_LFSR_3 + end_bit_LFSR_3) % 2;
            
            if (first_bit_LFSR_3 == 1)
                LFSR_3 |= LOGIC_ONE;
            else
                LFSR_3 &= first_bit_null;
        }
        else
        {
            if (bit_10_LFSR_1 == bit_11_LFSR_2)
            {
                LFSR_1 <<= 1;
                LFSR_2 <<= 1;
                
                uint64_t first_bit_LFSR_1 = (bit_10_LFSR_1 + end_bit_LFSR_1) % 2;
                
                if (first_bit_LFSR_1 == 1)
                    LFSR_1 |= LOGIC_ONE;
                else
                    LFSR_1 &= first_bit_null;
                
                uint64_t first_bit_LFSR_2 = (bit_11_LFSR_2 + end_bit_LFSR_2) % 2;
                
                if (first_bit_LFSR_2 == 1)
                    LFSR_2 |= LOGIC_ONE;
                else
                    LFSR_2 &= first_bit_null;
                
            }
            
            if (bit_10_LFSR_1 == bit_12_LFSR_3)
            {
                LFSR_1 <<= 1;
                LFSR_3 <<= 1;
                
                uint64_t first_bit_LFSR_1 = (bit_10_LFSR_1 + end_bit_LFSR_1) % 2;
                
                if (first_bit_LFSR_1 == 1)
                    LFSR_1 |= LOGIC_ONE;
                else
                    LFSR_1 &= first_bit_null;
                
                uint64_t first_bit_LFSR_3 = (bit_12_LFSR_3 + end_bit_LFSR_3) % 2;
                
                if (first_bit_LFSR_3 == 1)
                    LFSR_3 |= LOGIC_ONE;
                else
                    LFSR_3 &= first_bit_null;
                
            }
            
            if (bit_11_LFSR_2 == bit_12_LFSR_3)
            {
                LFSR_2 <<= 1;
                LFSR_3 <<= 1;
                
                uint64_t first_bit_LFSR_2 = (bit_11_LFSR_2 + end_bit_LFSR_2) % 2;
                
                if (first_bit_LFSR_2 == 1)
                    LFSR_2 |= LOGIC_ONE;
                else
                    LFSR_2 &= first_bit_null;
                
                uint64_t first_bit_LFSR_3 = (bit_12_LFSR_3 + end_bit_LFSR_3) % 2;
                
                if (first_bit_LFSR_3 == 1)
                    LFSR_3 |= LOGIC_ONE;
                else
                    LFSR_3 &= first_bit_null;
                
            }
            
        }
        
    }
    
private: uint8_t logicOutput()
    {
        const uint8_t bit_19_LFSR_1 = ((LFSR_1 >> 18) & LOGIC_ONE);
        const uint8_t bit_22_LFSR_2 = ((LFSR_2 >> 21) & LOGIC_ONE);
        const uint8_t bit_23_LFSR_3 = ((LFSR_3 >> 22) & LOGIC_ONE);
        
        const uint8_t out = (bit_19_LFSR_1 + bit_22_LFSR_2 + bit_23_LFSR_3);
        
        printf("bit_19_LFSR_1=%i, bit_22_LFSR_2=%i, bit_23_LFSR_3=%i, out=%i, mod=%i \n",bit_19_LFSR_1, bit_22_LFSR_2, bit_23_LFSR_3,  out, out%2);
        
//        cout << out << "\n";
        
        return (out % 2);
    }
    
    private:
        void setLFSR() {
            resetLFSR();
            this->LFSR_1 |= ((this->key >> 45) & R1MASK);
            this->LFSR_2 |= ((this->key >> 23) & R2MASK);
            this->LFSR_3 |= (this->key & R3MASK);
        }
    
private: void resetLFSR()
    {
        this->LFSR_1 = 0l;
        this->LFSR_2 = 0l;
        this->LFSR_3 = 0l;
    }
    
public: void setKey(uint64_t key)
    {
        this->key = key;
        setLFSR();
        iterations();
    }
};

int main(int argc, const char * argv[]) {
    // insert code here...
    
    uint64_t secretKey = key;
    
    SecretKey* skey = new SecretKey();
    
    skey->setKey(secretKey);
    
    for(int i = 0; i < 92; i++)
        cout << *(skey->getLogicBits() + i) << " ";
    
    delete skey;
    
    std::cout << "Hello, World!\n";
    return 0;
}
////
////
//
///*#include <stdio.h>
//
// Masks for the three shift registers */
//#define R1MASK  0x07FFFF /* 19 bits, numbered 0..18 */
//#define R2MASK  0x3FFFFF /* 22 bits, numbered 0..21 */
//#define R3MASK  0x7FFFFF /* 23 bits, numbered 0..22 */
//
///* Middle bit of each of the three shift registers, for clock control */
//#define R1MID   0x000100 /* bit 8 */
//#define R2MID   0x000400 /* bit 10 */
//#define R3MID   0x000400 /* bit 10 */
//
///* Feedback taps, for clocking the shift registers.
// * These correspond to the primitive polynomials
// * x^19 + x^5 + x^2 + x + 1, x^22 + x + 1,
// * and x^23 + x^15 + x^2 + x + 1. */
//#define R1TAPS  0x072000 /* bits 18,17,16,13 */
//#define R2TAPS  0x300000 /* bits 21,20 */
//#define R3TAPS  0x700080 /* bits 22,21,20,7 */
//
///* Output taps, for output generation */
//#define R1OUT   0x040000 /* bit 18 (the high bit) */
//#define R2OUT   0x200000 /* bit 21 (the high bit) */
//#define R3OUT   0x400000 /* bit 22 (the high bit) */
//
//typedef unsigned char byte;
//typedef unsigned long word;
//typedef word bit;
//
///* Calculate the parity of a 32-bit word, i.e. the sum of its bits modulo 2 */
//bit parity(word x) {
//    x ^= x>>16;
//    x ^= x>>8;
//    x ^= x>>4;
//    x ^= x>>2;
//    x ^= x>>1;
//    return x&1;
//}
//
///* Clock one shift register */
//word clockone(word reg, word mask, word taps) {
//    word t = reg & taps;
//    reg = (reg << 1) & mask;
//    reg |= parity(t);
//    return reg;
//}
//
///* The three shift registers.  They're in global variables to make the code
// * easier to understand.
// * A better implementation would not use global variables. */
//word R1, R2, R3;
//
///* Look at the middle bits of R1,R2,R3, take a vote, and
// * return the majority value of those 3 bits. */
//bit majority() {
//    int sum;
//    sum = parity(R1&R1MID) + parity(R2&R2MID) + parity(R3&R3MID);
//    if (sum >= 2)
//        return 1;
//    else
//        return 0;
//}
//
///* Clock two or three of R1,R2,R3, with clock control
// * according to their middle bits.
// * Specifically, we clock Ri whenever Ri's middle bit
// * agrees with the majority value of the three middle bits.*/
//void clock() {
//    bit maj = majority();
//    if (((R1&R1MID)!=0) == maj)
//        R1 = clockone(R1, R1MASK, R1TAPS);
//    if (((R2&R2MID)!=0) == maj)
//        R2 = clockone(R2, R2MASK, R2TAPS);
//    if (((R3&R3MID)!=0) == maj)
//        R3 = clockone(R3, R3MASK, R3TAPS);
//}
//
///* Clock all three of R1,R2,R3, ignoring their middle bits.
// * This is only used for key setup. */
//void clockallthree() {
//    R1 = clockone(R1, R1MASK, R1TAPS);
//    R2 = clockone(R2, R2MASK, R2TAPS);
//    R3 = clockone(R3, R3MASK, R3TAPS);
//}
//
///* Generate an output bit from the current state.
// * You grab a bit from each register via the output generation taps;
// * then you XOR the resulting three bits. */
//bit getbit() {
//    return parity(R1&R1OUT)^parity(R2&R2OUT)^parity(R3&R3OUT);
//}
//
///* Do the A5/1 key setup.  This routine accepts a 64-bit key and
// * a 22-bit frame number. */
//void keysetup(byte key[8], word frame) {
//    int i;
//    bit keybit, framebit;
//    
//    /* Zero out the shift registers. */
//    R1 = R2 = R3 = 0;
//    
//    /* Load the key into the shift registers,
//     * LSB of first byte of key array first,
//     * clocking each register once for every
//     * key bit loaded.  (The usual clock
//     * control rule is temporarily disabled.) */
//    for (i=0; i<64; i++) {
//        clockallthree(); /* always clock */
//        keybit = (key[i/8] >> (i&7)) & 1; /* The i-th bit of the
//                                           key */
//        R1 ^= keybit; R2 ^= keybit; R3 ^= keybit;
//    }
//    
//    /* Load the frame number into the shift
//     * registers, LSB first,
//     * clocking each register once for every
//     * key bit loaded.  (The usual clock
//     * control rule is still disabled.) */
//    for (i=0; i<22; i++) {
//        clockallthree(); /* always clock */
//        framebit = (frame >> i) & 1; /* The i-th bit of the frame #
//                                      */
//        R1 ^= framebit; R2 ^= framebit; R3 ^= framebit;
//    }
//    
//    /* Run the shift registers for 100 clocks
//     * to mix the keying material and frame number
//     * together with output generation disabled,
//     * so that there is sufficient avalanche.
//     * We re-enable the majority-based clock control
//     * rule from now on. */
//    for (i=0; i<100; i++) {
//        clock();
//    }
//    
//    /* Now the key is properly set up. */
//}
//
///* Generate output.  We generate 228 bits of
// * keystream output.  The first 114 bits is for
// * the A->B frame; the next 114 bits is for the
// * B->A frame.  You allocate a 15-byte buffer
// * for each direction, and this function fills
// * it in. */
//void run(byte AtoBkeystream[], byte BtoAkeystream[]) {
//    int i;
//    
////    /* Zero out the output buffers. */
////    for (i=0; i<=113/8; i++)
////        AtoBkeystream[i] = BtoAkeystream[i] = 0;
//    
//    /* Generate 114 bits of keystream for the
//     * A->B direction.  Store it, MSB first. */
//    for (i=0; i<114; i++) {
//        clock();
//        AtoBkeystream[i/8] |= getbit() << (7-(i&7));
//    }
//    
////    for (i=0; i<114; i++) {
////        clock();
////        AtoBkeystream[i/8] |= getbit();
////    }
//    
//    /* Generate 114 bits of keystream for the
//     * B->A direction.  Store it, MSB first. */
////    for (i=0; i<114; i++) {
////        clock();
////        BtoAkeystream[i/8] |= getbit() << (7-(i&7));
////    }
//}
//
///* Test the code by comparing it against
// * a known-good test vector. */
//void test() {
//    byte key[8] = {0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
//    word frame = 0x134;
//    byte goodAtoB[15] = { 0x53, 0x4E, 0xAA, 0x58, 0x2F, 0xE8, 0x15,
//        0x1A, 0xB6, 0xE1, 0x85, 0x5A, 0x72, 0x8C, 0x00 };
//    byte goodBtoA[15] = { 0x24, 0xFD, 0x35, 0xA3, 0x5D, 0x5F, 0xB6,
//        0x52, 0x6D, 0x32, 0xF9, 0x06, 0xDF, 0x1A, 0xC0 };
//    byte AtoB[15] = {0x41, 0x42, 0x43, 0x44, 0x45, 0x45, 0x46, 0x47, 0x48,
//            0x49, 0x50, 0x51, 0x52, 0x53, 0x54}, BtoA[15];
//    int i, failed=0;
//    
//    for (i=0; i<15; i++)
//        printf("%c", AtoB[i]);
//    printf("\n");
//    
//    keysetup(key, frame);
//    run(AtoB, BtoA);
//    
//    for (i=0; i<15; i++)
//        printf("%c", AtoB[i]);
//    printf("\n");
//    
//    
////    byte A[15] = {'S', 'N', , 0x44, 0x45, 0x45, 0x46, 0x47, 0x48,
////        0x49, 0x50, 0x51, 0x52, 0x53, 0x54};
//    
//    keysetup(key, frame);
//    run(AtoB, BtoA);
//    
//    for (i=0; i<15; i++)
//        printf("%c", AtoB[i]);
//    printf("\n");
//    
//    /* Compare against the test vector. */
//    for (i=0; i<15; i++)
//        if (AtoB[i] != goodAtoB[i])
//            failed = 1;
//    for (i=0; i<15; i++)
//        if (BtoA[i] != goodBtoA[i])
//            failed = 1;
//    
//    /* Print some debugging output. */
//    printf("key: 0x");
//    int j =0;
//    for (i=0; i<8; i++)
//        printf("%02X", key[i]);
//    printf("\n");
//    printf("frame number: 0x%06X\n", (unsigned int)frame);
//    printf("known good output:\n");
//    printf(" A->B: 0x");
//    for (i=0; i<15; i++)
//        printf("%02X", goodAtoB[i]);
//    printf("  B->A: 0x");
//    for (i=0; i<15; i++)
//        printf("%02X", goodBtoA[i]);
//    printf("\n");
//    printf("observed output:\n");
//    printf(" A->B: 0x");
//    for (i=0; i<15; i++)
//        printf("%02X", AtoB[i]);
//    printf("  B->A: 0x");
//    for (i=0; i<15; i++)
//        printf("%02X", BtoA[i]);
//    printf("\n");
//    
//    if (!failed) {
//        printf("Self-check succeeded: everything looks ok.\n");
//        return;
//    } else {
//        /* Problems!  The test vectors didn't compare*/
//        printf("\nI don't know why this broke; contact the authors.\n");
//        //        exit(1);
//    }
//}
//
//int main(void) {
//    test();
//    return 0;
//}
//
//
//
//



