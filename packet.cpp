#include "packet.h"

PacketBuilder::PacketBuilder()
{
  clear();
}

PacketBuilder::~PacketBuilder()
{

}

//-----------------------------------------------------------------------------

// Clear the packet data
void PacketBuilder::clear()
{
  // Clear out the data
  memset(dataPointer, 0, 8192);
  size = 6;
}

//-----------------------------------------------------------------------------

// Get the packet buffer
tPacket *PacketBuilder::getPacket()
{
  return (tPacket *)dataPointer;
}

//-----------------------------------------------------------------------------

// Return the packet size
WORD PacketBuilder::getSize()
{
  return size;
}

//-----------------------------------------------------------------------------

// Set the packet's opcode
void PacketBuilder::setOpcode(WORD opcode)
{
  clear();
  LPWORD stream = (LPWORD)dataPointer;
  stream++;
  (*stream) = opcode;
}

//-----------------------------------------------------------------------------

// Add data function
void PacketBuilder::appendByte(BYTE byte)
{
        LPBYTE stream = (LPBYTE)dataPointer;
        stream += size;
        (*stream) = byte;
        ++size;
        WORD tmpSize = size - 6;
        memcpy(dataPointer, &tmpSize, 2);
}

//-----------------------------------------------------------------------------

// Add data function
void PacketBuilder::appendWord(WORD word)
{
        LPBYTE stream = (LPBYTE)dataPointer;
        stream += size;
        (*(LPWORD)stream) = word;
        size += 2;
        WORD tmpSize = size - 6;
        memcpy(dataPointer, &tmpSize, 2);
}

//-----------------------------------------------------------------------------

// Add data function
void PacketBuilder::appendDword(DWORD dword)
{
        LPBYTE stream = (LPBYTE)dataPointer;
        stream += size;
        (*(LPDWORD)stream) = dword;
        size += 4;
        WORD tmpSize = size - 6;
        memcpy(dataPointer, &tmpSize, 2);
}

//-----------------------------------------------------------------------------

// Append a string
void PacketBuilder::appendString(const char *str)
{
        LPBYTE stream = (LPBYTE)dataPointer;
        stream += size;
        memcpy(stream, str, strlen(str));
        size += (WORD)strlen(str);
        WORD tmpSize = size - 6;
        memcpy(dataPointer, &tmpSize, 2);
}

//-----------------------------------------------------------------------------

// Append a string
void PacketBuilder::appendWideString(const wchar_t *str)
{
        static int constSize = sizeof(wchar_t);
        LPBYTE stream = (LPBYTE)dataPointer;
        stream += size;
        memcpy(stream, str, constSize *wcslen(str));
        size += (WORD)(constSize *wcslen(str));
        WORD tmpSize = size - 6;
        memcpy(dataPointer, &tmpSize, 2);
}

//-----------------------------------------------------------------------------

// Append a string
void PacketBuilder::appendArray(int length, void * buffer)
{
        LPBYTE stream = (LPBYTE)dataPointer;
        stream += size;
        memcpy((char*)stream, buffer, length);
        size += length;
        WORD tmpSize = size - 6;
        memcpy(dataPointer, &tmpSize, 2);
}

//-----------------------------------------------------------------------------

// Append a double
void PacketBuilder::appendDouble(double value)
{
        static int constSize = sizeof(double);
        LPBYTE stream = (LPBYTE)dataPointer;
        stream += size;
        (*(double*)stream) = value;
        size += constSize;
        WORD tmpSize = size - 6;
        memcpy(dataPointer, &tmpSize, 2);
}

//-----------------------------------------------------------------------------

// Append a float
void PacketBuilder::appendFloat(float value)
{
        static int constSize = sizeof(float);
        LPBYTE stream = (LPBYTE)dataPointer;
        stream += size;
        (*(float*)stream) = value;
        size += constSize;
        WORD tmpSize = size - 6;
        memcpy(dataPointer, &tmpSize, 2);
}

//-----------------------------------------------------------------------------

// Copy this packet into the buffer
void PacketBuilder::copy(tPacket *outPacket)
{
        memcpy(outPacket, dataPointer, size);
}

//-----------------------------------------------------------------------------

PacketReader::PacketReader()
{
        packet = 0;
        index = 0;
}

//-----------------------------------------------------------------------------

PacketReader::~PacketReader()
{

}

//-----------------------------------------------------------------------------

