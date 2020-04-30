#pragma once

#ifndef _DYNAMIK_CENTRAL_DATA_HUB_OBJECT_SPECIFICATIONS_H
#define _DYNAMIK_CENTRAL_DATA_HUB_OBJECT_SPECIFICATIONS_H

namespace Dynamik {
	/* OBJECT SPECIFICATIONS */
	// Main object types:
	enum class DMKMainObjectTypes {
		DMK_RENDERABLE_OBJECT = 0,
		DMK_NONE_RENDERABLE_OBJECT = 1,
		DMK_SCRIPT_FILE = 2,
		DMK_AI_OBJECT = 3
	};

	// Object types:
	enum class DMKObjectType {
		DMK_OBJECT_TYPE_IMAGE_2D,
		DMK_OBJECT_TYPE_DEBUG_OBJECT,

		DMK_OBJECT_TYPE_MESH,					// per object ubo
		DMK_OBJECT_TYPE_STATIC,					// terrain ubo
		DMK_OBJECT_TYPE_INTERACTIVE_OBJECT,		// per object ubo
		DMK_OBJECT_TYPE_PLAYER,					// per object ubo
		DMK_OBJECT_TYPE_NPC,					// per object ubo
		DMK_OBJECT_TYPE_TEXTURE_UI,
		DMK_OBJECT_TYPE_SKYBOX,
		DMK_OBJECT_TYPE_SPRITES,
		DMK_OBJECT_TYPE_FONT,
		DMK_OBJECT_TYPE_TEXT_OVERLAY,
		DMK_OBJECT_TYPE_PARTICLE, //
		DMK_OBJECT_TYPE_DESTRUCTION,
		DMK_OBJECT_TYPE_DESTRUCTOR,
		DMK_OBJECT_TYPE_SKELETAL_ANIMATION,
		DMK_OBJECT_TYPE_LIGHT,
		DMK_OBJECT_TYPE_CAMERA,

		DMK_OBJECT_TYPE_AUDIO,
		DMK_OBJECT_TYPE_VIDEO,
		DMK_OBJECT_TYPE_WIND,
		DMK_OBJECT_TYPE_AI,
		DMK_OBJECT_TYPE_SCRIPT,
		DMK_OBJECT_TYPE_KEY_BINDINGS,

		DMK_OBJECT_TYPE_MAX_RENDERABLE_TYPES = DMK_OBJECT_TYPE_CAMERA,

		DMK_OBJECT_TYPE_CUSTOM
	};

	// Destruction types:
	enum class DMKObjectDestructionType {
		DMK_OBJECT_DESTRUCTION_NONE = 0,
		DMK_OBJECT_DESTRUCTION_SOLID = 1,
		DMK_OBJECT_DESTRUCTION_LIQUID = 2,
		DMK_OBJECT_DESTRUCTION_PLASTIC = 3,
		DMK_OBJECT_DESTRUCTION_FRAGILE = 4,
		DMK_OBJECT_DESTRUCTION_WOOD = 5,
		DMK_OBJECT_DESTRUCTION_BLOCKS = 6,
		DMK_OBJECT_DESTRUCTION_PAPER = 7,
		DMK_OBJECT_DESTRUCTION_CUSTOM = 8
	};

	// Destructor types:
	enum class DMKObjectDestructorType {
		DMK_OBJECT_DESTRUCTOR_NONE = 0,
		DMK_OBJECT_DESTRUCTOR_DEFAULT = 1,
		DMK_OBJECT_DESTRUCTOR_PROJECTILE = 2,
		DMK_OBJECT_DESTRUCTOR_EXPLOSIVE = 3,
		DMK_OBJECT_DESTRUCTOR_CUSTOM
	};

	// Object behaviours:
	enum class DMKObjectBehaviour {
		DMK_OBJECT_BEHAVIOUR_NONE,
		DMK_OBJECT_BEHAVIOUR_DEFAULT,
		DMK_OBJECT_BEHAVIOUR_SCRIPT,
		DMK_OBJECT_BEHAVIOUR_EXPLOSIVE,
		DMK_OBJECT_BEHAVIOUR_PROJECTILE,
		DMK_OBJECT_BEHAVIOUR_LIGHT
	};

