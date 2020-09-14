#pragma once

#include <glm/glm.hpp>

namespace XEON {

	struct OrthographicCameraBounds {
		float left, right, bottom, top, n, f;
		OrthographicCameraBounds(float left, float right, float bottom, float top, float n, float f) 
			: left(left), right(right), bottom(bottom), top(top), n(n), f(f) {}
		float getWidth() { return right - left; }
		float getHeight() { return top - bottom; }
	};

	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float n = -1.0F, float f = 1.0F);
		
		const glm::vec3& getPosition() const { return position; }
		void setPosition(const glm::vec3& position) { this->position = position; recalculateViewMatrix(); }

		float getZRotation() const { return zRotation; }
		void setRotation(float rotation) { this->zRotation = rotation; recalculateViewMatrix(); }

		void setProjection(float left, float right, float bottom, float top, float n = -1.0F, float f = 1.0F);

		const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
		const glm::mat4& getViewMatrix() const { return viewMatrix; }
		const glm::mat4& getViewProjectionMatrix() const { return viewProjectionMatrix; }

		const OrthographicCameraBounds& getBounds() const { return bounds; }

	private:
		float zRotation = 0.0F;
		glm::vec3 position;
		OrthographicCameraBounds bounds;
		glm::mat4 projectionMatrix, viewMatrix, viewProjectionMatrix;

		void recalculateViewMatrix();

	};

}
