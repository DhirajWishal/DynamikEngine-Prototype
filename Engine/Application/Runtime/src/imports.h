#pragma once

#ifndef _DYNAMIK_APPLICATION_IMPORTS_H
#define _DYNAMIK_APPLICATION_IMPORTS_H

#include "Dynamik.h"

Dynamik::I32 Dynamik::utils::max(Dynamik::I32, Dynamik::I32);
Dynamik::UI32 Dynamik::utils::max(Dynamik::UI32, Dynamik::UI32);
float Dynamik::utils::max(float, float);

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

	for (std::string i; iss.getline(i.data(), '\n');) {
		std::replace(i.begin(), i.end(), '\r', ' ');
		std::replace(i.begin(), i.end(), (char)'\"', ' ');

		if (i[0] == ' ') i.replace(0, 1, "");
		if (i[i.size() - 1] == ' ') i.replace(i.size() - 1, 1, "");

		paths.push_back(i);
	}

	return paths;
}

#endif // !_DYNAMIK_ADGR_IMPORTS_H
