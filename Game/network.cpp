#include "network.h"
using namespace FCE;
NETWORK::NETWORK(LOGGER* log)
{
    this->log = log;
    modeSet = false;
    script = new SCRIPT(log);
    script->init();
}

NETWORK::~NETWORK()
{
    RakNet::RakPeerInterface::DestroyInstance(peer);
}

void NETWORK::init(std::string filename)
{
    script->run(filename);
    peer = RakNet::RakPeerInterface::GetInstance();
    if(mode == FCLIENT)
    {
        RakNet::SocketDescriptor sd;
        peer->Startup(1, &sd, 1);
        log->logData("Connecting to", connectedIP.c_str());
        peer->Connect(connectedIP.c_str(), port, 0, 0);
    }else
    {
        RakNet::SocketDescriptor sd(port, 0);
        peer->Startup(maxClients, &sd, 1);
        peer->SetMaximumIncomingConnections(maxClients);
        log->logData("Starting Server");
    }
}
void NETWORK::update()
{
    RakNet::Packet* pack;
    for(pack=peer->Receive(); pack; peer->DeallocatePacket(pack), pack=peer->Receive())
    {
        switch(pack->data[0])
        {
        case ID_REMOTE_DISCONNECTION_NOTIFICATION:
            log->debugData(MINOR, "Remote client disconnected");
            break;
        case ID_REMOTE_CONNECTION_LOST:
            log->debugData(MINOR, "Remote client lost connection");
            break;
        case ID_REMOTE_NEW_INCOMING_CONNECTION:
            log->debugData(MINOR, "Remote client connected");
            break;
        case ID_CONNECTION_REQUEST_ACCEPTED:
            log->debugData(MINOR, "Accepted connection request", pack->systemAddress.ToString());
            break;
        case ID_NEW_INCOMING_CONNECTION:
            log->debugData(MINOR, "New connection incoming", pack->systemAddress.ToString());
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            log->debugData(MINOR, "Server full");
            break;
        case ID_CONNECTION_LOST:
            log->debugData(MINOR, "Lost connection");
            break;
        case ID_DISCONNECTION_NOTIFICATION:
            log->debugData(MINOR, "Disconnected", pack->systemAddress.ToString());
            break;
        case ID_CONNECTION_ATTEMPT_FAILED:
            log->debugData(MINOR, "Failed to connect to", connectedIP);
            break;
        case ID_ALREADY_CONNECTED:
            log->debugData(MINOR, "Already connected");
            break;
        case ID_USER_DATA:
            {
            log->debugData(MINOR, "Got data");
            lua_getglobal(script->L, "onReceive");
            RakNet::BitStream in(pack->data, pack->length, false);
            NETPACKET* tmp = new NETPACKET();
            tmp->pack.Write(in);
            tmp->pack.IgnoreBytes(sizeof(RakNet::MessageID));
            luaW_push<NETPACKET>(script->L, tmp);
            lua_pushstring(script->L, pack->systemAddress.ToString());
            if(lua_pcall(script->L, 2, 0, 0)!=0)
            {
                log->debugData(MAJOR, "Failed to run onReceive function", lua_tostring(script->L, -1));
                break;
            }
            }
            break;
        default:
            log->debugData(MAJOR, "Default network packet case");
            break;
        }

    }
}

void NETWORK::setMode(bool mode)
{
    if(modeSet==false)
    {
        this->mode = mode;
        modeSet = true;
    }

}
void NETWORK::setConnectionIP(std::string ip)
{
    this->connectedIP = ip;
}
void NETWORK::setPort(u32 port)
{
    this->port = port;
}
void NETWORK::setMaxClients(u32 clients)
{
    this->maxClients = clients;
}
void NETWORK::disconnect()
{
    RakNet::SystemAddress a;
    a.FromString(connectedIP.c_str());
    peer->CloseConnection(a, true);
}
NETPACKET* NETWORK::createPacket()
{
    NETPACKET* n = new NETPACKET();
    n->pack.Write((RakNet::MessageID)ID_USER_DATA);
    return n;
}
void NETWORK::sendPacket(NETPACKET* toSend, RakNet::SystemAddress address)
{
    log->debugData(MINOR, "Sending packet", address.ToString());
    RakNet::BitStream tmp;
    tmp.Write(toSend->pack);
    u32 worked = peer->Send(&tmp, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address, false);
    if(worked==0)
        log->debugData(MINOR, "Failed to send");
    else
        log->debugData(MINOR, "Success", worked);
}
void NETWORK::broadcastPacket(NETPACKET* toSend)
{
    log->debugData(MINOR, "broadcasting packet");
    RakNet::BitStream tmp;
    tmp.Write(toSend->pack);
    u32 worked = peer->Send(&tmp, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), true);
    if(worked==0)
        log->debugData(MINOR, "Failed to send");
    else
        log->debugData(MINOR, "Success", worked);
}
