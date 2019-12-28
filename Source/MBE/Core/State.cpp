#include <MBE/Core/State.h>
#include <MBE/Core/StateManager.h>

using namespace mbe;

State::State(StateManager & stateManager, Context& context) :
	stateManager(stateManager),
	context(context)
{
}

State::Context::Context(std::unique_ptr<sf::RenderWindow> windowPtr, TextureWrapperHolder<std::string> & textures, SoundBufferHolder<std::string> & sounds, FontHolder<std::string> & fonts, FilePathDictionary<std::string> & musicFilePaths) :
	windowPtr(std::move(windowPtr)),
	textureWrappers(textures),
	sounds(sounds),
	fonts(fonts),
	musicFilePaths(musicFilePaths)
{
}
