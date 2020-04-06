#include "adgrafx.h"
#include "CameraReflect.h"

namespace Dynamik {
	namespace ADGR {
		UBO_MPL CameraReflect::updateCamera(std::deque<DMKEventContainer> container, DMKUpdateInfo updateInfo)
		{
			float angelX = updateInfo.rotationX;
			float angelY = updateInfo.rotationY;
			float angelZ = updateInfo.rotationZ;

			const float cameraSpeed = 0.05f; // adjust accordingly

			for (int i = 0; i < container.size(); i++) {
				DMKEventContainer eventContainer = container.back();
				container.pop_back();

				if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_PRESS)
				{
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
						cameraPos += cameraSpeed * cameraUp;
						break;
					case DMK_KEY_DOWN:
						cameraPos -= cameraSpeed * cameraUp;
						break;
					case DMK_KEY_LEFT:
						bias += 0.5f;
						break;
					case DMK_KEY_RIGHT:
						bias -= 0.5f;
						break;
					}
				}
				else if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT)
				{
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
						cameraPos += cameraSpeed * cameraUp;
						break;
					case DMK_KEY_DOWN:
						cameraPos -= cameraSpeed * cameraUp;
						break;
					case DMK_KEY_LEFT:
						break;
					case DMK_KEY_RIGHT:
						break;
					}
				}
				else if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_MOUSE_MOVED)
				{
					_rotX = eventContainer.xAxis;
					_rotY = eventContainer.yAxis;
				}
			}

			if (updateInfo.useRadians)
			{
				updateInfo.fieldOfView = glm::radians(updateInfo.fieldOfView);
				angelX = glm::radians(_rotX);
				angelY = glm::radians(_rotY);
				angelZ = glm::radians(updateInfo.rotationZ);
			}

			glm::mat4 viewMatrix = glm::mat4(1.0f);
			UBO_MPL uboVS;
			uboVS.proj = glm::perspective(glm::radians(60.0f), updateInfo.aspectRatio, 0.001f, 256.0f);
			viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, bias));

			uboVS.model = glm::mat4(1.0f);
			uboVS.model = viewMatrix * glm::translate(uboVS.model, cameraPos);
			uboVS.model = glm::rotate(uboVS.model, glm::radians(angelX), glm::vec3(1.0f, 0.0f, 0.0f));
			uboVS.model = glm::rotate(uboVS.model, glm::radians(angelY), glm::vec3(0.0f, 1.0f, 0.0f));
			uboVS.model = glm::rotate(uboVS.model, glm::radians(angelZ), glm::vec3(0.0f, 0.0f, 1.0f));
			uboVS.proj[1][1] *= -1;

			return uboVS;
		}
	}
}