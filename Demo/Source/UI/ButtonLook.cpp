/*
  ==============================================================================

    ButtonLook.cpp
    Created: 4 May 2022 2:54:41pm
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
    btn[0] = ImageCache::getFromMemory(BinaryData::btn1_png, BinaryData::btn1_pngSize);
    btn[1] = ImageCache::getFromMemory(BinaryData::btn2_png, BinaryData::btn2_pngSize);
    btn[2] = ImageCache::getFromMemory(BinaryData::btn3_png, BinaryData::btn3_pngSize);
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
void ButtonLook::setStage(int n)
{
    stage = n;
}

//==============================================================================
int ButtonLook::getStage()
{
    return stage;
}

/*
=================================================================================
ButtonLook2
=================================================================================
*/

ButtonLook2::ButtonLook2()
{
    btn[0] = ImageCache::getFromMemory(BinaryData::switchBtn_png, BinaryData::switchBtn_pngSize);
    btn[1] = ImageCache::getFromMemory(BinaryData::switchBtn2_png, BinaryData::switchBtn2_pngSize);
}


ButtonLook2::~ButtonLook2()
{
}

//==============================================================================
void ButtonLook2::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
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
void ButtonLook2::setStage(int n)
{
    stage = n;
}

//==============================================================================
int ButtonLook2::getStage()
{
    return stage;
}