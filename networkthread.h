#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include "packet.h"
#include "packetprocessor.h"
#include <QTcpSocket>
#include <QtNetwork>
#include <QThread>

class NetworkThread : public QThread
{
  Q_OBJECT

public:
  NetworkThread();
  ~NetworkThread();

  // Socket
  QTcpSocket *sSocket;

  // Packet Processing
  PacketProcessor *pProcessor;

  // Run
  virtual void run();

public slots:

  // Data Available
  void readData();

  // Socket Disconnected
  void socketDisconnected();

private:

  // Receive Buffer
  char recvBuffer[8192];

  // Receive Index
  int recvIndex;

  // On Data Received
  int onData(char *data, int size);


};

#endif // NETWORKTHREAD_H