	// Texture types:
	enum class DMKObjectTextureType {
		DMK_OBJECT_TEXTURE_TYPE_NONE = 0,
		DMK_OBJECT_TEXTURE_TYPE_2D_GREY = 1,
		DMK_OBJECT_TEXTURE_TYPE_2D_RGB = 2,
		DMK_OBJECT_TEXTURE_TYPE_2D_RGBA = 3,
		DMK_OBJECT_TEXTURE_TYPE_3D_GREY = 4,
		DMK_OBJECT_TEXTURE_TYPE_3D_RGB = 5,
		DMK_OBJECT_TEXTURE_TYPE_3D_RGBA = 6
	};

	/* AUDIO SPECIFICATIONS */
	// Output options:
	enum class DMKAudioOutputOptions {
		DMK_AUDIO_OUTPUT_DEFAULT = 0,
		DMK_AUDIO_OUTPUT_SPEAKERS_STERIO = 1,
		DMK_AUDIO_OUTPUT_SPEAKERS_MONO = 2,
		DMK_AUDIO_OUTPUT_SPEAKERS_LEFT = 3,
		DMK_AUDIO_OUTPUT_SPEAKERS_RIGHT = 4,
		DMK_AUDIO_OUTPUT_HEADPHONES_STERIO = 5,
		DMK_AUDIO_OUTPUT_HEADPHONES_MONO = 6,
		DMK_AUDIO_OUTPUT_HEADPHONES_LEFT = 7,
		DMK_AUDIO_OUTPUT_HEADPHONES_RIGHT = 8
	};

	// Priority options:
	enum class DMKAudioPriorityOptions {
		DMK_AUDIO_PRIORITY_DEFAULT = 0,
		DMK_AUDIO_PRIORITY_LOW = 1,
		DMK_AUDIO_PRIORITY_HIGH = 2
	};

	/* CAMERA SPECIFICATIONS */
	// Camera types:
	enum class DMKCameraType {
		DMK_CAMERA_TYPE_DEFAULT = 0,
		DMK_CAMERA_TYPE_ORTHOGRSPHIC = 1,
		DMK_CAMERA_TYPE_FRUSTUM = 2
	};

	/* LIGHTING SPECIFICATIONS */
	// Lighting types:
	enum class DMKLightingType {
		DMK_LIGHTING_TYPE_SPECULAR = 0,
		DMK_LIGHTING_TYPE_DIRECTIONAL = 1,
		DMK_LIGHTING_TYPE_EMISSIVE = 2
	};

	/* KEY BINDINGS */
	// Human Interfce Device(s) (HID) binding options:
	enum class DMKHumanInterfaceDeviceBindingOptions {
		DMK_HID_UNKNOWN,
		DMK_HID_KEY_BINDING_UNKNOWN,

		DMK_HID_KEY_BINDING_MOUSE_BUTTON_RELEASE,
		DMK_HID_KEY_BINDING_MOUSE_BUTTON_PRESS,
		DMK_HID_KEY_BINDING_MOUSE_BUTTON_REPEAT,

		DMK_HID_KEY_BINDING_HAT_CENTERED,
		DMK_HID_KEY_BINDING_HAT_UP,
		DMK_HID_KEY_BINDING_HAT_RIGHT,
		DMK_HID_KEY_BINDING_HAT_DOWN,
		DMK_HID_KEY_BINDING_HAT_LEFT,
		DMK_HID_KEY_BINDING_HAT_RIGHT_UP,
		DMK_HID_KEY_BINDING_HAT_RIGHT_DOWN,
		DMK_HID_KEY_BINDING_HAT_LEFT_UP,
		DMK_HID_KEY_BINDING_HAT_LEFT_DOWN,

