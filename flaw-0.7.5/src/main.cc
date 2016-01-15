/*
 * F.L.A.W.
 * Copyright (C) Denis Comtesse 2008 <denis@problematic.de>
 *
 * F.L.A.W. is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * F.L.A.W. is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define VERSION "0.7.5"

#ifdef _WIN32
#undef main
#endif

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#ifdef __linux__
//#	define PATH "data/"
#	define PATH PACKAGE_DATA_DIR"/flaw/"
#   define CONF "%s/.flaw", getenv("HOME")
#else
#	define PATH "D:/data/"
#   define CONF "flaw-config"
#endif

#include <iostream>
#include <fstream>
#include <ctime>

#include <cstdlib>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "menu.h"
#include "game.h"


int main (int argc, char *argv[])
{

	char configfile[60];
	sprintf(configfile, CONF);

	// *** initialize SDL ***
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) {
		printf("Can't init SDL:  %s\n", SDL_GetError());
		exit(1);
	}

	if(TTF_Init()==-1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}

	//SDL_ShowCursor (SDL_DISABLE);
	srand (time(NULL)); // random numbers


	// Load titlescreen, menu icons, fonts:
	SDL_Surface *wizardicon[5], *pointicon, *emptypointicon, *noicon, *title;

	if (!(title = IMG_Load (PATH"misc/title.png"))){
		cout << IMG_GetError();
	exit(1);
	}

	if (!(wizardicon[0] = IMG_Load (PATH"misc/wizard_blue.png"))){
		cout << IMG_GetError();
		exit(1);
	}
	if (!(wizardicon[1] = IMG_Load (PATH"misc/wizard_red.png"))){
		cout << IMG_GetError();
		exit(1);
	}
	if (!(wizardicon[2] = IMG_Load (PATH"misc/wizard_green.png"))){
		cout << IMG_GetError();
		exit(1);
	}
	if (!(wizardicon[3] = IMG_Load (PATH"misc/wizard_purple.png"))){
		cout << IMG_GetError();
		exit(1);
	}
	if (!(wizardicon[4] = IMG_Load (PATH"misc/wizard_black.png"))){
		cout << IMG_GetError();
		exit(1);
	}

	
	if (!(pointicon = IMG_Load (PATH"misc/point.png"))){
		cout << IMG_GetError();
		exit(1);
	}
	if (!(emptypointicon = IMG_Load (PATH"misc/nopoint.png"))){
		cout << IMG_GetError();
		exit(1);
	}
	if (!(noicon = IMG_Load (PATH"misc/blank.png"))){
		cout << IMG_GetError();
		exit(1);
	}
	

	TTF_Font *smallfont;
	smallfont = TTF_OpenFont(PATH"tuffy_bold.ttf", 11);
	if(!smallfont) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}


	TTF_Font *normalfont;
	normalfont = TTF_OpenFont(PATH"tuffy.ttf", 26);
	if(!normalfont) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}

	TTF_Font *boldfont;
	boldfont = TTF_OpenFont(PATH"tuffy_bold.ttf", 26);
	if(!boldfont) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}


	Game game; //create "game" object

	//init sound, joysticks
	game.AudioSetup();

	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	int numjoy = SDL_NumJoysticks();
	if (numjoy > 5) numjoy = 5;
	SDL_JoystickEventState (SDL_ENABLE);

	SDL_Joystick *joy[5];

	for (Uint8 n = 0; n < numjoy; n++){
		joy[n] = SDL_JoystickOpen(n);
	}


	// ******* MENU *******

	//Setup Menus
	Menu menu (4, 52, false, true);
	menu.setfonts (normalfont, boldfont);
	menu.nameelement (0, "Player Setup");
	menu.nameelement (1, "Game Options");
	menu.nameelement (2, "Start the Game");
	menu.nameelement (3, "Exit");

	Menu setup (7, 56, true, true);
	setup.setfonts (normalfont, boldfont);
	setup.nameelement (0, "Number of Wizards:");
	setup.nameelement (1, "   ");
	setup.nameelement (2, "   ");
	setup.nameelement (3, "   ");
	setup.nameelement (4, "   ");
	setup.nameelement (5, "   ");
	setup.nameelement (6, "Main Menu");

	setup.seticon (0, noicon);
	setup.seticon (1, wizardicon[0]);
	setup.seticon (2, wizardicon[1]);
	setup.seticon (3, wizardicon[2]);
	setup.seticon (4, wizardicon[3]);
	setup.seticon (5, wizardicon[4]);
	setup.seticon (6, noicon);
		

	setup.initoptions (0, 4);
	/* 给选项命名 */
	for (Uint8 n = 0; n < 4; n++) {
		char num[6];
		sprintf (num, " %d", n+2);
		setup.nameoption (0, n, num);
	}
	/************************************************************************/
	/* 这个循环还看不懂                                                                     */
	/************************************************************************/
	for (Uint8 n = 1 ; n <= 5; n++) 
	{
		setup.initoptions (n, 7);
		for (Uint8 m = 1 ; m <= 5; m++) 
		{
			char txt[12];
			sprintf (txt, "Joystick %d", m);
			setup.nameoption (n, m-1, txt);
			if (m > numjoy) 
				setup.optionactive (n, m-1, false);
		}
		setup.nameoption (n, 5, "Keyboard");
		setup.nameoption (n, 6, "Computer");
	}

	Menu options (6, 52, false, true);
	options.setfonts (normalfont, boldfont);
	options.nameelement (0, "Winning Score: ");
	options.nameelement (1, "Music: ");
	options.nameelement (2, "Sound Effects: ");
	options.nameelement (3, "Graphics: ");
	options.nameelement (4, "Fullscreen: ");
	options.nameelement (5, "Main Menu");


	options.initoptions (0, 8);
	options.initoptions (1, 2);
	options.initoptions (2, 2);
	options.initoptions (3, 3);
	options.initoptions (4, 2);

	for (Uint8 n = 0 ; n < 8; n++) {
		char txt[4];
		sprintf (txt, "%d", n+3);
		options.nameoption (0, n, txt);
	}


	options.nameoption (1, 0, "off");
	options.nameoption (1, 1, "on");
	options.nameoption (2, 0, "off");
	options.nameoption (2, 1, "on");
	options.nameoption (3, 0, "8 Bit");
	options.nameoption (3, 1, "16 Bit");
	options.nameoption (3, 2, "24 Bit");
	options.nameoption (4, 0, "off");
	options.nameoption (4, 1, "on");

	//load options:
	ifstream file(configfile);
	if(!file.fail()) {
		char x[2];
		for (Uint8 n=0; n<6; n++)
		{
			file >> x;
			setup.setoption (n,atoi(x));
		}
		for (Uint8 n=0; n<5; n++)
		{
			file >> x;
			options.setoption (n,atoi(x));
		}
		file.close();
	}else {
		setup.setoption(0, 3);
		setup.setoption(1, 5);
		setup.setoption(2, 6);
		setup.setoption(3, 6);
		setup.setoption(4, 6);
		setup.setoption(5, 6);
		options.setoption (0, 2);
		options.setoption (1, 1);
		options.setoption (2, 1);
		options.setoption (3, 1);
		options.setoption (4, 0);
	}

    if (options.getoption(3) == 0) 
		game.gfx = 8;
    else if (options.getoption(3) == 1) 
		game.gfx = 16;
    else 
		game.gfx = 24;

    if (options.getoption(4) == 1) 
		game.fullscreen = true;
    else 
		game.fullscreen = false;

	game.DisplaySetup();

	// Game Setup
	game.WizardSetup ();
	game.FireballSetup ();
	game.framerate.setfps (50);
	game.framerate.init ();

	if (numjoy > 0) {
		menu.setjoystick(joy[0]);
		setup.setjoystick(joy[0]);
		options.setjoystick(joy[0]);
	}


	SDL_Surface *textsurface1, *textsurface2;
	SDL_Rect textrect1, textrect2;
	SDL_Color orange = {220,120,0};
	SDL_Color black = {0,0,0};
	textsurface1 = TTF_RenderText_Shaded (smallfont, "F.L.A.W. Version "VERSION, orange, black);
	textsurface2 = TTF_RenderText_Shaded (smallfont, "(C) 2009 Denis Comtesse", orange, black);
	textrect1.x = 0;
	textrect2.x = 0;
	textrect1.y = 450;
	textrect2.y = 465;
	textrect1.w = textsurface1->w;
	textrect2.w = textsurface2->h;
	textrect1.h = textsurface1->w;
	textrect2.h = textsurface2->h;

	//the big loop:

	do{
		bool done = false;

		//MAIN MENU:
		while (!done) {
			Uint8 selected;
			menu.select(0);
			SDL_FillRect (game.screen, NULL, 0x000000);
			SDL_BlitSurface (title, NULL, game.screen, NULL);
			SDL_BlitSurface (textsurface1, NULL, game.screen, &textrect1);
			SDL_BlitSurface (textsurface2, NULL, game.screen, &textrect2);

			selected = menu.run(game.screen, 0, 144, 640, 256);

			//Exit:
			if (selected == 3){

				//save options:
				ofstream file(configfile, ios::trunc);
				if(!file.fail())
				{
					char x[2];
					for (Uint8 n=0; n<6; n++){
						sprintf (x, "%d", setup.getoption (n));
						file << x << endl;
					}
					for (Uint8 n=0; n<5; n++){
						sprintf (x, "%d", options.getoption (n));
						file << x << endl;
					}
					file.close();
				}
				else cerr << "Could not create config file!";

				SDL_Quit();
				exit(0);
			}

			//Start the Game:
			else if (selected == 2) done = true;

			//Player Setup:
			else if (selected == 0) {
				SDL_FillRect (game.screen, NULL, 0x000000);
				setup.select(0);

				//Update Joysticks before Setup
				SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
				SDL_InitSubSystem(SDL_INIT_JOYSTICK);

				int numjoy = SDL_NumJoysticks();
				if (numjoy > 5) numjoy = 5;

				for (Uint8 n = 0; n < numjoy; n++){
					joy[n] = SDL_JoystickOpen(n);
				}

                SDL_Event event;
                SDL_PollEvent(&event);

				if (numjoy > 0){
					menu.setjoystick(joy[0]);
					setup.setjoystick(joy[0]);
					options.setjoystick(joy[0]);
				}
				else{
					menu.nojoystick();
					setup.nojoystick();
					options.nojoystick();
				}

				for (Uint8 n = 1 ; n <= 5; n++) {
					for (Uint8 m = 1 ; m <= 5; m++) {
						if (m > numjoy){
							setup.optionactive (n, m-1, false);
						}
						else {
							setup.optionactive (n, m-1, true);
						}
					}
					if ((setup.getoption(n) >= numjoy) && (setup.getoption(n) < 5)) setup.setoption (n, 6);
				}

				do {
                    for (Uint8 n = 2; n < 6; n++) {
                        if (n - 2 <= setup.getoption (0)) {
                            setup.elementactive (n, true);
                        }
                        else {
                            setup.elementactive (n, false);
                        }
                    }
					selected = setup.run(game.screen, 0, 40, 640, 440);
				}while (selected != 6);
			}

			//Game Options:
			else if (selected == 1) {
				SDL_FillRect (game.screen, NULL, 0x000000);
				options.select(0);
				do {
					selected = options.run(game.screen, 0, 80, 640, 400);
				}while (selected != 5);

				bool changedmode = false;

				if (options.getoption(3) == 0){
					if ((game.gfx == 16)||(game.gfx == 24)){
						game.gfx = 8;
						changedmode = true;
					}
				}

				if (options.getoption(3) == 1){
					if ((game.gfx == 8)||(game.gfx == 24)){
						game.gfx = 16;
						changedmode = true;
					}
				}

				if (options.getoption(3) == 2){
					if ((game.gfx == 8)||(game.gfx == 16)){
						game.gfx = 24;
						changedmode = true;
					}
				}

				if (options.getoption(4) == 0){
					if (game.fullscreen){
						game.fullscreen = false;
						changedmode = true;
					}
				}


				if (options.getoption(4) == 1){
					if (!game.fullscreen){
						game.fullscreen = true;
						changedmode = true;
					}
				}

				if (changedmode){
					game.WizardSetup();
					game.FireballSetup();
					game.DisplaySetup();
				}
			}
		}

		//start a game:
		game.numplayers = setup.getoption(0) + 2;
		game.maxpoints = options.getoption(0) + 3;
		game.playmusic = options.getoption (1);
		game.playsounds = options.getoption (2);

		for (Uint8 n = 0; n < game.numplayers; n++) {
			Uint8 option = setup.getoption(n+1);
			if (option == 5) {
				game.wizard[n].keyboard = true;
				game.wizard[n].ai = false;
				game.wizard[n].joystick = false;
				game.wizard[n].points = 0;
			}
			else if (option == 6) {
				game.wizard[n].keyboard = false;
				game.wizard[n].ai = true;
				game.wizard[n].joystick = false;
				game.wizard[n].points = 0;
			}
			else {
				game.wizard[n].keyboard = false;
				game.wizard[n].ai = false;
				game.wizard[n].joystick = true;
				game.wizard[n].joy = joy[option];
				game.wizard[n].points = 0;
			}
		}


		// ******* Start Game *******

		game.RandomizeAI();

		game.quit = false;
		game.finished = false;
		if (game.playmusic) game.StartMusic();

		while ((!game.quit) && (!game.finished)){

			game.SetStartPositions();
			game.numfireballs = 0;
			game.numdead = 0;
			game.fireballtimer = game.framerate.framespersecond*6;
			game.stop = false;
			game.FadeIn();
			game.framerate.update();

			//game loop
			while (!game.stop) {
				game.DisplayUpdate();
				game.Events();
				game.framerate.update();
				int numupdates = game.framerate.updates;
				for (numupdates; (numupdates >= 0) && (!game.stop); numupdates--){
					game.GameUpdate();
					if (game.playsounds) game.PlaySounds();
					game.AI();
				}
			}
			if (!game.quit) game.FinishAnimations();
			if (!game.quit) game.Score();
			if (!game.quit) game.FadeOut();
			if (!game.quit) game.ScoreBoard(boldfont, wizardicon, pointicon, emptypointicon);
		}
		game.Winner(boldfont);
		game.StopMusic();

	} while (true);

}

