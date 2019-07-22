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
#include <fstream>

#include "core/core.h"
#include "core/utils/DMK_Error.h"
#include "core/utils/DMK_DataTypes.h"

// Dynamik namespace
namespace Dynamik {

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
		None
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
	class DMK_API GameObject {

		// Transform Component
		struct Transform {
			enum COORDINATES {
				X, Y, Z
			};

			float myPosition[3] = { DEFAULT_POSITION_X, DEFAULT_POSITION_Y, DEFAULT_POSITION_Z };
			float myRotation[3] = { DEFAULT_ROTATION_X, DEFAULT_ROTATION_Y, DEFAULT_ROTATION_Z };
			float myScale[3] = { DEFAULT_SCALE_X, DEFAULT_SCALE_Y, DEFAULT_SCALE_Z };
		};

		// Camera Component
		struct Camera {
			enum COLOR {
				RED, GREEN,
				BLUE, ALPHA
			};

			uint8 flags = Skybox;
			float background[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// r, g, b, a
			uint8 mask = Everything;
			uint8 projection = Perspective;
			int FOV = DEFAULT_FOV;

			float depth = 1.0f;

			bool isHDR = false;
			bool isOcclusionCulling = true;
		};

		// GUI Layer Component
		struct Lighting {

		};

		// Flare Layer Component
		struct Flare_Layer {

		};

		// Audio Component
		struct Audio {
			std::string clipPath;

			uint8 output = None;
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
		};

	public:
		/* ---------- ########## \\\\\\\\\\ CONSTRUCTOR AND DESTRUCTOR ////////// ########## ---------- */
		inline GameObject(std::string name, std::string path, float x, float y, float z);
		inline GameObject(const char* name, const  char* path, float x, float y, float z);

		inline ~GameObject();

		/* ---------- ########## \\\\\\\\\\ MAIN FUNCTIONS ////////// ########## ---------- */
		inline void init();
		inline void draw();

		/* ---------- ########## \\\\\\\\\\ BIND THE GAME OBJECT ////////// ########## ---------- */
		inline void bindGameObject();
		inline void unbindGameObject();

		/* ---------- ########## \\\\\\\\\\ TRANSFORM ////////// ########## ---------- */
		inline void initTransform(float* pos[3], float* rot[3], float* scl[3]);
		inline void setTransformPosition(float* x, float* y, float* z);
		inline void setTransformRotation(float* x, float* y, float* z);
		inline void setTransformScale(float* x, float* y, float* z);

		/* ---------- ########## \\\\\\\\\\ CAMERA ////////// ########## ---------- */
		inline void initCamera(uint8* flg, float* bgcl[4],
			uint8* msk, uint8* prj, int* fov, float* dpth, bool* hdr, bool* oc);
		inline void setCameraFlags(uint8* flg);
		inline void setCameraBackgroundColor(float* red, float* green, float* blue, float* alpha);
		inline void setCameraMask(uint8* msk);
		inline void setCameraProjection(uint8* prj);
		inline void setCameraFOV(int* fov);
		inline void setCameraDepth(float* dpth);
		inline void setCameraHDR(bool* hdr);
		inline void setCameraOcclusionCulling(bool* oc);

		/* ---------- ########## \\\\\\\\\\ AUDIO ////////// ########## ---------- */
		inline void initAudio(std::string& path, uint8* out, uint8* pri, uint8* vol,
			uint8* pit, int8* sp, bool* mute, bool* be, bool* ble, bool* brz, bool* poa, bool* loop);
		inline void setAudioClipPath(std::string& path);
		inline void setAudioOutput(uint8* out);
		inline void setAudioPriority(uint8* pri);
		inline void setAudioVolume(uint8* vol);
		inline void setAudioPitch(uint8* pit);
		inline void setAudioStereoPan(int8* sp);
		inline void setAudioMute(bool* mute);
		inline void setAudioBypassEffects(bool* be);
		inline void setAudioBypassListnerEffects(bool* ble);
		inline void setAudioBypassReverbZones(bool* brz);
		inline void setAudioPlayOnAwake(bool* poa);
		inline void setLoop(bool* loop);

		/* ---------- ########## \\\\\\\\\\ CLEAR/ DEFAULT ////////// ########## ---------- */
		inline void clear();

		/* !DYNAMIK ENGINE SIDED! */
#if defined(DYNAMIK_ENGINE) || defined(TEST_BUILD) || defined(ENGINE_API) || defined(GRAPHICS_API)
		std::vector<float> getObjectVertex() {
			return { 0.0f };
		}

		// ----------
#endif

	private:
		// object name
		std::string myName = "new GameObject";

		// object
		std::fstream objFile;

		// allocating the structs in the heap
		Transform* TRFM = new Transform;
		Camera* CMR = new Camera;
		Lighting* LGHT = new Lighting;
		Flare_Layer* FLARE = new Flare_Layer;
		Audio* AUDIO = new Audio;
	};
}
