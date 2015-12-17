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
    }
    else
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
    }
    else
    {
        services->setVertexShaderConstant(worldView.pointer(), 4, 4);
    }

    f32 timept = device->getTimer()->getTime();
    if(useHighLevel)
    {
        services->setVertexShaderConstant("mWorldTime", &timept, 1);
    }
    for(std::map<std::string, f32>::iterator it = vsConstants.begin(); it != vsConstants.end(); it++)
    {
        f32 pt = it->second;
        services->setVertexShaderConstant(it->first.c_str(), &pt, 1);
    }
    for(std::map<std::string, f32>::iterator it = fsConstants.begin(); it != fsConstants.end(); it++)
    {
        f32 pt = it->second;
        services->setPixelShaderConstant(it->first.c_str(), &pt, 1);
    }
}
