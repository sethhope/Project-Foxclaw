#include "PostShader.h"

using namespace irr;
using namespace FCE;

POSTSHADER::POSTSHADER(IrrlichtDevice* device, LOGGER* log)
{
	this->log = log;
	this->device = device;
}

POSTSHADER::~POSTSHADER()
{
	
}

void POSTSHADER::load(std::string vert, std::string frag)
{
	log->debugData(MAJOR, "Loading post shaders...");
	log->debugData(MAJOR, vert, frag);
	plane = device->getSceneManager()->addBillboardSceneNode(0, (core::dimension2d<f32>)device->getVideoDriver()->getCurrentRenderTargetSize(), core::vector3df(0, 0, 100));
	plane->setVisible(false);
	io::path vsFile = vert.c_str();
	io::path psFile = frag.c_str();

	ShaderCallback* scb = new ShaderCallback();
	scb->device = device;
	video::IGPUProgrammingServices* gpu = device->getVideoDriver()->getGPUProgrammingServices();
	const video::E_GPU_SHADING_LANGUAGE sLang = video::EGSL_DEFAULT;
	s32 mat1 = gpu->addHighLevelShaderMaterialFromFiles(vsFile, "vertexMain", video::EVST_VS_1_1, psFile, "pixelMain", video::EPST_PS_1_1, scb, video::EMT_SOLID, 0, sLang);
	
	scb->drop();

	plane->setMaterialFlag(video::EMF_LIGHTING, false);
	plane->setMaterialType((video::E_MATERIAL_TYPE)mat1);
	
	log->debugData(MAJOR, "Shaders loaded");
}

void POSTSHADER::init()
{
	if (device->getVideoDriver()->queryFeature(video::EVDF_RENDER_TO_TARGET))
    {
        target = device->getVideoDriver()->addRenderTargetTexture(device->getVideoDriver()->getCurrentRenderTargetSize(), "rt);
        shaderCam = device->getSceneManager()->addCameraSceneNode();
        shaderCam->setNearValue(0);
    }else
    {
    	log->logData("Render to texture not supported! Failed to create texture!");
	}
}

void POSTSHADER::update()
{
	
}

void POSTSHADER::render(video::ITexture* planeImage)
{
	device->getVideoDriver()->setRenderTarget(target, true, true, video::SColor(255, 255, 0, 0));
	plane->setMaterialTexture(0, planeImage);
	plane->setVisible(true);
	device->getSceneManager()->setActiveCamera(shaderCam);
	device->getSceneManager()->drawAll();
	plane->setVisible(false);
}