		DMK_HID_KEY_SPACE,
		DMK_HID_KEY_APOSTROPHE,
		DMK_HID_KEY_COMA,
		DMK_HID_KEY_MINUS,
		DMK_HID_KEY_PERIOD,
		DMK_HID_KEY_SLASH,
		DMK_HID_KEY_0,
		DMK_HID_KEY_1,
		DMK_HID_KEY_2,
		DMK_HID_KEY_3,
		DMK_HID_KEY_4,
		DMK_HID_KEY_5,
		DMK_HID_KEY_6,
		DMK_HID_KEY_7,
		DMK_HID_KEY_8,
		DMK_HID_KEY_9,
		DMK_HID_KEY_SEMICOLON,
		DMK_HID_KEY_EQUAL,
		DMK_HID_KEY_A,
		DMK_HID_KEY_B,
		DMK_HID_KEY_C,
		DMK_HID_KEY_D,
		DMK_HID_KEY_E,
		DMK_HID_KEY_F,
		DMK_HID_KEY_G,
		DMK_HID_KEY_H,
		DMK_HID_KEY_I,
		DMK_HID_KEY_J,
		DMK_HID_KEY_K,
		DMK_HID_KEY_L,
		DMK_HID_KEY_M,
		DMK_HID_KEY_N,
		DMK_HID_KEY_O,
		DMK_HID_KEY_P,
		DMK_HID_KEY_Q,
		DMK_HID_KEY_R,
		DMK_HID_KEY_S,
		DMK_HID_KEY_T,
		DMK_HID_KEY_U,
		DMK_HID_KEY_V,
		DMK_HID_KEY_W,
		DMK_HID_KEY_X,
		DMK_HID_KEY_Y,
		DMK_HID_KEY_Z,
		DMK_HID_KEY_LEFT_BRACKET,
		DMK_HID_KEY_BACKLASH,
		DMK_HID_KEY_RIGHT_BRACKET,
		DMK_HID_KEY_RIGHT_GRAVE_ACCENT,
		DMK_HID_KEY_WORLD_1,
		DMK_HID_KEY_WORLD_2,
		DMK_HID_KEY_ESCAPE,
		DMK_HID_KEY_ENTER,
		DMK_HID_KEY_TAB,
		DMK_HID_KEY_BACKSPACE,
		DMK_HID_KEY_INSERT,
		DMK_HID_KEY_DELETE,
		DMK_HID_KEY_RIGHT,
		DMK_HID_KEY_LEFT,
		DMK_HID_KEY_DOWN,
		DMK_HID_KEY_UP,
		DMK_HID_KEY_PAGE_UP,
		DMK_HID_KEY_PAGE_DOWN,
		DMK_HID_KEY_HOME,
		DMK_HID_KEY_END,
		DMK_HID_KEY_CAPS_LOCK,
		DMK_HID_KEY_SCROLL_LOCK,
		DMK_HID_KEY_NUM_LOCK,
		DMK_HID_KEY_PRINT_SCREEN,
		DMK_HID_KEY_PAUSE,
		DMK_HID_KEY_F1,
		DMK_HID_KEY_F2,
		DMK_HID_KEY_F3,
		DMK_HID_KEY_F4,
		DMK_HID_KEY_F5,
		DMK_HID_KEY_F6,
		DMK_HID_KEY_F7,
		DMK_HID_KEY_F8,
		DMK_HID_KEY_F9,
		DMK_HID_KEY_F10,
		DMK_HID_KEY_F11,
		DMK_HID_KEY_F12,
		DMK_HID_KEY_F13,
		DMK_HID_KEY_F14,
		DMK_HID_KEY_F15,
		DMK_HID_KEY_F16,
		DMK_HID_KEY_F17,
		DMK_HID_KEY_F18,
		DMK_HID_KEY_F19,
		DMK_HID_KEY_F20,
		DMK_HID_KEY_F21,
		DMK_HID_KEY_F22,
		DMK_HID_KEY_F23,
		DMK_HID_KEY_F24,
		DMK_HID_KEY_F25,
		DMK_HID_KEY_KP_0,
		DMK_HID_KEY_KP_1,
		DMK_HID_KEY_KP_2,
		DMK_HID_KEY_KP_3,
		DMK_HID_KEY_KP_4,
		DMK_HID_KEY_KP_5,
		DMK_HID_KEY_KP_6,
		DMK_HID_KEY_KP_7,
		DMK_HID_KEY_KP_8,
		DMK_HID_KEY_KP_9,
		DMK_HID_KEY_KP_DECIMAL,
		DMK_HID_KEY_KP_DIVIDE,
		DMK_HID_KEY_KP_MULTIPLY,
		DMK_HID_KEY_KP_SUBTRACT,
		DMK_HID_KEY_KP_ADD,
		DMK_HID_KEY_KP_ENTER,
		DMK_HID_KEY_KP_EQUAL,
		DMK_HID_KEY_LEFT_SHIFT,
		DMK_HID_KEY_LEFT_CONTROL,
		DMK_HID_KEY_LEFT_ALT,
		DMK_HID_KEY_LEFT_SUPER,
		DMK_HID_KEY_RIGHT_SHIFT,
		DMK_HID_KEY_RIGHT_CONTROL,
		DMK_HID_KEY_RIGHT_ALT,
		DMK_HID_KEY_RIGHT_SUPER,
		DMK_HID_KEY_MENU,
		DMK_HID_KEY_LAST,

