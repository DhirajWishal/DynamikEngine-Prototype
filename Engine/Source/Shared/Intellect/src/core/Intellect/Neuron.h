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
			Neuron() {}	// default constructor
			Neuron(UI32 index) : myIndex(index) {}	// default constructor
			virtual ~Neuron() {}	// default virtual destructor

			virtual F32 process(std::vector<Neuron*> neurons) { return myActivation; }	// virtual process function

			UI32 myIndex = 0;	// neuron index
			UI32 myLayerNumber = 0;	// neuron layer number

			F32 myActivation = 0.0f;	// neuron activation
			F32 myWeight = 0.0f;	// neuron activation weight
			F32 myBias = 0.0f;	// neuron activation bias
		};
	}
}

#endif // !_DYNAMIK_INTELLECT_NEURON_H
