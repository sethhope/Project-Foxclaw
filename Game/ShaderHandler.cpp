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

void ShaderHandler::addShader(std::string xmlFile, video::E_MATERIAL_TYPE curMaterial, u32 materialIndex)
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
    core::stringw type;
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
                    type = xml->getAttributeValue(0);
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
                    core::stringc fileBase;
                    if(type.equals_ignore_case("GLSL"))
                    {
                        fileBase = "Shaders/GLSL/";
                    }else if(type.equals_ignore_case("HLSL"))
                    {
                        fileBase = "Shaders/HLSL/";
                    }else
                    {
                        log->debugData(MINOR, "Failed to find shader type (is your .xml file updated?)");
                        break;
                    }
                    core::stringc vsFileB = fileBase;
                    core::stringc fsFileB = fileBase;
                    vsFileB.append(vsfile.c_str());
                    fsFileB.append(fsfile.c_str());
                    io::path vsFile = vsFileB.c_str();
                    io::path psFile = fsFileB.c_str();
                    log->debugData(MINOR, "Applying shader");
                    ShaderCallback* scb = new ShaderCallback();
                    scb->device = device;
                    scb->fsConstants = fsConstants;
                    scb->vsConstants = vsConstants;
                    video::IGPUProgrammingServices* gpu = device->getVideoDriver()->getGPUProgrammingServices();
                    u32 m;
                    const video::E_GPU_SHADING_LANGUAGE sLang = video::EGSL_DEFAULT;
                    video::E_MATERIAL_TYPE currM = curMaterial;
                    m = gpu->addHighLevelShaderMaterialFromFiles(vsFile, "vertexMain", video::EVST_VS_1_1, psFile, "pixelMain", video::EPST_PS_1_2, scb, currM, 0, sLang);
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

