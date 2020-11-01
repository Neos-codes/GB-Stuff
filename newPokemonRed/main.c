#include <gb/gb.h>
#include "Sprites/trainer.c"
#include "Sprites/PikachuSprites.c"
#include "TileMaps/insideFloor.c"
#include "Maps/testBKG.c"

typedef struct {
    UINT16 x, y;
}GameObject;


void LoadData_Trainer();
void LoadData_Companion();
void LoadData_TestBKG();
UINT8 Input();
void scrollBKG(UINT8 x, UINT8 y, UINT8 dir);        // Se usa en TrainerMovement
void TrainerWalkingAnim(UINT8 dir);
void CompanionWalkingAnim(UINT8 dir);

GameObject trainer;
GameObject pikachu;
UINT8 animFrame = 0;
UINT8 isMove = 0;
UINT8 dirX, dirY;
UINT8 hasCompanion = 1, pikaFlip = 0;


void main(){

    LoadData_Trainer();
    LoadData_Companion();
    LoadData_TestBKG();

    SHOW_SPRITES;
    SHOW_BKG;

    while(1){
        Input();
    }

}

void LoadData_Trainer(){
    // Load Data
    set_sprite_data(0, 38, trainerTiles);
    // Sprites 0 - 37
    // Set Sprites
    set_sprite_tile(0, 0);
    set_sprite_tile(1, 1);
    set_sprite_tile(2, 2);
    set_sprite_tile(3, 3);
    // Position of Sprites
    move_sprite(0, 72, 80);
    move_sprite(1, 80, 80);
    move_sprite(2, 72, 88);
    move_sprite(3, 80, 88);
    // Set (x,y) coordinates
    trainer.x = 9;
    trainer.y = 8;

}

void LoadData_Companion(){
    // Load Data
    set_sprite_data(38, 12, PikachuSprites);
    // Sprites 38 - 49
    // Set Sprites
    set_sprite_tile(4, 38);    // Pikachu Tiles
    set_sprite_tile(5, 39);
    set_sprite_tile(6, 40);
    set_sprite_tile(7, 41);
    // Position of Sprites
    move_sprite(4, 72, 56);
    move_sprite(5, 80, 56);
    move_sprite(6, 72, 64);
    move_sprite(7, 80, 64);
}

void LoadData_TestBKG(){
    set_bkg_data(0, 4, insideFloor);
    set_bkg_tiles(0, 0, 20, 18, testBKG);
}

UINT8 Input(){
    
    if(joypad() & J_UP){
        if(testBKG[trainer.x + 20 * (trainer.y - 1)] != 0x03){   // OK
            isMove = 1;
            //set_bkg_tiles(trainer.x, trainer.y - 2, 1, 1, 0);
            trainer.y -= 2;
            scrollBKG(0, -1, 1);
        }
    }
    else if(joypad() & J_DOWN){
        if(testBKG[trainer.x + (20 * (trainer.y + 2))] != 0x03){   // Aqui está el error
            isMove = 1;
            //set_bkg_tiles(trainer.x, trainer.y + 2, 1, 1, 0);
            trainer.y += 2;
            scrollBKG(0, 1, 2);
        }
    }
    else if(joypad() & J_RIGHT){
        if(testBKG[trainer.x + 1 + 20 * trainer.y] != 0x03){
            isMove = 1;
            //set_bkg_tiles(trainer.x + 1, trainer.y, 1, 1, 0);
            trainer.x += 2;
            scrollBKG(1, 0, 3);
        }
    }
    else if(joypad() & J_LEFT){
        if(testBKG[trainer.x - 2 + (20 * trainer.y)] != 0x03){
            isMove = 1;
            //set_bkg_tiles(trainer.x - 2, trainer.y, 1, 1, 0);
            trainer.x -= 2;
            scrollBKG(-1, 0, 4);
        }
    }
    else{
        return 0;
    }

    return isMove;
}

void scrollBKG(UINT8 x, UINT8 y, UINT8 dir){
    UINT8 steps = 0;
    UINT8 frame = 1;
    while(steps < 16){
        if(steps == 0 || steps == 7){
            TrainerWalkingAnim(dir);
        }
        scroll_bkg(x, y);
        steps++;
        wait_vbl_done();
    }
}

