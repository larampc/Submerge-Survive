#include "shop.h"
#include "../graphics/alphabet.h"
#include "../graphics/spriteCache.h"

Shop* create_shop(){
  Shop* shop = (Shop*)malloc(sizeof(Shop));
  for(int i = 0; i < MAX_ITEMS_SIZE; i++)
    shop->items[i] = NULL;
  return shop;
}

void add_item(Shop* shop, uint16_t x_, uint16_t y_, uint16_t price, Sprite* sprite, void (*on_click) (void*), uint8_t position) {
    Item* newItem = (Item*)malloc(sizeof(Item));
    *newItem = (Item){.price = price, .sprite = sprite, .x = x_, .y = y_, .on_click_effect = on_click, .buyable = false};
    shop->items[position] = newItem;
    return;
}

Item* get_clicked_item(Shop* shop, uint16_t x, uint16_t y, ITEMS* selected_item){
  for(int i = 0; i < MAX_ITEMS_SIZE; i++){
    Item* item = shop->items[i];
    if(item == NULL) continue;
    if(x >= item->x - item->sprite->width/2  && x < item->x + item->sprite->width/2 && y >= item->y- item->sprite->height/2 && y < item->y + item->sprite->height/2){
      *selected_item = i;
      return shop->items[i];
    }
  }
  return NULL;
}

void draw_shop(Shop* shop, uint16_t budget){
  for(int i = 0; i < MAX_ITEMS_SIZE; i++){
    Item* item = shop->items[i];
    if(item == NULL) continue;
    draw_sprite_centered(item->sprite, item->x, item->y);
    if (!item->buyable) draw_sprite_centered(get_sprite(CROSS_STONE), item->x, item->y);
    //draw cross if out of stock
    char price[16];
    sprintf(price, "%d", item->price);
    draw_string(price, 16, item->x - ((22*strlen(price))/2), item->y + (item->sprite->height/2) + 2, false, (item->price <= budget) ? YELLOW : RED);
  }
}

void destroy_item(Item* item){
  if(item == NULL) return;
  item->sprite = NULL;
  free(item);
}

void destroy_shop(Shop* shop) {
  if (shop == NULL) return;
  for (int i = 0; i < MAX_ITEMS_SIZE; i++) {
    if (shop->items[i] == NULL ) continue; 
    destroy_item(shop->items[i]);
    shop->items[i] = NULL;
  }
  free(shop);
}
