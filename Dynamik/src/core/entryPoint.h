#pragma once
/*
 Dynamik Engine (Pre-build)
 Entry Point

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019
*/


#ifdef DMK_PLATFORM_WINDOWS
extern Dynamik::Application* Dynamik::createApplication();

int main(int argc, char** argv) {
	printf("Welcome to the Dynamik Engine!\n");

	Dynamik::GameObject obj("NONE", "NONE", 0.0f, 0.0f, 0.0f);

	DMK_FATAL("Hello World");
	DMK_CORE_ERROR("NULL_PTR");

	int x = Dynamik::utils::max(10, 20);
	DMK_INFO((const char*)x);

	Dynamik::utils::minHeap<int> myHeap(100);

	myHeap.insert(100);
	myHeap.insert(200);
	myHeap.insert(1);

	auto application = Dynamik::createApplication();
	application->run();

	delete application;
}

#endif