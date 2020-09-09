#include "xeonpch.h"

#include "XEON/renderer/orthographic_camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace XEON {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float n, float f) 
		: projectionMatrix(glm::ortho(left, right, bottom, top, n, f)), viewMatrix(1.0F), viewProjectionMatrix(projectionMatrix * viewMatrix), position(glm::vec3(0.0F, 0.0F, 0.0F)) {}

	void OrthographicCamera::setProjection(float left, float right, float bottom, float top, float n, float f) {
		projectionMatrix = glm::ortho(left, right, bottom, top, n, f);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	void OrthographicCamera::recalculateViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0F), position) * glm::rotate(glm::mat4(1.0F), glm::radians(zRotation), glm::vec3(0, 0, 1));
		viewMatrix = glm::inverse(transform);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

}
