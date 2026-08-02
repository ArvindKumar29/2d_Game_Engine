#include <Hazle/Core/Hazle.h>
#include "EditorLayer.h"
#include "Hazle/Core/EntryPoint.h"

//#include "imgui.h"
namespace Hazle
{
	class HazleNut : public Application
	{
	public:
		HazleNut()
			: Application("Hazle Nut")
		{
			PushLayer(new EditorLayer());
		}
		~HazleNut() 
		{
		}
	};

	Application* createApplication()
	{
		return new HazleNut(); // returning a layer of HazleNut to entry point of hazle to create hazlenut application
	}
}