#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


#define PIXELS_PER_TILE          16
#define HEIGHT_MAP_SIZE          512
#define HEIGHT_MAP_PADDING       16
int bbElevations[HEIGHT_MAP_SIZE][HEIGHT_MAP_SIZE];

int bbTileCoords_getElevation(int i, int j){
    i = i + HEIGHT_MAP_PADDING;
    j = j + HEIGHT_MAP_PADDING;


    if (i < 0 || j < 0 || i > HEIGHT_MAP_SIZE || j > HEIGHT_MAP_SIZE){
        return 0;
        //elevation out of bounds
    }
    return bbElevations[i][j];
}

int floordiv (int a , int b){

    if (b == 0) printf("divide by 0\n");

    int c = a / b;

    if (a % b == 0) return c;

    if ((a < 0 && b > 0) || (a > 0 && b < 0)) return (c - 1);

    return c;

}

float lanczos(float x){

    if (x < 0.01 && x>-0.01){

        return (
                1.0
                - 5.0 * M_PI * M_PI * x * x / 24.0
                + 91 * M_PI * M_PI * M_PI * M_PI * x * x * x * x / 5760.0
        );
    }

    if (x>= 2 || x <= -2) return 0;

    return 2*sin(x * M_PI) * sin( x * M_PI/2.0) / (M_PI * M_PI * x * x);

}

float lanczos_Elevation(int mci, int mcj){

    int tci = floordiv (mci,  PIXELS_PER_TILE);
    int tcj = floordiv (mcj,  PIXELS_PER_TILE);



    int residual_i, residual_j;
    float normalised_residual_i, normalised_residual_j;

    residual_i = mci % (PIXELS_PER_TILE);
    residual_j = mcj % (PIXELS_PER_TILE);

    normalised_residual_i =
            (residual_i * 1.0) / (1.0 * (PIXELS_PER_TILE));

    normalised_residual_j =
            (residual_j * 1.0) / (1.0 * (PIXELS_PER_TILE));

    float S = 0; float s;
    int i, j;

    for(i=-5; i < 5; i++){
        for (j=-5; j < 5; j++){

            s = bbTileCoords_getElevation(tci + i, tcj + j);
            S+= s*lanczos(i - normalised_residual_i) *lanczos( j - normalised_residual_j);
        }


    }


    return S / 512.0;

};

int main (void){

    /* declare structures */
    sfVideoMode mode = {544, 544, 32};
    sfRenderWindow* window;

    sfImage*          Hill_Shading_CPU;
    sfVector2u        Hill_Shading_Size;
    sfUint8*          Hill_Shading_Data;
    sfRenderTexture*  Hill_Shading_Render_Texture;
    sfTexture*        Hill_Shading_Texture;
    sfSprite*         Hill_Shading_Sprite;



    /* Create the main window */
    window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);
    //bbAssert(window != NULL, "sfRenderWindow_create failed\n");

    sfRenderWindow_clear(window, sfCyan);

    /* do the maths */

    Hill_Shading_CPU = sfImage_createFromFile("../data/HEIGHTMAP.png");
    Hill_Shading_Size = sfImage_getSize (Hill_Shading_CPU);
    Hill_Shading_Data = sfImage_getPixelsPtr(Hill_Shading_CPU);

    for (int i = 0; i < HEIGHT_MAP_SIZE; i++){
        for (int j = 0; j < HEIGHT_MAP_SIZE; j++) {
            if (i >= Hill_Shading_Size.x || j >= Hill_Shading_Size.y){
                bbElevations[i][j] = 0;
            } else {
                bbElevations[i][j] =  Hill_Shading_Data[(j + i * Hill_Shading_Size.x) * 4];
                //*4 because we want the R coodinate of RGBA values.
                //y -> i, x -> j
            }
        }
    }
    sfImage_destroy (Hill_Shading_CPU);


    Hill_Shading_Data = calloc(512*512*4, sizeof (sfUint8));
    for(int i = 0; i <512; i++){
        for (int j = 0; j < 512; j++){

            float output = lanczos_Elevation(i,j);

            if (output <= 0) output = 0;
            if (output >= 1) output = 1;

            int Dest_Coord = (i + 512 * j) * 4;

            Hill_Shading_Data[Dest_Coord+0] = output * 255;
            Hill_Shading_Data[Dest_Coord+1] = output * 255;
            Hill_Shading_Data[Dest_Coord+2] = output * 255;
            Hill_Shading_Data[Dest_Coord+3] = 255;

        }
    }

    Hill_Shading_CPU = sfImage_createFromPixels (512,512, Hill_Shading_Data);

    sfIntRect rect = {0,0,512, 512};
    Hill_Shading_Texture = sfTexture_createFromImage(Hill_Shading_CPU, &rect);

    Hill_Shading_Sprite = sfSprite_create();

    sfSprite_setTexture(Hill_Shading_Sprite, Hill_Shading_Texture, sfTrue);

    sfVector2f position = {16, 16};
    sfSprite_setPosition(Hill_Shading_Sprite, position);

    sfRenderWindow_drawSprite(window, Hill_Shading_Sprite, NULL);
    sfRenderWindow_display(window);

    while (1) {
        sfEvent event;
        sfRenderWindow_waitEvent(window, &event);
        if (event.type == sfEvtKeyPressed) break;
    }

    exit(EXIT_SUCCESS);
}