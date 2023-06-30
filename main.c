#include <raylib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define SIRINA 1000
#define VISINA 750

#define BLOK 50

#define MAX_DULJINA SIRINA/BLOK * VISINA/BLOK

#define GLAVA (Color){0, 255, 0, 255}
#define REP1 (Color){0, 204, 0, 255}
#define REP2 (Color){0, 180, 0, 255}
#define ZID (Color){25, 25, 25, 255}
#define SVJETLO (Color){89, 89, 89, 255}
#define TAMNO (Color){51, 51, 51, 255}

typedef enum{
    Gore,
    Dolje,
    Lijevo,
    Desno,
}Smjer;

typedef struct{
    int x;
    int y;
}Vec2;

struct Zmija{
    Vec2 glava;
    Vec2 rep[MAX_DULJINA];

    int duljina;
};

struct Zmija zmija_init(int x, int y){
    struct Zmija temp;
    
    temp.glava.x = x;
    temp.glava.y = y;

    temp.duljina = 3;

    temp.rep[0].x = x-1;
    temp.rep[0].y = y;
    return temp;
}

Vec2 gen_jabuka(struct Zmija z){
    Vec2 jabuka;
    bool provjera = true;

    do{
        jabuka.x = GetRandomValue(1, SIRINA/BLOK -2);
        jabuka.y = GetRandomValue(1, VISINA/BLOK -2);

        if(jabuka.x == z.glava.x && jabuka.y == z.glava.y)
            provjera = false;

        for(int i = 0; i < z.duljina -1; i++){
            if(jabuka.x == z.rep[i].x && jabuka.y == z.rep[i].y){
                provjera = false;
                break;
            }
        }
    }while(!provjera);

    return jabuka;
}

void ploca(){
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 20; j++){
            if(i == 0 || i == VISINA/BLOK - 1 || j == 0 || j == SIRINA/BLOK - 1)
                DrawRectangle(j*BLOK, i*BLOK, BLOK, BLOK, ZID);
            else if((i+j)%2 == 1)
                DrawRectangle(j*BLOK, i*BLOK, BLOK, BLOK, TAMNO);
            else
                DrawRectangle(j*BLOK, i*BLOK, BLOK, BLOK, SVJETLO);
        }
    }
}

void nacrtaj_zmiju(struct Zmija z, Vec2 jabuka){
    DrawRectangle(jabuka.x*BLOK, jabuka.y*BLOK, BLOK, BLOK, RED);

    DrawRectangle(z.glava.x*BLOK, z.glava.y*BLOK, BLOK, BLOK, GLAVA);

    for(int i = 0; i < z.duljina - 1; i++){
        if(i%2==0) 
            DrawRectangle(z.rep[i].x*BLOK, z.rep[i].y*BLOK, BLOK, BLOK, REP1);
        else
            DrawRectangle(z.rep[i].x*BLOK, z.rep[i].y*BLOK, BLOK, BLOK, REP2);
    }
}

struct Zmija zmija_pomak(struct Zmija zmija, Smjer d, Vec2 jabuka){    
    zmija.rep[0].x = zmija.glava.x;
    zmija.rep[0].y = zmija.glava.y;
    
    for(int i = zmija.duljina - 1; i > 0; i--){
        zmija.rep[i].x = zmija.rep[i-1].x;
        zmija.rep[i].y = zmija.rep[i-1].y;
    }
    
    switch (d)
    {
        case Lijevo:
            zmija.glava.x-=1;
            break;
        case Desno:
            zmija.glava.x+=1;
            break;
        case Gore:
            zmija.glava.y-=1;
            break;
        case Dolje:
            zmija.glava.y+=1;
            break;
    }

    if(zmija.glava.x == jabuka.x && zmija.glava.y == jabuka.y){
        zmija.duljina ++;
    }
    return zmija;
}

bool provjeri_smrt(struct Zmija z){
    for(int i = 0; i < z.duljina; i++){
        if(z.glava.x == z.rep[i].x && z.glava.y == z.rep[i].y)
            return true;
    }

    if(z.glava.x == 0 || z.glava.x == SIRINA/BLOK-1 || z.glava.y == 0 || z.glava.y == VISINA/BLOK-1)
        return true;
    return false;
}

int main(){
    InitWindow(SIRINA, VISINA, "Zmija");

    SetWindowState(FLAG_VSYNC_HINT);

    struct Zmija zmija;
    zmija = zmija_init(7, 5);
    
    Vec2 jabuka;
    jabuka = gen_jabuka(zmija);

    Smjer dir = Desno;
    size_t br_frame = 0;

    bool mrtva = false;
    while(!WindowShouldClose()){
        BeginDrawing();

        ClearBackground(BLACK);

        if(IsKeyPressed(KEY_UP)) dir = Gore;
        if(IsKeyPressed(KEY_DOWN)) dir = Dolje;
        if(IsKeyPressed(KEY_LEFT)) dir = Lijevo;
        if(IsKeyPressed(KEY_RIGHT)) dir = Desno;

        br_frame++;

        if(br_frame/20 == 1 && !mrtva){
            zmija = zmija_pomak(zmija, dir, jabuka);

            if(zmija.glava.x == jabuka.x && zmija.glava.y == jabuka.y){
                jabuka = gen_jabuka(zmija);
            }

            mrtva = provjeri_smrt(zmija);

            br_frame = 0;
        }

        ploca();
        nacrtaj_zmiju(zmija, jabuka);

        if(!mrtva)
            DrawText(TextFormat("duljina: %d", zmija.duljina -1), 10, 10, 30, WHITE);
        else
            DrawText(TextFormat("Kraj\nDuljina: %d", zmija.duljina -1), SIRINA/2-200, VISINA/2 - 200, 80, WHITE);
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}