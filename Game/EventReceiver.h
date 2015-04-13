#ifndef _FEVENT_RECEIVER_H_
#define _FEVENT_RECEIVER_H_
#include "stdafx.h"
#include "irrlicht.h"
#include "GUI.h"
#include "Objects/LIGHT.h"

#define MLEFT 0
#define MMIDDLE 1
#define MRIGHT 2

using namespace irr;
namespace FCE
{
class FEventReceiver : public IEventReceiver
{
public:
    FEventReceiver();
    ~FEventReceiver();
    virtual bool OnEvent(const SEvent& event);
    virtual bool KeyDown(EKEY_CODE keyCode);
    virtual bool MouseButton(u8 button);
    virtual core::position2di mousePos();
    virtual void addGUI(GUI* gui);

    virtual void OnPreRender(core::array<scene::ISceneNode*> & lightList);
    virtual void OnPostRender();
    virtual void OnRenderPassPreRender(scene::E_SCENE_NODE_RENDER_PASS renderPass);
    virtual void OnRenderPassPostRender(scene::E_SCENE_NODE_RENDER_PASS renderPass);
    virtual void OnNodePreRender(scene::ISceneNode* node);
    virtual void OnNodePostRender(scene::ISceneNode* node);

    std::vector<GUI*> callers;
private:
    scene::E_SCENE_NODE_RENDER_PASS CurrentRenderPass;
    scene::ISceneNode * CurrentSceneNode;
    bool mouseButtons[3];
    core::position2di Position;
    bool keys[KEY_KEY_CODES_COUNT];
    core::array<scene::ISceneNode*> *lights;

    class LightDistanceElement
    {
    public:
        LightDistanceElement() {};

        LightDistanceElement(scene::ISceneNode* n, f64 d)
            : node(n), distance(d) { }

        scene::ISceneNode* node;
        f64 distance;

        // Lower distance elements are sorted to the start of the array
        bool operator < (const LightDistanceElement& other) const
        {
            return (distance < other.distance);
        }
    };
};
}
#endif
