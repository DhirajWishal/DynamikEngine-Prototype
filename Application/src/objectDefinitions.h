#pragma once

#include <Level.h>
using namespace Dynamik;

class moon : public GameObject {
public:
	moon(GameObjectProperties& props) :GameObject(props) {}

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

		bool operator==(const vertex& other);
	};

	std::vector<DMKVertexInputBindingDescription> getVertexInputBindingDescription(int bindCount) override;

	std::vector<DMKVertexInputAttributeDescription> getVertexInputAttributeDescription() override;
};

struct uniformBufferObject : public Uniform {
	alignas(16) glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 color;
};

class mars : public GameObject {
public:
	mars() {}
	mars(GameObjectProperties& props) : GameObject(props) {}

	DMKUpdateInfo draw(std::deque<DMKEventContainer>& eventContainers) override;

	//vertexDataType = &vertexDataContainer();
};

class explosion : public GameObject {
public:
	explosion() {}
	explosion(GameObjectProperties& props) : GameObject(props) {}
};
