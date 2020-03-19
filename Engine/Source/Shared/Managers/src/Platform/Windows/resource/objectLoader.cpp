/*
 3D object loader for Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		22/12/2019
 IDE:		MS Visual Studio Community 2019

 objectLoader.cpp file
*/

#include "mngafx.h"
#include "objectLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "core/log.h"

#define _IS_SPACE(character)			(character == ' ')
#define _IS_CHARACTER(character, test)	(character == test)

#define _FIRST_INDEX					0
#define _DEFAULT_POSITION				1.0f
#define _DEFAULT_TEXTURE_COORDINATE		0.0f
#define _DEFAULT_NORMAL					1.0f
#define _DEFAULT_SPACE					1.0f
#define _DEFAULT_EMPTY_LINE				2

#define _NEXT_LINE						std::getline(_objectFile, _line, '\n')

namespace Dynamik {
	namespace resources {
		//bool ObjectData::loadObject(std::string path) {
		//	return tinyobj::LoadObj(&attributes, &shapes, &materials, &warnings, &errors, path.c_str());
		//}

		template<typename Alloc>
		void loadObject(std::string& path, ARRAY<Alloc>* vertices, ARRAY<uint32_t>* indices) {
			tinyobj::attrib_t attrib;
			ARRAY<tinyobj::shape_t> shapes;
			ARRAY<tinyobj::material_t> materials;
			std::string warn, err;

			if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
				DMK_CORE_FATAL(warn + err);

			std::unordered_map<Alloc, uint32_t> uniqueVertices = {};

			for (const auto& shape : shapes) {
				for (const auto& index : shape.mesh.indices) {
					Alloc vertex = {};

					vertex.pos = {
						myObject.attributes.vertices[3 * index.vertex_index + 0],
						myObject.attributes.vertices[3 * index.vertex_index + 1],
						myObject.attributes.vertices[3 * index.vertex_index + 2]
					};

					vertex.texCoord = {
						myObject.attributes.texcoords[2 * index.texcoord_index + 0],
						1.0f - myObject.attributes.texcoords[2 * index.texcoord_index + 1]
					};

					vertex.color = { 1.0f, 1.0f, 1.0f };

					if (uniqueVertices.count(vertex) == 0) {
						uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
						vertices.push_back(vertex);
					}

					indices.push_back(uniqueVertices[vertex]);
				}
			}
		}
	}
	namespace utils {
		// Object load function
		ARRAY<DMKObjectDataContainer> loadObjFileData(std::string_view path) {
			std::ifstream myObjectFile(path.data());
			if (!myObjectFile)
				return ARRAY<DMKObjectDataContainer>();

			ARRAY<DMKObjectDataContainer> _containers = {};
			uint32_t _objectIndex = 0;
			ARRAY<float> _positionData = {};
			ARRAY<float> _texData = {};
			ARRAY<float> _normalData = {};
			ARRAY<float> _PSData = {};
			ARRAY<uint32_t> _lineElementsData = {};
			ARRAY<std::string> _splits = {};
			ARRAY<std::string> _subSplits = {};

			bool _singePass = true;
			uint32_t _vertexTotalCount = 0;
			uint32_t _vertexCount = 0;

			std::string _line = "";
			_containers.push_back(DMKObjectDataContainer{});
			while (myObjectFile.getline(_line.data(), '\n')) {
				uint32_t _lineSize = _line.size();

				if (_lineSize < _DEFAULT_EMPTY_LINE) continue; // empty line
				else if (_line[_FIRST_INDEX] == '#') continue;	// comment
				else if (_line[_FIRST_INDEX] == 'o') { // new object
					if (!_singePass) {
						_containers.push_back(DMKObjectDataContainer{});
						_objectIndex++;
					}
					_singePass = false;
					continue;
				}
				else if (_line[_FIRST_INDEX] == 'v') {	// vertex data
					if (_IS_SPACE(_line[1])) {
						_positionData.resize(4, _DEFAULT_POSITION);
						_splits = splitLine(_line.substr(2, (_lineSize - 2)).c_str(), ' ');
						for (int i = 0; i < _splits.size(); i++)
							_positionData[i] = std::stof(_splits[i]);

						_containers[_objectIndex].myVertices.push_back(_positionData);
						_vertexTotalCount++;
						continue;
					}
					else if (_line[_FIRST_INDEX] == 't') {	// texture coordinates
						_texData.resize(3, _DEFAULT_TEXTURE_COORDINATE);
						_splits = splitLine(_line.substr(3, (_lineSize - 3)).c_str(), ' ');
						for (int i = 0; i < _splits.size(); i++)
							_texData[i] = std::stof(_splits[i]);

						_containers[_objectIndex].myTextureCoordinates.push_back(_texData);
						continue;
					}
					else if (_line[1] == 'n') {	// vertex normals
						_normalData.resize(3, _DEFAULT_NORMAL);
						_splits = splitLine(_line.substr(3, (_lineSize - 3)).c_str(), ' ');
						for (int i = 0; i < _splits.size(); i++)
							_normalData[i] = std::stof(_splits[i]);

						_containers[_objectIndex].myNormals.push_back(_normalData);
						continue;
					}
					else if (_line[1] == 'p') {	// parameter spaces
						_PSData.resize(3, _DEFAULT_SPACE);
						_splits = splitLine(_line.substr(3, (_lineSize - 3)).c_str(), ' ');
						for (int i = 0; i < _splits.size(); i++)
							_PSData[i] = std::stof(_splits[i]);

						_containers[_objectIndex].myParameterSpaces.push_back(_PSData);
						continue;
					}
				}
				else if (_line[_FIRST_INDEX] == 'f') {	// face elements
					_splits = splitLine(_line.substr(2, (_lineSize - 2)).c_str(), ' ');
					for (std::string_view _split : _splits) {
						if (_split.find('/') != std::string::npos) {
							_subSplits = splitLine(_split, '/');
							if (_subSplits.size() >= 1)
								_containers[_objectIndex].myVertexIndex.push_back(std::stoi(_subSplits[0]) - 1);
							if (_subSplits.size() >= 2)
								_containers[_objectIndex].myTextureCoordinateIndex.push_back(std::stoi(_subSplits[1]) - 1);
							if (_subSplits.size() >= 3)
								_containers[_objectIndex].myNormalIndex.push_back(std::stoi(_subSplits[2]) - 1);
						}
						else
							if (_split.size() >= 1)
								_containers[_objectIndex].myVertexIndex.push_back(std::stoi(_split.data()) - 1);
						_containers[_objectIndex].myIndexCount++;
					}

					continue;
				}
				else if (_line[_FIRST_INDEX] == 'l') {	// line elements
					_splits = splitLine(_line.substr(2, (_lineSize - 2)).c_str(), ' ');
					for (std::string_view _split : _splits)
						_lineElementsData.push_back(std::stoi(_split.data()));

					_containers[_objectIndex].myLineElements.push_back(_lineElementsData);
					continue;
				}
				else if (_line[_FIRST_INDEX] == 'm') {	// MTL file reference
					if (_line.find("mtllib") == std::string::npos) continue;
					_containers[_objectIndex].myMTLFile.push_back(_line.substr(7, _lineSize - 7));
					continue;
				}
			}

			myObjectFile.close();
			return _containers;
		}

