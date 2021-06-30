#include <gb/gb.h>
#include "Sprites/slime.c"

//========================
// ----- Structs ----- //
//========================

//============================
// ----- Functions ----- //
//===========================
// Slime
// ------Animations
void Slime_idle_anim();

// Utils
//------VBlanks
void vbl_update();

//================================
// ----- Global Variables ----- //
//================================

UINT8 vbl_count = 0;
UINT8 state = 0;

void main(){

    // Set sprites
    set_sprite_data(0, 9, slime);
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

        Slime_idle_anim();

    }

}

void Slime_idle_anim(){
    if(state){
            set_sprite_tile(0, 5);
            set_sprite_tile(1, 6);
            set_sprite_tile(2, 7);
            set_sprite_tile(3, 8);
            state = !state;
            delay(1000);
        }
        else{
            set_sprite_tile(0, 0);
            set_sprite_tile(1, 1);
            set_sprite_tile(2, 2);
            set_sprite_tile(3, 3);
            state = !state;
            delay(1000);
        }
}

void vbl_update(){
    vbl_count++;
}