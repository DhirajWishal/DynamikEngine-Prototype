#include "dmkafx.h"
#include "neuralNetwork.h"

namespace Dynamik {
	namespace Intellect {
		NeuralNetwork::NeuralNetwork()
		{
		}

		NeuralNetwork::NeuralNetwork(ARRAY<ARRAY<Neuron*>> neuralLayers)
		{
			setNeuronLayers(neuralLayers);
		}

		NeuralNetwork::~NeuralNetwork()
		{
		}

		void NeuralNetwork::addNeuron(Neuron* neuron, UI32 layerNumber)
		{
			if (layerNumber >= myLayerCount)
				return;

			myNeuralLayers[layerNumber].push_back(neuron);
		}

		void NeuralNetwork::addNeurons(ARRAY<Neuron*> neurons, UI32 layerNumber)
		{
			if (layerNumber >= myLayerCount)
				return;

			for (Neuron* element : neurons) {
				element->myIndex = myNeuralLayers[layerNumber].size();
				element->myLayerNumber = layerNumber;
				myNeuralLayers[layerNumber].push_back(element);
			}
		}

		void NeuralNetwork::setNeurons(ARRAY<Neuron*> neurons, UI32 layerNumber)
		{
			if (layerNumber >= myLayerCount)
				return;

			myNeuralLayers[layerNumber].clear();
			for (Neuron* element : neurons) {
				element->myIndex = myNeuralLayers[layerNumber].size();
				element->myLayerNumber = layerNumber;
				myNeuralLayers[layerNumber].push_back(element);
			}
		}

		void NeuralNetwork::addNeuronLayer(ARRAY<Neuron*> neurons)
		{
			myNeuralLayers.push_back(neurons);
		}

		void NeuralNetwork::setNeuronLayers(ARRAY<ARRAY<Neuron*>> neuralLayers)
		{
			Neuron* _tempNeuron = nullptr;
			myNeuralLayers.resize(neuralLayers.size());
			for (UI32 _layer = 0; _layer < neuralLayers.size(); _layer++) {
				for (UI32 _index = 0; _index < neuralLayers[_layer].size(); _index++) {
					_tempNeuron = neuralLayers[_layer][_index];
					_tempNeuron->myIndex = _index;
					_tempNeuron->myLayerNumber = _layer;
					myNeuralLayers[_layer].push_back(_tempNeuron);
				}
			}
		}

		NeuralNetwork& NeuralNetwork::operator=(ARRAY<ARRAY<Neuron*>> neuralLayers)
		{
			this->setNeuronLayers(neuralLayers);
			return *this;
		}
	}
}