#pragma once

/*
 Main Neural Network class for Intellect Library

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		05/03/2020
 IDE:		MS Visual Studio Community 2019

 neuralNetwork.h file
*/

#ifndef _DYNAMIK_INTELLECT_NEURAL_NETWORK_H
#define _DYNAMIK_INTELLECT_NEURAL_NETWORK_H

#include "Neuron.h"

namespace Dynamik {
	namespace Intellect {
		/*
		 MAIN NEURAL NETWORK CLASS
		*/
		class DMK_API NeuralNetwork {
		public:
			NeuralNetwork();
			NeuralNetwork(ARRAY<ARRAY<Neuron*>> neuralLayers);
			~NeuralNetwork();

			void addNeuron(Neuron* neuron, UI32 layerNumber = 0);
			void addNeurons(ARRAY<Neuron*> neurons, UI32 layerNumber = 0);
			void setNeurons(ARRAY<Neuron*> neurons, UI32 layerNumber = 0);
			void addNeuronLayer(ARRAY<Neuron*> neurons);
			void setNeuronLayers(ARRAY<ARRAY<Neuron*>> neuralLayers);

			NeuralNetwork& operator=(ARRAY<ARRAY<Neuron*>> neuralLayers);

		private:
			ARRAY<ARRAY<Neuron*>> myNeuralLayers = {};
			F32 myActivationRangeMin = 0.0f;
			F32 myActivationRangeMax = 1.0f;
			UI32 myNeuronCount = 0;
			UI32 myLayerCount = 0;
		};
	}
}

#endif // !_DYNAMIK_INTELLECT_NEURAL_NETWORK_H
