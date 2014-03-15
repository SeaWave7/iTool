#ifndef PACKET_H
#define PACKET_H

#include <qt_windows.h>


struct tPacket
{
  WORD size;
  WORD opcode;
  BYTE securityCount;
  BYTE securityCRC;
  BYTE dataPointer[8186];
};

// Packet Builder
class PacketBuilder
{
public:
  PacketBuilder();

  ~PacketBuilder();

  //----------------------------------------

  // Clear the packet data
  void clear();

  //----------------------------------------

  // Get the packet buffer
  tPacket *getPacket();

  // Return the packet size
  WORD getSize();

  //----------------------------------------

  // Set the packet's opcode
  void setOpcode(WORD opcode);

  //----------------------------------------

  // Add data function
  void appendByte(BYTE byte);

  // Add data function
  void appendWord(WORD word);

  // Add data function
  void appendDword(DWORD dword);

  // Append a string
  void appendString(const char *str);

  // Append a wide string
  void appendWideString(const wchar_t *str);

  // Append a string
  void appendArray(int length, void *buffer);

  // Append a double
  void appendDouble(double value);

  // Append a double
  void appendFloat(float value);

  // Copy this packet into another packet
  void copy(tPacket *outPacket);

private:
  BYTE dataPointer[8192];
  WORD size;

};

//----------------------------------------

// Packet Reader
class PacketReader
{
public:
  PacketReader();

  PacketReader(tPacket *p);

  ~PacketReader();

  int index;

  //----------------------------------------

  void setPacket(tPacket *p);

  //----------------------------------------

  // Returns the packet's opcode
  WORD getPacketOpcode();

  // Returns the packet's size
  WORD getPacketSize();

  // Returns how much data can be read
  int getBytesLeft();

  // Returns the packet associated with this reader
  tPacket *getPacket();

  // Returns the current index the reader is at
  DWORD getIndex();

  //----------------------------------------

  // Read  in specific data types
  BYTE readByte();

  // Read in specific data types
  WORD readWord();

  // Read in specific data types
  DWORD readDword();

  // Read in specific data types
  int readInt();

  // Read in specific data types
  void readArray(int size, void *outBuffer);

  // Read in specific data types
  void readString(int size, char *outBuffer);

  // Read in specific data types
  void readWideString(int size, wchar_t *outBuffer);

  // Read in specific data types
  double readDouble();

  // Read in specific data types
  float readFloat();

  // Read in specific data types
  __int64 read64();


private:
  tPacket *packet;

};



#endif // PACKET_H
