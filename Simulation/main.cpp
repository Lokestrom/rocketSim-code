#include "run.hpp"
#include "helpers/controles.hpp"
#include "FileSystem/fileSystem.hpp"

#include "FileSystem/LoadManager.hpp"
#include "FileSystem/RocketInstructions.hpp"
#include "FileSystem/logging.hpp"

#include "Vulkan/App.hpp"

int main()
{
	windows::createWindowData();
	CharacterGlyphCache::setFontFolder("C:\\Windows\\Fonts\\");
	CharacterGlyphCache::setFont("arial");

	run();

	return 0;
}