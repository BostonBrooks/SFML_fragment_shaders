#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <SFML/System.h>
#include <SFML/OpenGL.h>
#include <stdio.h>
#include "engine/logic/bbTerminal.h"

int main (void){

    sfVideoMode mode = {544, 544, 32};
    sfRenderWindow* window;
    window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);
    bbAssert(window != NULL, "sfRenderWindow_create failed\n");



    char* str = glGetString(GL_VERSION);

    printf("GL_VERSION = %s\n", str);


}