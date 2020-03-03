#pragma once

#ifndef _DYNAMIK_MANAGERS_WINDOWS_OBJECT_LOADER_H
#define _DYNAMIK_MANAGERS_WINDOWS_OBJECT_LOADER_H

namespace Dynamik {
	namespace resources {
		//struct ObjectData {
		//	tinyobj::attrib_t attributes;
		//	std::vector<tinyobj::shape_t> shapes;
		//	std::vector<tinyobj::material_t> materials;
		//	std::string warnings, errors;
		//
		//	bool loadObject(std::string path);
		//};

		template<typename Alloc>
		void loadObject(std::string& path, std::vector<Alloc>* vertices, std::vector<uint32_t>* indices);
	}
}

#endif // !_DYNAMIK_MANAGERS_OBJECT_LOADER_H
