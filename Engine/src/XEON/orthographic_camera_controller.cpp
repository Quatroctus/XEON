#include "xeonpch.h"

#include "XEON/orthographic_camera_controller.h"

#include "XEON/core/input.h"
#include "XEON/events/key_codes.h"
#include "XEON/core/maths.h"

namespace XEON {
	
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation, ControllerData data)
		: rotation(rotation), aspectRatio(aspectRatio), camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel), data(data) { }

	void OrthographicCameraController::onUpdate(Timestep delta) {
		const float move = data.movementSpeed * delta;
		const float cosMove = std::cosf(glm::radians(cameraRotation)) * move;
		const float sinMove = std::sinf(glm::radians(cameraRotation)) * move;
		if (Input::isKeyPressed(XEON_KEY_W)) {
			cameraPosition.x += -sinMove;
			cameraPosition.y += cosMove;
		}
		if (Input::isKeyPressed(XEON_KEY_S)) {
			cameraPosition.x -= -sinMove;
			cameraPosition.y -= cosMove;
		}
		if (Input::isKeyPressed(XEON_KEY_A)) {
			cameraPosition.x -= cosMove;
			cameraPosition.y -= sinMove;
		}
		if (Input::isKeyPressed(XEON_KEY_D)) {
			cameraPosition.x += cosMove;
			cameraPosition.y += sinMove;
		}
		
		camera.setPosition(cameraPosition);

		if (rotation) {
			if (Input::isKeyPressed(XEON_KEY_Q))
				cameraRotation = wrap(cameraRotation - (data.rotationSpeed * delta), data.minRotation, data.maxRotation);
			if (Input::isKeyPressed(XEON_KEY_E))
				cameraRotation = wrap(cameraRotation + (data.rotationSpeed * delta), data.minRotation, data.maxRotation);

			camera.setRotation(cameraRotation);
		}
	}

	void OrthographicCameraController::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(XEON_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(XEON_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e) {
		zoomLevel -= e.getYOffset() * data.zoomSpeed;
		zoomLevel = std::clamp(zoomLevel, data.zoomMin, data.zoomMax);
		camera.setProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e) {
		aspectRatio = (float) e.getWidth() / (float) e.getHeight();
		camera.setProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}

}
