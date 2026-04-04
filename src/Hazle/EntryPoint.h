#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazle::Application* Hazle::createApplication();

void main(int argc, char** argv) {
	printf("Hazle Engine started!!!\n");
	auto app = Hazle::createApplication();
	app->run();
	delete app;
}

#endif