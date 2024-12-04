#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdio.h>



int main (void){
	sfVideoMode mode = {544, 544, 32};
	sfRenderWindow* window;
	sfTexture* texture;
	sfSprite* sprite;

	/* Create the main window */
	window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);
	if (!window)
		return EXIT_FAILURE;

	sfRenderWindow_clear(window, sfBlue);

	/* Load a sprite to display */
	texture = sfTexture_createFromFile("../data/MAGENTA.png", NULL);
	if (!texture)
		return EXIT_FAILURE;


	sprite = sfSprite_create();
	sfSprite_setTexture(sprite, texture, sfTrue);
	sfVector2f position = {16, 16};

	sfSprite_setPosition(sprite, position);

	sfRenderWindow_drawSprite(window, sprite, NULL);
	sfRenderWindow_display(window);
	int character = getchar();

	exit(EXIT_SUCCESS);
}