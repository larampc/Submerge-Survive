#include <lcom/lcf.h>
#include "video.h"
#include <stdlib.h>
static void *video_mem;		/* Process (virtual) address to which VRAM is mapped */

static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bytes_per_pixel;
vbe_mode_info_t mode_info;
uint8_t drawned_page = 0;

int (vbe_get_mode_inf)(uint16_t mode, vbe_mode_info_t* vmi_p) {
  reg86_t r86;
  mmap_t map;
  lm_alloc(sizeof(vbe_mode_info_t), &map);
  memset(&r86, 0, sizeof(r86));	/* zero the structure */
  r86.ax = GET_VBE_MODE;   
  r86.intno = 0x10; /* BIOS video services */
  r86.es = PB2BASE(map.phys);
  r86.di = PB2OFF(map.phys);
  r86.cx = mode;

  /* Make the BIOS call */

  if( sys_int86(&r86) != OK ) {
    printf("\tvg_init(): sys_int86() failed \n");
    return 1;
  }
  memcpy(vmi_p, map.virt, sizeof(vbe_mode_info_t));
  lm_free(&map);
  return 0;
}

int (vg_initialize)(uint16_t mode) {
  vbe_get_mode_inf(mode, &mode_info);
  h_res = mode_info.XResolution;
  v_res = mode_info.YResolution;
  bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    struct minix_mem_range mr;
    unsigned int vram_base = mode_info.PhysBasePtr;  /* VRAM's physical addresss */
    unsigned int vram_size = 2*h_res*v_res*(bytes_per_pixel);  /* VRAM's size, but you can use
                        the frame-buffer size, instead */
    int r;				    
    
    /* Allow memory mapping */

    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return 1;
    }

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED) {
        panic("couldn't map video memory");
        return 1;    
    }
    uint8_t* page = (uint8_t*)video_mem + h_res*v_res*(bytes_per_pixel);
    video_mem = page;

  reg86_t r86;
   
  /* Specify the appropriate register values */
  
  memset(&r86, 0, sizeof(r86));	/* zero the structure */

  r86.intno = 0x10; /* BIOS video services */
  r86.bx = (1 << 14) | mode;    /* Set Video Mode function */
  r86.ax = SET_VBE_MODE;   
  
  /* Make the BIOS call */

  if( sys_int86(&r86) != OK ) {
    printf("\tvg_init(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x < 0 || y < 0 || x > h_res - 1 || y > v_res - 1) return 1;
  if (color == 0) return 0;
  uint8_t* place = (uint8_t*)video_mem;
  place += (h_res * y + x) * bytes_per_pixel;
  if (memcpy(place, &color, bytes_per_pixel) == NULL) return 1;
  return 0;
}

int (vg_is_black_pixel)(uint16_t x, uint16_t y) {
  if (x < 0 || y < 0 || x > h_res - 1 || y > v_res-1) return 0;
  uint8_t* place = video_mem;
  place += (h_res * y + x) * bytes_per_pixel;
  if (!((*(uint32_t*)place) & 0x00FFFFFF)) {
    return 1;
  }
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned i = 0 ; i < len && x+i < h_res; i++)   
    if (vg_draw_pixel(x+i, y, color) != 0) return 1;
  return 0;
}

int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned i = 0 ; i < len && x+i < h_res; i++)   
    if (vg_draw_pixel(x, y+i, color) != 0) return 1;
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for(unsigned i = 0; i < height && y+i < v_res; i++) {
    if (vg_draw_hline(x, y+i, width, color) != 0) {
      vg_exit();
      return 1;
    }
  }
  return 0;
}

int (clear_buffer)() {
  memset(video_mem, 0x00, bytes_per_pixel*h_res*v_res);
  return 0;
}

int (vg_page_flip)() {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.ax = SET_VBE_DYSPLAY_START; // VBE call, function 07 -- set VBE display start
	r86.bh = 0x00;
	r86.bl = 0x00;
	r86.cx = 0x00;
	r86.dx = (drawned_page==0)? v_res : 0x00;
  r86.intno = 0x10; /* BIOS video services */
  if( sys_int86(&r86) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
	if(r86.al != 0x4F) {
		printf("vg_page_flip: mode is not supported\n");
		return 1;
	}
	if(r86.ah != 0x00) {
		switch(r86.ah) {
      case 0x1:
        printf("vg_page_flip: function call failed\n");
        break;
      case 0x2:
        printf("vg_page_flip: function call is not supported in current hardware configuration\n");
        break;
      case 0x3:
        printf("vg_page_flip: function call invalid in current video mode\n");
        break;
      default:
        printf("vg_page_flip: unknown VBE function error\n");
        break;
    }
		return 1;
	}
  drawned_page = !drawned_page;
  uint8_t* page;
  if (drawned_page == 0) page = (uint8_t*)video_mem + h_res*v_res*(bytes_per_pixel);
  else page = (uint8_t*)video_mem - h_res*v_res*(bytes_per_pixel);
  video_mem = page;

  return 0;
}


void (draw_sprite_full_screen)(Sprite* sp) {
    memcpy(video_mem, sp->map, sp->height*sp->width*bytes_per_pixel);
}

int (draw_larger_sprite_line)(Sprite *sp, int x, int y) {
  for (uint16_t i = 0; i < v_res; i++) {
    uint8_t* place = (uint8_t*)video_mem + ((h_res * i) * bytes_per_pixel);
    if ( memcpy(place, sp->map + ((sp->width * (y + i) + x) * bytes_per_pixel), h_res * bytes_per_pixel) == NULL) {
      return 1;
    }
  }
  return 0;
}

int (draw_splitted_sprites_line)(Sprite *sp1, Sprite *sp2, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t line) {
  for (uint16_t i = 0; i < height; i++) {
    uint8_t* place1 = (uint8_t*)video_mem + ((h_res * (i + y) + x) * bytes_per_pixel);
    if (i > 3 && i < 10) {
      if (memcpy(place1, sp1->map + ((sp1->width * (line + sp1->height/2)) * bytes_per_pixel), width * bytes_per_pixel) == NULL) return 1;
    } else {
      if (memcpy(place1, sp1->map + ((sp1->width * line) * bytes_per_pixel), width * bytes_per_pixel) == NULL) return 1;
    }
    uint8_t* place2 = (uint8_t*)video_mem + ((h_res * (i + y) + x + width) * bytes_per_pixel);
    if (memcpy(place2, sp2->map + ((sp2->width * i) * bytes_per_pixel), (sp2->width - width) * bytes_per_pixel) == NULL) return 1;
  }
  return 0;
}

int (draw_smaller_sprite_line)(Sprite *sp, uint16_t x, uint16_t y) {
  uint16_t maxy = (sp->height < v_res) ? sp->height : v_res;
  for (uint16_t i = 0; i < maxy; i++) {
    uint8_t* place = (uint8_t*)video_mem + ((h_res * (y + i)) * bytes_per_pixel);
    if ( memcpy(place, sp->map + ((sp->width * i + x) * bytes_per_pixel), h_res * bytes_per_pixel) == NULL) {
      return 1;
    }
  }
  return 0;
}
