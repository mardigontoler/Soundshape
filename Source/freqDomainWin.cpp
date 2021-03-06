/*
  ==============================================================================

    freqDomainWin.cpp
    Created: 28 Dec 2018 11:48:50am
    Author:  Daniel Reiter
	Component: Render current frequency seleced frequency profile and allow users to 
			   add and modify frequency spikes using sliders and buttons
  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "freqDomainWin.h"

//==============================================================================
//  freqDomainWin constructor set default values to variables to allow a GUI to open
//==============================================================================
freqDomainWin::freqDomainWin()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	setComponentID((String)FREQ_DOMAIN);
	parent = NULL;
	buttonParent = NULL;
	size = 0;
	first = -1;           
	int temp = -1;		  
	add = &temp;
	harm = &temp;
	laf = new CustomLookAndFeel();
	laf->initColors(Colour(SoundshapeLAFs::base1ID), Colour(SoundshapeLAFs::base1textID),
		Colour(SoundshapeLAFs::base2ID), Colour(SoundshapeLAFs::base2textID),
		Colour(SoundshapeLAFs::background1ID), Colour(SoundshapeLAFs::background2ID),
		Colour(SoundshapeLAFs::background3ID));
	resetColors();

}


freqDomainWin::~freqDomainWin()
{
}
//==============================================================================

void freqDomainWin::resetColors()
{

	laf->setColors();

}

//==============================================================================
//  Paint Funciton
//==============================================================================
void freqDomainWin::paint (Graphics& g)
{

	float pixel = getWidth() * .01f;
	int n = getWidth() * 10;
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	g.fillAll(laf->findColour(SoundshapeLAFs::background2ID));

	g.setColour(laf->findColour(SoundshapeLAFs::background2ID));
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
	g.setColour(Colours::black); // freq-domain scale/axes
	g.setFont(14.0f);
	float margin = getHeight() *.10f;
	Line<float> vLine(margin, margin, margin, getHeight() - margin);
	Line<float> hLine(margin, getHeight() - margin, getWidth() - margin, getHeight() - margin);
	g.drawLine(vLine);
	g.drawLine(hLine);
	float vTick = vLine.getLength() / 10;
	float hTick = hLine.getLength() / 100;
	float smallTick = hLine.getLength() / 4000;
	Rectangle<float> vArea(.5f * margin, margin, margin, vLine.getLength());
	Rectangle<float> hArea(margin, getHeight() - (1.5f * margin), hLine.getLength(), margin);
	Rectangle<float> smallArea(margin, getHeight() - (1.25f *margin), hLine.getLength(), margin / 2);
	Rectangle<float> testPrint(margin, 1.25f *margin, hLine.getLength(), margin / 2);
	g.setFont(vTick * .8f);
	Rectangle<float> hTickArea(hArea);

	// draw horizontal scale
	for (int i = 0; i < size; i++)
	{

		String temp = std::to_string(i);
		smallArea.removeFromLeft(smallTick);
		hTickArea.removeFromLeft(smallTick);
		if (i < 100 && i % 10 == 0)
		{
			g.drawText(temp, vArea, Justification::bottomLeft, true);
			vArea.removeFromBottom(vTick);
			g.drawLine(vArea.getRight(), vArea.getBottom(), vArea.getWidth(), vArea.getBottom());
		}
		if (i % 10 == 0)
		{
			g.drawText(temp, hTickArea, Justification::bottomLeft, true);
			Line<float> tLine(hTickArea.getBottomLeft(), hTickArea.getTopLeft());
			g.drawLine(tLine);
		}

		Line<float> tLine(smallArea.getBottomLeft(), smallArea.getTopLeft());
		if (i % 2 == 0)
		{
			components[(int)(i/2)]->setBounds(smallArea.getX() - (margin / 6), smallArea.getCentreY() - (margin / 6), margin / 3, margin / 3);
			sliders[(int)(i/2)]->setBounds(smallArea.getX() - (margin / 16), margin, margin / 4, getHeight() - (2 * margin));
			
		}
		g.drawLine(tLine);
		testPrint.removeFromLeft(smallTick);
	}

	margin = getHeight() *.10f;
	Rectangle<float> btnArea(margin, getHeight() - (1.25f *margin), getWidth() - (2 * margin), margin / 4);
	float tick = btnArea.getWidth() / 1024;

	// loop sets location, bounds, and visiblity of sliders and buttons
	for (int i = 0; i < size/2; i++)
	{

		DrawablePath normal, down, over;
		btnArea.removeFromLeft(tick);

		if (sliders[i]->getValue() <= 0)
		{
			// set laction and bounds for each slider and button

			
			// If add button is on - set add buttons visilbe to true
			if (*add > 0 && !sliders[i]->isVisible())
			{
				if ((*harm > 0 && (2*i) % 440 != 0) || i == 0)
				{
					components[i]->setVisible(false);
				}
				// no harmonic correctness case
				else
				{
					components[i]->setVisible(true);
				}
			}
			// hide add buttons
			else
			{

				components[i]->setVisible(false);
			}
		}
	}
}
//==============================================================================

//==============================================================================
//  Resize Funciton
//==============================================================================
void freqDomainWin::resized()
{

}
//==============================================================================

//==============================================================================
//  setBase passes need references to freqDomainWin
//==============================================================================
void freqDomainWin::setBase(int * _harm, int * _add, Slider::Listener* _parent, Button::Listener* _bParent, Converter* _profile, int _size, int *_chunk, CustomLookAndFeel * _laf)
{
	harm = _harm;                // flag for harmonic correctness is on/off
	add = _add;                  // flag for add buttons visiblity on/off
	parent = _parent;            // slider listener 
	profile = _profile;          // set frequency profile values
	buttonParent = _bParent;	 // button listener
	chunk = _chunk;
	size = _size;
	setProfileControl();
	laf = _laf;

}
//==============================================================================

//==============================================================================
//  setProfileControl Funciton sets values of a frequency profile to a list of 
//  sliders, declares and instaniates a list of sliders and a list of buttons
//==============================================================================
void freqDomainWin::setProfileControl()
{
	// sets list to empty
	emptyList();

	for (int i = 0; i < size; i = i+2)
	{
		auto * tb = addToList(new TextButton(""));
		tb->setComponentID(String(i));
		tb->setClickingTogglesState(false);
		tb->setVisible(false);
		//tb->setLookAndFeel(laf);
		tb->setColour(TextButton::buttonColourId, Colours::red);
		if (buttonParent != NULL)
		{
			tb->addListener(buttonParent);
		}

		auto * sb = createSlider();
		sb->setRange(0.0, 600.0, 0.1);
		sb->setSliderStyle(Slider::LinearBarVertical);
		sb->setComponentID(String(i));
		double test = (double)(profile->getFrequencyValue(*chunk, i));
		sb->setValue(test);
		if (i % 440 == 0 && i != 0)
		{
			sb->setColour(Slider::trackColourId, Colours::red);
		}
		else
		{
			sb->setColour(Slider::trackColourId, Colours::blue);
		}
		sb->setTextBoxIsEditable(false);
		sb->setPopupDisplayEnabled(true, true, this);
		if (i == 440)
		{
			sb->setVisible(true);
		}
		else if (sb->getValue() > 0)
		{
			sb->setVisible(true);
		}
		else
		{
			sb->setVisible(false);

		}
		if (parent != NULL)
		{
			sb->addListener(parent);
		}
	}

}
//==============================================================================

//==============================================================================
//  setProfileControl Funciton sets values of a frequency profile to a list of 
//  sliders, declares and instaniates a list of sliders and a list of buttons
//==============================================================================
void freqDomainWin::setProfile()
{

	// declares and instaniates a list of sliders and a list of buttons
	for (int i = 0; i < size/2; i++)
	{
		sliders[i]->removeListener(parent);
		double test = (double)(profile->getFrequencyValue(*chunk, (2*i)));
		sliders[i]->setValue(test);
		sliders[i]->addListener(parent);
		if (i == 440)
		{
			sliders[i]->setVisible(true);
		}
		else if(sliders[i]->getValue() > 0)
		{
			sliders[i]->setVisible(true);
		}
		else
		{
			sliders[i]->setVisible(false);

		}
	}
	
}
//==============================================================================
