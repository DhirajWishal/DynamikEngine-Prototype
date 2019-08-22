#include "tbafx.h"
#include "loader.h"

#define DMK_PLATFORM_WINDOWS
#include "Dynamik.h"

class charlet : public Dynamik::GameObject {
public:
	charlet(Dynamik::GameObjectProperties props) : GameObject(props), myProps(props) {

	}
	~charlet() {}

private:
	Dynamik::GameObjectProperties myProps;
};

Dynamik::InputContainer Dynamik::submit() {

	Dynamik::KeyBindings myBindings;

	Dynamik::InputContainer myContainer;

	Dynamik::GameObjectProperties props;
	props.name = "Charlet";
	props.ID = "0001";
	props.type = Dynamik::MODEL;
	props.objPath = "E:/Projects/Dynamik Engine/Dynamik/core assets/models/chalet.obj";
	props.texPath = "E:/Projects/Dynamik Engine/Dynamik/core assets/textures/chalet.jpg";
	props.audioProperties.clipPath = "NONE";
	props.behaviourProperties.type = Dynamik::PHYSICAL_BODY;

	charlet myCharlet(props);

	//Dynamik::GameObjectProperties props2;
	//props.name = "HandGun";
	//props.ID = "0002";
	//props.type = Dynamik::MODEL;
	//props.objPath = "E:/Projects/Dynamik Engine/Dynamik/core assets/blend/Handgun_Packed.blend";
	//props.texPath = "E:/Projects/Dynamik Engine/Dynamik/core assets/blend/textures/handgun_C.jpg";
	//props.audioProperties.clipPath = "NONE";
	//props.behaviourProperties.type = Dynamik::PHYSICAL_BODY;

	charlet myCharlet2(props);

	myContainer.addToAssetsQueue(myCharlet);
	myContainer.addToAssetsQueue(myCharlet2);

	//Dynamik::submit(submit);

	return myContainer;
}
