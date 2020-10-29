#include <gb/gb.h>
#include "Sprites/trainer.c"
#include "TileMaps/insideFloor.c"
#include "Maps/testBKG.c"

typedef struct {
    UINT16 x, y;
}GameObject;


void LoadData_Trainer();
void LoadData_TestBKG();
UINT8 Input();
void scrollBKG(UINT8 x, UINT8 y);        // Se usa en TrainerMovement

GameObject trainer;
UINT8 isMove = 0;
UINT8 dirX, dirY;


void main(){

    LoadData_Trainer();
    LoadData_TestBKG();

    SHOW_SPRITES;
    SHOW_BKG;

    while(1){
        Input();
    }

}

void LoadData_Trainer(){
    // Load Data
    set_sprite_data(0, 4, trainerTiles);
    // Set Sprites
    set_sprite_tile(0, 0);
    set_sprite_tile(1, 1);
    set_sprite_tile(2, 2);
    set_sprite_tile(3, 3);
    // Position of Sprites
    move_sprite(0, 72, 80);
    move_sprite(1, 80, 80);   // Este es el punto de referencia  y -= 16 para funcionar
    move_sprite(2, 72, 88);
    move_sprite(3, 80, 88);
    // Set (x,y) coordinates
    trainer.x = 9;
    trainer.y = 8;

}

void LoadData_TestBKG(){
    set_bkg_data(0, 4, insideFloor);
    set_bkg_tiles(0, 0, 20, 18, testBKG);
}

UINT8 Input(){
    
    if(joypad() & J_UP){
        if(testBKG[trainer.x + 20 * (trainer.y - 1)] != 0x00){   // OK
            isMove = 1;
            //set_bkg_tiles(trainer.x, trainer.y - 2, 1, 1, 0);
            trainer.y -= 2;
            scrollBKG(0, -1);
        }
    }
    else if(joypad() & J_DOWN){
        if(testBKG[trainer.x + (20 * (trainer.y + 2))] != 0x00){   // Aqui está el error
            isMove = 1;
            //set_bkg_tiles(trainer.x, trainer.y + 2, 1, 1, 0);
            trainer.y += 2;
            scrollBKG(0, 1);
        }
    }
    else if(joypad() & J_RIGHT){
        if(testBKG[trainer.x + 1 + 20 * trainer.y] != 0x00){
            isMove = 1;
            //set_bkg_tiles(trainer.x + 1, trainer.y, 1, 1, 0);
            trainer.x += 2;
            scrollBKG(1, 0);
        }
    }
    else if(joypad() & J_LEFT){
        if(testBKG[trainer.x - 2 + (20 * trainer.y)] != 0x00){
            isMove = 1;
            //set_bkg_tiles(trainer.x - 2, trainer.y, 1, 1, 0);
            trainer.x -= 2;
            scrollBKG(-1, 0);
        }
    }
    else{
        return 0;
    }

    return isMove;
}

void scrollBKG(UINT8 x, UINT8 y){
    UINT8 steps = 0;

    while(steps < 16){
        scroll_bkg(x, y);
        steps++;
        wait_vbl_done();
    }
}