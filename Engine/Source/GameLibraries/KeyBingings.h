#pragma once

#ifndef _DYNAMIK_KEY_BINDINGS_H
#define _DYNAMIK_KEY_BINDINGS_H

#include "Events/keyCodes.h"


namespace Dynamik {
	/* KEY BINDINGS FOR THE DYNAMIK ENGINE */
	struct KeyBindings {
		/* WEAPON BINDINGS */
		unsigned int fireWeapon = DMK_KEY_UNKNOWN;
		unsigned int aimWeapon = DMK_KEY_UNKNOWN;
		unsigned int changeWeapon_1 = DMK_KEY_UNKNOWN;
		unsigned int changeWeapon_2 = DMK_KEY_UNKNOWN;
		unsigned int changeWeapon_3 = DMK_KEY_UNKNOWN;
		unsigned int changeToMlee = DMK_KEY_UNKNOWN;
		unsigned int changeMlee_1 = DMK_KEY_UNKNOWN;
		unsigned int changeMlee_2 = DMK_KEY_UNKNOWN;
		unsigned int changeMlee_3 = DMK_KEY_UNKNOWN;
		unsigned int mleeAttack = DMK_KEY_UNKNOWN;
		unsigned int reloadWeapon = DMK_KEY_UNKNOWN;
		unsigned int pickWeapon = DMK_KEY_UNKNOWN;
		unsigned int upgreadWeapon = DMK_KEY_UNKNOWN;
		unsigned int throwWeapon = DMK_KEY_UNKNOWN;

		/* CHARACTER CONSTOL BINDINGS */
		unsigned int walkForward = DMK_KEY_UNKNOWN;
		unsigned int walkBackward = DMK_KEY_UNKNOWN;
		unsigned int walkLeft = DMK_KEY_UNKNOWN;
		unsigned int walkRight = DMK_KEY_UNKNOWN;
		unsigned int sprintKey = DMK_KEY_UNKNOWN;
		unsigned int crouch = DMK_KEY_UNKNOWN;
		unsigned int prone = DMK_KEY_UNKNOWN;
		unsigned int leanLeft = DMK_KEY_UNKNOWN;
		unsigned int leanRight = DMK_KEY_UNKNOWN;
		unsigned int jump = DMK_KEY_UNKNOWN;
		unsigned int holdBreath = DMK_KEY_UNKNOWN;

		/* VEHICLE CONTROL BINDINGS */
		unsigned int driveForward = DMK_KEY_UNKNOWN;
		unsigned int driveBackward = DMK_KEY_UNKNOWN;
		unsigned int driveLeft = DMK_KEY_UNKNOWN;
		unsigned int driveRight = DMK_KEY_UNKNOWN;
		unsigned int brake = DMK_KEY_UNKNOWN;
		unsigned int getInVehicle = DMK_KEY_UNKNOWN;
		unsigned int getOutVehicle = DMK_KEY_UNKNOWN;
		unsigned int horn = DMK_KEY_UNKNOWN;
		unsigned int radio = DMK_KEY_UNKNOWN;
		unsigned int boost = DMK_KEY_UNKNOWN;

		/* OTHER CONTROL BINDINGS */
		unsigned int useHeal = DMK_KEY_UNKNOWN;
		unsigned int drink = DMK_KEY_UNKNOWN;

		/* CORE BINDINGS */
		unsigned int openChat = DMK_KEY_UNKNOWN;
		unsigned int openDevCon = DMK_KEY_UNKNOWN;
		unsigned int sendMessage = DMK_KEY_UNKNOWN;

		/* DEFAULT CONSTRUCTOR */
		KeyBindings(
			unsigned int fireWeapon = DMK_MOUSE_BUTTON_LEFT,
			unsigned int aimWeapon = DMK_MOUSE_BUTTON_RIGHT,
			unsigned int changeWeapon1 = DMK_KEY_1,
			unsigned int changeWeapon2 = DMK_KEY_2,
			unsigned int changeWeapon3 = DMK_KEY_3,
			unsigned int changeToMlee = DMK_KEY_V,
			unsigned int changeMlee1 = DMK_KEY_4,
			unsigned int changeMlee2 = DMK_KEY_5,
			unsigned int changeMlee3 = DMK_KEY_6,
			unsigned int mleeAttack = DMK_KEY_V,
			unsigned int reloadWeapon = DMK_KEY_R,
			unsigned int pickWeapon = DMK_KEY_F,
			unsigned int upgreadWeapon = DMK_KEY_TAB,
			unsigned int throwWeapon = DMK_KEY_T,

			unsigned int walkFront = DMK_KEY_W,
			unsigned int walkBack = DMK_KEY_S,
			unsigned int walkLeft = DMK_KEY_A,
			unsigned int walkRight = DMK_KEY_D,
			unsigned int sprint = DMK_KEY_LEFT_SHIFT,
			unsigned int crouch = DMK_KEY_C,
			unsigned int prone = DMK_KEY_Z,
			unsigned int leanLeft = DMK_KEY_Q,
			unsigned int leanRight = DMK_KEY_E,
			unsigned int jump = DMK_KEY_SPACE,
			unsigned int holdBreath = DMK_KEY_LEFT_SUPER,

			unsigned int driveFront = DMK_KEY_UP,
			unsigned int driveBack = DMK_KEY_DOWN,
			unsigned int driveLeft = DMK_KEY_LEFT,
			unsigned int driveRight = DMK_KEY_RIGHT,
			unsigned int brake = DMK_KEY_SPACE,
			unsigned int getIn = DMK_KEY_F,
			unsigned int getOut = DMK_KEY_F,
			unsigned int horn = DMK_KEY_H,
			unsigned int radio = DMK_KEY_R,
			unsigned int boost = DMK_KEY_LEFT_SHIFT,

			unsigned int useHeal = DMK_KEY_H,
			unsigned int drink = DMK_KEY_U,

			unsigned int chat = DMK_KEY_L,
			unsigned int console = DMK_KEY_GRAVE_ACCENT,
			unsigned int send = DMK_KEY_ENTER
			) : fireWeapon(fireWeapon), aimWeapon(aimWeapon), changeWeapon_1(changeWeapon1),
			changeWeapon_2(changeWeapon2), changeWeapon_3(changeWeapon3), changeToMlee(changeToMlee),
			changeMlee_1(changeMlee1), changeMlee_2(changeMlee2), changeMlee_3(changeMlee3),
			mleeAttack(mleeAttack), reloadWeapon(reloadWeapon), pickWeapon(pickWeapon),
			upgreadWeapon(upgreadWeapon), throwWeapon(throwWeapon),
			walkForward(walkFront), walkBackward(walkBack), walkLeft(walkLeft), walkRight(walkRight),
			sprintKey(sprint), crouch(crouch), prone(prone), leanLeft(leanLeft), leanRight(leanRight),
			jump(jump), holdBreath(holdBreath),
			useHeal(useHeal), drink(drink),
			openChat(chat), openDevCon(console), sendMessage(send)
		{
		}
	};
}

#endif // !_DYNAMIK_KEY_BINDINGS_H
