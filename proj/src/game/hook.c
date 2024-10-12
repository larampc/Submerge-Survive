
#include "hook.h"

Hook* create_hook(Sprite* sp) {
    Hook *hook = (Hook*) malloc (sizeof(Hook));
    hook->hook = sp;
    hook->height = 0;
    return hook;
}

void destroy_hook(Hook* hook){
    if(hook == NULL) return;
    hook->hook = NULL;
    free(hook);
}
