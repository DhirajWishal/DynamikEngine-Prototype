#include <Dynamik.h>

class Engine : public Dynamik::Application {
public:
	Engine() {

	}

	~Engine() {

	}
};

Dynamik::Application* Dynamik::createApplication() {
	return new Engine();
}