		DMK_HID_MOD_SHIFT,
		DMK_HID_MOD_CONTROL,
		DMK_HID_MOD_ALT,
		DMK_HID_MOD_SUPER,
		DMK_HID_MOD_CAPS_LOCK,
		DMK_HID_MOD_NUM_LOCK,

		DMK_HID_MOUSE_BUTTON_1,
		DMK_HID_MOUSE_BUTTON_2,
		DMK_HID_MOUSE_BUTTON_3,
		DMK_HID_MOUSE_BUTTON_4,
		DMK_HID_MOUSE_BUTTON_5,
		DMK_HID_MOUSE_BUTTON_6,
		DMK_HID_MOUSE_BUTTON_7,
		DMK_HID_MOUSE_BUTTON_8,
		DMK_HID_MOUSE_BUTTON_LAST,
		DMK_HID_MOUSE_BUTTON_LEFT,
		DMK_HID_MOUSE_BUTTON_RIGHT,
		DMK_HID_MOUSE_BUTTON_MIDDLE,

		DMK_HID_JOYSTICK_1,
		DMK_HID_JOYSTICK_2,
		DMK_HID_JOYSTICK_3,
		DMK_HID_JOYSTICK_4,
		DMK_HID_JOYSTICK_5,
		DMK_HID_JOYSTICK_6,
		DMK_HID_JOYSTICK_7,
		DMK_HID_JOYSTICK_8,
		DMK_HID_JOYSTICK_9,
		DMK_HID_JOYSTICK_10,
		DMK_HID_JOYSTICK_11,
		DMK_HID_JOYSTICK_12,
		DMK_HID_JOYSTICK_13,
		DMK_HID_JOYSTICK_14,
		DMK_HID_JOYSTICK_15,
		DMK_HID_JOYSTICK_16,

		DMK_HID_GAMEPAD_BUTTON_A,
		DMK_HID_GAMEPAD_BUTTON_B,
		DMK_HID_GAMEPAD_BUTTON_X,
		DMK_HID_GAMEPAD_BUTTON_Y,
		DMK_HID_GAMEPAD_BUTTON_LEFT_BUMPER,
		DMK_HID_GAMEPAD_BUTTON_RIGHT_BUMPER,
		DMK_HID_GAMEPAD_BUTTON_BACK,
		DMK_HID_GAMEPAD_BUTTON_START,
		DMK_HID_GAMEPAD_BUTTON_GUIDE,
		DMK_HID_GAMEPAD_BUTTON_LEFT_THUMB,
		DMK_HID_GAMEPAD_BUTTON_RIGHT_THUMB,
		DMK_HID_GAMEPAD_BUTTON_DPAD_UP,
		DMK_HID_GAMEPAD_BUTTON_DPAD_RIGHT,
		DMK_HID_GAMEPAD_BUTTON_DPAD_DOWN,
		DMK_HID_GAMEPAD_BUTTON_DPAD_LEFT,
		DMK_HID_GAMEPAD_BUTTON_LAST,

		DMK_HID_GAMEPAD_BUTTON_CROSS,
		DMK_HID_GAMEPAD_BUTTON_CIRCLE,
		DMK_HID_GAMEPAD_BUTTON_SQUARE,
		DMK_HID_GAMEPAD_BUTTON_TRIANGLE,

