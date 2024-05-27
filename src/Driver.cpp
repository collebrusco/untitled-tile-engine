#include "Driver.h"
#include <cmath>
#include <flgl/logger.h>
#include <flgl.h>
LOG_MODULE(driver);

GameDriver::GameDriver() :  dt(_dt), launch_timer(_launch_timer),
                            _close(false),
                            _dt(1.f/60.f),
                            _launch_timer(SECONDS),
                            delta_timer(SECONDS)
{}

void GameDriver::exit() {
    user_destroy();
}

#define COMPLICATED_LOOP 0

#define min(x,y) ((x)<(y)?(x):(y))
void GameDriver::loop() {

#if COMPLICATED_LOOP == 1
    if (_dt > 1.f/30.f) {
        while (_dt > 0.f) {
            user_update(min(_dt, 1.f/60.f), window.keyboard, window.mouse);
            _dt -= 1.f/60.f;
        }
    } else {
        user_update(_dt, window.keyboard, window.mouse);
    }
#else 
    user_update(_dt, window.keyboard, window.mouse);
#endif

    user_render();

    window.update();

    _dt = delta_timer.stop_reset_start();
    if (std::isinf(_dt) || std::isnan(_dt)){
        _dt = 1.f / 80.f; //TODO: hate this...
    }
}

void GameDriver::start() {
    _launch_timer.start();
    user_create();
    LOG_INF("created, loop:");
    while (!_close && !window.should_close()) {
       loop();
    }
    LOG_INF("out of loop, exiting...");
    exit();
}

void GameDriver::close() {
	_close = true;
}

