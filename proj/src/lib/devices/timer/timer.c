#include <lcom/lcf.h>
#include "i8254.h"
#include "timer.h"

static int timer_id;
uint32_t count = 0;

void (timer_ih)(){
    count++;
}

int (timer_subscribe_int)(uint8_t* bit_no){
    if(bit_no == NULL) return 1;
    timer_id = *bit_no;
    *bit_no = BIT(*bit_no);
    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_id) != 0) {
        printf("in timer_subscribe_int: failed to set timer policy\n");
        return 1;
    }
    return 0;
}

int (timer_unsubscribe_int)(){
    if (sys_irqrmpolicy(&timer_id) != 0){
        printf("in timer_unsubscribe_int: failed to remove timer policy\n");
        return 1;
    }
    return 0;
}