		DMK_HID_GAMEPAD_AXIS_LEFT_X,
		DMK_HID_GAMEPAD_AXIS_LEFT_Y,
		DMK_HID_GAMEPAD_AXIS_RIGHT_X,
		DMK_HID_GAMEPAD_AXIS_RIGHT_Y,
		DMK_HID_GAMEPAD_AXIS_LEFT_TRIGGER,
		DMK_HID_GAMEPAD_AXIS_RIGHT_TRIGGER,
		DMK_HID_GAMEPAD_AXIS_LAST
	};

	/* Internal formats of the Dynamik Engine */
	enum class DMKFormat {
		DMK_FORMAT_UNDEFINED,

		DMK_FORMAT_R_8_INT,
		DMK_FORMAT_R_8_UINT,
		DMK_FORMAT_R_8_SINT,
		DMK_FORMAT_R_8_F32,
		DMK_FORMAT_R_8_UF32,
		DMK_FORMAT_R_8_SF32,
		DMK_FORMAT_R_8_UNIFORM,
		DMK_FORMAT_R_16_INT,
		DMK_FORMAT_R_16_UINT,
		DMK_FORMAT_R_16_SINT,
		DMK_FORMAT_R_16_F32,
		DMK_FORMAT_R_16_UF32,
		DMK_FORMAT_R_16_SF32,
		DMK_FORMAT_R_16_UNIFORM,
		DMK_FORMAT_R_32_INT,
		DMK_FORMAT_R_32_UINT,
		DMK_FORMAT_R_32_SINT,
		DMK_FORMAT_R_32_F32,
		DMK_FORMAT_R_32_UF32,
		DMK_FORMAT_R_32_SF32,
		DMK_FORMAT_R_32_UNIFORM,

		DMK_FORMAT_RG_8_INT,
		DMK_FORMAT_RG_8_UINT,
		DMK_FORMAT_RG_8_SINT,
		DMK_FORMAT_RG_8_F32,
		DMK_FORMAT_RG_8_UF32,
		DMK_FORMAT_RG_8_SF32,
		DMK_FORMAT_RG_8_UNIFORM,
		DMK_FORMAT_RG_16_INT,
		DMK_FORMAT_RG_16_UINT,
		DMK_FORMAT_RG_16_SINT,
		DMK_FORMAT_RG_16_F32,
		DMK_FORMAT_RG_16_UF32,
		DMK_FORMAT_RG_16_SF32,
		DMK_FORMAT_RG_16_UNIFORM,
		DMK_FORMAT_RG_32_INT,
		DMK_FORMAT_RG_32_UINT,
		DMK_FORMAT_RG_32_SINT,
		DMK_FORMAT_RG_32_F32,
		DMK_FORMAT_RG_32_UF32,
		DMK_FORMAT_RG_32_SF32,
		DMK_FORMAT_RG_32_UNIFORM,

		DMK_FORMAT_RGB_8_INT,
		DMK_FORMAT_RGB_8_UINT,
		DMK_FORMAT_RGB_8_SINT,
		DMK_FORMAT_RGB_8_F32,
		DMK_FORMAT_RGB_8_UF32,
		DMK_FORMAT_RGB_8_SF32,
		DMK_FORMAT_RGB_8_UNIFORM,
		DMK_FORMAT_RGB_16_INT,
		DMK_FORMAT_RGB_16_UINT,
		DMK_FORMAT_RGB_16_SINT,
		DMK_FORMAT_RGB_16_F32,
		DMK_FORMAT_RGB_16_UF32,
		DMK_FORMAT_RGB_16_SF32,
		DMK_FORMAT_RGB_16_UNIFORM,
		DMK_FORMAT_RGB_32_INT,
		DMK_FORMAT_RGB_32_UINT,
		DMK_FORMAT_RGB_32_SINT,
		DMK_FORMAT_RGB_32_F32,
		DMK_FORMAT_RGB_32_UF32,
		DMK_FORMAT_RGB_32_SF32,
		DMK_FORMAT_RGB_32_UNIFORM,

