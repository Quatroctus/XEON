#pragma once

#include "XEON/renderer/orthographic_camera.h"
#include "XEON/core/timestep.h"

#include "XEON/events/application_event.h"
#include "XEON/events/mouse_event.h"

#include <glm/glm.hpp>

namespace XEON {
	
	struct ControllerData {
		float movementSpeed = 5.0F;
		float zoomSpeed = 0.1F, zoomMin = 0.2F, zoomMax = 4.0F;
		float rotationSpeed = 45.0F, minRotation = 0.0F, maxRotation = 360.0F;
	};

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation=false, ControllerData data = ControllerData());

		void onUpdate(Timestep delta);
		void onEvent(Event& e);

		OrthographicCamera& getCamera() { return camera; }
		const OrthographicCamera& getCamera() const { return camera; }
		
		ControllerData& getData() { return data; }

	private:
		bool rotation;
		float aspectRatio, zoomLevel = 1.0F;
		OrthographicCamera camera;

		float cameraRotation = 0.0F;
		glm::vec3 cameraPosition{ 0.0F };
		ControllerData data;

		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);

	};

}
