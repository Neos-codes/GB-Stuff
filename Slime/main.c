#include <gb/gb.h>
#include "Sprites/slime.c"

UINT8 vbl_count = 0;

void vbl_update(){
    vbl_count++;
}

void main(){

    // Set sprites
    set_sprite_data(0, 4, slime);
    set_sprite_tile(0, 0);
    set_sprite_tile(1, 1);
    set_sprite_tile(2, 2);
    set_sprite_tile(3, 3);
    move_sprite(0, 80, 90);
    move_sprite(1, 80, 82);
    move_sprite(2, 88, 82);
    move_sprite(3, 88, 90);

    // Para fluidez
    disable_interrupts();
    add_VBL(vbl_update);
    set_interrupts(VBL_IFLAG);
    enable_interrupts();

    SHOW_SPRITES;

    // Loop de juego
    while(1){
        if(!vbl_count)
            wait_vbl_done();
        vbl_count = 0;


    }

}