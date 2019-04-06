/*
  ==============================================================================

    laf.cpp
    Created: 5 Apr 2019 8:37:27pm
    Author:  Gregory

  ==============================================================================
*/

#include "laf.h"

SoundshapeLAFs::SoundshapeLAFs()
{

	

}

SoundshapeLAFs::~SoundshapeLAFs() {}

void SoundshapeLAFs::setDefaultColors(LookAndFeel& laf) // there will be one of these for each theme
{// need to change the colors based on the yet-unwritten SSColours enum

	laf.setColour(Slider::thumbColourId, Colours::orange);
	laf.setColour(Slider::trackColourId, Colours::orange);
	laf.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);

	laf.setColour(TextButton::buttonColourId, laf.findColour(ResizableWindow::backgroundColourId));
	laf.setColour(TextButton::textColourOffId, Colours::white);

	laf.setColour(TextButton::buttonOnColourId, laf.findColour(TextButton::textColourOffId));
	laf.setColour(TextButton::textColourOnId, Colours::black);

}
// this didn't prove useful
//void CustomLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
//	float sliderPos, float minSliderPos, float maxSliderPos,
//	const Slider::SliderStyle style, Slider& slider)
//{
//	g.fillAll(slider.findColour(Slider::backgroundColourId));
//
//	if (slider.getComponentID().getIntValue() == 2001) // 2001 is the value for the VOLUME_SLIDER macro in GuiFunc.h
//	{
//		// paints the graphics surounding the volume slider
//		g.fillAll(Colours::darkgrey);
//		g.setColour(Colours::orange);
//		g.drawRect(slider.getLocalBounds(), 1);
//		Path tri1, tri2, squ, squ1;
//		float h = slider.getHeight() * .25f;
//		float w = slider.getWidth() * (h * 6);
//		//tri1.addTriangle(h, h, h, getHeight() - h, 3*h, getHeight()/2);
//		tri1.addTriangle(h, height / 2, 3 * h, height - h, 3 * h, h);
//		tri2.addTriangle(w - h, h, w - h, height - h, w - (3 * h), height / 2);
//		squ.addRectangle(w - (7 / 2 * h), (height / 2) - (h / 2), h, h);
//		squ1.addRectangle((3 / 2 * h), (height / 2) - (h / 2), h, h);
//		g.fillPath(tri1);
//		g.fillPath(tri2);
//		g.fillPath(squ);
//		g.fillPath(squ1);
//	}
//	drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
//	drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
//}