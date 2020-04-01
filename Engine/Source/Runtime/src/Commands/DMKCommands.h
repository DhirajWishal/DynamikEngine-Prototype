#pragma once
#ifndef _DYNAMIK_COMMANDS_H
#define _DYNAMIK_COMMANDS_H

/*
 Commands API for the Dynamik Engine.
 Client Application can use these commands to request, submit and update commands or resource in the Engine.

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Data:		31/3/2020
 IDE:		MS Visual Studio Community 2019
*/

#include "DMKCommand.h"
#include "DMKCommandQueue.h"
#include "DMKDescriptor.h"

#include <string>

namespace Dynamik {
	class DMKSubmitObjectCommand : public DMKCommand {
	public:
		DMKCommandType type = DMKCommandType::DMK_COMMAND_TYPE_SUBMIT_OBJECT;

		ARRAY<std::string> objectPaths;
		ARRAY<std::string> texturePaths;

		std::string vertexShaderPath;
		std::string tessellationShaderPath;
		std::string geometryShaderPath;
		std::string fragmentShaderPath;
	};

	/* SINGLETON 
	 * Dynamik Commands
	 */
	class DMKCommands {
		DMKCommands() {}
		DMKCommands(const DMKCommands&) = delete;
		DMKCommands& operator=(const DMKCommands&) = delete;

		static DMKCommands& getInstance();

	public:
		~DMKCommands() {}

		static COMPTR getCommandQueuePointer();
		static void submitObject(COMPTR pointer, DMKSubmitObjectCommand descriptor);

	private:
		COMPTR _getCommandQueuePointer();
	};
}

#endif // !_DYNAMIK_COMMANDS_H
