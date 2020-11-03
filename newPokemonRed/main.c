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
void scrollBKG(UINT8 x, UINT8 y, UINT8 dir, UINT8 companionFlag);        // Se usa en TrainerMovement
void TrainerWalkingAnim(UINT8 dir);
void CompanionWalkingAnim(UINT8 dir);
void GetCompanionDir(UINT8 flag);   // flag = 0 se mueve en X, flag 1 en eje Y , flag = companionFlag
void CompanionMove(UINT8 flag);

GameObject trainer;
GameObject companion;
UINT8 flag = 0;             // bit 0: companionMove() flag
UINT8 lastPosX, lastPosY;   // last trainer position (to calculate direction of companion)
UINT8 trainerAnimFrame = 0; // Animation Frame of Trainer
UINT8 isMove = 0;           // Boolean that indicates if Trainer is moving
UINT8 trainerDir = 2, trainerLastDir = 2;   // direction of trainer movement
UINT8 hasCompanion = 1, companionFlip = 0, CompanionDir, companionAnimFrame = 0;


void main(){

    LoadData_Trainer();
    LoadData_Companion();
    LoadData_TestBKG();
    //set_sprite_tile(39, 38);   // For Debug

    SHOW_SPRITES;
    SHOW_BKG;

    while(1){
        if(!Input()){

        }
        else{

        }
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

    // Set Coordinates
    companion.x = 9;    // This must variate in every map
    companion.y = 7;
    // Load Data
    set_sprite_data(38, 24, PikachuSprites);
    // Sprites 38 - 49
    // Set Sprites
    set_sprite_tile(4, 38);    // Pikachu Tiles
    set_sprite_tile(5, 39);
    set_sprite_tile(6, 40);
    set_sprite_tile(7, 41);
    // Position of Sprites
    move_sprite(4, 72, 60);   // 1 Tile de distancia con Trainer
    move_sprite(5, 80, 60);
    move_sprite(6, 72, 68);
    move_sprite(7, 80, 68);
}

void LoadData_TestBKG(){
    set_bkg_data(0, 4, insideFloor);
    set_bkg_tiles(0, 0, 20, 18, testBKG);
}

UINT8 Input(){
    
    if(joypad() & J_UP){
        if(testBKG[trainer.x + 20 * (trainer.y - 1)] != 0x03){   // OK
            isMove = 1;
            lastPosY = trainer.y;
            trainer.y -= 2;
            trainerLastDir = trainerDir;
            trainerDir = 1;
            scrollBKG(0, -1, 1, 1);
        }
    }
    else if(joypad() & J_DOWN){
        if(testBKG[trainer.x + (20 * (trainer.y + 2))] != 0x03){   // Aqui está el error
            isMove = 1;
            lastPosY = trainer.y;
            trainer.y += 2;
            trainerLastDir = trainerDir;
            trainerDir = 2;
            scrollBKG(0, 1, 2, 1);
        }
    }
    else if(joypad() & J_RIGHT){
        if(testBKG[trainer.x + 1 + 20 * trainer.y] != 0x03){
            isMove = 1;
            lastPosX = trainer.x;
            trainer.x += 2;
            trainerLastDir = trainerDir;
            trainerDir = 3;
            scrollBKG(1, 0, 3, 0);
        }
    }
    else if(joypad() & J_LEFT){
        if(testBKG[trainer.x - 2 + (20 * trainer.y)] != 0x03){
            isMove = 1;
            lastPosX = trainer.x;
            trainer.x -= 2;
            trainerLastDir = trainerDir;
            trainerDir = 4;
            scrollBKG(-1, 0, 4, 0);
        }
    }
    else{
        return 0;
    }
    return isMove;
}

void scrollBKG(UINT8 x, UINT8 y, UINT8 dir, UINT8 companionFlag){
    UINT8 steps = 0;
    //companionLastDir = CompanionDir;    // Se guarda la direccion anterior antes de obtener la nueva
    GetCompanionDir(companionFlag);
    while(steps < 16){
        CompanionMove(!(steps%4));
        if(steps == 0 || steps == 7){   // Para mezclar animaciones de trainer y companion
            TrainerWalkingAnim(dir);    // separar cada step en un if
            if(hasCompanion){
                CompanionWalkingAnim(CompanionDir);
            }
        }
        else if((steps == 3 || steps == 11) && hasCompanion){
            CompanionWalkingAnim(CompanionDir);
        }
        scroll_bkg(x, y);
        steps++;
        wait_vbl_done();
    }
    /*if(flag){
        steps = 0;
        flag = 0;
        while(steps < 4){
            CompanionMove();
            steps++;
            wait_vbl_done();
        }
    }*/
}

void TrainerWalkingAnim(UINT8 dir){
    
    if(dir == 1){   // Si va hacia arriba
        if(trainerAnimFrame == 0){             // Paso izquierdo
            set_sprite_tile(0, 26);
            set_sprite_tile(1, 27);
            set_sprite_tile(2, 28);
            set_sprite_tile(3, 29);
            trainerAnimFrame = 1;
        }
        else if(trainerAnimFrame == 2){         // Paso derecho
            set_sprite_tile(0, 35);
            set_sprite_tile(1, 34);
            set_sprite_tile(2, 37);
            set_sprite_tile(3, 36);
            trainerAnimFrame++;
        }
        else if(trainerAnimFrame == 1 || trainerAnimFrame == 3){  // Centro
            set_sprite_tile(0, 30);
            set_sprite_tile(1, 31);
            set_sprite_tile(2, 32);
            set_sprite_tile(3, 33);
            if(trainerAnimFrame == 3)
                trainerAnimFrame = 0;
            else
                trainerAnimFrame = 2;
        }
    }
    if(dir == 2){   // Si va hacia abajo
        if(trainerAnimFrame == 0){             // Paso izquierdo
            set_sprite_tile(0, 4);
            set_sprite_tile(1, 5);
            set_sprite_tile(2, 6);
            set_sprite_tile(3, 7);
            trainerAnimFrame = 1;
        }
        else if(trainerAnimFrame == 2){         // Paso derecho
            set_sprite_tile(0, 4);
            set_sprite_tile(1, 5);
            set_sprite_tile(2, 8);
            set_sprite_tile(3, 9);
            trainerAnimFrame++;
        }
        else if(trainerAnimFrame == 1 || trainerAnimFrame == 3){  // Centro
            set_sprite_tile(0, 0);
            set_sprite_tile(1, 1);
            set_sprite_tile(2, 2);
            set_sprite_tile(3, 3);
            if(trainerAnimFrame == 3)
                trainerAnimFrame = 0;
            else
                trainerAnimFrame = 2;
        }
    }
    if(dir == 3){    // Hacia la derecha
        if(trainerAnimFrame == 0 || trainerAnimFrame == 2){  // Paso
            set_sprite_tile(0, 19);
            set_sprite_tile(1, 18);
            set_sprite_tile(2, 21);
            set_sprite_tile(3, 20);
            trainerAnimFrame++;
        }
        else if(trainerAnimFrame == 1 || trainerAnimFrame == 3){  // Centro
            set_sprite_tile(0, 23);
            set_sprite_tile(1, 22);
            set_sprite_tile(2, 25);
            set_sprite_tile(3, 24);
            if(trainerAnimFrame == 3)
                trainerAnimFrame = 0;
            else
                trainerAnimFrame++;
            
        }
    }
    if(dir == 4){    // Hacia la izquierda
        if(trainerAnimFrame == 0 || trainerAnimFrame == 2){  // Paso
            set_sprite_tile(0, 10);
            set_sprite_tile(1, 11);
            set_sprite_tile(2, 12);
            set_sprite_tile(3, 13);
            trainerAnimFrame++;
        }
        else if(trainerAnimFrame == 1 || trainerAnimFrame == 3){  // Centro
            set_sprite_tile(0, 14);
            set_sprite_tile(1, 15);
            set_sprite_tile(2, 16);
            set_sprite_tile(3, 17);
            if(trainerAnimFrame == 3)
                trainerAnimFrame = 0;
            else
                trainerAnimFrame++;
            
        }
    }
}

void CompanionWalkingAnim(UINT8 dir){

    // Hacia arriba
    if(dir == 1){
        if(companionAnimFrame == 0){             // Paso izquierdo
            set_sprite_tile(4, 54);
            set_sprite_tile(5, 55);
            set_sprite_tile(6, 56);
            set_sprite_tile(7, 57);
            set_sprite_prop(4, 0x00);
            set_sprite_prop(5, 0x00);
            set_sprite_prop(6, 0x00);
            set_sprite_prop(7, 0x00);
            companionFlip = 0;
            companionAnimFrame++;
        }
        else if(companionAnimFrame == 2){         // Paso derecho
            set_sprite_tile(4, 55);
            set_sprite_tile(5, 54);
            set_sprite_tile(6, 57);
            set_sprite_tile(7, 56);
            set_sprite_prop(4, S_FLIPX);
            set_sprite_prop(5, S_FLIPX);
            set_sprite_prop(6, S_FLIPX);
            set_sprite_prop(7, S_FLIPX);
            companionFlip = 1;
            companionAnimFrame++;
        }
        else if(companionAnimFrame == 1 || companionAnimFrame == 3){  // Centro
            set_sprite_tile(4, 58);
            set_sprite_tile(5, 59);
            set_sprite_tile(6, 60);
            set_sprite_tile(7, 61);
            if(companionFlip){
                set_sprite_prop(4, 0x00);
                set_sprite_prop(5, 0x00);
                set_sprite_prop(6, 0x00);
                set_sprite_prop(7, 0x00);
                companionFlip = 0;
            }
            if(companionAnimFrame == 3)
                companionAnimFrame = 0;
            else
                companionAnimFrame++;
        }
    }
    // Hacia Abajo
    else if(dir == 2){
        if(companionAnimFrame == 0){             // Paso izquierdo
            set_sprite_tile(4, 52);
            set_sprite_tile(5, 53);
            set_sprite_tile(6, 50);
            set_sprite_tile(7, 51);
            set_sprite_prop(4, 0x00);
            set_sprite_prop(5, 0x00);
            set_sprite_prop(6, 0x00);
            set_sprite_prop(7, 0x00);
            companionFlip = 0;
            companionAnimFrame++;
        }
        else if(companionAnimFrame == 2){         // Paso derecho
            set_sprite_tile(4, 53);
            set_sprite_tile(5, 52);
            set_sprite_tile(6, 51);
            set_sprite_tile(7, 50);
            set_sprite_prop(4, S_FLIPX);
            set_sprite_prop(5, S_FLIPX);
            set_sprite_prop(6, S_FLIPX);
            set_sprite_prop(7, S_FLIPX);
            companionFlip = 1;
            companionAnimFrame++;
        }
        else if(companionAnimFrame == 1 || companionAnimFrame == 3){  // Centro
            set_sprite_tile(4, 38);
            set_sprite_tile(5, 39);
            set_sprite_tile(6, 40);
            set_sprite_tile(7, 41);
            if(companionFlip){
                set_sprite_prop(4, 0x00);
                set_sprite_prop(5, 0x00);
                set_sprite_prop(6, 0x00);
                set_sprite_prop(7, 0x00);
                companionFlip = 0;
            }
            if(companionAnimFrame == 3)
                companionAnimFrame = 0;
            else
                companionAnimFrame++;
            
        }
    }
    // Hacia la derecha
    else if(dir == 3){
        if(companionAnimFrame == 0 || companionAnimFrame == 2){  // Paso
            companionFlip = 1;   // Bandera que avisa inversion de Sprites
            set_sprite_tile(4, 45);
            set_sprite_tile(5, 44);
            set_sprite_tile(6, 42);
            set_sprite_tile(7, 43);
            set_sprite_prop(4, S_FLIPX);
            set_sprite_prop(5, S_FLIPX);
            set_sprite_prop(6, S_FLIPX);
            set_sprite_prop(7, S_FLIPX);
            companionAnimFrame++;
        }
        else if(companionAnimFrame == 1 || companionAnimFrame == 3){  // Centro
            set_sprite_tile(4, 49);
            set_sprite_tile(5, 48);
            set_sprite_tile(6, 47);
            set_sprite_tile(7, 46);
            if(companionAnimFrame == 3)
                companionAnimFrame = 0;
            else
                companionAnimFrame++;  
        }
    }
    // Hacia la izquierda
    else if(dir == 4){
        if(companionAnimFrame == 0 || companionAnimFrame == 2){  // Paso
            if(companionFlip){
                set_sprite_prop(4, 0x00);  // Quitar FlipX
                set_sprite_prop(5, 0x00);
                set_sprite_prop(6, 0x00);
                set_sprite_prop(7, 0x00);
                companionFlip = 0;
            }
            set_sprite_tile(4, 44);
            set_sprite_tile(5, 45);
            set_sprite_tile(6, 43);
            set_sprite_tile(7, 42);
            companionAnimFrame++;
        }
        else if(companionAnimFrame == 1 || companionAnimFrame == 3){  // Centro
            set_sprite_tile(4, 48);
            set_sprite_tile(5, 49);
            set_sprite_tile(6, 46);
            set_sprite_tile(7, 47);
            if(companionAnimFrame == 3)
                companionAnimFrame = 0;
            else
                companionAnimFrame++;   
        }
    }
}

void GetCompanionDir(UINT8 flag){
    char cDirX = (char)lastPosX - (char)trainer.x;
    char cDirY = (char)lastPosY - (char)trainer.y;

    if(cDirX < 0 && !flag){       // Se mueve hacia la derecha
        CompanionDir = 3;
    }
    else if(cDirX > 0 && !flag){   // Se mueve hacia la izquierda
        CompanionDir = 4;
    }
    else if(cDirY < 0 && flag){   // Se mueve hacia abajo
        CompanionDir = 2;
    }
    else if(cDirY > 0 && flag){   // Se mueve hacia arriba
        CompanionDir = 1;
    }
}

void CompanionMove(UINT8 flag){
    if(trainerLastDir != trainerDir){
        if((trainerLastDir == 3 && trainerDir == 1) || (trainerLastDir == 2 && trainerDir == 4)){
            scroll_sprite(4, 1, 1);
            scroll_sprite(5, 1, 1);
            scroll_sprite(6, 1, 1);
            scroll_sprite(7, 1, 1);
            if(flag){
            scroll_sprite(4, 0, 1);
            scroll_sprite(5, 0, 1);
            scroll_sprite(6, 0, 1);
            scroll_sprite(7, 0, 1);
            }
        }
        else if((trainerLastDir == 3 && trainerDir == 2) || (trainerLastDir == 1 && trainerDir == 4)){   // TLD = 3       // De derecha hacia abajo
            scroll_sprite(4, 1, -1);
            scroll_sprite(5, 1, -1);
            scroll_sprite(6, 1, -1);
            scroll_sprite(7, 1, -1);
            if(flag){
            scroll_sprite(4, 0, -1);
            scroll_sprite(5, 0, -1);
            scroll_sprite(6, 0, -1);
            scroll_sprite(7, 0, -1);
            }
        }
        else if((trainerLastDir == 4 && trainerDir == 1) || (trainerLastDir == 2 && trainerDir == 3)){
            scroll_sprite(4, -1, 1);
            scroll_sprite(5, -1, 1);
            scroll_sprite(6, -1, 1);
            scroll_sprite(7, -1, 1);
            if(flag){
            scroll_sprite(4, 0, 1);
            scroll_sprite(5, 0, 1);
            scroll_sprite(6, 0, 1);
            scroll_sprite(7, 0, 1);
            }
        }
        else if((trainerLastDir == 4 && trainerDir == 2) || (trainerLastDir == 1 && trainerDir == 3)){
            scroll_sprite(4, -1, -1);
            scroll_sprite(5, -1, -1);
            scroll_sprite(6, -1, -1);
            scroll_sprite(7, -1, -1);
            if(flag){
            scroll_sprite(4, 0, -1);
            scroll_sprite(5, 0, -1);
            scroll_sprite(6, 0, -1);
            scroll_sprite(7, 0, -1);
            }
        }
        else if(trainerDir == 3 && trainerLastDir == 4){
            scroll_sprite(4, -2, 0);
            scroll_sprite(5, -2, 0);
            scroll_sprite(6, -2, 0);
            scroll_sprite(7, -2, 0);
        }
        else if(trainerDir == 4 && trainerLastDir == 3){
            scroll_sprite(4, 2, 0);
            scroll_sprite(5, 2, 0);
            scroll_sprite(6, 2, 0);
            scroll_sprite(7, 2, 0); 
        }
        else if(trainerDir == 2 && trainerLastDir == 1){
            scroll_sprite(4, 0, -2);
            scroll_sprite(5, 0, -2);
            scroll_sprite(6, 0, -2);
            scroll_sprite(7, 0, -2);
            if(flag){
                scroll_sprite(4, 0, -2);
                scroll_sprite(5, 0, -2);
                scroll_sprite(6, 0, -2);
                scroll_sprite(7, 0, -2);  
            }
        }
        else if(trainerDir == 1 && trainerLastDir == 2){
            scroll_sprite(4, 0, 2);
            scroll_sprite(5, 0, 2);
            scroll_sprite(6, 0, 2);
            scroll_sprite(7, 0, 2);
            if(flag){
                scroll_sprite(4, 0, 2);
                scroll_sprite(5, 0, 2);
                scroll_sprite(6, 0, 2);
                scroll_sprite(7, 0, 2);
            }
        }
    }
}