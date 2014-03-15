#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include "packet.h"
#include <QDebug>

struct tPacket_5000
{
  WORD size;
  WORD opcode;
  BYTE securityCount;
  BYTE securityCRC;
  BYTE flag;
};

struct tPacket_5000_E
{
  WORD size;
  WORD opcode;
  BYTE securityCount;
  BYTE securityCRC;
  BYTE flag;
  BYTE blowfish[8];
  DWORD seedCount;
  DWORD seedCRC;
  DWORD seedSecurity[5];
};

struct tPacket_5000_10
{
  WORD size;
  WORD opcode;
  BYTE securityCount;
  BYTE securityCRC;
  BYTE flag;
  DWORD challenge[2];
};

class cBlowFish
{
public:
  cBlowFish();

  ~cBlowFish();

  void initialize(BYTE key[], int keybytes);

  DWORD getOutputLength(DWORD lInputLong);

  DWORD encode(BYTE *pInput, BYTE *pOutput, DWORD lSize);

  void decode(BYTE *pInput, BYTE *pOutput, DWORD lSize);


private:
  DWORD* PArray;

  DWORD (*SBoxes)[256];

  void blowfish_encipher(DWORD *xl, DWORD *xr);

  void blowfish_decipher(DWORD *xl, DWORD *xr);
};

class HandShake
{
public:
  HandShake();

  ~HandShake();

  cBlowFish blowfish;


  //HandShake packets
  int packetReceive(void* data);
  int packetReceive(void* data, LPBYTE outdata, DWORD* outDataSize);

  unsigned char generateCheckByte(char* packet, int length, unsigned long seed);

  unsigned char generateCountByte(void);

  //Returns the random value
  DWORD getRandomNumber() { return random; }

  //Sets the random value
  void setRandomNumber(DWORD value);

  DWORD getCRCSeed() { return firstPacket.seedCRC; }

  //First HandShake packet
  tPacket_5000_E firstPacket;

  //Second HandShake packet
  tPacket_5000_10 secondPacket;

private:

  //Random number
  DWORD random;

  //HandShake seeds
  unsigned char byte1seeds[3];

  unsigned long generateValue(unsigned long *Ptr);
  void setupCountByte(unsigned long seed);

  DWORD dwArgs5;
  DWORD dwArgs6;


};

#endif // HANDSHAKE_H
