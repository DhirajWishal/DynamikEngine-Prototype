#pragma once

#ifndef _DYNAMIK_GAME_OBJECT_H
#define _DYNAMIK_GAME_OBJECT_H

/*
 GameObject data type for Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		19/06/2019	(DEVELOPMENTS ONGOING)
 IDE:		MS Visual Studio Community 2019

 GameObject.h file.
*/

/* MAIN INCLUDES */
#include <deque>

#include <Data Types/Uniform.h>
#include <Data Types/Vertex.h>
#include <core/data store/containers.h>
#include <core/data store/objectSpecifications.h>

#include "core/core.h"
#include "core/utils/DMK_Error.h"
#include "CentralDataHub.h"
#include "event.h"

#include "KeyBingings.h"

#include "core/log.h"

// Dynamik namespace
namespace Dynamik {
	struct VertexContainer {

	};

	// Renderable object properties
	struct RenderableObjectProperties {
		std::string vertexShaderPath = "NONE";
		std::string tessellationShaderPath = "NONE";
		std::string geometryShaderPath = "NONE";
		std::string fragmentShaderPath = "NONE";

		std::string scriptPath = "NONE";
		std::string codePath = "NONE";
	};

	// Transform
	struct TransformProperties {
		std::vector<float> location = { 0.0f, 0.0f, 0.0f };
		std::vector<float> rotation = { 0.0f, 0.0f, 0.0f };
		std::vector<float> scale = { 1.0f, 1.0f, 1.0f };
		float movementSpeed = 1.0f;
		float rotationSpeed = 2.5f;
	};

	// Audio
	struct AudioProperties {
		DMKAudioOutputOptions outputOptions = DMKAudioOutputOptions::DMK_AUDIO_OUTPUT_DEFAULT;
		DMKAudioPriorityOptions priority = DMKAudioPriorityOptions::DMK_AUDIO_PRIORITY_DEFAULT;
		uint32_t volume = 100;
		uint32_t pitch = 1;
		int32_t stereoPan = 0;

		bool isMute = false;
		bool isBypassEffects = false;
		bool isBypassListnerEffects = false;
		bool isBypassReverbZones = false;
		bool isPlayOnAwake = true;
		bool loop = false;

		bool __pad[2];
	};

	// Video
	struct VideoProperties {
		uint32_t width = 0;
		uint32_t height = 0;
	};

	// Camera
	struct CameraProperties {
		DMKCameraType type = DMKCameraType::DMK_CAMERA_TYPE_DEFAULT;
		float background[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// r, g, b, a
		UI32 FOV = 120;
		float depth = 1.0f;
		bool isHDR = false;
		bool isOcclusionCulling = true;

		bool __pad[2];
	};

	// Lighting
	struct LightingProperties {
		DMKLightingType type = DMKLightingType::DMK_LIGHTING_TYPE_SPECULAR;
		std::vector<float> color = { 0.0f, 0.0f, 0.0f, 1.0f };	// r, g, b, a
	};

	// Game Object Properties
	struct GameObjectProperties {
		GameObjectProperties() {}
		~GameObjectProperties() {}

		// Object info:
		std::string name = "New Object";
		std::string ID = "0000";
		uint32_t instanceCount = 1;

		// Location data:
		std::string location = "";

		std::vector<std::string> objectPath = {};
		std::vector<std::string> texturePaths = {};
#ifdef DMK_BEHAVIOUR_SCRIPTING
		std::string behaviourPath = "";
#endif

		// Object characteristics:
		DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT;
		DMKObjectDestructionType destructionType = DMKObjectDestructionType::DMK_OBJECT_DESTRUCTION_NONE;
		DMKObjectDestructorType destructorType = DMKObjectDestructorType::DMK_OBJECT_DESTRUCTOR_NONE;
		DMKObjectBehaviour behaviourType = DMKObjectBehaviour::DMK_OBJECT_BEHAVIOUR_NONE;

		// Object data:
		TransformProperties transformProperties = {};
		AudioProperties audioProperties = {};
		CameraProperties cameraProperties = {};
		LightingProperties ligitingProperties = {};

		RenderableObjectProperties renderableObjectProperties = {};
	};

	// Destructor: Projectile
	struct ProjectileProperties {
		GameObjectProperties objectProps = {};

		std::string name = "";
		std::string playerID = "";

		float initialVelocity = 0.0f;
		float acceleration = 0.0f;
		float deceleration = 0.0f;
		float currentVelocity = 0.0f;
		float weight = 0.0f;
		float size = 1.0f;
	};

	struct DMKVertexInputBindingDescription {
		uint32_t binding = 0;
		uint32_t stride = 0;
	};

	struct DMKVertexInputAttributeDescription {
		uint32_t binding = 0;
		uint32_t location = 0;
		DMKFormat format;
		uint32_t offset = 0;
	};

