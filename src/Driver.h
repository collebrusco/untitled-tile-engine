#ifndef DRIVER_H
#define DRIVER_H
#include <stdint.h>
#include <cstddef>
#include <flgl.h>
#include "../lib/sw/Stopwatch.h"


class GameDriver {
protected:
	void close();
public:
    void start();
    void loop();
    GameDriver();

    float const& dt;
    Stopwatch const& launch_timer;

private:
	bool _close;
	float _dt;
	Stopwatch _launch_timer;
    Stopwatch delta_timer;
    virtual void user_create() = 0;
    virtual void user_update(float dt, Keyboard const& kb, Mouse const& mouse) = 0;
    virtual void user_render() = 0;
    virtual void user_destroy() = 0;
    void exit();
};


#endif
