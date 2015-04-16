#include "GUI.h"
using namespace FCE;
using namespace irr;

GUI::GUI(gui::IGUIEnvironment* environment, LOGGER* log)
{
    //initialize all values to their starting values
    this->log = log;
    this->env = environment;
    hasFocus = false;
    s=NULL;
}

GUI::~GUI()
{
    //nothing yet.
}

void GUI::addScript(SCRIPT* script)
{
    //set s to script... pretty simple, right?
    this->s = script;
}
//===================================
//BEGIN GUI INITIALIZATION FUNCTIONS
//===================================
void GUI::initButton(u32 id, core::stringw text, core::stringw text2, rect<s32> position, gui::IGUIElement* parent)
{
    type = FBUTTON;
    guiCaller = id;
    skin = env->getSkin();
    element = env->addButton(position, parent, -1, text.c_str(), text2.c_str());
}

void GUI::initScrollBar(u32 id, bool hor, rect<s32> position, gui::IGUIElement* parent)
{
    type = FSCROLL;
    guiCaller = id;
    skin = env->getSkin();
    element = env->addScrollBar(hor, position, parent);
}

void GUI::initCheckBox(u32 id, bool checked, core::stringw text, rect<s32> position, gui::IGUIElement* parent)
{
    type = FCHECK;
    guiCaller = id;
    skin = env->getSkin();
    element = env->addCheckBox(checked, position, parent, -1, text.c_str());
}

void GUI::initWindow(u32 id, core::stringw title, rect<s32> position, bool closeVisible, gui::IGUIElement* parent)
{
    type = FWINDOW;
    guiCaller = id;
    skin = env->getSkin();
    element = env->addWindow(position, false, title.c_str(), parent);
    ((gui::IGUIWindow*)element)->getCloseButton()->setVisible(closeVisible);

}

void GUI::initEditBox(u32 id, core::stringw defaultText, rect<s32> position, bool border, gui::IGUIElement* parent)
{
    type = FEDITBOX;
    guiCaller = id;
    skin = env->getSkin();
    element = env->addEditBox(defaultText.c_str(), position, border, parent);
}

void GUI::initListBox(u32 id, rect<s32> position, gui::IGUIElement* parent)
{
    type = FLIST;
    guiCaller = id;
    skin = env->getSkin();
    element = env->addListBox(position, parent);
}

void GUI::initText(u32 id, core::stringw text, bool border, bool wrap, rect<s32> position, gui::IGUIElement* parent)
{
    type = FTEXT;
    guiCaller = id;
    skin = env->getSkin();
    element = env->addStaticText(text.c_str(), position, border, wrap, parent);
}
void GUI::initImage(u32 id, video::ITexture* image, rect<s32> position, gui::IGUIElement* parent)
{
    type = FIMAGE;
    guiCaller = id;
    skin = env->getSkin();
    element = env->addImage(position, parent);
    ((gui::IGUIImage*)element)->setImage(image);
    ((gui::IGUIImage*)element)->setScaleImage(true);
}
void GUI::initFileDiag(u32 id, core::stringw title, gui::IGUIElement* parent)
{
    type = FFILEDIAG;
    guiCaller = id;
    skin = env->getSkin();
    element = env->addFileOpenDialog(title.c_str(), true, parent);
    workingDir = env->getFileSystem()->getWorkingDirectory();
}
u32 GUI::addItemToList(core::stringw text)
{
    return ((gui::IGUIListBox*)element)->addItem(text.c_str());
}

void GUI::removeItemFromList(u32 id)
{
    ((gui::IGUIListBox*)element)->removeItem(id);
}
//=======================================
//END INITIALIZATION FUNCTIONS
//=======================================
void GUI::setTransparency(s32 alpha)
{
    //sets the transparency for all GUI objects to (alpha)
    for(s32 i = 0; i < irr::gui::EGDC_COUNT; i++)
    {
        video::SColor col = skin->getColor((gui::EGUI_DEFAULT_COLOR)i);
        col.setAlpha(alpha);
        skin->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
    }
}

void GUI::setParent(GUI* parent)
{
    //Read the function name... It does that.
    parent->element->addChild(this->element);
    this->parent = parent;
    parent->children.push_back(this);
}

