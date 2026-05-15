#include <Hazle/Hazle.h>

class ExampleLayer : public Hazle::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		HZ_INFO("ExampleLayer::Update");
	}

	void OnEvent(Hazle::Event& event) override
	{
		//HZ_TRACE("{0}", event);
	}
};

class sandbox : public Hazle::Application 
{

public:
	sandbox() 
	{
		PushLayer(new ExampleLayer());
	};
	~sandbox() 
	{
	};

};

Hazle::Application* Hazle::createApplication() 
{
	return new sandbox();
}