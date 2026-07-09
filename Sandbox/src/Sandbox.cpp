#include <Hazle/Core/Hazle.h>
#include "Sandbox2D.h"
#include "Hazle/Core/EntryPoint.h"

//#include "imgui.h"

class sandbox : public Hazle::Application 
{

public:
	sandbox() 
		: Application("Sandbox")
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}; 
	~sandbox() 
	{
	};

};

Hazle::Application* Hazle::createApplication() 
{
	return new sandbox();
}