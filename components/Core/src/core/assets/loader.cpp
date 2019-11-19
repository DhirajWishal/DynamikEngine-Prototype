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

// final
Dynamik::InputContainer Dynamik::submit() {
	std::vector<std::string> (*function)(std::vector<char>&) = getTexturePath;

	auto paths = getTexturePath(readFile("E:/Projects/Dynamik Engine/Dynamik/components/Core/paths.txt"));

	Dynamik::KeyBindings myBindings;

	Dynamik::InputContainer myContainer;

	std::vector<std::vector<float>> locations = {
		{0.0f,	0.0f,	0.0f},
		{0.0f,	5.0f,	0.0f},
		{0.0f,	-5.0f,	0.0f},
		{0.0f,	10.0f,	0.0f},
		{0.0f,	-10.0f,	0.0f},

		{5.0f,	0.0f,	0.0f},
		{5.0f,	5.0f,	0.0f},
		{5.0f,	-5.0f,	0.0f},
		{5.0f,	10.0f,	0.0f},
		{5.0f,	-10.0f,	0.0f},

		{-5.0f,	0.0f,	0.0f},
		{-5.0f,	5.0f,	0.0f},
		{-5.0f,	-5.0f,	0.0f},
		{-5.0f,	10.0f,	0.0f},
		{-5.0f,	-10.0f,	0.0f},

		{10.0f,	0.0f,	0.0f},
		{10.0f,	5.0f,	0.0f},
		{10.0f,	-5.0f,	0.0f},
		{10.0f,	10.0f,	0.0f},
		{10.0f,	-10.0f,	0.0f},

		{-10.0f,	0.0f,	0.0f},
		{-10.0f,	5.0f,	0.0f},
		{-10.0f,	-5.0f,	0.0f},
		{-10.0f,	10.0f,	0.0f},
		{-10.0f,	-10.0f,	0.0f},
	};

	for (int i = 0; i < paths.size(); i++) {
		Dynamik::GameObjectProperties props;
		props.name = "Charlet";
		props.ID = std::to_string(i);
		props.location = paths[i];
		props.transformProperties.location = locations[i];

		myContainer.assets.push_back(charlet(props));
	}

	return myContainer;
}