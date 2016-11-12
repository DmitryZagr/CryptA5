//
//  Reform.h
//  Reform
//
//  Created by Robert Enderson on 10.11.16.
//  Copyright © 2016 Robert Enderson. All rights reserved.
//

#ifndef Reform_h
#define Reform_h

#include <stdio.h>
#include <ctype.h>
#include "stdlib.h"

#define BIT_SIZE   456
#define BLOCKS     8
#define SIZE       BIT_SIZE / 8

void Reform(uint8_t *in, uint8_t *out);
void Reform_1(uint8_t *in, uint8_t *out);

#endif /* Reform_h */
