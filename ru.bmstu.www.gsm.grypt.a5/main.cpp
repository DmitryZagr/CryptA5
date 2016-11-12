//
//  main.cpp
//  ru.bmstu.www.gsm.grypt.a5
//
//  Created by Дмитрий  Загребаев on 12.11.16.
//  Copyright © 2016 Дмитрий  Загребаев. All rights reserved.
//

#include <iostream>
#include <stdint.h>
#include "SecretKey.h"

/* Masks for the three shift registers */
#define R1MASK  0x07FFFF /* 19 bits, numbered 0..18 */
#define R2MASK  0x3FFFFF /* 22 bits, numbered 0..21 */
#define R3MASK  0x7FFFFF /* 23 bits, numbered 0..22 */

#define SIZE 456

using namespace std;

class SecretKey {
    
private:
    uint64_t LFSR_1;
    uint64_t LFSR_2;
    uint64_t LFSR_3;
    uint64_t key;
    uint64_t logicBits[114];
    
private:
    static const uint64_t LOGIC_ONE = 1;
    static const uint64_t first_bit_null = 0xFFFFFFFFFFFFFFFEl;
    
public:
    SecretKey() {
        memset(logicBits, 0, sizeof(uint64_t) * 114);
    }
    
public:
    uint64_t* getLogicBits() {
        return this->logicBits;
    }
    
private:
    void iterations() {
        int j = 0;
        for (uint16_t i = 0; i < 192; i++)
        {
            if(i >= 100) {
                *(logicBits + j) =  logicOutput();
                j++;
            } else {
                logicOutput();
            }
            shiftLFSR();
        }
    }
    
private:
    void shiftLFSR() {
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
        else {
            if (bit_10_LFSR_1 == bit_11_LFSR_2) {
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
            
            if (bit_10_LFSR_1 == bit_12_LFSR_3) {
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
            
            if (bit_11_LFSR_2 == bit_12_LFSR_3) {
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
    
private:
    uint8_t logicOutput() {
        const uint8_t bit_19_LFSR_1 = ((LFSR_1 >> 18) & LOGIC_ONE);
        const uint8_t bit_22_LFSR_2 = ((LFSR_2 >> 21) & LOGIC_ONE);
        const uint8_t bit_23_LFSR_3 = ((LFSR_3 >> 22) & LOGIC_ONE);
        
        const uint8_t out = (bit_19_LFSR_1 + bit_22_LFSR_2 + bit_23_LFSR_3);
        
        return (out % 2);
    }
    
private:
    void setLFSR() {
        resetLFSR();
        this->LFSR_1 |= ((this->key >> 45) & R1MASK);
        this->LFSR_2 |= ((this->key >> 23) & R2MASK);
        this->LFSR_3 |= (this->key & R3MASK);
    }
    
private:
    void resetLFSR() {
        this->LFSR_1 = 0l;
        this->LFSR_2 = 0l;
        this->LFSR_3 = 0l;
    }
    
public:
    void setKey(uint64_t key, uint32_t publicKey) {
        this->key = key;
        setLFSR();
        iterations();
        addPublicKey(publicKey);
    }
    
private:
    void addPublicKey(uint32_t publicKey) {
        int j = 21;
        for(int i = 92; i < 114; i++) {
            *(logicBits + i) = (publicKey >> j) & 1;
            j++;
        }
    }
    
    
};

class CryptA5 {
public:
    CryptA5(){};
    
    void crypt(uint8_t* in, uint8_t* out, SecretKey* secretKey, uint32_t* publicKey) {
        secretKey->getLogicBits();
        for (uint16_t i = 0, j = 0; i < 456; i++) {
            
            switch (i) {
                case 0:
                    secretKey->setKey(key, publicKey[0]);
                    j = 0;
                case 114:
                    secretKey->setKey(key, publicKey[1]);
                    j = 0;
                    break;
                case 228:
                    secretKey->setKey(key, publicKey[2]);
                    j = 0;
                    break;
                case 342:
                    secretKey->setKey(key, publicKey[3]);
                    j = 0;
                default:
                    break;
            }
            
            *(out + i) = *(in + i) ^ *(secretKey->getLogicBits() + j);
            j++;
        }
    }
};

int main(int argc, const char * argv[]) {
    
    SecretKey* skey = new SecretKey();
    
    CryptA5* crypt = new CryptA5();
    
//    uint8_t in[456] = {65, 66, 67, 68, 69, 70, 71, 72, 73};
    

    
    uint8_t in[SIZE];
    memset(in, 0, sizeof(uint8_t) * SIZE);
    
    for(int i = 0; i < SIZE; i++)
        cin >> *(in + i);
    
    uint8_t out[456];
    memset(out, 0, sizeof(uint8_t) * SIZE);
    uint32_t pk[4] = {143135, 623515 ,323512 ,523552};
    
    
//    while(true) {
//        
//        memset(out, 0, sizeof(uint8_t) * SIZE);
//        memset(in, 0, sizeof(uint8_t) * SIZE);
//        for(int i = 0; i < SIZE; i++)
//            cin >> *(in + i);
    
    printf("in=");
    for(int i = 0; i < SIZE; i++) {
        printf(" %c", in[i]);
    }
    
    crypt->crypt(in, out, skey, pk);
    
    printf("\ncrypt: ");
    
    for(int i = 0; i < SIZE; i++) {
        printf(" %c", out[i]);
    }
    
    memset(in, 0, sizeof(uint8_t) * 456);
    
    crypt->crypt(out, in, skey, pk);
    
    printf("\ndecrupt: ");
    
    for(int i = 0; i < SIZE; i++) {
        cout << in[i];
    }
//    }
    
    cout << "\n";
    
    delete crypt;
    delete skey;
    
    return 0;
}
