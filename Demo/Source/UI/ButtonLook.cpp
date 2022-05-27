/*
  ==============================================================================

    ButtonLook.cpp
    Created: 26 May 2022 3:57:41pm
    Author:  bobo

  ==============================================================================
*/

#include "ButtonLook.h"


/*
=================================================================================
ButtonLook
=================================================================================
*/

ButtonLook::ButtonLook()
{
    btn[0] = ImageCache::getFromMemory(BinaryData::switchBtn_png, BinaryData::switchBtn_pngSize);
    btn[1] = ImageCache::getFromMemory(BinaryData::switchBtn2_png, BinaryData::switchBtn2_pngSize);
}


ButtonLook::~ButtonLook()
{
}

//==============================================================================
void ButtonLook::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
    bool, bool isButtonDown)
{
    auto buttonArea = button.getLocalBounds();

    g.drawImageWithin(btn[stage],
        buttonArea.getX(),
        buttonArea.getY(),
        buttonArea.getWidth(),
        buttonArea.getHeight(),
        RectanglePlacement::centred);
}

//==============================================================================
void ButtonLook::setStage(int state)
{
    stage = state;
}

//==============================================================================
int ButtonLook::getStage()
{
    return stage;
}