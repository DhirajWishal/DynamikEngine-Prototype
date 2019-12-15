#pragma once

#ifndef _DYNAMIK_APPLICATION_IMPORTS_H
#define _DYNAMIK_APPLICATION_IMPORTS_H

DMK_API Dynamik::int32 Dynamik::utils::max(Dynamik::int32, Dynamik::int32);
DMK_API Dynamik::uint32 Dynamik::utils::max(Dynamik::uint32, Dynamik::uint32);
DMK_API float Dynamik::utils::max(float, float);

class charlet : public Dynamik::GameObject {
public:
	charlet(Dynamik::GameObjectProperties props) : GameObject(props) {
	}
	~charlet() {}
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
//Dynamik::InputContainer Dynamik::submit() {
//	std::vector<std::string>(*function)(std::vector<char>&) = getTexturePath;
//
//	//auto paths = getTexturePath(readFile("paths.txt"));
//	//auto paths = getTexturePath(readFile("C:/Dynamik Engine/Engine/paths.txt"));
//	auto paths = getTexturePath(readFile("E:/Projects/Dynamik Engine/Dynamik/Application/paths.txt"));
//
//	/*
//		TODO:
//			Dynamic resource paths
//			Move the Manager definition to the CDH for centralized data distrubution
//				Render data abstraction
//	*/
//
//	std::vector<Dynamik::GameObject*> gameObjects = {};
//
//	std::vector<std::vector<float>> locations = {
//		{0.0f,	0.0f,	0.0f},
//		{0.0f,	5.0f,	0.0f},
//		{0.0f,	-5.0f,	0.0f},
//		{0.0f,	10.0f,	0.0f},
//		{0.0f,	-10.0f,	0.0f},
//
//		{5.0f,	0.0f,	0.0f},
//		{5.0f,	5.0f,	0.0f},
//		{5.0f,	-5.0f,	0.0f},
//		{5.0f,	10.0f,	0.0f},
//		{5.0f,	-10.0f,	0.0f},
//
//		{-5.0f,	0.0f,	0.0f},
//		{-5.0f,	5.0f,	0.0f},
//		{-5.0f,	-5.0f,	0.0f},
//		{-5.0f,	10.0f,	0.0f},
//		{-5.0f,	-10.0f,	0.0f},
//
//		{10.0f,	0.0f,	0.0f},
//		{10.0f,	5.0f,	0.0f},
//		{10.0f,	-5.0f,	0.0f},
//		{10.0f,	10.0f,	0.0f},
//		{10.0f,	-10.0f,	0.0f},
//
//		{-10.0f,	0.0f,	0.0f},
//		{-10.0f,	5.0f,	0.0f},
//		{-10.0f,	-5.0f,	0.0f},
//		{-10.0f,	10.0f,	0.0f},
//		{-10.0f,	-10.0f,	0.0f},
//	};
//
//	std::vector<DMKObjectType> objTypes = {
//		DMK_OBJECT_TYPE_INTERACTIVE_OBJECT,
//		DMK_OBJECT_TYPE_INTERACTIVE_OBJECT,
//		DMK_OBJECT_TYPE_STATIC_OBJECT,
//		DMK_OBJECT_TYPE_AI,
//		DMK_OBJECT_TYPE_STATIC_OBJECT,
//	};
//
//	for (int i = 0; i < paths.size(); i++) {
//		Dynamik::GameObjectProperties props;
//		props.name = "Charlet";
//		props.ID = std::to_string(i);
//		props.location = paths[i];
//		props.transformProperties.location = locations[i];
//		//props.type = objTypes[i];
//
//		gameObjects.push_back(charlet(props));
//	}
//
//	return myContainer;
//}

#endif // !_DYNAMIK_ADGR_IMPORTS_H
