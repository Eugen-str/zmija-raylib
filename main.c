#include <raylib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define SIRINA 1000
#define VISINA 750

#define BLOK 50

#define MAX_DULJINA 50

#define GLAVA (Color){0, 255, 0, 255}
#define REP1 (Color){0, 204, 0, 255}
#define REP2 (Color){0, 180, 0, 255}

#define SVJETLO (Color){89, 89, 89, 255}
#define TAMNO (Color){51, 51, 51, 255}


typedef uint8_t u8;
typedef size_t usize;

typedef enum{
    Gore,
    Dolje,
    Lijevo,
    Desno,
}Smjer;

typedef struct{
    u8 x;
    u8 y;
}Vec2;

struct Zmija{
    Vec2 glava;
    Vec2 rep[MAX_DULJINA];

    u8 duljina;
};

struct Zmija zmija_init(u8 x, u8 y){
    struct Zmija temp;
    
    temp.glava.x = x;
    temp.glava.y = y;

    temp.duljina = 3;

    temp.rep[0].x = x-1;
    temp.rep[0].y = y;
    return temp;
}

Vec2 gen_jabuka(){
    Vec2 jabuka;
    
    jabuka.x = GetRandomValue(0, SIRINA/BLOK -1);
    jabuka.y = GetRandomValue(0, VISINA/BLOK -1);

    return jabuka;
}

void ploca(){
    for (usize i = 0; i < 15; i++){
        for (usize j = 0; j < 20; j++){
            Color boja = TAMNO;
            if((i+j)%2 == 0) 
                boja = SVJETLO;

            DrawRectangle(j*BLOK, i*BLOK, BLOK, BLOK, boja);
        }
    }
}

void nacrtaj_zmiju(struct Zmija z, Vec2 jabuka){
    DrawRectangle(jabuka.x*BLOK, jabuka.y*BLOK, BLOK, BLOK, RED);

    DrawRectangle(z.glava.x*BLOK, z.glava.y*BLOK, BLOK, BLOK, GLAVA);

    for(usize i = 0; i < z.duljina - 1; i++){
        if(i%2==0) 
            DrawRectangle(z.rep[i].x*BLOK, z.rep[i].y*BLOK, BLOK, BLOK, REP1);
        else
            DrawRectangle(z.rep[i].x*BLOK, z.rep[i].y*BLOK, BLOK, BLOK, REP2);
    }
}

struct Zmija zmija_pomak(struct Zmija zmija, Smjer d, Vec2 jabuka){    
    zmija.rep[0].x = zmija.glava.x;
    zmija.rep[0].y = zmija.glava.y;
    
    for(usize i = zmija.duljina - 1; i > 0; i--){
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

int main(){
    InitWindow(SIRINA, VISINA, "Zmija");

    SetWindowState(FLAG_VSYNC_HINT);

    struct Zmija zmija;
    zmija = zmija_init(7, 5);
    
    Vec2 jabuka;
    jabuka = gen_jabuka();

    Smjer dir = Lijevo;
    usize br_frame = 0;

    while(!WindowShouldClose()){
        BeginDrawing();

        ClearBackground(BLACK);

        if(IsKeyPressed(KEY_UP)) dir = Gore;
        if(IsKeyPressed(KEY_DOWN)) dir = Dolje;
        if(IsKeyPressed(KEY_LEFT)) dir = Lijevo;
        if(IsKeyPressed(KEY_RIGHT)) dir = Desno;

        br_frame++;


        if(br_frame/30 == 1){
            zmija = zmija_pomak(zmija, dir, jabuka);
            if(zmija.glava.x == jabuka.x && zmija.glava.y == jabuka.y){
                jabuka = gen_jabuka();
            }
            br_frame = 0;
        }

        ploca();
        nacrtaj_zmiju(zmija, jabuka);

        DrawText(TextFormat("duljina: %d", zmija.duljina -1), 10, 10, 30, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}