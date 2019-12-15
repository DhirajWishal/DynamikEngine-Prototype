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
		DMK_OBJECT_TYPE_MESH,
		DMK_OBJECT_TYPE_STATIC_OBJECT,
		DMK_OBJECT_TYPE_INTERACTIVE_OBJECT,
		DMK_OBJECT_TYPE_PLAYER,
		DMK_OBJECT_TYPE_NPC,
		DMK_OBJECT_TYPE_TEXTURE_UI,
		DMK_OBJECT_TYPE_SKYBOX,
		DMK_OBJECT_TYPE_SPRITES,
		DMK_OBJECT_TYPE_FONT,
		DMK_OBJECT_TYPE_PARTICLE, //
		DMK_OBJECT_TYPE_DESTRUCTION,
		DMK_OBJECT_TYPE_DESTRUCTOR,
		DMK_OBJECT_TYPE_CAMERA,
		DMK_OBJECT_TYPE_AUDIO,
		DMK_OBJECT_TYPE_VIDEO,
		DMK_OBJECT_TYPE_WIND,
		DMK_OBJECT_TYPE_AI,
		DMK_OBJECT_TYPE_SCRIPT,
		DMK_OBJECT_TYPE_KEY_BINDINGS,
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

	// directly compatible with vulkan
	enum DMKFormat {
		DMK_FORMAT_UNDEFINED = 0,
		DMK_FORMAT_R4G4_UNORM_PACK8 = 1,
		DMK_FORMAT_R4G4B4A4_UNORM_PACK16 = 2,
		DMK_FORMAT_B4G4R4A4_UNORM_PACK16 = 3,
		DMK_FORMAT_R5G6B5_UNORM_PACK16 = 4,
		DMK_FORMAT_B5G6R5_UNORM_PACK16 = 5,
		DMK_FORMAT_R5G5B5A1_UNORM_PACK16 = 6,
		DMK_FORMAT_B5G5R5A1_UNORM_PACK16 = 7,
		DMK_FORMAT_A1R5G5B5_UNORM_PACK16 = 8,
		DMK_FORMAT_R8_UNORM = 9,
		DMK_FORMAT_R8_SNORM = 10,
		DMK_FORMAT_R8_USCALED = 11,
		DMK_FORMAT_R8_SSCALED = 12,
		DMK_FORMAT_R8_UINT = 13,
		DMK_FORMAT_R8_SINT = 14,
		DMK_FORMAT_R8_SRGB = 15,
		DMK_FORMAT_R8G8_UNORM = 16,
		DMK_FORMAT_R8G8_SNORM = 17,
		DMK_FORMAT_R8G8_USCALED = 18,
		DMK_FORMAT_R8G8_SSCALED = 19,
		DMK_FORMAT_R8G8_UINT = 20,
		DMK_FORMAT_R8G8_SINT = 21,
		DMK_FORMAT_R8G8_SRGB = 22,
		DMK_FORMAT_R8G8B8_UNORM = 23,
		DMK_FORMAT_R8G8B8_SNORM = 24,
		DMK_FORMAT_R8G8B8_USCALED = 25,
		DMK_FORMAT_R8G8B8_SSCALED = 26,
		DMK_FORMAT_R8G8B8_UINT = 27,
		DMK_FORMAT_R8G8B8_SINT = 28,
		DMK_FORMAT_R8G8B8_SRGB = 29,
		DMK_FORMAT_B8G8R8_UNORM = 30,
		DMK_FORMAT_B8G8R8_SNORM = 31,
		DMK_FORMAT_B8G8R8_USCALED = 32,
		DMK_FORMAT_B8G8R8_SSCALED = 33,
		DMK_FORMAT_B8G8R8_UINT = 34,
		DMK_FORMAT_B8G8R8_SINT = 35,
		DMK_FORMAT_B8G8R8_SRGB = 36,
		DMK_FORMAT_R8G8B8A8_UNORM = 37,
		DMK_FORMAT_R8G8B8A8_SNORM = 38,
		DMK_FORMAT_R8G8B8A8_USCALED = 39,
		DMK_FORMAT_R8G8B8A8_SSCALED = 40,
		DMK_FORMAT_R8G8B8A8_UINT = 41,
		DMK_FORMAT_R8G8B8A8_SINT = 42,
		DMK_FORMAT_R8G8B8A8_SRGB = 43,
		DMK_FORMAT_B8G8R8A8_UNORM = 44,
		DMK_FORMAT_B8G8R8A8_SNORM = 45,
		DMK_FORMAT_B8G8R8A8_USCALED = 46,
		DMK_FORMAT_B8G8R8A8_SSCALED = 47,
		DMK_FORMAT_B8G8R8A8_UINT = 48,
		DMK_FORMAT_B8G8R8A8_SINT = 49,
		DMK_FORMAT_B8G8R8A8_SRGB = 50,
		DMK_FORMAT_A8B8G8R8_UNORM_PACK32 = 51,
		DMK_FORMAT_A8B8G8R8_SNORM_PACK32 = 52,
		DMK_FORMAT_A8B8G8R8_USCALED_PACK32 = 53,
		DMK_FORMAT_A8B8G8R8_SSCALED_PACK32 = 54,
		DMK_FORMAT_A8B8G8R8_UINT_PACK32 = 55,
		DMK_FORMAT_A8B8G8R8_SINT_PACK32 = 56,
		DMK_FORMAT_A8B8G8R8_SRGB_PACK32 = 57,
		DMK_FORMAT_A2R10G10B10_UNORM_PACK32 = 58,
		DMK_FORMAT_A2R10G10B10_SNORM_PACK32 = 59,
		DMK_FORMAT_A2R10G10B10_USCALED_PACK32 = 60,
		DMK_FORMAT_A2R10G10B10_SSCALED_PACK32 = 61,
		DMK_FORMAT_A2R10G10B10_UINT_PACK32 = 62,
		DMK_FORMAT_A2R10G10B10_SINT_PACK32 = 63,
		DMK_FORMAT_A2B10G10R10_UNORM_PACK32 = 64,
		DMK_FORMAT_A2B10G10R10_SNORM_PACK32 = 65,
		DMK_FORMAT_A2B10G10R10_USCALED_PACK32 = 66,
		DMK_FORMAT_A2B10G10R10_SSCALED_PACK32 = 67,
		DMK_FORMAT_A2B10G10R10_UINT_PACK32 = 68,
		DMK_FORMAT_A2B10G10R10_SINT_PACK32 = 69,
		DMK_FORMAT_R16_UNORM = 70,
		DMK_FORMAT_R16_SNORM = 71,
		DMK_FORMAT_R16_USCALED = 72,
		DMK_FORMAT_R16_SSCALED = 73,
		DMK_FORMAT_R16_UINT = 74,
		DMK_FORMAT_R16_SINT = 75,
		DMK_FORMAT_R16_SFLOAT = 76,
		DMK_FORMAT_R16G16_UNORM = 77,
		DMK_FORMAT_R16G16_SNORM = 78,
		DMK_FORMAT_R16G16_USCALED = 79,
		DMK_FORMAT_R16G16_SSCALED = 80,
		DMK_FORMAT_R16G16_UINT = 81,
		DMK_FORMAT_R16G16_SINT = 82,
		DMK_FORMAT_R16G16_SFLOAT = 83,
		DMK_FORMAT_R16G16B16_UNORM = 84,
		DMK_FORMAT_R16G16B16_SNORM = 85,
		DMK_FORMAT_R16G16B16_USCALED = 86,
		DMK_FORMAT_R16G16B16_SSCALED = 87,
		DMK_FORMAT_R16G16B16_UINT = 88,
		DMK_FORMAT_R16G16B16_SINT = 89,
		DMK_FORMAT_R16G16B16_SFLOAT = 90,
		DMK_FORMAT_R16G16B16A16_UNORM = 91,
		DMK_FORMAT_R16G16B16A16_SNORM = 92,
		DMK_FORMAT_R16G16B16A16_USCALED = 93,
		DMK_FORMAT_R16G16B16A16_SSCALED = 94,
		DMK_FORMAT_R16G16B16A16_UINT = 95,
		DMK_FORMAT_R16G16B16A16_SINT = 96,
		DMK_FORMAT_R16G16B16A16_SFLOAT = 97,
		DMK_FORMAT_R32_UINT = 98,
		DMK_FORMAT_R32_SINT = 99,
		DMK_FORMAT_R32_SFLOAT = 100,
		DMK_FORMAT_R32G32_UINT = 101,
		DMK_FORMAT_R32G32_SINT = 102,
		DMK_FORMAT_R32G32_SFLOAT = 103,
		DMK_FORMAT_R32G32B32_UINT = 104,
		DMK_FORMAT_R32G32B32_SINT = 105,
		DMK_FORMAT_R32G32B32_SFLOAT = 106,
		DMK_FORMAT_R32G32B32A32_UINT = 107,
		DMK_FORMAT_R32G32B32A32_SINT = 108,
		DMK_FORMAT_R32G32B32A32_SFLOAT = 109,
		DMK_FORMAT_R64_UINT = 110,
		DMK_FORMAT_R64_SINT = 111,
		DMK_FORMAT_R64_SFLOAT = 112,
		DMK_FORMAT_R64G64_UINT = 113,
		DMK_FORMAT_R64G64_SINT = 114,
		DMK_FORMAT_R64G64_SFLOAT = 115,
		DMK_FORMAT_R64G64B64_UINT = 116,
		DMK_FORMAT_R64G64B64_SINT = 117,
		DMK_FORMAT_R64G64B64_SFLOAT = 118,
		DMK_FORMAT_R64G64B64A64_UINT = 119,
		DMK_FORMAT_R64G64B64A64_SINT = 120,
		DMK_FORMAT_R64G64B64A64_SFLOAT = 121,
		DMK_FORMAT_B10G11R11_UFLOAT_PACK32 = 122,
		DMK_FORMAT_E5B9G9R9_UFLOAT_PACK32 = 123,
		DMK_FORMAT_D16_UNORM = 124,
		DMK_FORMAT_X8_D24_UNORM_PACK32 = 125,
		DMK_FORMAT_D32_SFLOAT = 126,
		DMK_FORMAT_S8_UINT = 127,
		DMK_FORMAT_D16_UNORM_S8_UINT = 128,
		DMK_FORMAT_D24_UNORM_S8_UINT = 129,
		DMK_FORMAT_D32_SFLOAT_S8_UINT = 130,
		DMK_FORMAT_BC1_RGB_UNORM_BLOCK = 131,
		DMK_FORMAT_BC1_RGB_SRGB_BLOCK = 132,
		DMK_FORMAT_BC1_RGBA_UNORM_BLOCK = 133,
		DMK_FORMAT_BC1_RGBA_SRGB_BLOCK = 134,
		DMK_FORMAT_BC2_UNORM_BLOCK = 135,
		DMK_FORMAT_BC2_SRGB_BLOCK = 136,
		DMK_FORMAT_BC3_UNORM_BLOCK = 137,
		DMK_FORMAT_BC3_SRGB_BLOCK = 138,
		DMK_FORMAT_BC4_UNORM_BLOCK = 139,
		DMK_FORMAT_BC4_SNORM_BLOCK = 140,
		DMK_FORMAT_BC5_UNORM_BLOCK = 141,
		DMK_FORMAT_BC5_SNORM_BLOCK = 142,
		DMK_FORMAT_BC6H_UFLOAT_BLOCK = 143,
		DMK_FORMAT_BC6H_SFLOAT_BLOCK = 144,
		DMK_FORMAT_BC7_UNORM_BLOCK = 145,
		DMK_FORMAT_BC7_SRGB_BLOCK = 146,
		DMK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK = 147,
		DMK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK = 148,
		DMK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK = 149,
		DMK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK = 150,
		DMK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK = 151,
		DMK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK = 152,
		DMK_FORMAT_EAC_R11_UNORM_BLOCK = 153,
		DMK_FORMAT_EAC_R11_SNORM_BLOCK = 154,
		DMK_FORMAT_EAC_R11G11_UNORM_BLOCK = 155,
		DMK_FORMAT_EAC_R11G11_SNORM_BLOCK = 156,
		DMK_FORMAT_ASTC_4x4_UNORM_BLOCK = 157,
		DMK_FORMAT_ASTC_4x4_SRGB_BLOCK = 158,
		DMK_FORMAT_ASTC_5x4_UNORM_BLOCK = 159,
		DMK_FORMAT_ASTC_5x4_SRGB_BLOCK = 160,
		DMK_FORMAT_ASTC_5x5_UNORM_BLOCK = 161,
		DMK_FORMAT_ASTC_5x5_SRGB_BLOCK = 162,
		DMK_FORMAT_ASTC_6x5_UNORM_BLOCK = 163,
		DMK_FORMAT_ASTC_6x5_SRGB_BLOCK = 164,
		DMK_FORMAT_ASTC_6x6_UNORM_BLOCK = 165,
		DMK_FORMAT_ASTC_6x6_SRGB_BLOCK = 166,
		DMK_FORMAT_ASTC_8x5_UNORM_BLOCK = 167,
		DMK_FORMAT_ASTC_8x5_SRGB_BLOCK = 168,
		DMK_FORMAT_ASTC_8x6_UNORM_BLOCK = 169,
		DMK_FORMAT_ASTC_8x6_SRGB_BLOCK = 170,
		DMK_FORMAT_ASTC_8x8_UNORM_BLOCK = 171,
		DMK_FORMAT_ASTC_8x8_SRGB_BLOCK = 172,
		DMK_FORMAT_ASTC_10x5_UNORM_BLOCK = 173,
		DMK_FORMAT_ASTC_10x5_SRGB_BLOCK = 174,
		DMK_FORMAT_ASTC_10x6_UNORM_BLOCK = 175,
		DMK_FORMAT_ASTC_10x6_SRGB_BLOCK = 176,
		DMK_FORMAT_ASTC_10x8_UNORM_BLOCK = 177,
		DMK_FORMAT_ASTC_10x8_SRGB_BLOCK = 178,
		DMK_FORMAT_ASTC_10x10_UNORM_BLOCK = 179,
		DMK_FORMAT_ASTC_10x10_SRGB_BLOCK = 180,
		DMK_FORMAT_ASTC_12x10_UNORM_BLOCK = 181,
		DMK_FORMAT_ASTC_12x10_SRGB_BLOCK = 182,
		DMK_FORMAT_ASTC_12x12_UNORM_BLOCK = 183,
		DMK_FORMAT_ASTC_12x12_SRGB_BLOCK = 184,
		DMK_FORMAT_G8B8G8R8_422_UNORM = 1000156000,
		DMK_FORMAT_B8G8R8G8_422_UNORM = 1000156001,
		DMK_FORMAT_G8_B8_R8_3PLANE_420_UNORM = 1000156002,
		DMK_FORMAT_G8_B8R8_2PLANE_420_UNORM = 1000156003,
		DMK_FORMAT_G8_B8_R8_3PLANE_422_UNORM = 1000156004,
		DMK_FORMAT_G8_B8R8_2PLANE_422_UNORM = 1000156005,
		DMK_FORMAT_G8_B8_R8_3PLANE_444_UNORM = 1000156006,
		DMK_FORMAT_R10X6_UNORM_PACK16 = 1000156007,
		DMK_FORMAT_R10X6G10X6_UNORM_2PACK16 = 1000156008,
		DMK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16 = 1000156009,
		DMK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16 = 1000156010,
		DMK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16 = 1000156011,
		DMK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16 = 1000156012,
		DMK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16 = 1000156013,
		DMK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16 = 1000156014,
		DMK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16 = 1000156015,
		DMK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16 = 1000156016,
		DMK_FORMAT_R12X4_UNORM_PACK16 = 1000156017,
		DMK_FORMAT_R12X4G12X4_UNORM_2PACK16 = 1000156018,
		DMK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16 = 1000156019,
		DMK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16 = 1000156020,
		DMK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16 = 1000156021,
		DMK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16 = 1000156022,
		DMK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16 = 1000156023,
		DMK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16 = 1000156024,
		DMK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16 = 1000156025,
		DMK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16 = 1000156026,
		DMK_FORMAT_G16B16G16R16_422_UNORM = 1000156027,
		DMK_FORMAT_B16G16R16G16_422_UNORM = 1000156028,
		DMK_FORMAT_G16_B16_R16_3PLANE_420_UNORM = 1000156029,
		DMK_FORMAT_G16_B16R16_2PLANE_420_UNORM = 1000156030,
		DMK_FORMAT_G16_B16_R16_3PLANE_422_UNORM = 1000156031,
		DMK_FORMAT_G16_B16R16_2PLANE_422_UNORM = 1000156032,
		DMK_FORMAT_G16_B16_R16_3PLANE_444_UNORM = 1000156033,
		DMK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG = 1000054000,
		DMK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG = 1000054001,
		DMK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG = 1000054002,
		DMK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG = 1000054003,
		DMK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG = 1000054004,
		DMK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG = 1000054005,
		DMK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG = 1000054006,
		DMK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG = 1000054007,
		DMK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT = 1000066000,
		DMK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT = 1000066001,
		DMK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT = 1000066002,
		DMK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT = 1000066003,
		DMK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT = 1000066004,
		DMK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT = 1000066005,
		DMK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT = 1000066006,
		DMK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT = 1000066007,
		DMK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT = 1000066008,
		DMK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT = 1000066009,
		DMK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT = 1000066010,
		DMK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT = 1000066011,
		DMK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT = 1000066012,
		DMK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT = 1000066013,
		DMK_FORMAT_MAX_ENUM = 0x7FFFFFFF
	};

	/* RENDERING TECHNOLOGY ENUM */
	enum class DMK_ADGR_RENDERING_TECHNOLOGY {
		DMK_ADGR_RENDER_VERTEX = 0,					// Render individual vertexes
		DMK_ADGR_RENDER_INDEXED = 1,				// Render indexed vertexes
		DMK_ADGR_RENDER_INDIRECT = 2,				// Render indirectly (only if supported)
		DMK_ADGR_RENDER_INDEXED_INDIRECT = 3,		// Render indexed vertexes indirectly (only if supported)

		DMK_ADGR_RENDER_SKYBOX_VERTEX = 4,			// Render skybox individual vertexes
		DMK_ADGR_RENDER_SKYBOX_INDEXED = 5,			// Render skybox indexed vertexes
		DMK_ADGR_RENDER_SKYBOX_INDIRECT = 6,		// Render skybox indirectly (only if supported)
		DMK_ADGR_RENDER_SKYBOX_INDEXED_INDIRECT = 7	// Render skybox indexed vertexes indirectly (only if supported)
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_OBJECT_SPECIFICATIONS_H
