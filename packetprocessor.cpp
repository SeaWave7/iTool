#include "packetprocessor.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

extern MainWindow *mainw;
extern DWORD getTickCount();

//MainWindow mainw;

void log(const char *format, ...)
{
//  char output[256] = {0};

//  va_list args;
//  va_start(args, format);
//  vsprintf(output, format, args);
//  va_end(args);



  //ui->logWidget->addItem(output);

 // QListWidgetItem *item = ui->logWidget->item(ui->logWidget->count() - 1);
  //if(item)
   // {
    //  ui->logWidget->scrollToItem(item, QAbstractItemView::EnsureVisible);
   // }
}

PacketProcessor::PacketProcessor(QTcpSocket *socket)
{
  sSocket = socket;

  // Clear buffers
  doPingPacket = false;
  doLogin = false;
  pingPacketTime = 0;
  loginPacketTime = 0;
  count_600D = 0;
  serverid = 0;
  memset(server, 0, 32);
  memset(username, 0, 32);
  memset(password, 0, 32);


}

PacketProcessor::~PacketProcessor()
{

}


void PacketProcessor::onPacket(tPacket *packet, bool encrypted)
{
    // Set the packet
    reader.setPacket(packet);
    QString hexadecimal;
    hexadecimal.setNum(packet->opcode, 16);
    // Display the opcode
    qDebug() << "Packetwwwwww: 0x%.4X" <<  packet->opcode;
    qDebug() << "0x" + hexadecimal;
    //log("Packet: 0x%.4X", packet->opcode);
    //Log("Packet: 0x%.4X", packet->opcode);

    switch(packet->opcode)
    {
        qDebug() << "Executing case...";

        case 0x5000: // HandShake packets
        {         
           qDebug() << "Opcode is 0x5000";
            unsigned long outsize = 0;
            unsigned char outdata[256] = {0};
            HandShakeApi.packetReceive(packet, outdata, &outsize);

            qDebug() << "Writing socket";
            sSocket->write((char*)outdata, outsize);
            qDebug() << sSocket->errorString();
            qDebug() << "Socket failed";

        }break;

        case 0x2001:
        {
            memset(server, 0, 32);

            WORD len = reader.readWord();
            reader.readString(len, server);

            if(strcmp(server, "GatewayServer") == 0)
            {
                builder.setOpcode(0x6100);
                builder.appendByte(18); //locale
                builder.appendWord(9);
                builder.appendString("SR_Client");
                builder.appendDword(252); //version
                injectPacket(builder.getPacket(), true);
            }
        }break;
        case 0x600D:
        {
            count_600D++;

            if(count_600D == 6)
            {
                if(packet->size == 2)
                {
                    builder.setOpcode(0x6101);
                    injectPacket(builder.getPacket(), true);
                }
                else
                {
                    sSocket->disconnectFromHost();
                }
            }
            else if(count_600D == 4)
            {
                if(strcmp(server, "AgentServer") == 0)
                {
                    builder.setOpcode(0x6103);
                    builder.appendDword(loginID);           //Login ID
                    builder.appendWord(strlen(username));   //Username length
                    builder.appendString(username);         //Username
                    builder.appendWord(strlen(password));   //Password length
                    builder.appendString(password);         //Password
                    builder.appendByte(18);                 //Locale
                    builder.appendByte(0);
                    builder.appendByte(2);
                    builder.appendDword(getTickCount());    //MAC
                    injectPacket(builder.getPacket(), true);
                }
            }
        }break;
        case 0xA101:
        {
            SilkroadServer nServer;

            //mainw.ui->serverList->clear();

            serverList.clear();

            reader.index += 2;
            WORD len = reader.readWord();
            reader.index += len;
            reader.index++;

            char name[64] = {0};

            BYTE b = reader.readByte();
            while(b)
            {
                memset(name, 0, 64);

                WORD id = reader.readWord();

                len = reader.readWord();
                reader.readString(len, name);

                if((int)strstr(name, "TR") == 0)
                {
                    for(size_t x = 0; x < strlen(name); ++x)
                    {
                        if(name[x] == '(')
                        {
                            name[x - 1] = 0;
                            break;
                        }
                    }
                }

                nServer.id = id;
                sprintf(nServer.name, "%s", name);
                serverList.push_back(nServer);
                qDebug() << name;
                //mainw.ui->serverList->addItem(name);

                reader.index += 2;  // Current
                reader.index += 2;  // Max
                reader.index++;     // State

                b = reader.readByte();
            }

           // Log("Connected successfully");
            qDebug() << "Connected sucessfully";
            //mainw->ui.Login->setEnabled(true);
            doPingPacket = true;
        }break;
        case 0xA102:
        {
            BYTE success = reader.readByte();
            if(success == 1)
            {
                doLogin = false;
                doPingPacket = false;

                // Login ID sent to the world server upon connect
                loginID = reader.readDword();

                WORD len = reader.readWord();
                char* worldIP = new char[len + 2];
                reader.readString(len, worldIP);
                WORD worldPort = reader.readWord();

                // Connect to the world server
                sSocket->disconnectFromHost();
                sSocket->connectToHost(worldIP, worldPort);

                // Free memory
                delete [] worldIP;
            }
            else
            {
                BYTE error = reader.readByte();
                if(error == 5)
                {
                    // Login delay
                    loginPacketTime = reader.readInt();

                    // Can start the auto login process
                    doLogin = true;
                }
                else
                {
                    // Reset boxes
                    //mainw->ui.Username->setText("");
                    //mainw->ui.Password->setText("");
                    //mainw->ui.Login->setEnabled(true);
                    doLogin = false;

                    if(error == 1)
                    {
                        qDebug() << "Invalid username/password";
                      //Log("Invalid username/password");
                    }
                    else
                    {
                        qDebug() << "Unknown error at the login server %.2X\n" << error;
                      //Log("Unknown error at the login server %.2X\n", error);
                    }
                }
            }
        }break;
        case 0xA103:
        {
            if(reader.readByte() == 1)
            {
                qDebug() << "Login success. Select your character";
               //Log("Login success. Select your character");
            }
            else
            {
                BYTE error = reader.readByte();
                if(error == 5)
                {
                    qDebug() << "Unknown error while connecting to the world server %.2X" << error;
                    //Log("Unknown error while connecting to the world server %.2X", error);
                }
            }
        }break;
        default:
        {
        }
    }
}

