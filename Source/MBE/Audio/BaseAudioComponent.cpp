#include <MBE/Audio/BaseAudioComponent.h>

using namespace mbe;

BaseAudioComponent::BaseAudioComponent(EventManager & eventManager, Entity & parentEntity)
	: Component(eventManager, parentEntity)
{
	AudioData::IncrementNumberOfSounds();
}

BaseAudioComponent::~BaseAudioComponent()
{
	AudioData::DecrementNumberOfSounds();
}