		DMK_FORMAT_BGR_8_INT,
		DMK_FORMAT_BGR_8_UINT,
		DMK_FORMAT_BGR_8_SINT,
		DMK_FORMAT_BGR_8_F32,
		DMK_FORMAT_BGR_8_UF32,
		DMK_FORMAT_BGR_8_SF32,
		DMK_FORMAT_BGR_8_UNIFORM,
		DMK_FORMAT_BGR_16_INT,
		DMK_FORMAT_BGR_16_UINT,
		DMK_FORMAT_BGR_16_SINT,
		DMK_FORMAT_BGR_16_F32,
		DMK_FORMAT_BGR_16_UF32,
		DMK_FORMAT_BGR_16_SF32,
		DMK_FORMAT_BGR_16_UNIFORM,
		DMK_FORMAT_BGR_32_INT,
		DMK_FORMAT_BGR_32_UINT,
		DMK_FORMAT_BGR_32_SINT,
		DMK_FORMAT_BGR_32_F32,
		DMK_FORMAT_BGR_32_UF32,
		DMK_FORMAT_BGR_32_SF32,
		DMK_FORMAT_BGR_32_UNIFORM,

		DMK_FORMAT_RGBA_8_INT,
		DMK_FORMAT_RGBA_8_UINT,
		DMK_FORMAT_RGBA_8_SINT,
		DMK_FORMAT_RGBA_8_F32,
		DMK_FORMAT_RGBA_8_UF32,
		DMK_FORMAT_RGBA_8_SF32,
		DMK_FORMAT_RGBA_8_UNIFORM,
		DMK_FORMAT_RGBA_16_INT,
		DMK_FORMAT_RGBA_16_UINT,
		DMK_FORMAT_RGBA_16_SINT,
		DMK_FORMAT_RGBA_16_F32,
		DMK_FORMAT_RGBA_16_UF32,
		DMK_FORMAT_RGBA_16_SF32,
		DMK_FORMAT_RGBA_16_UNIFORM,
		DMK_FORMAT_RGBA_32_INT,
		DMK_FORMAT_RGBA_32_UINT,
		DMK_FORMAT_RGBA_32_SINT,
		DMK_FORMAT_RGBA_32_F32,
		DMK_FORMAT_RGBA_32_UF32,
		DMK_FORMAT_RGBA_32_SF32,
		DMK_FORMAT_RGBA_32_UNIFORM,

		DMK_FORMAT_BGRA_8_INT,
		DMK_FORMAT_BGRA_8_UINT,
		DMK_FORMAT_BGRA_8_SINT,
		DMK_FORMAT_BGRA_8_F32,
		DMK_FORMAT_BGRA_8_UF32,
		DMK_FORMAT_BGRA_8_SF32,
		DMK_FORMAT_BGRA_8_UNIFORM,
		DMK_FORMAT_BGRA_16_INT,
		DMK_FORMAT_BGRA_16_UINT,
		DMK_FORMAT_BGRA_16_SINT,
		DMK_FORMAT_BGRA_16_F32,
		DMK_FORMAT_BGRA_16_UF32,
		DMK_FORMAT_BGRA_16_SF32,
		DMK_FORMAT_BGRA_16_UNIFORM,
		DMK_FORMAT_BGRA_32_INT,
		DMK_FORMAT_BGRA_32_UINT,
		DMK_FORMAT_BGRA_32_SINT,
		DMK_FORMAT_BGRA_32_F32,
		DMK_FORMAT_BGRA_32_UF32,
		DMK_FORMAT_BGRA_32_SF32,
		DMK_FORMAT_BGRA_32_UNIFORM,
	};

	/* RENDERING TECHNOLOGY ENUM */
	enum class DMKRenderingTechnology {
		DMK_RENDERING_TECHNOLOGY_VERTEX,						// Render individual vertexes
		DMK_RENDERING_TECHNOLOGY_INDEXED,						// Render indexed vertexes
		DMK_RENDERING_TECHNOLOGY_INDIRECT,						// Render indirectly (only if supported)
		DMK_RENDERING_TECHNOLOGY_INDEXED_INDIRECT,				// Render indexed vertexes indirectly (only if supported)

		DMK_RENDERING_TECHNOLOGY_SKYBOX_VERTEX,					// Render skybox individual vertexes
		DMK_RENDERING_TECHNOLOGY_SKYBOX_INDEXED,				// Render skybox indexed vertexes
		DMK_RENDERING_TECHNOLOGY_SKYBOX_INDIRECT,				// Render skybox indirectly (only if supported)
		DMK_RENDERING_TECHNOLOGY_SKYBOX_INDEXED_INDIRECT,		// Render skybox indexed vertexes indirectly (only if supported)

