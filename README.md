# **JUCE-plugin** ##

> Build audio plug-in with JUCE framework.

## **Demo** ##

- **Program** 

  > An audio plug-in with the spectrum and the level meter. 
  
  <img height= 300 src="https://user-images.githubusercontent.com/89720769/166882493-39eabea1-13f0-48aa-8618-a968b705c6fa.png"> 
  
  We can use Input Gain Slider and Output Gain Slider to control the distortion level and the output level of this plug-in. The button in the middle of can change the different distortion types. There are four of them to choose, including tanh, arctan, soft clipping and hard clipping. The window below the distortion label shows the characteristic function of the current using distortion type. The button with two arrows can turn on and of the spectrum. The vertical level meters are the input level and the output level while horizontal ones are the left and right channel.
  
- **Component** 
  
  - **UI/SliderLook**
  
    For customizing the appearence of the sliders, e.g., input/output gain slider.
    
  - **UI/ButtonLook**

    For customizing the appearence of the buttons, e.g., mode choose button and switch button.
    
  - **UI/SpectrumComponent**

    Spectrum

  - **UI/LevelMeterComponent**

    Level meters, including vertical and horizontal ones. For monitoring the levels of input, output, left and left channel.

  - **UI/CharacteristicFunctionComponent**

    Providing different distortion type to choose from.
