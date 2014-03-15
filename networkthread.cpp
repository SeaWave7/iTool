#include "networkthread.h"

DWORD getTickCount()
{
    //return (DWORD)time(0);
    return 0;
}

NetworkThread::NetworkThread()
{
}

NetworkThread::~NetworkThread()
{

}



void NetworkThread::socketDisconnected()
{
    qDebug() << "Disconnected";
}

int NetworkThread::onData(char *data, int size)
{
  if(size < 2) return 0;

  tPacket *packet = (tPacket*)data;
  int processed = 0;
  bool encrypted = false;

  // Encrypted
  if(packet->size & 0x8000)
  {
      qDebug() << "NetworkThread: Packet encrypted";
      encrypted = true;

      int realSize = packet->size & 0x7FFF;
      int bfSize = pProcessor->HandShakeApi.blowfish.getOutputLength(realSize + 4);
      if((bfSize + 2) > size) return 0;

      pProcessor->HandShakeApi.blowfish.decode((BYTE*)data + 2, (BYTE*)data + 2, bfSize);
      processed = (bfSize + 2);
      packet->size &= 0x7FFF;
  }
  //Not encrypted
  else
  {
      qDebug() << "NetworkThread: Packet is not encrypted";
      if((packet->size + 6) > size)
          return 0;

      processed = (packet->size + 6);
  }

  // Forward the packet to the packet processor
  pProcessor->onPacket(packet, false);

  return processed;
}

void NetworkThread::run()
{
  // Create a new socket
  sSocket = new QTcpSocket(this);

  // Create a new packet processor
  pProcessor = new PacketProcessor(sSocket);

  // Set the receive index
  recvIndex = 0;

  // Connect the read event to a slot
  connect(sSocket, SIGNAL(readyRead()), this, SLOT(readData()));

  // Disconnected
  connect(sSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

  // Random seed
  srand(getTickCount());

  // Random server
  switch(rand() % 4)
  {
  case 0:
      sSocket->connectToHost("121.128.133.26", 15779);
      break;
  case 1:
      sSocket->connectToHost("121.128.133.27", 15779);
      break;
  case 2:
      sSocket->connectToHost("121.128.133.28", 15779);
      break;
  case 3:
      sSocket->connectToHost("121.128.133.29", 15779);
      break;
  case 4:
      sSocket->connectToHost("121.128.133.30", 15779);
      break;
  default:
      sSocket->connectToHost("121.128.133.26", 15779);
  }

  // Create a new timer
  //timer1 = new QTimer(this);

  // Connect the timer with the slot
 // connect(timer1, SIGNAL(timeout()), this, SLOT(Timer()));

  // Execute the timer (1 second)
// timer1->start(1000);

  // Process thread events
  qDebug() << "Exec";
  exec();
  qDebug() << "Exec finish";

}

void NetworkThread::readData()
{
  QDataStream in(sSocket);
  in.setVersion(QDataStream::Qt_4_6);

  // Get the number of bytes available
  int bytesAvailable = sSocket->bytesAvailable();

  // Read data
  in.readRawData(recvBuffer + recvIndex, 8192 - recvIndex);

  // Processing data
  int bytesProcesed = 0;
  recvIndex += bytesAvailable;

  do
  {
      bytesProcesed = onData(recvBuffer, recvIndex);

      if(bytesProcesed == 0)
      {
      }
      else if(bytesProcesed == recvIndex)
      {
          recvIndex = 0;
      }
      else
      {
          memmove(recvBuffer, recvBuffer + bytesProcesed, recvIndex - bytesProcesed);
          recvIndex -= bytesProcesed;
      }
  }
  while(bytesProcesed && recvIndex);
}
