#pragma bank 7
#include "world_routes.h"
const WorldRoute world_routes[WORLD_ROUTE_COUNT] = {
{0,"Kingsroad",8,8,6,6,1},{1,"Green Road",6,6,11,11,4},{2,"Iron Road",11,11,11,5,12},{3,"North Road",11,5,5,3,18},{4,"Marsh Road",6,6,4,11,8},{5,"Frost Road",4,11,5,3,28},{6,"Dragon Road",5,3,13,13,40},{7,"Tower Road",13,13,12,4,55},{8,"Shadow Road",12,4,13,6,50},{9,"Astral Road",8,8,2,2,65}
};
static bool contains(const WorldRoute *r,uint8_t x,uint8_t y){uint8_t lx=r->from_x<r->to_x?r->from_x:r->to_x,hx=r->from_x>r->to_x?r->from_x:r->to_x,ly=r->from_y<r->to_y?r->from_y:r->to_y,hy=r->from_y>r->to_y?r->from_y:r->to_y;return x>=lx&&x<=hx&&y>=ly&&y<=hy;}
uint8_t world_route_between(uint8_t cx,uint8_t cy) BANKED {uint8_t i;for(i=0;i<WORLD_ROUTE_COUNT;i++)if(contains(&world_routes[i],cx,cy))return i;return ROUTE_NONE;}
bool world_route_allowed(uint8_t cx,uint8_t cy,uint8_t level) BANKED {uint8_t r=world_route_between(cx,cy);return r==ROUTE_NONE||level>=world_routes[r].min_level;}
