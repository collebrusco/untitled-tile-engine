/** 
 * CoreDriver.h
 * created 07/21/24 by frank collebrusco 
 *                |                           |          
 *      Input     |         main loop         |         bringup engine        
 *  OS  win     --|->  DVR  win,gfx init    --|->  CD   bringup renderers             
 *      timing    |         win update        |         give inp data -> eng -> new state 
 *      gfx       |         maintain dt       |         give inp data -> renderer -> draw
 *                |                           |         quit mechanism
 * 
 *      In the future somewhere there needs to be a layer that makes the CD layer nonspecific
 *      so we can do different sets of high level game stuff
 *      and im being vague because idk what thats gonna look like so lets use this first & add
 *      ooh im liking the idea of an fsm that starts diff core drivers & listens for an event to pop em out w an input code to trans states. maybe 
 * 
 */
#ifndef CORE_DRIVER_H
#define CORE_DRIVER_H
#include <flgl.h>
#include <flgl/tools.h>
#include <flgl/glm.h>
#include "Driver.h"
#include "game/World.h"
#include "rendering/RegionRenderer.h"
#include "render_pipeline_structs.h"
#include "rendering/BufferRenderer.h"
#include "rendering/PostRenderer.h"
#include "game/State.h"
#include "game/components.h"
#include "game/LCamControl.h"
#include "game/Actor.h"


class CoreDriver final : public GameDriver {
public:
	CoreDriver();
	virtual ~CoreDriver() = default;
private:
    virtual void user_create() override final;
    virtual void user_update(float dt, Keyboard const& kb, Mouse const& mouse) override final;
    virtual void user_render() override final;
    virtual void user_destroy() override final;

	glm::vec2 world_mpos() const;
	glm::vec2 world_mdelt() const;

	State state;

	/* Rendering stuff to be moved to one container */
	BufferRenderer brenderer;
	PostRenderer prenderer;
	TextRenderer text_renderer;

    /* systems and such? */
    LCamControl lcam_control;

};

#endif /* CORE_DRIVER_H */
