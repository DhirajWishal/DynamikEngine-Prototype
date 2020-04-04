#include "adgrafx.h"
#include "Camera3D.h"

namespace Dynamik {
	namespace ADGR {
		UniformBufferObject Camera3D::updateCamera(std::deque<DMKEventContainer> container, DMKUpdateInfo updateInfo)
		{
			float FOV = updateInfo.fieldOfView;
			float angelX = updateInfo.rotationX;
			float angelY = updateInfo.rotationY;
			float angelZ = updateInfo.rotationZ;

			if (updateInfo.useRadians)
			{
				FOV = glm::radians(updateInfo.fieldOfView);
				angelX = glm::radians(updateInfo.rotationX);
				angelY = glm::radians(updateInfo.rotationY);
				angelZ = glm::radians(updateInfo.rotationZ);
			}

			const float cameraSpeed = 0.05f; // adjust accordingly
			Dynamik::ARRAY<std::future<void>, DMKArrayDestructorCallMode::DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_ALL> threads;

			for (int i = 0; i < container.size(); i++) {
				DMKEventContainer eventContainer = container.back();
				container.pop_back();

				if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_PRESS)
					switch (eventContainer.code) {
					case DMK_KEY_W:
						cameraPos += cameraSpeed * cameraFront;
						break;
					case DMK_KEY_A:
						cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
						break;
					case DMK_KEY_S:
						cameraPos -= cameraSpeed * cameraFront;
						break;
					case DMK_KEY_D:
						cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
						break;
					case DMK_KEY_UP:
						break;
					case DMK_KEY_DOWN:
						break;
					case DMK_KEY_LEFT:
						break;
					case DMK_KEY_RIGHT:
						break;
					}
				else if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT)
					switch (eventContainer.code) {
					case DMK_KEY_W:
						cameraPos += cameraSpeed * cameraFront;
						break;
					case DMK_KEY_A:
						cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
						break;
					case DMK_KEY_S:
						cameraPos -= cameraSpeed * cameraFront;
						break;
					case DMK_KEY_D:
						cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
						break;
					case DMK_KEY_UP:
						break;
					case DMK_KEY_DOWN:
						break;
					case DMK_KEY_LEFT:
						break;
					case DMK_KEY_RIGHT:
						break;
					}
			}

			glm::mat4 _rotationX = glm::rotate(
				glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
					//glm::vec3(updateInfo.leftRight, updateInfo.frontBack, updateInfo.upDown)),
				angelX,
				glm::vec3(0.0f, 0.0f, 1.0f));

			glm::mat4 _rotationY = glm::rotate(
				glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
				angelY,
				glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 _rotationZ = glm::rotate(
				glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
				angelZ,
				glm::vec3(1.0f, 0.0f, 0.0f));

			UniformBufferObject ubo = {};
			ubo.model = _rotationX * _rotationY * _rotationZ;
			/*
			ubo.view = glm::lookAt(
				glm::vec3(0.5f, 3.0f, 0.5f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f));
				*/

			ubo.view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

			ubo.proj = glm::perspective(
				FOV,
				updateInfo.frameWidth / updateInfo.frameHeight,
				updateInfo.near,
				updateInfo.far);

			ubo.proj[1][1] *= -1;

			return ubo;
		}
	}
}