bool GUI::OnEvent(const SEvent& event)
{
    if(event.EventType == EET_GUI_EVENT)
    {
        if(s!=NULL && s->L != NULL && type != FTEXT)
        {
            if(parent)
            {
                luaW_push<GUI>(s->L, parent);
                lua_setglobal(s->L, "parent");
            }
            luaW_push<GUI>(s->L, this);
            lua_setglobal(s->L, "current");
        }
        s32 id = event.GUIEvent.Caller->getID();
        if(id == element->getID() && element->isVisible() == false)
        {
            log->debugData(MAJOR, "Element not visible... deleting.", id);
            for(std::vector<GUI*>::iterator it = children.begin(); it < children.end(); it++)
            {
                (*it)->guiCaller = 0;
            }
            guiCaller = 0;
        }
        switch(event.GUIEvent.EventType)
        {
        case gui::EGET_SCROLL_BAR_CHANGED:
            if(id == element->getID() && type == FSCROLL)
            {
                s32 pos = ((gui::IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
                lua_getglobal(s->L, "onScroll");
                lua_pushnumber(s->L, pos);
                if(lua_pcall(s->L, 1, 0, 0)!=0)
                {
                    log->logData("Failed to run onScroll function", lua_tostring(s->L, -1));
                    return false;
                }
            }
            return true;
            break;
        case gui::EGET_BUTTON_CLICKED:
            if(id == element->getID() && type == FBUTTON)
            {
                lua_getglobal(s->L, "onClick");
                if(lua_pcall(s->L, 0, 0, 0)!=0)
                {
                    log->logData("Failed to run onClick function", lua_tostring(s->L, -1));
                    return false;
                }
            }
            return true;
            break;
        case gui::EGET_EDITBOX_ENTER:
            if(id == element->getID() && type == FEDITBOX)
            {
                lua_getglobal(s->L, "onEnter");
                core::stringc text = ((gui::IGUIEditBox*)event.GUIEvent.Caller)->getText();
                lua_pushstring(s->L, text.c_str());
                if(lua_pcall(s->L, 1, 0, 0)!=0)
                {
                    log->logData("Failed to run onEnter function", lua_tostring(s->L, -1));
                    return false;
                }
            }
            return true;
            break;
        case gui::EGET_EDITBOX_CHANGED:
            if(id == element->getID() && type == FEDITBOX)
            {
                lua_getglobal(s->L, "onChange");
                if(lua_pcall(s->L, 0, 0, 0)!=0)
                {
                    log->logData("Failed to run onChange function", lua_tostring(s->L, -1));
                    return false;
                }
            }
            return true;
            break;
        case gui::EGET_EDITBOX_MARKING_CHANGED:
            if(id == element->getID() && type == FEDITBOX)
            {
                lua_getglobal(s->L, "onMarkChange");
                if(lua_pcall(s->L, 0, 0, 0)!=0)
                {
                    log->logData("Failed to run onMarkChange function", lua_tostring(s->L, -1));
                    return false;
                }
            }
            return true;
            break;
        case gui::EGET_LISTBOX_CHANGED:
            if(id == element->getID() && type == FLIST)
            {
                lua_getglobal(s->L, "onChange");
                if(lua_pcall(s->L, 0, 0, 0)!=0)
                {
                    log->logData("Failed to run onChange function", lua_tostring(s->L, -1));
                    return false;
                }
            }
            return true;
            break;
        case gui::EGET_LISTBOX_SELECTED_AGAIN:
            if(id == element->getID() && type == FLIST)
            {
                lua_getglobal(s->L, "onSelect");
                s32 sel = ((gui::IGUIListBox*)element)->getSelected();
                lua_pushnumber(s->L, sel);
                if(lua_pcall(s->L, 1, 0, 0)!=0)
                {
                    log->logData("Failed to run onSelect function", lua_tostring(s->L, -1));
                    return false;
                }
            }
            return true;
            break;
        case gui::EGET_FILE_SELECTED:
            if(id == element->getID() && type == FFILEDIAG)
            {
                lua_getglobal(s->L, "onSelect");
                core::stringw file = ((gui::IGUIFileOpenDialog*)element)->getDirectoryName();
                file.append("/");
                file.append(((gui::IGUIFileOpenDialog*)element)->getFileName());
                core::stringc cfile = core::stringc(file.c_str());
                std::string sfile = cfile.c_str();
                sfile.erase(0, 2);
                lua_pushstring(s->L, sfile.c_str());
                if(lua_pcall(s->L, 1, 0, 0) != 0)
                {
                    log->logData("Failed to run onSelect function", lua_tostring(s->L, -1));
                    return false;
                }
                env->getFileSystem()->changeWorkingDirectoryTo(workingDir);
                guiCaller = 0;
            }
            return true;
            break;
        case gui::EGET_FILE_CHOOSE_DIALOG_CANCELLED:
            if(id == element->getID() && type == FFILEDIAG)
            {
                lua_getglobal(s->L, "onCancel");
                if(lua_pcall(s->L, 0, 0, 0) != 0)
                {
                    log->logData("Failed to run onCancel function", lua_tostring(s->L, -1));
                    return false;
                }
                env->getFileSystem()->changeWorkingDirectoryTo(workingDir);
                guiCaller = 0;
            }
            return true;
            break;
        case gui::EGET_ELEMENT_FOCUSED:
            if(id == element->getID() && type != FIMAGE)
            {
                hasFocus = true;
            }
            return true;
            break;
        case gui::EGET_ELEMENT_FOCUS_LOST:
            if(id == element->getID() && type != FIMAGE)
            {
                hasFocus = false;
            }
            return true;
            break;
        case gui::EGET_ELEMENT_CLOSED:
            if(id == element->getID())
            {
                hasFocus = false;
                if(type == FWINDOW)
                {
                    lua_getglobal(s->L, "onClose");
                    if(lua_pcall(s->L, 0, 0, 0)!=0)
                    {
                        log->logData("Failed to run onClose function", lua_tostring(s->L, -1));
                        return false;
                    }
                    for(std::vector<GUI*>::iterator it = children.begin(); it < children.end(); it++)
                    {
                        (*it)->guiCaller = 0;
                    }
                }
                guiCaller = 0;
            }
            return true;
            break;
        default:
            break;
        }
    }
    if(hasFocus)
    {
        return true;
    }
    return false;
}
