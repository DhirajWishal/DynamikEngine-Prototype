#pragma once

#include <Level.h>
using namespace Dynamik;

class moon : public GameObject {
public:
	moon(GameObjectProperties& props) :GameObject(props) {
		myProperties.name = "Moon";
		myProperties.location = "E:/Projects/Dynamik Engine/Game Repository/assets/assets/moon";
		myProperties.transformProperties.location = { 0.0f,	0.0f,	0.0f };
	}

	void update(Event& currentEvent) override;
};

class sceneOne : public Scene {
public:
	sceneOne(std::vector<GameObject*> objects) : Scene(objects) {}
	~sceneOne() {}
};

class vertexDataContainer : public Dynamik::VertexDataContainer {
	struct vertex : public Vertex {
		glm::vec3 Position;
		glm::vec3 Color;
		glm::vec3 Normals;
		glm::vec2 TexCoordinates;
	} vertexData;

	void setVertexData(DMKObjectVertexDataType dataType, std::vector<float> data) override;

	std::vector<DMKVertexInputBindingDescription> getVertexInputBindingDescription(int bindCount) override;

	std::vector<DMKVertexInputAttributeDescription> getVertexInputAttributeDescription() override;
};

struct uniformBufferObject : public Uniform {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
	alignas(16) glm::vec3 color;
};

class mars : public GameObject {
public:
	mars();
	mars(GameObjectProperties& props) : GameObject(props) {
		myProperties.location = "E:/Projects/Dynamik Engine/Game Repository/assets/assets/mars";
		myProperties.name = "Mars";
	}

	DMKUpdateInfo draw(std::deque<DMKEventContainer>& eventContainers) override;
	void init() override;
};

class explosion : public GameObject {
public:
	explosion() {}
	explosion(GameObjectProperties& props) : GameObject(props) {}
};
