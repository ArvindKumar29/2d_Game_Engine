#pragma once

#include "Hazle/Renderer/GraphicsContext.h"
#include "glfw/glfw3.h"

struct GLFWwindow;

namespace Hazle
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	
	private:
		GLFWwindow* m_WindowHandle;
	};
}