// Ctor - build from a packet
PacketReader::PacketReader(tPacket *p)
{
        setPacket(p);
}

//-----------------------------------------------------------------------------

// Set a packet for the object
void PacketReader::setPacket(tPacket *p)
{
        packet = p;
        index = 0;
}

//-----------------------------------------------------------------------------

// Returns the packet opcode
WORD PacketReader::getPacketOpcode()
{
        return packet->opcode;
}

//-----------------------------------------------------------------------------

// Returns how much data can be read from the packet
int PacketReader::getBytesLeft()
{
        if(index < packet->size)
                return packet->size - index;
        return 0;
}

//-----------------------------------------------------------------------------

// Returns the packet's size
WORD PacketReader::getPacketSize()
{
        return packet->size;
}

//-----------------------------------------------------------------------------

// Read in specific data types
BYTE PacketReader::readByte()
{
        static int constSize = sizeof(BYTE);
        if(index + constSize > packet->size) return 0;
        LPBYTE stream = packet->dataPointer;
        stream += index;
        index += constSize;
        return (*stream);
}

//-----------------------------------------------------------------------------

// Read in specific data types
WORD PacketReader::readWord()
{
        static int constSize = sizeof(WORD);
        if(index + constSize > packet->size) return 0;
        LPBYTE stream = packet->dataPointer;
        stream += index;
        index += constSize;
        return *((LPWORD)(stream));
}

//-----------------------------------------------------------------------------

// Read in specific data types
DWORD PacketReader::readDword()
{
        static int constSize = sizeof(DWORD);
        if(index + constSize > packet->size) return 0;
        LPBYTE stream = packet->dataPointer;
        stream += index;
        index += constSize;
        return *((LPDWORD)(stream));
}

//-----------------------------------------------------------------------------

// Read in specific data types
int PacketReader::readInt()
{
        static int constSize = sizeof(int);
        if(index + constSize > packet->size) return 0;
        LPBYTE stream = packet->dataPointer;
        stream += index;
        index += constSize;
        return *((LPDWORD)(stream));
}

//-----------------------------------------------------------------------------

// Read in specific data types
void PacketReader::readString(int size, char *outBuffer)
{
        if(index + size > packet->size) return;
        LPBYTE stream = packet->dataPointer;
        stream += index;
        index += size;
        memcpy(outBuffer, stream, size);
        outBuffer[size] = 0;
}

//-----------------------------------------------------------------------------

// Read in specific data types
void PacketReader::readWideString(int size, wchar_t *outBuffer)
{
        static int constSize = sizeof(wchar_t);
        if(index + (size *constSize) > packet->size) return;
        LPBYTE stream = packet->dataPointer;
        stream += index;
        index += (size *constSize);
        memcpy(outBuffer, stream, size * constSize);
        outBuffer[size *constSize] = 0;
}

//-----------------------------------------------------------------------------

// Read in specific data types
float PacketReader::readFloat()
{
        static int constSize = sizeof(float);
        if(index + constSize > packet->size) return 0;
        LPBYTE stream = packet->dataPointer;
        stream += index;
        index += constSize;
        return *((float*)(stream));
}

//-----------------------------------------------------------------------------

// Read in specific data types
double PacketReader::readDouble()
{
        static int constSize = sizeof(double);
        if(index + constSize > packet->size) return 0;
        LPBYTE stream = packet->dataPointer;
        stream += index;
        index += constSize;
        return *((double*)(stream));
}

//-----------------------------------------------------------------------------

// Read in specific data types
__int64 PacketReader::read64()
{
        static int constSize = sizeof(__int64);
        if(index + constSize > packet->size) return 0;
        LPBYTE stream = packet->dataPointer;
        stream += index;
        index += constSize;
        return *((__int64*)(stream));
}

//-----------------------------------------------------------------------------

// Read in specific data types
void PacketReader::readArray(int size, void *outBuffer)
{
        if(index + size > packet->size) return;
        LPBYTE stream = packet->dataPointer;
        stream += index;
        index += size;
        memcpy(outBuffer, stream, size);
}

//-----------------------------------------------------------------------------

// Returns the packet associated with this reader
tPacket * PacketReader::getPacket()
{
        return packet;
}

//-----------------------------------------------------------------------------

// Returns the current index the reader is at
DWORD PacketReader::getIndex()
{
        return index;
}

//-------------------------------------------
















