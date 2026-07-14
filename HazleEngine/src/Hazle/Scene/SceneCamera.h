#pragma once

#include "Hazle/Renderer/Camera.h"

namespace Hazle
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType {Perspective = 0, Orthographic = 1};

		SceneCamera();
		virtual ~SceneCamera();

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float veticleFOV, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetPerspectiveVerticleFOV() { return m_PerspectiveFOV; }
		void  SetPerspectiveVerticleFOV(float verticleFOV) { m_PerspectiveFOV = verticleFOV; ReCalculateProjection(); }
		float GetPerspectiveNearClip() { return m_PerspectiveNear; }
		void  SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; ReCalculateProjection(); }
		float GetPerspectiveFarClip() { return m_PerspectiveFar; }
		void  SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; ReCalculateProjection(); }
		
		float GetOrthographicSize() { return m_OrthographicSize; }
		void  SetOrthographicSize(float size) { m_OrthographicSize = size; ReCalculateProjection(); }
		float GetOrthographicNearClip()					{ return m_OrthographicNear; }
		void  SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; ReCalculateProjection(); }
		float GetOrthographicFarClip()					{ return m_OrthographicFar; }
		void  SetOrthographicFarClip(float farClip)		{ m_OrthographicFar = farClip; ReCalculateProjection(); }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; }

	private:
		void ReCalculateProjection();

		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 10.0f;

		float m_AspectRatio = 1.0f;
	};
}