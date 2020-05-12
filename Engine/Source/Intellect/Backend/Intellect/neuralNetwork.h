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
		class NeuralNetwork {
		public:
			NeuralNetwork();
			NeuralNetwork(std::vector<std::vector<Neuron*>> neuralLayers);
			~NeuralNetwork();

			void addNeuron(Neuron* neuron, UI32 layerNumber = 0);
			void addNeurons(std::vector<Neuron*> neurons, UI32 layerNumber = 0);
			void setNeurons(std::vector<Neuron*> neurons, UI32 layerNumber = 0);
			void addNeuronLayer(std::vector<Neuron*> neurons);
			void setNeuronLayers(std::vector<std::vector<Neuron*>> neuralLayers);

			NeuralNetwork& operator=(std::vector<std::vector<Neuron*>> neuralLayers);

		private:
			std::vector<std::vector<Neuron*>> myNeuralLayers = {};
			F32 myActivationRangeMin = 0.0f;
			F32 myActivationRangeMax = 1.0f;
			UI32 myNeuronCount = 0;
			UI32 myLayerCount = 0;
		};
	}
}

#endif // !_DYNAMIK_INTELLECT_NEURAL_NETWORK_H