	// vertex Data
	class VertexDataContainer {
	public:
		struct Vertex {};

		virtual void setVertexData(DMKObjectVertexDataType dataType, std::vector<float> data) {}
		virtual void setVertexDataF(DMKObjectVertexDataType& dataType, std::vector<float>& data) {}
		virtual void setVertexDataI(DMKObjectVertexDataType& dataType, std::vector<int32_t>& data) {}
		virtual void setVertexDataUI(DMKObjectVertexDataType& dataType, std::vector<uint32_t>& data) {}

		virtual std::vector<DMKVertexInputBindingDescription> getVertexInputBindingDescription(int bindCount) {
			return std::vector<DMKVertexInputBindingDescription>();
		}
		virtual std::vector<DMKVertexInputAttributeDescription> getVertexInputAttributeDescription() {
			return std::vector<DMKVertexInputAttributeDescription>();
		}
	};

	/*
		 GameObject definition.
		 GameObject class can be used to send and get data from the Rendering API specifically designed
		 for the Dynamik Engine for fast and easy deployment.
		 - Position Manipulation
		 - Rotation Manipulation
		 - Scale/ Size Manipulation
		 - Camera Manipulation
		 - Audio Control
		 - Lighting
		 - Upload and Retrieval of data
		 - Drawing of the selected file/ object
		 * STACK ALLOCATION IS OPTIONAL YET RECOMMENDED *
		 * PASS ALL VALUES BY REFERENCE (MEMORY AND CODE OPTIMIZATION) *
		 - Asset files must be inside its own folder with a "modelData.dai" file stating the model and textures
		 - In that file, models are stated as "M model.obj" and textures as "I texture.jpeg"
	*/
	class GameObject {
	public:
		/* ---------- ########## \\\\\\\\\\ CONSTRUCTOR AND DESTRUCTOR ////////// ########## ---------- */
		GameObject() {}
		GameObject(GameObjectProperties& props) : myProperties(props) {}
		virtual ~GameObject() {}

#ifdef DMK_CORE
		GameObjectProperties getProperties() { return myProperties; }

#endif

		/* ---------- ########## \\\\\\\\\\ MAIN FUNCTIONS ////////// ########## ---------- */
		virtual void init() {}
		virtual DMKUpdateInfo draw(std::deque<DMKEventContainer>& eventContainers) { return DMKUpdateInfo(); }

		virtual void update(Event& currentEvent) {}
		virtual void update(DMKUpdateInfo& info) {}

		virtual void logic() {}

		/* ---------- ########## \\\\\\\\\\ TRANSFORM ////////// ########## ---------- */
		virtual void initTransform() {}
		virtual void setTransform(TransformProperties& properties) {}
		virtual void setTransformPosition(float* x, float* y, float* z) {}
		virtual void setTransformRotation(float* x, float* y, float* z) {}
		virtual void setTransformScale(float* x, float* y, float* z) {}

		/* ---------- ########## \\\\\\\\\\ BIND THE TEXTURES ////////// ########## ---------- */
		virtual void loadTexture() {}
		virtual void deleteTexture() {}

		/* ---------- ########## \\\\\\\\\\ BIND THE GAME OBJECT ////////// ########## ---------- */
		virtual void loadGameObject() {}
		virtual void deleteGameObject() {}

		/* ---------- ########## \\\\\\\\\\ DESTRUCTION/ DESTRUCTOR ////////// ########## ---------- */
		virtual void onLoad() {}
		virtual void onActivation(std::string& playerID, GameObjectProperties& objectProps) {}
		virtual void inAir() {}
		virtual void onImpact(GameObjectProperties& objectProps) {}

		/* ---------- ########## \\\\\\\\\\ CAMERA ////////// ########## ---------- */
		virtual void initCamera() {}
		virtual void initCamera(CameraProperties& props) {}

		/* ---------- ########## \\\\\\\\\\ AUDIO ////////// ########## ---------- */
		virtual void initAudio() {}
		virtual void initAudio(AudioProperties& props) {}

		/* ---------- ########## \\\\\\\\\\ VIDEO ////////// ########## ---------- */
		virtual void initVideo() {}
		virtual void initVideo(VideoProperties& props) {}

		/* ---------- ########## \\\\\\\\\\ CLEAR/ DEFAULT ////////// ########## ---------- */
		virtual void clear() {}

	public:
		// object properties
		GameObjectProperties myProperties = {};

		// Renderable Objects:
		DMKUpdateInfo myUpdateProperties = {};

		// Data Types
		VertexDataContainer myVertexDataContainer = {};
		Uniform* uniformDataType = nullptr;

		//typeid(uniformDataType) == 
	};
}

#endif // !_DYNAMIK_GAME_OBJECT_H
