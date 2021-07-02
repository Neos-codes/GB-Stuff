#include <gb/gb.h>
#include "Sprites/slime.c"

//========================
// ----- Structs ----- //
//========================

//============================
// ----- Functions ----- //
//===========================
// Slime
// ------ Mechanics
void Slime_move();
// ------ Animations
void Slime_idle_anim();

// Utils
// ----- INPUT
void Input();
// ----- VBlanks
void vbl_update();

//================================
// ----- Global Variables ----- //
//================================
// Slime
UINT8 state = 0;        // For animations (0 - 1)
UINT8 input = 0;
UINT8 slime_dir = 0;    // 0 up, 1 down, 2 right, 3 left
UINT8 isMoving = FALSE;     // If slime is moving
UINT8 pixels_moved = 0; // number of frames that slime is moving by input
UINT8 frames_anim = 0;  // Use for animations (example, 20 frames = 1 state of sprite in idle_slime)

// Utils
//----- VBlanks
UINT8 vbl_count = 0;

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

    // Flag sprite
    set_sprite_tile(4, 4);
    move_sprite(4, 0x8, 0x10);


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
        
        // Solo se puede recibir input cuando slime está quieto
        if(!isMoving)
            input = joypad();
        Slime_move();

    }

}

void Slime_move(){

    // Si no se esta moviendo, abierto a recibir inputs de direccion
    if(!isMoving){
        if(input & J_RIGHT || input & J_LEFT || input & J_UP || input & J_DOWN){
            isMoving = TRUE;
            slime_dir = input;   // Se guarda el ultimo input de direccion
            scroll_sprite(4, 1, 1);  // Flag de captura de movimiento
        }
    }
    
    // Si se está moviendo, terminar de mover 8 pixeles con la ultima direccion obtenida
    if(isMoving){
        if(slime_dir & J_RIGHT){
            scroll_sprite(0, 1, 0);
            scroll_sprite(1, 1, 0);
            scroll_sprite(2, 1, 0);
            scroll_sprite(3, 1, 0);
        }
        else if(slime_dir & J_LEFT){
            scroll_sprite(0, -1, 0);
            scroll_sprite(1, -1, 0);
            scroll_sprite(2, -1, 0);
            scroll_sprite(3, -1, 0);
        }
        else if(slime_dir & J_UP){
            scroll_sprite(0, 0, -1);
            scroll_sprite(1, 0, -1);
            scroll_sprite(2, 0, -1);
            scroll_sprite(3, 0, -1);
        }
        else if(slime_dir & J_DOWN){
            scroll_sprite(0, 0, 1);
            scroll_sprite(1, 0, 1);
            scroll_sprite(2, 0, 1);
            scroll_sprite(3, 0, 1);
        }

        // Se mueve por defecto 16 pixeles
        pixels_moved++;
        if(pixels_moved == 16){
            slime_dir = 0;
            pixels_moved = 0;
            isMoving = FALSE;
        }
        // Una vez se mueve los 16 pixeles, se puede recibir input de movimiento nuevamente

    }
}
    
void Slime_idle_anim(){
    frames_anim++;
    if(frames_anim == 30){
        if(state){
                set_sprite_tile(0, 5);
                set_sprite_tile(1, 6);
                set_sprite_tile(2, 7);
                set_sprite_tile(3, 8);
                state = !state;
        }
        else{
            set_sprite_tile(0, 0);
            set_sprite_tile(1, 1);
            set_sprite_tile(2, 2);
            set_sprite_tile(3, 3);
            state = !state;
        }
        frames_anim = 0;
    }
}

void vbl_update(){
    vbl_count++;
}