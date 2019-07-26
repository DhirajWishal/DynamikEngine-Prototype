/*
 Main application file.

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019
*/

#include <Dynamik.h>
#include "src/imports.h"

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