void PacketProcessor::injectPacket(tPacket* packet, bool encrypted)
{
    // Ping packet delay
    pingPacketTime = getTickCount();

    // Size of the packet
    DWORD realSize = packet->size;

    // Temp buffer
    BYTE tmpBuffer[8192] = {0};

    // Copy over the packet to send
    memcpy(tmpBuffer, packet, realSize + 6);

    // Make a packet pointer
    tPacket *tmpPacket = (tPacket*)tmpBuffer;

    // Fix the size if it's encrypted
    if(encrypted)
        tmpPacket->size |= 0x8000;

    // Generate count byte
    tmpPacket->securityCount = HandShakeApi.generateCountByte();

    // Generate CRC byte
    tmpPacket->securityCRC = 0;
    tmpPacket->securityCRC = HandShakeApi.generateCheckByte((char*)tmpBuffer, realSize + 6, HandShakeApi.getCRCSeed());

    // Check to see if we need to encode the packet
    if(encrypted)
    {
        // Get blowfish data size
        DWORD bfSize = HandShakeApi.blowfish.getOutputLength(realSize + 4);

        // Encode the packet
        HandShakeApi.blowfish.encode((BYTE*)tmpBuffer + 2, (BYTE*)tmpBuffer + 2, realSize + 4);

        sSocket->write((char*)tmpBuffer, bfSize + 2);
    }
    else
    {
        // Forward the packet
        sSocket->write((char*)tmpBuffer, realSize + 6);
    }
}