		void loadObjFileData(InternalFormat* myInternalFormat) {
			uint32_t _objectCount = myInternalFormat->myGameObject->myProperties.objectPath.size();
			myInternalFormat->myVertexBufferObjects.resize(_objectCount);
			myInternalFormat->myVertexCounts.resize(_objectCount);
			myInternalFormat->myIndexBufferObjects.resize(_objectCount);
			myInternalFormat->myIndexCounts.resize(_objectCount);

			for (uint32_t _itr = 0; _itr < _objectCount; _itr++) {
				std::ifstream _objectFile(myInternalFormat->myGameObject->myProperties.objectPath[_itr]);
				if (!_objectFile)
					continue;

				ARRAY<DMKObjectDataContainer> _containers = {};
				uint32_t _localIndex = 0;
				uint32_t _objectIndex = 0;
				ARRAY<float> _positionData = {};
				ARRAY<float> _texData = {};
				ARRAY<float> _normalData = {};
				ARRAY<float> _PSData = {};
				ARRAY<uint32_t> _lineElementsData = {};
				ARRAY<std::string> _splits = {};
				ARRAY<std::string> _subSplits = {};

				bool _singePass = true;
				uint32_t _vertexTotalCount = 0;
				uint32_t _vertexCount = 0;

				std::string _line = "";
				_containers.push_back(DMKObjectDataContainer{});
				while (_objectFile.getline(_line.data(), '\n')) {
					uint32_t _lineSize = _line.size();
					Vertex _localVertex = {};

					if (_lineSize < _DEFAULT_EMPTY_LINE) continue; // empty line
					else if (_line[_FIRST_INDEX] == '#') continue;	// comment
					else if (_line[_FIRST_INDEX] == 'o') { // new object
						if (!_singePass) {
							_containers.push_back(DMKObjectDataContainer{});
							_objectIndex++;
						}
						_singePass = false;
						continue;
					}
					else if (_line[_FIRST_INDEX] == 'v') {	// vertex data
						if (_IS_SPACE(_line[1])) {
							_positionData.resize(4, _DEFAULT_POSITION);
							_splits = splitLine(_line.substr(2, (_lineSize - 2)).c_str(), ' ');
							for (int i = 0; i < _splits.size(); i++)
								_positionData[i] = std::stof(_splits[i]);

							//_containers[_objectIndex].myVertices.push_back(_positionData);
							_localVertex.Position = {
								_positionData[0],
								_positionData[1],
								_positionData[2]
							};
							myInternalFormat->myVertexBufferObjects[_itr].push_back(_localVertex);
							_vertexTotalCount++;
							continue;
						}
						else if (_line[1] == 't') {	// texture coordinates
							_texData.resize(3, _DEFAULT_TEXTURE_COORDINATE);
							_splits = splitLine(_line.substr(3, (_lineSize - 3)).c_str(), ' ');
							for (int i = 0; i < _splits.size(); i++)
								_texData[i] = std::stof(_splits[i]);

							_containers[_objectIndex].myTextureCoordinates.push_back(_texData);
							continue;
						}
						else if (_line[1] == 'n') {	// vertex normals
							_normalData.resize(3, _DEFAULT_NORMAL);
							_splits = splitLine(_line.substr(3, (_lineSize - 3)).c_str(), ' ');
							for (int i = 0; i < _splits.size(); i++)
								_normalData[i] = std::stof(_splits[i]);

							_containers[_objectIndex].myNormals.push_back(_normalData);
							continue;
						}
						else if (_line[1] == 'p') {	// parameter spaces
							_PSData.resize(3, _DEFAULT_SPACE);
							_splits = splitLine(_line.substr(3, (_lineSize - 3)).c_str(), ' ');
							for (int i = 0; i < _splits.size(); i++)
								_PSData[i] = std::stof(_splits[i]);

							_containers[_objectIndex].myParameterSpaces.push_back(_PSData);
							continue;
						}
					}
					else if (_line[_FIRST_INDEX] == 'f') {	// face elements
						_localVertex.Color = {
							1.0f, 1.0f, 1.0f
						};
						_splits = splitLine(_line.substr(2, (_lineSize - 2)).c_str(), ' ');
						for (std::string_view _split : _splits) {
							if (_split.find('/') != std::string::npos) {
								_subSplits = splitLine(_split, '/');
								if (_subSplits.size() >= 1) {
									_localIndex = std::stoi(_subSplits[0]) - ((_objectIndex) ? _containers[_objectIndex - 1].myVertices.size() + 1 : 1);
									if (_localIndex)
										_localVertex.Position = {
											_containers[_objectIndex].myVertices[_localIndex][0],
											_containers[_objectIndex].myVertices[_localIndex][1],
											_containers[_objectIndex].myVertices[_localIndex][2]
									};
									else
										_localVertex.Position = { 1.0f, 1.0f, 1.0f };
								}
								if (_subSplits.size() >= 2) {
									_localIndex = std::stoi(_subSplits[1]) - ((_objectIndex) ? _containers[_objectIndex - 1].myVertices.size() + 1 : 1);
									if (_localIndex)
										_localVertex.TexCoordinates = {
											_containers[_objectIndex].myTextureCoordinates[_localIndex][0],
											1.0f - _containers[_objectIndex].myTextureCoordinates[_localIndex][1]
									};
									else
										_localVertex.TexCoordinates = { 0.5f, 0.5f };
								}
								if (_subSplits.size() >= 3) {
									_localIndex = std::stoi(_subSplits[2]) - ((_objectIndex) ? _containers[_objectIndex - 1].myVertices.size() + 1 : 1);
								}
							}
							else
								if (_split.size() >= 1) {
									_localIndex = std::stoi(_split.data()) - ((_objectIndex) ? _containers[_objectIndex - 1].myVertices.size() + 1 : 1);
									if (_localIndex)
										_localVertex.Position = {
											_containers[_objectIndex].myVertices[_localIndex][0],
											_containers[_objectIndex].myVertices[_localIndex][1],
											_containers[_objectIndex].myVertices[_localIndex][2]
									};
									else
										_localVertex.Position = { 1.0f, 1.0f, 1.0f };
								}

							_containers[_objectIndex].myIndexCount++;
							myInternalFormat->myVertexBufferObjects[_itr].push_back(_localVertex);
						}

						continue;
					}
					else if (_line[_FIRST_INDEX] == 'l') {	// line elements
						_splits = splitLine(_line.substr(2, (_lineSize - 2)).c_str(), ' ');
						for (std::string_view _split : _splits)
							_lineElementsData.push_back(std::stoi(_split.data()));

						_containers[_objectIndex].myLineElements.push_back(_lineElementsData);
						continue;
					}
					else if (_line[_FIRST_INDEX] == 'm') {	// MTL file reference
						if (_line.find("mtllib") == std::string::npos) continue;
						_containers[_objectIndex].myMTLFile.push_back(_line.substr(7, _lineSize - 7));
						continue;
					}
				}

				_objectFile.close();
			}
		}
	}
}