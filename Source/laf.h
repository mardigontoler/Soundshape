/*
  ==============================================================================

    laf.h
    Created: 5 Apr 2019 8:37:05pm
    Author:  Gregory

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SoundshapeLAFs
{
public:
	SoundshapeLAFs();
	~SoundshapeLAFs();
	static void setDefaultColors(LookAndFeel& laf); // there will be one of these for each theme.

	//TODO make custom enum for all Soundshape ColourIDs

private:
	
	//CustomLookAndFeel * claf;

};

struct CustomLookAndFeel : public LookAndFeel_V2
{

	// this didn't prove useful
	/*void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle style, Slider& slider) override;*/

};