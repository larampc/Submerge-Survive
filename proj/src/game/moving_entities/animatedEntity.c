#include <lcom/lcf.h>
#include "animatedEntity.h"

AnimatedEntity *create_animated_entity(int x, int y, int xspeed, int yspeed, uint8_t no_pic, GAME_XPM_ID pic1, ...) {

    AnimatedEntity *aentity = malloc(sizeof(AnimatedEntity));
    aentity->cur_aspeed = 2;
    aentity->num_fig = no_pic;
    aentity->cur_fig = 0;
    aentity->frame_number = 0;
    aentity->start_fig = 0;

    Sprite* sp = get_sprite(pic1);
    aentity->e = create_entity(sp,x,y,xspeed,yspeed);
    aentity->sprites = malloc((no_pic) * sizeof(Sprite *));
    aentity->sprites[0] = sp;
    
    va_list ap;
    va_start(ap, pic1);
    for(int  i = 1; i < no_pic; i++ ) {
        GAME_XPM_ID tmp = va_arg(ap, GAME_XPM_ID);
        if( (aentity->sprites[i] = get_sprite(tmp)) == NULL) {
            for(int j = 1; j < i; j++)
                free(aentity->sprites[i]);
            free(aentity->sprites);
            destroy_entity(aentity->e);
            free(aentity);
            va_end(ap);
            return NULL;
        }
    }
    va_end(ap);
    return aentity;
}

void destroy_animated_entity(AnimatedEntity* aentity){
    if( aentity == NULL ) return;
    destroy_entity(aentity->e);
    free(aentity->e);
    free(aentity);
    aentity = NULL;
}

void animate_entity(AnimatedEntity* aentity) {
    aentity->frame_number++;
    if(aentity->cur_aspeed == aentity->frame_number) {
        aentity->cur_fig++;
        if(aentity->cur_fig == aentity->num_fig + aentity->start_fig) {
            aentity->cur_fig = aentity->start_fig;
        }
        aentity->e->sprite = aentity->sprites[aentity->cur_fig];
        aentity->frame_number = 0;
    }
}

AnimatedEntity* clone(AnimatedEntity* original) {
    if (original == NULL) {
        return NULL; 
    }

    AnimatedEntity* clone = (AnimatedEntity*) malloc(sizeof(AnimatedEntity));
    clone->e = create_entity(original->e->sprite, original->e->x, original->e->y, original->e->xspeed, original->e->yspeed);

    clone->cur_aspeed = original->cur_aspeed;
    clone->frame_number = original->frame_number;
    clone->num_fig = original->num_fig;
    clone->cur_fig = original->cur_fig;
    clone->start_fig = original->start_fig;

    clone->sprites = (Sprite**) malloc((original->num_fig) * sizeof(Sprite *));
    for (int i = 0; i < original->num_fig; ++i) {
        clone->sprites[i] = original->sprites[i];
    }

    return clone;
}
