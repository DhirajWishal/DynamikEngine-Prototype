#pragma once

/* KEY BINDINGS FROM DMK */

namespace Dynamik {
#define DMK_RELEASE                0

#define DMK_PRESS                  1

#define DMK_REPEAT                 2

#define DMK_HAT_CENTERED           0
#define DMK_HAT_UP                 1
#define DMK_HAT_RIGHT              2
#define DMK_HAT_DOWN               4
#define DMK_HAT_LEFT               8
#define DMK_HAT_RIGHT_UP           (DMK_HAT_RIGHT | DMK_HAT_UP)
#define DMK_HAT_RIGHT_DOWN         (DMK_HAT_RIGHT | DMK_HAT_DOWN)
#define DMK_HAT_LEFT_UP            (DMK_HAT_LEFT  | DMK_HAT_UP)
#define DMK_HAT_LEFT_DOWN          (DMK_HAT_LEFT  | DMK_HAT_DOWN)

#define DMK_KEY_UNKNOWN            -1

#define DMK_KEY_SPACE              32
#define DMK_KEY_APOSTROPHE         39  /* ' */
#define DMK_KEY_COMMA              44  /* , */
#define DMK_KEY_MINUS              45  /* - */
#define DMK_KEY_PERIOD             46  /* . */
#define DMK_KEY_SLASH              47  /* / */
#define DMK_KEY_0                  48
#define DMK_KEY_1                  49
#define DMK_KEY_2                  50
#define DMK_KEY_3                  51
#define DMK_KEY_4                  52
#define DMK_KEY_5                  53
#define DMK_KEY_6                  54
#define DMK_KEY_7                  55
#define DMK_KEY_8                  56
#define DMK_KEY_9                  57
#define DMK_KEY_SEMICOLON          59  /* ; */
#define DMK_KEY_EQUAL              61  /* = */
#define DMK_KEY_A                  65
#define DMK_KEY_B                  66
#define DMK_KEY_C                  67
#define DMK_KEY_D                  68
#define DMK_KEY_E                  69
#define DMK_KEY_F                  70
#define DMK_KEY_G                  71
#define DMK_KEY_H                  72
#define DMK_KEY_I                  73
#define DMK_KEY_J                  74
#define DMK_KEY_K                  75
#define DMK_KEY_L                  76
#define DMK_KEY_M                  77
#define DMK_KEY_N                  78
#define DMK_KEY_O                  79
#define DMK_KEY_P                  80
#define DMK_KEY_Q                  81
#define DMK_KEY_R                  82
#define DMK_KEY_S                  83
#define DMK_KEY_T                  84
#define DMK_KEY_U                  85
#define DMK_KEY_V                  86
#define DMK_KEY_W                  87
#define DMK_KEY_X                  88
#define DMK_KEY_Y                  89
#define DMK_KEY_Z                  90
#define DMK_KEY_LEFT_BRACKET       91  /* [ */
#define DMK_KEY_BACKSLASH          92  /* \ */
#define DMK_KEY_RIGHT_BRACKET      93  /* ] */
#define DMK_KEY_GRAVE_ACCENT       96  /* ` */
#define DMK_KEY_WORLD_1            161 /* non-US #1 */
#define DMK_KEY_WORLD_2            162 /* non-US #2 */

#define DMK_KEY_ESCAPE             256
#define DMK_KEY_ENTER              257
#define DMK_KEY_TAB                258
#define DMK_KEY_BACKSPACE          259
#define DMK_KEY_INSERT             260
#define DMK_KEY_DELETE             261
#define DMK_KEY_RIGHT              262
#define DMK_KEY_LEFT               263
#define DMK_KEY_DOWN               264
#define DMK_KEY_UP                 265
#define DMK_KEY_PAGE_UP            266
#define DMK_KEY_PAGE_DOWN          267
#define DMK_KEY_HOME               268
#define DMK_KEY_END                269
#define DMK_KEY_CAPS_LOCK          280
#define DMK_KEY_SCROLL_LOCK        281
#define DMK_KEY_NUM_LOCK           282
#define DMK_KEY_PRINT_SCREEN       283
#define DMK_KEY_PAUSE              284
#define DMK_KEY_F1                 290
#define DMK_KEY_F2                 291
#define DMK_KEY_F3                 292
#define DMK_KEY_F4                 293
#define DMK_KEY_F5                 294
#define DMK_KEY_F6                 295
#define DMK_KEY_F7                 296
#define DMK_KEY_F8                 297
#define DMK_KEY_F9                 298
#define DMK_KEY_F10                299
#define DMK_KEY_F11                300
#define DMK_KEY_F12                301
#define DMK_KEY_F13                302
#define DMK_KEY_F14                303
#define DMK_KEY_F15                304
#define DMK_KEY_F16                305
#define DMK_KEY_F17                306
#define DMK_KEY_F18                307
#define DMK_KEY_F19                308
#define DMK_KEY_F20                309
#define DMK_KEY_F21                310
#define DMK_KEY_F22                311
#define DMK_KEY_F23                312
#define DMK_KEY_F24                313
#define DMK_KEY_F25                314
#define DMK_KEY_KP_0               320
#define DMK_KEY_KP_1               321
#define DMK_KEY_KP_2               322
#define DMK_KEY_KP_3               323
#define DMK_KEY_KP_4               324
#define DMK_KEY_KP_5               325
#define DMK_KEY_KP_6               326
#define DMK_KEY_KP_7               327
#define DMK_KEY_KP_8               328
#define DMK_KEY_KP_9               329
#define DMK_KEY_KP_DECIMAL         330
#define DMK_KEY_KP_DIVIDE          331
#define DMK_KEY_KP_MULTIPLY        332
#define DMK_KEY_KP_SUBTRACT        333
#define DMK_KEY_KP_ADD             334
#define DMK_KEY_KP_ENTER           335
#define DMK_KEY_KP_EQUAL           336
#define DMK_KEY_LEFT_SHIFT         340
#define DMK_KEY_LEFT_CONTROL       341
#define DMK_KEY_LEFT_ALT           342
#define DMK_KEY_LEFT_SUPER         343
#define DMK_KEY_RIGHT_SHIFT        344
#define DMK_KEY_RIGHT_CONTROL      345
#define DMK_KEY_RIGHT_ALT          346
#define DMK_KEY_RIGHT_SUPER        347
#define DMK_KEY_MENU               348

#define DMK_KEY_LAST               DMK_KEY_MENU

