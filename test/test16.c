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
	#define TILES_PER_SQUARE      32\n\
		#define HEIGHTMAP_PADDING     0\n\
\
\
	uniform float L_Kernel[65];\
    uniform float GD_Kernel[97];\
    uniform int square_x;\
    uniform int square_y;\
    uniform sampler2D HeightMap;\
    \
    void main()\
    {\
		int x = int(floor (gl_TexCoord[0].x * 512.0f));\
		int y = int(floor (gl_TexCoord[0].y * 512.0f));\
		int tile_x = x / PIXELS_PER_TILE;\
		int tile_y = y / PIXELS_PER_TILE;\
		int remainder_x = x % PIXELS_PER_TILE;\
		int remainder_y = y % PIXELS_PER_TILE;\
		int heghtmap_x = tile_x + HEIGHTMAP_PADDING + square_x / (512 /PIXELS_PER_TILE);\
		int heghtmap_y = tile_y + HEIGHTMAP_PADDING + square_y / (512 /PIXELS_PER_TILE);\
\
		float partial_x = 0;\
		float partial_y = 0;\
		\
		for (int i = -3; i<3; i++){\
		    for (int j = -2; j<2; j++){\
				int X = (i+1)*PIXELS_PER_TILE - remainder_x;\
				int Y = (j+1)*PIXELS_PER_TILE - remainder_y;\
				vec2 myCoord2;\
				myCoord2.x = (heghtmap_x + i) * (1.0f/512.0f); \
				myCoord2.y = (heghtmap_y + j) * (1.0f/512.0f); \
				float foo = texture2D(HeightMap, myCoord2).r / 2.0;\
		        partial_x += foo * GD_Kernel[X+48]*L_Kernel[Y+32];\
            }\
        }\
\
		\
		for (int i = -2; i<2; i++){\
		    for (int j = -3; j<3; j++){\
				int X = (i+1)*PIXELS_PER_TILE - remainder_x;\
				int Y = (j+1)*PIXELS_PER_TILE - remainder_y;\
				vec2 myCoord2;\
				myCoord2.x = (tile_x + i) * (1.0f/512.0f); \
				myCoord2.y = (tile_y + j) * (1.0f/512.0f); \
				float foo = texture2D(HeightMap, myCoord2).r / 2.0;\
		        partial_y += foo * L_Kernel[X+32]*GD_Kernel[Y+48];\
            }\
        }\
		vec3 surface_normal;\
		surface_normal.x = partial_x;\
        surface_normal.y = -partial_y;\
		surface_normal.z = 0.1;\
        surface_normal = normalize(surface_normal);\
\
		vec3 light_incedence;\
		light_incedence.x = 1.0;\
		light_incedence.y = 1.0;\
		light_incedence.z = 1.0;\
        light_incedence = normalize(light_incedence);\
\
		float hillshading = dot(surface_normal, light_incedence);\
		gl_FragColor.r = hillshading;\
		gl_FragColor.g = hillshading;\
		gl_FragColor.b = hillshading;\
		gl_FragColor.a = 1.0f;\
    }";


const float L_Kernel[65] = {0.000000, -0.001032, -0.004303, -0.009948,
                            -0.017905, -0.027892, -0.039389, -0.051644, -0.063684, -0.074349, -0.082335,
                            -0.086260, -0.084725, -0.076399, -0.060095, -0.034851, 0.000000, 0.044764,
                            0.099341, 0.163195, 0.235347, 0.314384, 0.398503, 0.485562, 0.573159, 0.658729,
                            0.739643, 0.813323, 0.877354, 0.929589, 0.968246, 0.991992, 1.000000, 0.991992,
                            0.968246, 0.929589, 0.877354, 0.813323, 0.739643, 0.658729, 0.573159, 0.485562,
                            0.398503, 0.314384, 0.235347, 0.163195, 0.099341, 0.044764, 0.000000, -0.034851,
                            -0.060095, -0.076399, -0.084725, -0.086260, -0.082335, -0.074349, -0.063684,
                            -0.051644, -0.039389, -0.027892, -0.017905, -0.009948, -0.004303, -0.001032,
                            0.000000};

