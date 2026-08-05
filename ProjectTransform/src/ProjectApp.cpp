#include "Hazle/Core/Hazle.h"
#include "Hazle/Core/EntryPoint.h"
#include "GameLayer.h"

namespace Transform
{
	class Transform : public Hazle::Application
	{
		Transform()
			: Application("project Transform")
		{
			PushLayer(new GameLayer());
		};
		~Transform()
		{};

		Hazle::Application* createApplication()
		{
			return new Transform();
		}
	};
}