	/*! @} */

	/*! @defgroup mods Modifier key flags
	 *  @brief Modifier key flags.
	 *
	 *  See [key input](@ref input_key) for how these are used.
	 *
	 *  @ingroup input
	 *  @{ */

	 /*! @brief If this bit is set one or more Shift keys were held down.
	 *
	 *  If this bit is set one or more Shift keys were held down.
	 */
#define DMK_MOD_SHIFT           0x0001
	 /*! @brief If this bit is set one or more Control keys were held down.
	  *
	  *  If this bit is set one or more Control keys were held down.
	  */
#define DMK_MOD_CONTROL         0x0002
	  /*! @brief If this bit is set one or more Alt keys were held down.
	   *
	   *  If this bit is set one or more Alt keys were held down.
	   */
#define DMK_MOD_ALT             0x0004
	   /*! @brief If this bit is set one or more Super keys were held down.
		*
		*  If this bit is set one or more Super keys were held down.
		*/
#define DMK_MOD_SUPER           0x0008
		/*! @brief If this bit is set the Caps Lock key is enabled.
		 *
		 *  If this bit is set the Caps Lock key is enabled and the @ref
		 *  DMK_LOCK_KEY_MODS input mode is set.
		 */
#define DMK_MOD_CAPS_LOCK       0x0010
		 /*! @brief If this bit is set the Num Lock key is enabled.
		  *
		  *  If this bit is set the Num Lock key is enabled and the @ref
		  *  DMK_LOCK_KEY_MODS input mode is set.
		  */
#define DMK_MOD_NUM_LOCK        0x0020

#define DMK_MOUSE_BUTTON_1         0
#define DMK_MOUSE_BUTTON_2         1
#define DMK_MOUSE_BUTTON_3         2
#define DMK_MOUSE_BUTTON_4         3
#define DMK_MOUSE_BUTTON_5         4
#define DMK_MOUSE_BUTTON_6         5
#define DMK_MOUSE_BUTTON_7         6
#define DMK_MOUSE_BUTTON_8         7
#define DMK_MOUSE_BUTTON_LAST      DMK_MOUSE_BUTTON_8
#define DMK_MOUSE_BUTTON_LEFT      DMK_MOUSE_BUTTON_1
#define DMK_MOUSE_BUTTON_RIGHT     DMK_MOUSE_BUTTON_2
#define DMK_MOUSE_BUTTON_MIDDLE    DMK_MOUSE_BUTTON_3

#define DMK_JOYSTICK_1             0
#define DMK_JOYSTICK_2             1
#define DMK_JOYSTICK_3             2
#define DMK_JOYSTICK_4             3
#define DMK_JOYSTICK_5             4
#define DMK_JOYSTICK_6             5
#define DMK_JOYSTICK_7             6
#define DMK_JOYSTICK_8             7
#define DMK_JOYSTICK_9             8
#define DMK_JOYSTICK_10            9
#define DMK_JOYSTICK_11            10
#define DMK_JOYSTICK_12            11
#define DMK_JOYSTICK_13            12
#define DMK_JOYSTICK_14            13
#define DMK_JOYSTICK_15            14
#define DMK_JOYSTICK_16            15

#define DMK_GAMEPAD_BUTTON_A               0
#define DMK_GAMEPAD_BUTTON_B               1
#define DMK_GAMEPAD_BUTTON_X               2
#define DMK_GAMEPAD_BUTTON_Y               3
#define DMK_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define DMK_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define DMK_GAMEPAD_BUTTON_BACK            6
#define DMK_GAMEPAD_BUTTON_START           7
#define DMK_GAMEPAD_BUTTON_GUIDE           8
#define DMK_GAMEPAD_BUTTON_LEFT_THUMB      9
#define DMK_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define DMK_GAMEPAD_BUTTON_DPAD_UP         11
#define DMK_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define DMK_GAMEPAD_BUTTON_DPAD_DOWN       13
#define DMK_GAMEPAD_BUTTON_DPAD_LEFT       14
#define DMK_GAMEPAD_BUTTON_LAST            DMK_GAMEPAD_BUTTON_DPAD_LEFT

#define DMK_GAMEPAD_BUTTON_CROSS       DMK_GAMEPAD_BUTTON_A
#define DMK_GAMEPAD_BUTTON_CIRCLE      DMK_GAMEPAD_BUTTON_B
#define DMK_GAMEPAD_BUTTON_SQUARE      DMK_GAMEPAD_BUTTON_X
#define DMK_GAMEPAD_BUTTON_TRIANGLE    DMK_GAMEPAD_BUTTON_Y

#define DMK_GAMEPAD_AXIS_LEFT_X        0
#define DMK_GAMEPAD_AXIS_LEFT_Y        1
#define DMK_GAMEPAD_AXIS_RIGHT_X       2
#define DMK_GAMEPAD_AXIS_RIGHT_Y       3
#define DMK_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define DMK_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define DMK_GAMEPAD_AXIS_LAST          DMK_GAMEPAD_AXIS_RIGHT_TRIGGER
}
