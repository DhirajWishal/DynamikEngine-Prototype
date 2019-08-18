#include "Audio.h"
#include <iostream>

#include <irrKlang.h>

int main() {
	auto audioCore = Dynamik::audio::AudioCore();
	using namespace Dynamik::audio;
	//auto a1 = LoopingAudio("fuck you 1", 0);

	std::cout << "Hello World Audio" << std::endl;

	
	
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

	if (!engine)
		return 0; // error starting up the engine

	  // play some sound stream, looped
	engine->play2D("somefile.mp3", true);

	char i = 0;
	std::cin >> i; // wait for user to press a key

	engine->drop(); // delete engine

	return 0;
}