void TrainerWalkingAnim(UINT8 dir){
    
    if(dir == 1){   // Si va hacia arriba
        if(animFrame == 0){             // Paso izquierdo
            set_sprite_tile(0, 26);
            set_sprite_tile(1, 27);
            set_sprite_tile(2, 28);
            set_sprite_tile(3, 29);
            /*if(hasCompanion){
                set_sprite_tile();
                set_sprite_tile();
                set_sprite_tile();
                set_sprite_tile();
            }*/
            animFrame = 1;
        }
        else if(animFrame == 2){         // Paso derecho
            set_sprite_tile(0, 35);
            set_sprite_tile(1, 34);
            set_sprite_tile(2, 37);
            set_sprite_tile(3, 36);
            animFrame++;
        }
        else if(animFrame == 1 || animFrame == 3){  // Centro
            set_sprite_tile(0, 30);
            set_sprite_tile(1, 31);
            set_sprite_tile(2, 32);
            set_sprite_tile(3, 33);
            if(animFrame == 3)
                animFrame = 0;
            else
                animFrame = 2;
        }
    }
    if(dir == 2){   // Si va hacia abajo
        if(animFrame == 0){             // Paso izquierdo
            set_sprite_tile(0, 4);
            set_sprite_tile(1, 5);
            set_sprite_tile(2, 6);
            set_sprite_tile(3, 7);
            animFrame = 1;
        }
        else if(animFrame == 2){         // Paso derecho
            set_sprite_tile(0, 4);
            set_sprite_tile(1, 5);
            set_sprite_tile(2, 8);
            set_sprite_tile(3, 9);
            animFrame++;
        }
        else if(animFrame == 1 || animFrame == 3){  // Centro
            set_sprite_tile(0, 0);
            set_sprite_tile(1, 1);
            set_sprite_tile(2, 2);
            set_sprite_tile(3, 3);
            if(animFrame == 3)
                animFrame = 0;
            else
                animFrame = 2;
        }
    }
    if(dir == 3){    // Hacia la derecha
        if(animFrame == 0 || animFrame == 2){  // Paso
            set_sprite_tile(0, 19);
            set_sprite_tile(1, 18);
            set_sprite_tile(2, 21);
            set_sprite_tile(3, 20);
            if(hasCompanion){
                pikaFlip = 1;   // Bandera que avisa inversion de Sprites
                set_sprite_tile(4, 45);
                set_sprite_tile(5, 44);
                set_sprite_tile(6, 42);
                set_sprite_tile(7, 43);
                set_sprite_prop(4, S_FLIPX);
                set_sprite_prop(5, S_FLIPX);
                set_sprite_prop(6, S_FLIPX);
                set_sprite_prop(7, S_FLIPX);
            }
            animFrame++;
        }
        else if(animFrame == 1 || animFrame == 3){  // Centro
            set_sprite_tile(0, 23);
            set_sprite_tile(1, 22);
            set_sprite_tile(2, 25);
            set_sprite_tile(3, 24);
            if(hasCompanion){
                set_sprite_tile(4, 49);
                set_sprite_tile(5, 48);
                set_sprite_tile(6, 47);
                set_sprite_tile(7, 46);
            }
            if(animFrame == 3)
                animFrame = 0;
            else
                animFrame++;
            
        }
    }
    if(dir == 4){    // Hacia la izquierda
        if(animFrame == 0 || animFrame == 2){  // Paso
            set_sprite_tile(0, 10);
            set_sprite_tile(1, 11);
            set_sprite_tile(2, 12);
            set_sprite_tile(3, 13);
            if(hasCompanion){
                if(pikaFlip){
                    set_sprite_prop(4, 0x00);  // Quitar FlipX
                    set_sprite_prop(5, 0x00);
                    set_sprite_prop(6, 0x00);
                    set_sprite_prop(7, 0x00);
                    pikaFlip = 0;
                }
                set_sprite_tile(4, 44);
                set_sprite_tile(5, 45);
                set_sprite_tile(6, 43);
                set_sprite_tile(7, 42);
            }
            animFrame++;
        }
        else if(animFrame == 1 || animFrame == 3){  // Centro
            set_sprite_tile(0, 14);
            set_sprite_tile(1, 15);
            set_sprite_tile(2, 16);
            set_sprite_tile(3, 17);
            if(hasCompanion){
                set_sprite_tile(4, 48);
                set_sprite_tile(5, 49);
                set_sprite_tile(6, 46);
                set_sprite_tile(7, 47);
            }
            if(animFrame == 3)
                animFrame = 0;
            else
                animFrame++;
            
        }
    }
}