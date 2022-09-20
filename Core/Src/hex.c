/*
 * hex.c
 *
 *  Created on: Sep 12, 2022
 *      Author: immor
 */


#include "hex.h"


static uint32_t start_addr;
uint8_t eof_reached = 0x00;

static uint8_t HexCharToByte(char c)
    {
        if(c>='0' && c<='9')
            return c -'0';
        else if(c>='a' && c<='f')
            return c-'a'+0x0a;
        else if(c>='A' && c <='F')
            return c-'A'+0x0a;

        return -1;
    }

int getByte (const char *str, int pos, uint8_t *output)
    {
        if (str[pos] != '\0' && str[pos+1] != '\0')
        {
            *output = HexCharToByte(str[pos]);
            *output <<=4;
            *output |= HexCharToByte(str[pos+1]);
            return 0;
        }
        return -1;
    }

int datacopy (uint8_t *indata, uint32_t *outdata)
	{
	memset(outdata, 0x00, 16);
	for (int i = 0; i<16; i++)
		{
		outdata[i/4] <<= 8;
		outdata[i/4] |= indata[i];

		}
	return 0;
	}

int crcCheck(const char *str, uint8_t len)
    {
        uint8_t checksum = 0x00;
        uint8_t res = 0x00;
        getByte(str, len-2, &checksum);
        uint8_t cc[20];
        memset(cc, 0x00, 20);
        for (size_t i = 1; i < len-2; i+=2)
        {
            getByte(str, i, &cc[i/2]);
        }
        for (size_t i = 0; i <20; i++)
        {
            res +=cc[i];
        }

        res = ~res;
        res += 0x01;

        if (res == checksum) return 1;
        else return 0;

    }

int getFromHex (const char *str, hexconv *pOutput)
    {
        if (str[0] != ':')
            {
                return -1;
            }
        uint8_t type, len;
        //uint16_t start_addr = 0x00;
        uint16_t end_addr = 0x00;
        uint8_t data[16] = {0x00};
        pOutput->addr = 0;
        pOutput->len = 0;

        if (getByte(str, 1, &len) != 0)
            {
                return -1;
            }

        if (!crcCheck(str, 11+len*2))  // compute crc
                    {
                        return -1;
                    }

        if (getByte(str, 7, &type) != 0)
            {
                return -1;
            }
        switch (type)
        {
        case 0x00:
            for (size_t i = 3; i < 7; i+=2)
                {
                    uint8_t buf;
                    end_addr <<= 8;
                    getByte(str, i, &buf);
                    end_addr |= buf;

                }
            pOutput->addr |= start_addr;
            pOutput->addr <<= 16;
            pOutput->addr |=end_addr;
            int j = 0;
            memset(data, 0x00, len);
            for (size_t i = 9; i < 9+2*len; i+=2, j++)
                {

                //uint8_t buf;
                //data[j/4] <<= 8;
                //getByte(str, i, &buf);
                getByte(str, i, &data[j]);
                //data[j] = buf;
                }

            //pOutput->data = data;
            //memcpy(pOutput->data, &data, len);
            datacopy(data, pOutput->data);
            pOutput->len = len/4;

            break;

        case 0x01:
            eof_reached = 0x01;
            break;


        case 0x04:
            for (size_t i = 9; i < 9+2*len; i+=2)
            {
                uint8_t buf;
                start_addr <<= 8;
                getByte(str, i, &buf);
                start_addr |= buf;
            }

            break;

        default:
        	return -1;
            break;
        }
        //pOutput->addr = out;
        //uint8_t len
        return 0;
    }



