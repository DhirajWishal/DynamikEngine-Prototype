#pragma once

/*
 Main Neuron base class for the Intellect Neural Network

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		05/03/2020
 IDE:		MS Visual Studio Community 2019

 Neuron.h file
*/

#ifndef _DYNAMIK_INTELLECT_NEURON_H
#define _DYNAMIK_INTELLECT_NEURON_H

#include "CentralDataHub.h"
#include "Platform.h"

namespace Dynamik {
	namespace Intellect {
		/*
		 BASE NEURON CLASS FOR THE INTELLECT NEURAL NETWORK
		*/
		class DMK_API Neuron {
		public:
			Neuron() {}
			Neuron(UI32 index) : myIndex(index) {}
			virtual ~Neuron() {}

			virtual F32 process(std::vector<Neuron*> neurons) { return myActivation; }

			UI32 myIndex = 0;
			UI32 myLayerNumber = 0;

			F32 myActivation = 0.0f;
			F32 myWeight = 0.0f;
			F32 myBias = 0.0f;
		};
	}
}

#endif // !_DYNAMIK_INTELLECT_NEURON_H