		DMK_RENDERING_TECHNOLOGY_VERTEX_MESH_ONLY,				// Render individual vertexes (meshes only)
		DMK_RENDERING_TECHNOLOGY_INDEXED_MESH_ONLY,				// Render indexed vertexes (meshes only)
		DMK_RENDERING_TECHNOLOGY_INDIRECT_MESH_ONLY,			// Render indirectly (only if supported) (meshes only)
		DMK_RENDERING_TECHNOLOGY_INDEXED_INDIRECT_MESH_ONLY		// Render indexed vertexes indirectly (only if supported) (meshes only)
	};

	/* VERTEX DATA TYPE ENUM */
	enum class DMKVertexData {
		DMK_VERTEX_DATA_POSITION,					// Positional vectors
		DMK_VERTEX_DATA_COLOR,						// Color vectors
		DMK_VERTEX_DATA_REFLECTANCE,				// Reflectance vectors
		DMK_VERTEX_DATA_TEXTURE_COORDINATES,		// Texture coordinates
		DMK_VERTEX_DATA_NORMAL_VECTORS,				// Normal vectors
		DMK_VERTEX_DATA_TANGENT_VECTORS,			// Tangent vectors
		DMK_VERTEX_DATA_BLEND_WEIGHTS,				// Blend weights
		DMK_VERTEX_DATA_BLEND_SHAPES,				// Blend shapes
		DMK_VERTEX_DATA_BOND_WEIGHTS,				// Bond weights
		DMK_VERTEX_DATA_SPACE_VECTORS,				// Space vectors

		DMK_VERTEX_DATA_INTEGRITY,					// Dynamik specific data (destruction)

		DMK_VERTEX_DATA_CUSTOM,						// Custom data type
		DMK_VERTEX_DATA_UNKNOWN						// Unknown
	};

	/* UNIFORM TYPE ENUM */
	enum class DMKUniformType {
		DMK_UNIFORM_TYPE_BUFFER_OBJECT,
		DMK_UNIFORM_TYPE_IMAGE_SAMPLER_2D,
		DMK_UNIFORM_TYPE_IMAGE_SAMPLER_3D,
		DMK_UNIFORM_TYPE_IMAGE_SAMPLER_CUBEMAP,
		DMK_UNIFORM_TYPE_CONSTANT,
	};

	/* UNIFORM DATA TYPE ENUM */
	enum class DMKUniformData {
		DMK_UNIFORM_DATA_MODEL,						// Model matrix
		DMK_UNIFORM_DATA_VIEW,						// View matrix
		DMK_UNIFORM_DATA_PROJECTION,				// Projection matrix
		DMK_UNIFORM_DATA_CUSTOM						// Custom data input
	};

	/* Dynamik Game Object Shader Location enum		*/
	/* Location to which the data are bount to		*/
	enum class DMKShaderLocation {
		DMK_SHADER_LOCATION_VERTEX,
		DMK_SHADER_LOCATION_TESSELLATION,
		DMK_SHADER_LOCATION_GEOMETRY,
		DMK_SHADER_LOCATION_FRAGMENT,
		DMK_SHADER_LOCATION_COMPUTE,
	};

	/* Directly translated to descriptors and objects */
	enum class DMKRenderAttachment {
		DMK_RENDER_ATTACHMENT_COLOR,
		DMK_RENDER_ATTACHMENT_DEPTH,

		DMK_RENDER_ATTACHMENT_SKYBOX,
		DMK_RENDER_ATTACHMENT_LIGHTING,
		DMK_RENDER_ATTACHMENT_IRRADIANCE_CUBE,
		DMK_RENDER_ATTACHMENT_PREFILTERED_CUBE,
		DMK_RENDER_ATTACHMENT_BRDF_TABLE,

		DMK_RENDER_ATTACHMENT_COMPUTE_TEXTURE,

		DMK_RENDER_ATTACHMENT_CUSTOM,
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_OBJECT_SPECIFICATIONS_H
