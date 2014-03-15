#ifndef PACKETPROCESSOR_H
#define PACKETPROCESSOR_H


#include "packet.h"
#include "handshake.h"
#include <QtNetwork>
#include <QVector>
#include <QDebug>

class PacketProcessor
{
public:
  PacketProcessor(QTcpSocket *socket);

  ~PacketProcessor();

  // Packet received
  void onPacket(tPacket *packet, bool encrypted);

  // Packet sending function
  void injectPacket(tPacket *packet, bool encrypted);


  HandShake HandShakeApi;
;
private:

  // Server information
  struct SilkroadServer
  {
    WORD id;
    char name[32];
  };

  QVector<SilkroadServer> serverList;

  // Socket
  QTcpSocket* sSocket;

  // Packet building class
  PacketBuilder builder;

  // Packet reading class
  PacketReader reader;

  // Server connected to
  char server[32];

  // Connection information
  int count_600D;

  // Login ID to be sent to the world server
  DWORD loginID;

  // Silkroad username
  char username[32];

  // Silkroad password
  char password[32];

  // Silkroad server id
  WORD serverid;

  // Whether or not to send the login information
  bool doLogin;

  // Login delay
  int loginPacketTime;

  // Whether or not to send the ping packet
  bool doPingPacket;

  // Ping packet delay
  int pingPacketTime;


};

#endif // PACKETPROCESSOR_H
