#pragma once

#include "Hazle/Renderer/OrthographicCamera.h"
#include "Hazle/Core/Timestep.h"
#include "Hazle/Event/ApplicationEvent.h"
#include "Hazle/Event/MouseEvent.h"


namespace Hazle
{
	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController();
		OrthographicCameraController(float width,float height, bool rotation = false);
		~OrthographicCameraController();
		
		void OnUpdate(float ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera;  }
		const OrthographicCamera& GetCamera() const { return m_Camera;  }

		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; CalculateView(); }
		float GetZoomLevel() const { return m_ZoomLevel; }
		OrthographicCameraBounds GetBounds() const { return m_Bounds; }

	private:
		void CalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e); 
		bool OnWindowResized(WindowResizeEvent& e);
		
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		bool m_Rotation;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 3.0f;
		float m_CameraRotationSpeed = 5.0f;

	};
}
