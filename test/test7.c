#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include "engine/logic/bbTerminal.h"
#include "engine/maths/bbGPU_Interpolation.h"

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
    #define PIXELS_PER_TILE       16\n\
	\
    const float[] L_Kernel = {0.000000, -0.001032, -0.004303, -0.009948, \
-0.017905, -0.027892, -0.039389, -0.051644, -0.063684, -0.074349, -0.082335, \
-0.086260, -0.084725, -0.076399, -0.060095, -0.034851, 0.000000, 0.044764, \
0.099341, 0.163195, 0.235347, 0.314384, 0.398503, 0.485562, 0.573159, 0.658729, \
0.739643, 0.813323, 0.877354, 0.929589, 0.968246, 0.991992, 1.000000, 0.991992, \
0.968246, 0.929589, 0.877354, 0.813323, 0.739643, 0.658729, 0.573159, 0.485562, \
0.398503, 0.314384, 0.235347, 0.163195, 0.099341, 0.044764, 0.000000, -0.034851,\
-0.060095, -0.076399, -0.084725, -0.086260, -0.082335, -0.074349, -0.063684,\
-0.051644, -0.039389, -0.027892, -0.017905, -0.009948, -0.004303, -0.001032,\
0.000000};\
\
    const float[] GD_Kernel = {-0.000370, -0.000525, -0.000739, -0.001032,\
-0.001429, -0.001962, -0.002670, -0.003605, -0.004826, -0.006407, -0.008433,\
-0.011006, -0.014242, -0.018272, -0.023241, -0.029305, -0.036631, -0.045388,\
-0.055742, -0.067850, -0.081849, -0.097841, -0.115890, -0.135997, -0.158099,\
-0.182045, -0.207594, -0.234401, -0.262014, -0.289875, -0.317321, -0.343598,\
-0.367879, -0.389285, -0.406913, -0.419876, -0.427337, -0.428549, -0.422896,\
-0.409929, -0.389400, -0.361286, -0.325806, -0.283425, -0.234853, -0.181023,\
-0.123062, -0.062256, 0.000000, 0.062256, 0.123062, 0.181023, 0.234853, 0.283425,\
0.325806, 0.361286, 0.389400, 0.409929, 0.422896, 0.428549, 0.427337, 0.419876,\
0.406913, 0.389285, 0.367879, 0.343598, 0.317321, 0.289875, 0.262014, 0.234401,\
0.207594, 0.182045, 0.158099, 0.135997, 0.115890, 0.097841, 0.081849, 0.067850,\
0.055742, 0.045388, 0.036631, 0.029305, 0.023241, 0.018272, 0.014242, 0.011006,\
0.008433, 0.006407, 0.004826, 0.003605, 0.002670, 0.001962, 0.001429, 0.001032,\
0.000739, 0.000525, 0.000000};\
    \
    void main()\
    {\
		int x = trunc (gl_TexCoord[0].x * 512.0f);\
		int y = trunc (gl_TexCoord[0].y * 512.0f); \
		int tile_x = x / PIXELS_PER_TILE;\
		int tile_y = y / PIXELS_PER_TILE;\
		int remainder_x = x % PIXELS_PER_TILE;\
		int remainder_y = y % PIXELS_PER_TILE;\
\
		int z = (x % 64);\
		float w = ((L_Kernel[z]) + 1.0)/2;\
		z = (y % 96);\
		float bar = ((GD_Kernel[z]) + 1.0)/2;\
\
		vec2 myCoord = gl_TexCoord[0].xy / PIXELS_PER_TILE;\
		gl_FragColor.r = bar;\
		gl_FragColor.g = 0.0f;\
		gl_FragColor.b = w;\
		gl_FragColor.a = 1.0f;\
\
\
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
	checkTexture = sfTexture_createFromFile("../data/CHECK.png", NULL);
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
	//sfShader_setTextureUniform(shader, "DFGDFG", checkTexture);

	/* draw to window */

	sfRenderTexture_drawSprite(renderTexture1, sprite, &renderStates);
	sfRenderTexture_display(renderTexture1);

	sfRenderWindow_drawSprite(window, sprite1, NULL);
	sfRenderWindow_display(window);

	while (1) {
		sfEvent event;
		sfRenderWindow_waitEvent(window, &event);
		if (event.type == sfEvtKeyPressed) break;
	}

	exit(EXIT_SUCCESS);
}
