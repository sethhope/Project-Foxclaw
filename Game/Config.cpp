#include "Config.h"
using namespace FCE;
using namespace irr;
Config::Config()
{
}

Config::~Config()
{
}

void Config::loadConfig(std::string filename)
{
    IrrlichtDevice* nullDevice = createDevice(video::EDT_NULL);
    bool inSettings=false;
    io::IXMLReader* xml = nullDevice->getFileSystem()->createXMLReader(filename.c_str());
    while(xml->read())
    {
        switch(xml->getNodeType())
        {
        case io::EXN_ELEMENT:
            if(inSettings==false&&core::stringw("config").equals_ignore_case(xml->getNodeName()))
            {
                inSettings = true;
            }
            if(inSettings==true && core::stringw("setting").equals_ignore_case(xml->getNodeName()))
            {
                core::stringw name = xml->getAttributeValue(L"name");
                f32 value = xml->getAttributeValueAsFloat(L"value");
                core::stringc namec = name.c_str();
                std::string names = namec.c_str();
                data[names] = value;
            }
            break;
        case io::EXN_ELEMENT_END:
            if(inSettings==true&&core::stringw("config").equals_ignore_case(xml->getNodeName()))
            {
                inSettings == false;
            }
            break;
        }
    }
    nullDevice->drop();
}

void Config::saveConfig(std::string filename)
{

}
