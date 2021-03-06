/*
  ==============================================================================

	bigTime.cpp
	Created: 20 Dec 2018 1:00:48pm
	Description: Component uses a view port to display the time domain adn contains buttons to allow the 
					user to select a magnified view in the small time window.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "bigTime.h"



//==============================================================================
//  Constructor
//==============================================================================
bigTime::bigTime(AudioProcessorValueTreeState& _valueTreeState):valueTreeState(_valueTreeState)
{
	
	xPoint = new int;

	time = new int;

	*xPoint = 0;
	*time = 10;
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

	//  Set viewport settings
	timeBase.setSize(1000, 1000);
	view.setViewedComponent(&timeBase, false);
	view.setScrollBarsShown(false, false);
	addAndMakeVisible(view);

	//  Play time slider 
    playTime = new Slider();
    addAndMakeVisible(playTime);
	playTime->setComponentID((String)PLAYTIME_SLIDER);
    playTime->setSliderStyle(Slider::TwoValueHorizontal);
	playTime->setRange(0, 50, 1);
	//playTime->setMinAndMaxValues(0, 50);
    playTime->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	playTime->setTooltip("Controls how much of the sound is played when the Play Button is pressed");

    // hook up as a listener for changes to the play slider
    _valueTreeState.addParameterListener("beginningChunk", this);
    _valueTreeState.addParameterListener("endingChunk", this);
    int lowerPosition = _valueTreeState.getParameter("beginningChunk")->getValue();
    int upperPosition = _valueTreeState.getParameter("endingChunk")->getValue();
    playTime->setMinAndMaxValues(lowerPosition, upperPosition);


	// Create buttons
	for (int i = 0; i < *time; ++i)
	{
		auto* tb = addToList(new TextButton("Sec " + String(i + 1)));
		tb->setRadioGroupId(TIME_SELECT_BUTTON);
		tb->setClickingTogglesState(false);
		tb->setComponentID(String(i));
	}
}

bigTime::~bigTime()
{
    valueTreeState.removeParameterListener("beginningChunk",this);
    valueTreeState.removeParameterListener("endingChunk",this);
    delete playTime;
}
//==============================================================================

//==============================================================================
//   Function paint
//==============================================================================
void bigTime::paint(Graphics& g)
{

	int btnWidth = (int)(getWidth() / (*time));

	Rectangle<float> BackGround(0, 0, 10 * btnWidth, getHeight());
	g.setColour(findColour(SoundshapeLAFs::base1ID));
	g.fillRect(BackGround);


	// Fills background color of selected time domain
	g.setColour(findColour(SoundshapeLAFs::background3ID));
	Rectangle<float> selected(*xPoint * btnWidth, 0.0f, getWidth()/(*time), getHeight() * .80f );
	g.fillRect(selected);
	

	for (int i = 0; i < components.size(); i++)
	{
		components[i]->setBounds(btnWidth * i, getHeight() - (getHeight() * .20f), btnWidth, getHeight() * .20f);
		components[i]->setConnectedEdges(((i != -1) ? Button::ConnectedOnLeft : 0)
			| ((i != -1) ? Button::ConnectedOnRight : 0));
	}

	// Set bounds and location for the play time slider
	//g.setColour(Colour(SoundshapeLAFs::background1ID));
    playTime->setBounds(timeBase.getX() - ((getHeight() * .15f) / 2.0f), (timeBase.getHeight() / 2.0f) - ((getHeight() * .1f) / 2.0f), (getWidth() + ((getHeight() * .15f) / 2.0f)), getHeight() * .1f);

}
//==============================================================================

//==============================================================================
//	Function repaint
//==============================================================================
void bigTime::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..

	// Set viewport bounds and locations
	int btnWidth = (int)(getWidth() / *time);
	Rectangle<float> rec(btnWidth, getHeight() * .20f);
	view.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
	timeBase.setSize(getWidth() - (getWidth()- (btnWidth* (*time))), getHeight() - rec.getHeight());

}
//==============================================================================


//==============================================================================
//  Function passes references from the parent to varibles in bigTime
//==============================================================================
void bigTime::setProfile(int * _Xpoint, int * _profile, int * _time, Button::Listener * _parent, Slider::Listener* _sliderParent, Converter* _cp)
{
	xProfile = _profile;
	xPoint = _Xpoint;
	time = _time;
	parent = _parent;	
	sliderParent = _sliderParent;

	playTime->addListener(sliderParent);
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->addListener(parent);
	}
	timeBase.setConverter(_cp);
	timeBase.setCurrentProfile(xProfile);
	timeBase.repaint();
}
//==============================================================================



void bigTime::parameterChanged(const String & parameterID, float newValue)
{
    // needs to repsond if the backend changes the position of the double-sided play range selector
    // (this can happen if the range is changed by the DAW or when a preset is loaded)

    // sliders shouldnt listen to a change they make to themselves
    
    valueTreeState.removeParameterListener("beginningChunk", this);
    valueTreeState.removeParameterListener("endingChunk", this);

    if (parameterID == "endingChunk") {
        // we we can't make maxvalue go below minvalue directly, so manually puill it down
        if ((int)newValue <= (int)playTime->getMinValue()) {
            playTime->setMinValue((int)newValue);
        }
        playTime->setMaxValue((int)newValue);
    }

    if (parameterID == "beginningChunk") {
        // we we can't make minvalue exceed maxvalue directly, so manually push maxvalue of necessary
        if ((int)newValue >= (int)playTime->getMaxValue()) {
            playTime->setMaxValue((int)newValue);
        }
        playTime->setMinValue((int)newValue);
    }


    valueTreeState.addParameterListener("beginningChunk", this);
    valueTreeState.addParameterListener("endingChunk", this);
}
