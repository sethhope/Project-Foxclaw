#include "ShaderHandler.h"
using namespace FCE;
ShaderHandler::ShaderHandler(IrrlichtDevice* device, LOGGER* log)
{
    this->device = device;
    this->log = log;
}

ShaderHandler::~ShaderHandler()
{

}

void ShaderHandler::addShader(std::string xmlFile)
{
    io::IXMLReader* xml = device->getFileSystem()->createXMLReader(xmlFile.c_str());
    if(!xml)
    {
        log->logData("FAILED TO LOAD SHADER XML", xmlFile.c_str());
        return;
    }
    bool inShader = false;
    core::stringw vsfile;
    core::stringw fsfile;
    while(xml->read())
    {
        switch(xml->getNodeType())
        {
            case io::EXN_ELEMENT:
            {
                if(inShader == false && core::stringw(L"shader").equals_ignore_case(xml->getNodeName()))
                {
                    log->debugData(MINOR, "Found shader");
                    inShader = true;
                    fsConstants.clear();
                    vsConstants.clear();
                }
                if(inShader == true && core::stringw(L"filename").equals_ignore_case(xml->getNodeName()))
                {
                    vsfile = xml->getAttributeValue(0);
                    fsfile = xml->getAttributeValue(1);
                }
                if(inShader == true && core::stringw(L"vsConstant").equals_ignore_case(xml->getNodeName()))
                {
                    core::stringc tmp = xml->getAttributeValue(0);
                    vsConstants[tmp.c_str()] = xml->getAttributeValueAsFloat(1);
                    log->debugData(MINOR, "Found vsConstant", tmp.c_str());
                }
                if(inShader == true && core::stringw(L"fsConstant").equals_ignore_case(xml->getNodeName()))
                {
                    core::stringc tmp = xml->getAttributeValue(0);
                    fsConstants[tmp.c_str()] = xml->getAttributeValueAsFloat(1);
                    log->debugData(MINOR, "Found fsConstant", tmp.c_str());
                }
            }break;
            case io::EXN_ELEMENT_END:
            {
                if(inShader == true && core::stringw(L"shader").equals_ignore_case(xml->getNodeName()))
                {
                    inShader = false;
                    io::path vsFile = vsfile.c_str();
                    io::path psFile = fsfile.c_str();
                    log->debugData(MINOR, "Applying shader");
                    ShaderCallback* scb = new ShaderCallback();
                    scb->device = device;
                    scb->fsConstants = fsConstants;
                    scb->vsConstants = vsConstants;
                    video::IGPUProgrammingServices* gpu = device->getVideoDriver()->getGPUProgrammingServices();
                    u32 m;
                    const video::E_GPU_SHADING_LANGUAGE sLang = video::EGSL_DEFAULT;
                    video::E_MATERIAL_TYPE currM = video::EMT_SOLID;
                    m = gpu->addHighLevelShaderMaterialFromFiles(vsFile, "vertexMain", video::EVST_VS_1_1, psFile, "pixelMain", video::EPST_PS_1_1, scb, currM, 0, sLang);
                    shaders.push_back(scb);
                    materials.push_back(m);
                }
            }break;
        }
    }
    xml->drop();
}

void ShaderHandler::addConstant(u32 shader, std::string key, f32 data)
{
    if(shader == FCE_VERT)
    {
        vsConstants[key] = data;
    }else if(shader == FCE_FRAG)
    {
        fsConstants[key] = data;
    }
}

