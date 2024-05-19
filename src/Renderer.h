#ifndef RENDERER_H
#define RENDERER_H

#include <string>

struct Renderer {
	Renderer() = default;
	virtual ~Renderer();

	// global gl calls
	static void context_init(std::string title, int w, int h);
	static void context_close();
	static void clear();
	static void frame_finish();

	// per-renderer logic
	virtual void init() = 0;
	virtual void prepare() = 0;
	virtual void render() = 0;
	virtual void destroy() = 0;
};

#endif


