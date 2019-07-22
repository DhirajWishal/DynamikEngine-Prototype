#pragma once
/*
 Dynamik Engine (Pre-build)
 Entry Point
*/


#ifdef DMK_PLATFORM_WINDOWS
extern Dynamik::Application* Dynamik::createApplication();

int main(int argc, char** argv) {
	printf("Welcome to the Dynamik Engine!\n");

	Dynamik::GameObject obj("NONE", "NONE", 0.0f, 0.0f, 0.0f);

	DMK_FATAL("Hello World");
	DMK_CORE_ERROR("NULL_PTR");

	auto application = Dynamik::createApplication();
	application->run();

	delete application;
}

#endif