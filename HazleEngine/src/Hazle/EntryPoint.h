#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazle::Application* Hazle::createApplication();

void main(int argc, char** argv) {
	Hazle::Log::init();
	HZ_CORE_CRITICAL("Chal raha hai matlab thik se!!!");
	HZ_ERROR("Bahut bakchodi hai bhai ye...");
	HZ_CORE_WARN("Pura chudne wale hai isme to!!!");

	printf("Hazle Engine started!!!\n");
	auto app = Hazle::createApplication();
	app->run();
	delete app;
}

#endif