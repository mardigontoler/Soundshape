/*
  ==============================================================================

    freqDomainWin.h
    Created: 28 Dec 2018 11:48:50am
    Author:  Daniel Reiter
	Component: Render current frequency seleced frequency profile and allow users to 
			   add and modify frequency spikes using sliders and buttons

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Converter.h"
#include "laf.h"

#define FREQ_DOMAIN 3000

//==============================================================================
/*
*/
class freqDomainWin    : public Component
{
public:
    freqDomainWin();
    ~freqDomainWin();

    void paint (Graphics&) override;
    void resized() override;
	void setBase(int * _harm, int * _add, Slider::Listener* _parent,Button::Listener* _bParent, Converter* _profile, int _size, int * _chunk, CustomLookAndFeel * _laf);
	//void setBase(int * _harm, int * _add, Slider::Listener* _parent, Button::Listener* _bParent, float* _profile, int _size, int * _chunk);
	void setProfileControl();
	void setProfile();
	void resetColors();


private:
	int first;                          // Variable to track for first harmonic value 
	int size;
	int * harm;                         // flag harmonic correctness is on/off 
	int *add;							// flag add button is on/off
	int *chunk;
	Converter * profile;				// reference to current frequency profile's values
	double currentProfile[4000];
	Slider::Listener* parent;			// reference to parent as a slider listener
	Button::Listener* buttonParent;     // reference to parent as a button listener
	CustomLookAndFeel * laf;

	// list of buttons to add frquency spikes
	OwnedArray<TextButton> components;		
	TextButton* addToList(TextButton * newComp)
	{
		components.add(newComp);
		addAndMakeVisible(newComp);
		newComp->onClick = [this] {
			auto * focused = Component::getCurrentlyFocusedComponent();
			float margin = this->getHeight() *.10f;
			if (this->first < 0)
			{
				this->first = focused->getComponentID().getIntValue();
			}
			focused->setVisible(false);

			sliders[(int)(focused->getComponentID().getIntValue()/2)]->setVisible(true);
		};
		return newComp;
	}

	// list of slides to modify frequnecy spikes
	OwnedArray<Slider> sliders;
	Slider * createSlider()
	{
		Slider * s = new Slider();
		sliders.add(s);
		addAndMakeVisible(s);
	    return s;
	}

	// clears both slider and button lists
	void emptyList()
	{
		components.clear(true);
		sliders.clear(true);
	}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (freqDomainWin)
};
