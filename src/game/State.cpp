#include "State.h"

State::State() : 
    world(
        std::make_unique<MapWSave>(
            std::make_unique<TestWorldGenerator>(0xFACEFACE)
        )
    )
{
    cam.e = (~(entID{0})); /* disabled */
}
