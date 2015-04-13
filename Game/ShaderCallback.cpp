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
}
