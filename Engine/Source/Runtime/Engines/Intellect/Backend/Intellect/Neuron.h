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




namespace Dynamik {
	namespace Intellect {
		class Neuron;

		/*
		 MAIN NEURAL CONNECTION STRUCTURE
		 This structure contains the necessary data needed for a connection between two or more
		 neurons.
		*/
		struct DMK_API NeuralConnection {
			Neuron* myNeuron = nullptr;	// neuron
			F32 myWeight = 0.0f;	// neuron activation weight
			F32 myBias = 0.0f;	// neuron activation bias
		};

		/*
		 BASE NEURON CLASS FOR THE INTELLECT NEURAL NETWORK
		*/
		class DMK_API Neuron {
		public:
			Neuron() {}	// default constructor
			Neuron(UI32 layer, UI32 index) : myLayerNumber(layer), myIndex(index) {}	// default constructor
			virtual ~Neuron() {}	// default virtual destructor

			virtual F32 process(ARRAY<NeuralConnection*> connections)	// virtual process function
			{
				myConnections = connections;
				return myActivation;
			}

			UI32 myIndex = 0;	// neuron index
			UI32 myLayerNumber = 0;	// neuron layer number

			F32 myActivation = 0.0f;	// neuron activation
			ARRAY<NeuralConnection*> myConnections = {};
		};
	}
}

#endif // !_DYNAMIK_INTELLECT_NEURON_H
