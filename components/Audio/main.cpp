#include "Audio.h"
#include <src/core/log.h>
#include <windows.h>

void fakeupdate() {
	using namespace Dynamik::Audio;
	AudioEngine engine = AudioEngine();
	//BackgroundAudioController contro("media/ophelia.mp3");
	//engine.addAudioController(contro);

	BasicAudioController contro2("media/explosion.wav");
	contro2.isLooped = true;

	while (true) {
		engine.update();
		char x;
		std::cin >> x;
		if (x == 'b') {
			//contro.isPaused = !contro.isPaused;
		}
		else if (x == 'g') {
			engine.addAudioController(contro2);
		}
	}
}

class AA {
public:
	int b = 0;
	static AA& getInstance() {
		static AA a;
		a.b++;
		return a;
	}
};

int main() {
	//while (true) std::cout << AA::getInstance().b;
	fakeupdate();
	return 0;
}