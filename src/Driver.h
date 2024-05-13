#ifndef DRIVER_H
#define DRIVER_H
#include <stdint.h>
#include <cstddef>
#include "../lib/sw/Stopwatch.h"

class GameDriver {
protected:
	void close();
public:
    bool create();
    void start();
    void loop();
    GameDriver();

    float const& dt;
    Stopwatch const& launch_timer;

private:
	bool _close;
	float _dt;
    size_t _ticks;
	Stopwatch _launch_timer;
    Stopwatch delta_timer;
    virtual void user_create() = 0;
    virtual void user_update(size_t tick, float dt) = 0;
    virtual void user_render() = 0;
    virtual void user_destroy() = 0;
    void exit();
};


#endif
