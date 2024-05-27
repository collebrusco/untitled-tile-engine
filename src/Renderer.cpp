#include "Renderer.h"
#include <flgl.h>


Renderer::~Renderer() {}

void Renderer::context_init(std::string title, int w, int h) {
	gl.init();
	window.create(title.c_str(), w, h);
}

void Renderer::context_close() {
	gl.destroy();
}

void Renderer::clear() {
	gl.clear();
}

