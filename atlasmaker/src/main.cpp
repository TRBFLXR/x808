#include <xe/app/application.hpp>
#include <xe/core/vfs.hpp>
#include "maker.hpp"

using namespace xe;

class AtlasMaker : public Application {
public:
	explicit AtlasMaker(const Config &config) :
			Application(config, "AtlasMaker") {

		VFS::mount("textures", "xe_sandbox_assets/textures/");

		Maker *m = new Maker();

		pushLayer(m);
	}

	~AtlasMaker() override { }

private:

};

int main() {
	initialize();

	Config config;
	config.width = 1280;
	config.height = 720;
	config.fullScreen = false;
	config.vSync = true;
	config.ups = 0;
	config.fps = 90;
	config.useSRGB = true;
	config.renderApi = RenderAPI::OpenGL;
	config.apiVersion = 330;
	config.maxPointLights2D = 1;

	AtlasMaker test(config);
	test.start();

	shutdown();

	return 0;
}