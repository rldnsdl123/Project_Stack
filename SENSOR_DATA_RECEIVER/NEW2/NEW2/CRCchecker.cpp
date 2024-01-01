#include "CRCchecker.h"
#include"stdafx.h"

//
//unsigned long getFileCRC() {
//	unsigned char buf[32768];
//	unsigned long CRC = 0;
//
//	size_t len;
//	while ((len = fread(buf, 1, sizeof(buf), s)) != NULL)
//		CRC = calcCRC(buf, (unsigned long)len, CRC);
//
//	return CRC;
//}

void makeCRCtable(unsigned long *table, unsigned long id) {
	unsigned long i, j, k;

	for (i = 0; i < 256; ++i) {
		k = i;
		for (j = 0; j < 8; ++j) {
			if (k & 1) k = (k >> 1) ^ id;
			else k >>= 1;
		}
		table[i] = k;
	}
}
unsigned long calcCRC(const unsigned char *mem, signed long size, unsigned long CRC) {
	unsigned long table[256];

	CRC = ~CRC;
	makeCRCtable(table, 0xEDB88320);

	while (size--)
		CRC = table[(CRC ^ *(mem++)) & 0xFF] ^ (CRC >> 8);

	return ~CRC;
}


