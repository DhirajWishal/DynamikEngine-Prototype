#pragma once

/*
 GameObject data type for Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		19/06/2019	(DEVELOPMENTS ONGOING)
 IDE:		MS Visual Studio Community 2019

 GameObject.h file.
*/

/* MAIN INCLUDES */
#include "core/core.h"
#include "core/utils/DMK_Error.h"
#include "CentralDataHub.h"

#include "core/log.h"

// Dynamik namespace
namespace Dynamik {
	using namespace utils;
	/* DEFINES AND MACROS (LOCAL/ GLOBAL) */
	/* DEFAULTS */
#define DEFAULT_POSITION_X 0.0f
#define DEFAULT_POSITION_Y 0.0f
#define DEFAULT_POSITION_Z 0.0f
#define DEFAULT_ROTATION_X 0.0f
#define DEFAULT_ROTATION_Y 0.0f
#define DEFAULT_ROTATION_Z 0.0f
#define DEFAULT_SCALE_X 1.0f
#define DEFAULT_SCALE_Y 1.0f
#define DEFAULT_SCALE_Z 1.0f

#define DEFAULT_FOV 60

#define DEFAULT_AUDIO_PRIORITY 128
#define DEFAULT_AUDIO_VOLUME 128
#define DEFAULT_AUDIO_PITCH 1

/* VARIABLES AND CONSTANTS (LOCAL/ GLOBAL) */
	// Transform Component
	enum objectType {
		TEXTURE,
		MODEL
	};

	enum mipLevels {
		DEFAULT,
		DYNAMIC
	};

	enum behaviour {
		PHYSICAL_BODY,
		VIRTUAL_BODY,
		PLAYER,
		ENEMY,
		ENTITY
	};

	enum CAM_FLAGS {
		Skybox
	};

	enum CAM_MASK {
		Everything
	};

	enum CAM_PROJECTION {
		Perspective
	};

	enum AUD_OUTPUT {
		NONE
	};

	struct TransformProperties {
		enum COORDINATES {
			X, Y, Z
		};

		float myPosition[3] = { DEFAULT_POSITION_X, DEFAULT_POSITION_Y, DEFAULT_POSITION_Z };
		float myRotation[3] = { DEFAULT_ROTATION_X, DEFAULT_ROTATION_Y, DEFAULT_ROTATION_Z };
		float myScale[3] = { DEFAULT_SCALE_X, DEFAULT_SCALE_Y, DEFAULT_SCALE_Z };

		float movementSpeed = 1.0f;
	};

	struct CameraProperties {
		enum COLOR {
			RED, GREEN,
			BLUE, ALPHA
		};

		uint8 flags = Skybox;
		float background[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// r, g, b, a
		uint8 mask = Everything;
		uint8 projection = Perspective;
		uint32 FOV = DEFAULT_FOV;

		float depth = 1.0f;

		bool isHDR = false;
		bool isOcclusionCulling = true;

		bool __pad[3];
	};

	struct LightingProperties {
	};

	struct Flare_Layer {
	};

	struct AudioProperties {
		std::string clipPath;

		uint8 output = NONE;
		uint8 priority = DEFAULT_AUDIO_PRIORITY;
		uint8 volume = DEFAULT_AUDIO_VOLUME;
		uint8 pitch = DEFAULT_AUDIO_PITCH;
		int8 stereoPan = 0;

		bool isMute = false;
		bool isBypassEffects = false;
		bool isBypassListnerEffects = false;
		bool isBypassReverbZones = false;
		bool isPlayOnAwake = true;
		bool loop = false;

		bool __pad[1];
	};

	struct BehaviourProperties {
		behaviour type = VIRTUAL_BODY;
	};

	struct GameObjectProperties {
		std::string name, ID;
		objectType type;
		std::string objPath, texPath;

		TransformProperties transformProperties;
		CameraProperties cameraProperties;
		LightingProperties ligitingProperties;
		AudioProperties audioProperties;

		BehaviourProperties behaviourProperties;
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
	 * HEAP ALLOCATION IS OPTIONAL YET RECOMMENDED *
	 * PASS ALL VALUES BY REFERENCE (MEMORY AND CODE OPTIMIZATION) *
	*/
	class GameObject {
	public:
		/* ---------- ########## \\\\\\\\\\ CONSTRUCTOR AND DESTRUCTOR ////////// ########## ---------- */
		GameObject(GameObjectProperties props) : myProperties(props) {}
		virtual ~GameObject() {}

#ifdef DMK_CORE

		GameObjectProperties getProperties() { return myProperties; }

#endif

	protected:
		/* ---------- ########## \\\\\\\\\\ MAIN FUNCTIONS ////////// ########## ---------- */
		virtual void init() {}
		virtual void draw() {}

		virtual void update() {}

		virtual void logic() {}

		/* ---------- ########## \\\\\\\\\\ BIND THE GAME OBJECT ////////// ########## ---------- */
		virtual void bindGameObject() {}
		virtual void unbindGameObject() {}

		/* ---------- ########## \\\\\\\\\\ BIND THE TEXTURES ////////// ########## ---------- */
		virtual void bindTexture() {}
		virtual void unbindTexture() {}

		/* ---------- ########## \\\\\\\\\\ TRANSFORM ////////// ########## ---------- */
		virtual void initTransform() {}
		virtual void initTransform(TransformProperties props) {}
		virtual void setTransformPosition(float* x, float* y, float* z) {}
		virtual void setTransformRotation(float* x, float* y, float* z) {}
		virtual void setTransformScale(float* x, float* y, float* z) {}

		/* ---------- ########## \\\\\\\\\\ CAMERA ////////// ########## ---------- */
		virtual void initCamera() {}
		virtual void initCamera(CameraProperties props) {}

		/* ---------- ########## \\\\\\\\\\ AUDIO ////////// ########## ---------- */
		virtual void initAudio() {}
		virtual void initAudio(AudioProperties props) {}

		/* ---------- ########## \\\\\\\\\\ CLEAR/ DEFAULT ////////// ########## ---------- */
		virtual void clear() {}

		// object properties
		GameObjectProperties myProperties;
	};
}
