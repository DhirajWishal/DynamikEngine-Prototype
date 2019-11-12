#include "tbafx.h"
#include "loader.h"

#define DMK_DEBUG

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

std::vector<char> readFile(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		std::runtime_error("Failed to Open file!");

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();
	return buffer;
}

std::vector<std::string> getTexturePath(std::vector<char>& file) {
	std::vector<std::string> paths;
	std::string path;

	for (auto i : file)
		path.push_back(i);

	std::istringstream iss;
	iss.str(path);

	for (std::string i; std::getline(iss, i);) {
		std::replace(i.begin(), i.end(), '\r', ' ');
		std::replace(i.begin(), i.end(), (char)'\"', ' ');

		if (i[0] == ' ') i.replace(0, 1, "");
		if (i[i.size() - 1] == ' ') i.replace(i.size() - 1, 1, "");

		paths.push_back(i);
	}

	return paths;
}

void loadPaths() {
}

// final
Dynamik::InputContainer Dynamik::submit() {
	auto tex = getTexturePath(readFile("E:/Projects/Dynamik Engine/Dynamik/components/Core/paths.txt"));
	auto mod = getTexturePath(readFile("E:/Projects/Dynamik Engine/Dynamik/components/Core/model.txt"));

	Dynamik::KeyBindings myBindings;

	Dynamik::InputContainer myContainer;

	Dynamik::GameObjectProperties props;
	props.name = "Charlet";
	props.ID = "0001";
	props.type = Dynamik::MODEL;
	props.objPath = mod[0];
	props.texPath = tex[0];
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