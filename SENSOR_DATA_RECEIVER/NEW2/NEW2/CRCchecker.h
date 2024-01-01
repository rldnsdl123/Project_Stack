#pragma once
#ifndef CRC_CHECKER
#define CRC_CHECKER

//unsigned long getFileCRC();
unsigned long calcCRC(const unsigned char *, signed long, unsigned long);
void makeCRCtable(unsigned long *, unsigned long);

#endif