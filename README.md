# **JUCE-plugin** ##

> Build audio plug-in with JUCE framework.

## **Distortion** ##

- **Program** 

  > Distortion plug-in with the spectrum and the level meter. 
  
  <img height= 300 src="https://user-images.githubusercontent.com/89720769/174529189-52b72653-5af7-47a3-aa85-c245391ed4d2.png"> 
  
- **Component** 
  
  - **UI/SliderLook**
  
    Slider appearence customizing, e.g., input/output gain slider.
    
  - **UI/ButtonLook**

    Buttons appearences customizing, e.g., mode choose button and switch button.
    
  - **UI/SpectrumComponent**

    Spectrum analyzer.
    
  - **UI/WaveformComponent**

    Waveform analyzer.

  - **UI/LevelMeterComponent**

    Level meters, including vertical and horizontal ones. For monitoring the input and output levels in left and right channel.

  - **UI/CharacteristicFunctionComponent**

    Providing different distortion type to choose from.

## **Spatializer** ##

- **Program** 

  > Spatializer with the vectorscope and waveform analyzer. 
  
  <img height= 300 src="https://user-images.githubusercontent.com/89720769/174529614-4e6a5805-3bc0-4f8d-8159-3802d0954e8c.png"> 
  
- **Component**
  
  - **UI/HRIR3D.h**

    Storing HRIRs.
  
  - **UI/SliderLook**
  
    Slider appearence customizing, e.g., input/output gain slider.
    
  - **UI/SliderLook2**

    Slider appearence customizing, e.g., rotary and vertical slider in panner component.
    
  - **UI/ButtonLook**

    On and off appearences beteen different mode, HRTF mode and regular mode.
    
  - **UI/ImportComponent**

    Load and Normalize buttons.
    
  - **UI/WaveformComponent

    Waveform analyzer.

  - **UI/LevelMeterComponent**

    Vertical Level meters. For monitoring the input and output levels of left and right channel.

  - **UI/ModeComponent**

    Switching between HRTF and regular mode.
    
  - **UI/PannerComponent**

    Adjusting azimuth and the elevation.
    
  - **UI/VectorScopeComponent**

    Vectorscope analyzer
  
## **HRTF** ##

- **Program** 

  > HRTF plug-in with the vectorscope and level meter. 
  
  <img height= 300 src="https://i.imgur.com/1TsqWFT.png"> 
  
- **Component**

  - **HRIR3D.h**

    Storing HRIRs
    
  - **LevelMeterComponent**

    Level meter for monitoring input and output level.
    
  - **PannerComponent**

    Adjusting azimuth and elevation.
    
  - **SliderLook**

    Slider appearence customizing, e.g., wet slider and gain slider.
    
  - **SliderLook2**

    Slider appearence customizing, e.g., azimuth slider and elevation slider.
    
  - **VectorScopeComponent**

    Vectorscope analyzer

## **Building Platform** ##

- Xcode (macOS)

- VisualStudio2022

   
