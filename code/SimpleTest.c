#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include "engine/logic/bbTerminal.h"

sfRenderStates bbViewport_renderer;
sfShader* bbViewport_shader;

char vertShader[] = "\
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

//Fragment Shader does not work
char fragShader2[] = "\
    \
    void main()\
    {\
	    if(floor(gl_TexCoord[0].x) == floor(gl_TexCoord[0].y)){\
	        gl_FragColor.r = 0.0;\
            gl_FragColor.g = 0.0;\
            gl_FragColor.b = 0.0;\
            gl_FragColor.a = 1.0;\
        } else if (floor(gl_TexCoord[0].x) == 0.0){\
		    gl_FragColor.r = 1.0;\
            gl_FragColor.g = 0.0;\
            gl_FragColor.b = 0.0;\
            gl_FragColor.a = 1.0;\
        }else if (floor(gl_TexCoord[0].y) == 0.0){\
		    gl_FragColor.r = 0.0;\
            gl_FragColor.g = 1.0;\
            gl_FragColor.b = 1.0;\
            gl_FragColor.a = 1.0;\
        } else {\
		    gl_FragColor = gl_Color;\
		}\
    };";

// (0,0) is top left, x points to the right, y points down
char fragShader3[] = "\
    \
    void main()\
    {\
	        gl_FragColor.r = gl_TexCoord[0].x;\
            gl_FragColor.g = gl_TexCoord[0].y;\
            gl_FragColor.b = 0.0;\
            gl_FragColor.a = 1.0;\
    };";

//gl_TexCoord[0].x and gl_TexCoord[0].y range from 0 to 1;
char fragShader4[] = "\
    \
    void main()\
    {\
		if(gl_TexCoord[0].x < 0.5 && gl_TexCoord[0].y < 0.5){\
	        gl_FragColor.r = gl_TexCoord[0].x;\
            gl_FragColor.g = gl_TexCoord[0].y;\
            gl_FragColor.b = 0.0;\
            gl_FragColor.a = 1.0;\
		} else {\
		    gl_FragColor = gl_Color;\
        }\
    };";

//gl_FragCoord gives the number of pixels from the bottom-left of the render target
// x points to the right, y points upward
char fragShader5[] = "\
    \
    void main()\
    {\
		if(gl_FragCoord.x < 256 && gl_FragCoord.y < 256){\
	        gl_FragColor.r = gl_FragCoord.x / 256;\
            gl_FragColor.g = gl_FragCoord.y / 256;\
            gl_FragColor.b = 0.0;\
            gl_FragColor.a = 1.0;\
		} else {\
		    gl_FragColor = gl_Color;\
        }\
    };";

char fragShader[] = "\
    \
    void main()\
    {\
		if(gl_FragCoord.x < 256 && gl_FragCoord.y < 256){\
	        gl_FragColor.r = gl_FragCoord.x / 256;\
            gl_FragColor.g = gl_FragCoord.y / 256;\
            gl_FragColor.b = 0.0;\
            gl_FragColor.a = 1.0;\
		} else {\
		    gl_FragColor = gl_Color;\
        }\
    };";

int main (void){
	sfVideoMode mode = {544, 544, 32};
	sfRenderWindow* window;
	sfTexture *magentaTexture, *blueTexture;
	sfSprite* sprite;
	sfShader* shader;
	sfRenderStates renderStates;

	/* Create the main window */
	window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);
	bbAssert(window != NULL, "sfRenderWindow_create failed\n");

	sfRenderWindow_clear(window, sfBlue);

	/* Load some textures */
	magentaTexture = sfTexture_createFromFile("../data/MAGENTA.png", NULL);
	bbAssert(magentaTexture != NULL, "sfTexture_createFromFile failed\n");
	blueTexture = sfTexture_createFromFile("../data/BLUE.png", NULL);
	bbAssert(blueTexture != NULL, "sfTexture_createFromFile failed\n");

	/* Load a sprite to display */
	sprite = sfSprite_create();
	sfSprite_setTexture(sprite, magentaTexture, sfTrue);
	sfVector2f position = {16, 16};

	sfSprite_setPosition(sprite, position);

	/* compile shaders */

	shader = sfShader_createFromMemory(vertShader, NULL, fragShader);
	bbAssert(shader != NULL, "sfShader_createFromMemory failed\n");

	renderStates.shader = shader;
	renderStates.blendMode = sfBlendAlpha;
	renderStates.transform = sfTransform_Identity;
	renderStates.texture = blueTexture;

	/* draw to window */

	sfRenderWindow_drawSprite(window, sprite, &renderStates);
	sfRenderWindow_display(window);

	int character = getchar();

	exit(EXIT_SUCCESS);
}