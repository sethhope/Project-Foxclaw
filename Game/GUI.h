#ifndef _FGUI_H_
#define _FGUI_H_
#include "stdafx.h"
#include "Misc/Script.h"
#include "Misc/Logger.h"
#include "irrlicht.h"
#include "lua.hpp"

#define FBUTTON 0
#define FSCROLL 1
#define FCHECK 2
#define FLIST 3
#define FMESSAGE 4
#define FEDITBOX 5
#define FWINDOW 6
#define FTEXT 7
#define FIMAGE 8
#define FFILEDIAG 9

using namespace irr;
namespace FCE
{
class GUI
{
public:
    GUI(gui::IGUIEnvironment* environment, LOGGER* log);
    ~GUI();

    void initButton(u32 id, core::stringw text, core::stringw text2, rect<s32> position, gui::IGUIElement* parent = 0);
    void initScrollBar(u32 id, bool hor, rect<s32> position, gui::IGUIElement* parent = 0);
    void initCheckBox(u32 id, bool checked, core::stringw text, rect<s32> position, gui::IGUIElement* parent = 0);
    void initWindow(u32 id, core::stringw title, rect<s32> position, bool closeVisible, gui::IGUIElement* parent = 0);
    void initEditBox(u32 id, core::stringw defaultText, rect<s32> position, bool border, gui::IGUIElement* parent = 0);
    void initListBox(u32 id, rect<s32> position, gui::IGUIElement* parent = 0);
    void initText(u32 id, core::stringw text, bool border, bool wrap, rect<s32> position, gui::IGUIElement* parent = 0);
    void initImage(u32 id, video::ITexture* image, rect<s32> position, gui::IGUIElement* parent = 0);
    void initFileDiag(u32 id, core::stringw title, gui::IGUIElement* parent = 0);
    void setTransparency(s32 alpha);
    u32 addItemToList(core::stringw text);
    u32 insertItemToList(u32 index, core::stringw text);
    void removeItemFromList(u32 id);
    bool OnEvent(const SEvent& event);
    void addScript(SCRIPT* script);
    void setParent(GUI* parent);
    gui::IGUIEnvironment* env;
    gui::IGUIElement* element;
    u32 guiCaller;
    std::vector<GUI*> children;
private:
    bool hasFocus;
    u8 type;
    gui::IGUISkin* skin;
    SCRIPT* s;
    LOGGER* log;
    GUI* parent;
    io::path workingDir;
};
}
#endif
