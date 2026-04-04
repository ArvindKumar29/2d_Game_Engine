#include <Hazle.h>

class sandbox : public Hazle::Application {

public:
	sandbox() {};
	~sandbox() {};

};

Hazle::Application* Hazle::createApplication() {
	return new sandbox();
}