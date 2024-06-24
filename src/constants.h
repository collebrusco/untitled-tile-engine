// 
// constants.h
// created 06/21/24 by frank collebrusco 
//
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define REGION_SIZE_LOG (4)
#define REGION_SIZE     (1<<REGION_SIZE_LOG)
#define REGION_SIZE_F   (static_cast<float>((1<<REGION_SIZE_LOG)))

#define WORLD_DIAMETER  (16)
#define WORLD_DIAMETER_F  (16.f)

#define TILE_PIXELS     (16)
#define TILE_PIXELS_F   (16.f)

#define TILE_SPRITESHEET_DIM    (64)
#define TILE_SPRITESHEET_DIM_F  (64.f)

#define FBUF_RES_LIMIT (1<<12)
#define FBUF_RES_LIMIT_F (static_cast<float>(1<<12))

#endif /* CONSTANTS_H */
