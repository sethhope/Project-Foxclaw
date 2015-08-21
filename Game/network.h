#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED
#include "stdio.h"
#include "irrlicht.h"
#include "Misc/Script.h"
#include "Misc/Logger.h"
#include "RakNetTypes.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"

#include "BitStream.h"

#define FCLIENT 0
#define FSERVER 1
using namespace irr;
namespace FCE
{
    class NETPACKET
    {
        public:
            NETPACKET(){};
            ~NETPACKET(){};
            RakNet::BitStream pack;
    };
    enum CustomMessages
    {
        ID_USER_DATA = ID_USER_PACKET_ENUM+1
    };
    class NETWORK
    {
    public:
        NETWORK(LOGGER* log);
        ~NETWORK();
        void setMode(bool mode);
        bool getMode();
        void setMaxClients(u32 clients);
        u32 getMaxClients();
        void setConnectionIP(std::string ip);
        void setPort(u32 port);
        void disconnect();
        void init(std::string filename);
        void update();
        NETPACKET* createPacket();
        void sendPacket(NETPACKET* toSend, RakNet::SystemAddress address);
        void broadcastPacket(NETPACKET* toSend);
        std::string connectedIP;
        RakNet::RakPeerInterface* peer;
        SCRIPT* script;
    private:
        bool mode;
        bool modeSet;
        u32 maxClients;


        u32 port;
        LOGGER* log;

    };
}

#endif // NETWORK_H_INCLUDED