const float I_Kernel[65] = {0.000000, 0.000000, 0.000000, 0.000000,
                            0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
                            0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
                            0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
                            0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
                            0.0, 1.000000, 1.000000,
                            1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
                            1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 0.000000, 0.000000,
                            0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
                            0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
                            0.000000};

const float GD_Kernel[97] = {-0.000370, -0.000525, -0.000739, -0.001032,
                             -0.001429, -0.001962, -0.002670, -0.003605, -0.004826, -0.006407, -0.008433,
                             -0.011006, -0.014242, -0.018272, -0.023241, -0.029305, -0.036631, -0.045388,
                             -0.055742, -0.067850, -0.081849, -0.097841, -0.115890, -0.135997, -0.158099,
                             -0.182045, -0.207594, -0.234401, -0.262014, -0.289875, -0.317321, -0.343598,
                             -0.367879, -0.389285, -0.406913, -0.419876, -0.427337, -0.428549, -0.422896,
                             -0.409929, -0.389400, -0.361286, -0.325806, -0.283425, -0.234853, -0.181023,
                             -0.123062, -0.062256, 0.000000, 0.062256, 0.123062, 0.181023, 0.234853, 0.283425,
                             0.325806, 0.361286, 0.389400, 0.409929, 0.422896, 0.428549, 0.427337, 0.419876,
                             0.406913, 0.389285, 0.367879, 0.343598, 0.317321, 0.289875, 0.262014, 0.234401,
                             0.207594, 0.182045, 0.158099, 0.135997, 0.115890, 0.097841, 0.081849, 0.067850,
                             0.055742, 0.045388, 0.036631, 0.029305, 0.023241, 0.018272, 0.014242, 0.011006,
                             0.008433, 0.006407, 0.004826, 0.003605, 0.002670, 0.001962, 0.001429, 0.001032,
                             0.000739, 0.000525, 0.000000};




int calcHillShading(sfRenderTexture** out, int square_i, int square_j){

	static sfRenderStates* hillShadingStates = NULL;
	if (hillShadingStates == NULL){
		hillShadingStates = malloc(sizeof (*hillShadingStates));

		sfTexture* hillTexture = sfTexture_createFromFile("../data/HEIGHTMAP2.png", NULL);
		bbAssert(hillTexture != NULL, "sfTexture_createFromFile failed\n");

		sfShader* shader = sfShader_createFromMemory(vertShader, NULL, fragShader);
		bbAssert(shader != NULL, "sfShader_createFromMemory failed\n");

		sfShader_setFloatUniformArray(shader, "L_Kernel", L_Kernel, 65);
		sfShader_setFloatUniformArray(shader, "GD_Kernel", GD_Kernel, 97);
		sfShader_setTextureUniform(shader, "HeightMap", hillTexture);



		hillShadingStates->shader = shader;
		hillShadingStates->blendMode = sfBlendAlpha;
		hillShadingStates->transform = sfTransform_Identity;
		hillShadingStates->texture = hillTexture;

	}

	sfShader_setIntUniform(hillShadingStates->shader, "square_x", square_i);
	sfShader_setIntUniform(hillShadingStates->shader, "square_y", square_j);
	sfSprite* sprite = sfSprite_create();
	sfSprite_setTexture(sprite, hillShadingStates->texture, sfTrue);

	sfRenderTexture* outTexture = sfRenderTexture_create(512, 512, false);

	sfRenderTexture_drawSprite(outTexture, sprite, hillShadingStates);
	sfRenderTexture_display(outTexture);

	*out = outTexture;

	return 0;
}

int main(void){

	sfVideoMode mode = {512, 512, 32};
	sfRenderWindow* window;
	window = sfRenderWindow_create(mode, "Test 16", sfResize | sfClose, NULL);
	bbAssert(window != NULL, "sfRenderWindow_create failed\n");

	sfRenderTexture* renderTexture;
	calcHillShading(&renderTexture, 0, 0);

	sfTexture* texture = sfRenderTexture_getTexture(renderTexture);

	sfSprite* sprite = sfSprite_create();
	sfSprite_setTexture(sprite, texture, sfTrue);
	sfRenderWindow_drawSprite(window, sprite, NULL);
	sfRenderWindow_display(window);
	sfSleep(sfSeconds(1));
}