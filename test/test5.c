#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include "engine/logic/bbTerminal.h"

sfRenderStates bbViewport_renderer;
sfShader* bbViewport_shader;

char vertShader[] = "\n\
    \
	uniform vec2 offset;\
    void main()\
    {\
    \
    vec4 offsetvertex = vec4(offset.x, offset.y, 0, 0);\
    gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + offsetvertex);\
    \
        gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\
    \
        gl_FrontColor = gl_Color;\
		\
    }";


// {trunc(gl_FragCoord.x), trunc(gl_FragCoord.y)} are the desired coordinates
// where the origin is in the bottom right
char fragShader[] = "\
    uniform sampler2D DFGDFG;\n\
    #define PIXELS_PER_TILE       16\n\
\
    void main()\
    {\
		\
		vec2 myCoord = gl_TexCoord[0].xy / PIXELS_PER_TILE;\
		gl_FragColor = texture2D(DFGDFG, myCoord);\
    }";

int main (void){



	sfVideoMode mode = {544, 544, 32};
	sfRenderWindow* window;
	sfTexture *magentaTexture, *blueTexture;
	sfSprite* sprite;
	sfShader* shader;
	sfRenderStates renderStates;
	sfTexture* checkTexture;

	sfRenderTexture* renderTexture1;
	sfTexture* texture1;
	sfSprite* sprite1;

	/* Create the main window */
	window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);
	bbAssert(window != NULL, "sfRenderWindow_create failed\n");

	sfRenderWindow_clear(window, sfCyan);

	/* Load some textures */
	magentaTexture = sfTexture_createFromFile("../data/MAGENTA.png", NULL);
	bbAssert(magentaTexture != NULL, "sfTexture_createFromFile failed\n");
	blueTexture = sfTexture_createFromFile("../data/BLUE.png", NULL);
	bbAssert(blueTexture != NULL, "sfTexture_createFromFile failed\n");
	checkTexture = sfTexture_createFromFile("../data/TEST5.png", NULL);
	bbAssert(checkTexture != NULL, "sfTexture_createFromFile failed\n");

	/* Load a sprite to display */
	sprite = sfSprite_create();
	sfSprite_setTexture(sprite, magentaTexture, sfTrue);


	renderTexture1 = sfRenderTexture_create(512, 512, sfFalse);
	texture1 = sfRenderTexture_getTexture(renderTexture1);
	sprite1 = sfSprite_create();
	sfSprite_setTexture(sprite1, texture1, sfTrue);
	sfVector2f position = {16, 16};
	sfSprite_setPosition(sprite1, position);

	/* compile shaders */

	shader = sfShader_createFromMemory(vertShader, NULL, fragShader);
	bbAssert(shader != NULL, "sfShader_createFromMemory failed\n");

	renderStates.shader = shader;
	renderStates.blendMode = sfBlendAlpha;
	renderStates.transform = sfTransform_Identity;
	renderStates.texture = blueTexture;

	/* set uniform(s) */
	sfShader_setTextureUniform(shader, "DFGDFG", checkTexture);

	/* draw to window */

	sfRenderTexture_drawSprite(renderTexture1, sprite, &renderStates);
	sfRenderTexture_display(renderTexture1);

	sfRenderWindow_drawSprite(window, sprite1, NULL);
	sfRenderWindow_display(window);

	int character = getchar();

	exit(EXIT_SUCCESS);
}