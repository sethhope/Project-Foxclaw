#include "Config.h"
using namespace FCE;
using namespace irr;
Config::Config(IrrlichtDevice* device, LOGGER* log)
{
    this->device = device;
    this->log = log;
}

Config::~Config()
{
}

void Config::loadConfig(std::string filename)
{
    IrrlichtDevice* nullDevice = device;
    io::IXMLReader* xml = nullDevice->getFileSystem()->createXMLReader(filename.c_str());
    if(!xml)
    {
        log->logData("Failed to open", filename);
        return;
    }
    bool inSettings=false;
    while(xml->read()==true)
    {
        log->debugData(MINOR, "Found node", core::stringc(xml->getNodeName()).c_str());
        switch(xml->getNodeType())
        {
        case io::EXN_ELEMENT:
            {
            if(inSettings==false&&core::stringw(L"config").equals_ignore_case(xml->getNodeName()))
            {
                log->debugData(MINOR, "Found config");
                inSettings = true;
            }
            if(inSettings==true&&core::stringw(L"setting").equals_ignore_case(xml->getNodeName()))
            {
                core::stringw name = xml->getAttributeValue(L"name");
                f32 value = xml->getAttributeValueAsFloat(L"value");
                core::stringc namec = name.c_str();
                std::string names = namec.make_lower().c_str();
                data[names] = value;
                log->debugData(MINOR, "Found setting", names);
            }
            break;
            }
        case io::EXN_ELEMENT_END:
            {
            if(inSettings==true&&core::stringw(L"config").equals_ignore_case(xml->getNodeName()))
            {
                inSettings == false;
            }
            break;
            }
        }
    }
    xml->drop();
}

void Config::saveConfig(std::string filename)
{
    io::IXMLWriter* writer = device->getFileSystem()->createXMLWriter(filename.c_str());
    writer->writeXMLHeader();
    writer->writeElement(L"Config");
    writer->writeLineBreak();
    for(std::map<std::string, f32>::iterator it = data.begin(); it != data.end(); it++)
    {
        core::stringw name = it->first.c_str();
        core::stringw value = core::stringw(it->second);
        writer->writeElement(L"setting", true, L"name", name.c_str(), L"value", value.c_str());
        writer->writeLineBreak();
    }
    writer->writeClosingTag(L"Config");
    writer->drop();
}
