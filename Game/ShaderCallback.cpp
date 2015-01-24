#include "ShaderCallback.h"
using namespace FCE;
using namespace irr;
ShaderCallback::ShaderCallback()
{
}

ShaderCallback::~ShaderCallback()
{
}

void ShaderCallback::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
	video::IVideoDriver*  driver = services->getVideoDriver();
	core::matrix4 invWorld = driver->getTransform(video::ETS_WORLD);
	invWorld.makeInverse();
	
	if(useHighLevel)
	{
		services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);
	}else
	{
		services->setVertexShaderConstant(invWorld.pointer(), 0, 4);
	}
	
	core::matrix4 worldView;
	worldView = driver->getTransform(video::ETS_PROJECTION);
	worldView *= driver->getTransform(video::ETS_VIEW);
	worldView *= driver->getTransform(video::ETS_WORLD);
	
	if(useHighLevel)
	{
		services->setVertexShaderConstant("mWorldViewProj", worldView.pointer(), 16);
	}else
	{
		services->setVertexShaderConstant(worldView.pointer(), 4, 4);
	}

	core::array<scene::ISceneNode*> lights;
	device->getSceneManager()->getSceneNodesFromType(scene::ESNT_LIGHT, lights);
	int tInt = 0;
	for(int i = 0; i < lights.size(); i++)
	{
		std::stringstream tmp;
		tmp.str("mLIGHTPOS");
		tmp >> tInt;
		std::string name = tmp.str();
		//std::cout<<name<<std::endl;
		core::vector3df pos = lights[i]->getAbsolutePosition();
		if(useHighLevel)
		{
			services->setVertexShaderConstant(name.c_str(), reinterpret_cast<f32*>(&pos), 3);
		}else
		{
			services->setVertexShaderConstant(reinterpret_cast<f32*>(&pos), 8, 1);
		}
		
		std::stringstream tmp1;
		tmp1.str("mLIGHTCOL");
		tmp1 >> tInt;
		std::string name1 = tmp1.str();
		//std::cout<<name1<<std::endl;
		video::SColorf col = ((scene::ILightSceneNode*)(lights[i]))->getLightData().DiffuseColor;

		if(useHighLevel)
		{
			services->setVertexShaderConstant(name1.c_str(), reinterpret_cast<f32*>(&col), 4);
		}else
		{
			services->setVertexShaderConstant(reinterpret_cast<f32*>(&col), 9, 1);
		}
		tInt++;
	}

	
	if(useHighLevel)
	{
		services->setVertexShaderConstant("mLights", &tInt, 1);
	}else
	{
		//services->setVertexShaderConstant(&tInt, 9, 1);
	}
	
	core::matrix4 world = driver->getTransform(video::ETS_WORLD);
	world = world.getTransposed();
	
	if(useHighLevel)
	{
		services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);
		s32 texturelayer = 0;
		services->setPixelShaderConstant("mTexture", &texturelayer, 1);
	}else
	{
		services->setVertexShaderConstant(world.pointer(), 10, 4);
	}
	s32 time = device->getTimer()->getRealTime();
	if(useHighLevel)
	{
		services->setVertexShaderConstant("mTime", &time, 1);
	}else
	{
		//services->setVertexShaderConstant(&tInt, 9, 1);
	}

}
