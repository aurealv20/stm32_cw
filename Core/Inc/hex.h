/*
 * hex.h
 *
 *  Created on: Sep 12, 2022
 *      Author: immor
 */

#ifndef INC_HEX_H_
#define INC_HEX_H_

#include "main.h"
//#include <stdlib.h>
#include <string.h>
//#include <stdio.h>
//#include <inttypes.h>




typedef struct
{
    /* data */
    uint8_t len;
    uint32_t addr;
    uint32_t data[8];

}hexconv;

int getFromHex (const char *str, hexconv *pOutput);

#endif /* INC_HEX_H_ */
