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

#include "game.h"
#include <cmath>

#ifdef __linux__
//#	define PATH "data/"
#	define PATH PACKAGE_DATA_DIR"/flaw/"
#else
#	define PATH "D:/data/"
#endif

// ******* setup functions *******

void Game::DisplaySetup(void){

	if (gfx == 8) {
		if (!(arena = IMG_Load (PATH"misc/arena8.png"))){
			cout << IMG_GetError();
			exit(1);
		}
	}else {
		if (!(arena = IMG_Load (PATH"misc/arena.png"))){
			cout << IMG_GetError();
			exit(1);
		}	
	}

	if (fullscreen) {
		screen = SDL_SetVideoMode(640, 480, gfx, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
		if (screen == NULL) {
			cout << "Can't set video mode: %s\n" << SDL_GetError();
			exit(1);
		}
	}else{
		screen = SDL_SetVideoMode(640, 480, gfx, SDL_SWSURFACE | SDL_DOUBLEBUF);
		if (screen == NULL) {
			cout << "Can't set video mode: %s\n" << SDL_GetError();
			exit(1);
		}
	}
	if (gfx == 8) 
		SDL_SetPalette(screen,  SDL_LOGPAL | SDL_PHYSPAL, arena->format->palette->colors, 0, 256);
}


void Game::AudioSetup(void){

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) < 0){
		printf("Error initializing SDL_mixer: %s\n", Mix_GetError());
		exit(1);
	}

	Mix_AllocateChannels(8);
	swing = Mix_LoadWAV(PATH"audio/swing.ogg");
	hit = Mix_LoadWAV(PATH"audio/hit.ogg");
	collide = Mix_LoadWAV(PATH"audio/fireball.ogg");
	die = Mix_LoadWAV(PATH"audio/die.ogg");
	shoot = Mix_LoadWAV(PATH"audio/shoot.ogg");
	invoke = Mix_LoadWAV(PATH"audio/invoke.ogg");

	music=Mix_LoadMUS(PATH"audio/music.xm");
	Mix_Volume(7, 16);
	soundchannel = 0;

	for (Uint8 n = 0; n < 7; n++){
		Mix_Volume(n, 32);
	}


}

void Game::StartMusic(void){
	Mix_VolumeMusic(128);
	Mix_PlayMusic(music, -1);
}

void Game::StopMusic(void){
	Mix_FadeOutMusic(1000);
}


void Game::FireballSetup(void){

	for (Sint8 f = 0; f < 10; f++){

		char filename[60];
		sprintf (filename, PATH"fireball/fly_%02d.png", f+1);

		if (!(fireballfly[f] = IMG_Load (filename))){
			cout << IMG_GetError();
			exit(1);
		}

		if (gfx > 8) 
			SDL_SetAlpha(fireballfly[f], SDL_SRCALPHA, 192);
	}

	for (Sint8 f = 0; f < 10; f++){

		char filename[60];
		sprintf (filename, PATH"fireball/glow_%02d.png", f+1);

		if (!(fireballglow[f] = IMG_Load (filename))){
			cout << IMG_GetError();
			exit(1);
		}

		if (gfx > 8) SDL_SetAlpha(fireballglow[f], SDL_SRCALPHA, 192);
	}

	for (Sint8 f = 0; f < 9; f++){

		char filename[60];
		sprintf (filename, PATH"fireball/start_%02d.png", f+1);

		if (!(fireballstart[f] = IMG_Load (filename))){
			cout << IMG_GetError();
			exit(1);
		}

		if (gfx > 8) SDL_SetAlpha(fireballstart[f], SDL_SRCALPHA, 32 + f*20);
	}

	for (Sint8 f = 0; f < 8; f++){

		char filename[60];
		sprintf (filename, PATH"fireball/end_%02d.png", f+1);

		if (!(fireballend[f] = IMG_Load (filename))){
			cout << IMG_GetError();
			exit(1);
		}

		if (gfx > 8) SDL_SetAlpha(fireballend[f], SDL_SRCALPHA, 192 - f*20);
	}

	for (Sint8 n = 0; n < MAX_FIREBALL_NUM; n++){
		fireball[n].init(fireballfly, fireballstart, fireballend, fireballglow);
	}
}


void Game::WizardSetup(void){
    Uint8 spriteset;
    if (gfx > 8) 
		spriteset = 16;
    else 
		spriteset = 8;
	wizard[0].load("blue", spriteset);
	wizard[1].load("red", spriteset);
	wizard[2].load("green", spriteset);
	wizard[3].load("purple", spriteset);
	wizard[4].load("black", spriteset);
}


void Game::RandomizeAI(void){
	for (Uint8 n = 0; n < numplayers; n++){
		wizard[n].aggressive = double((5 + rand()%6))/10;
		wizard[n].idle = double((5 + rand()%6))/10;
		wizard[n].risk = double((5 + rand()%6))/10;
		wizard[n].fear = double((5 + rand()%6))/10;
	}
}


void Game::SetStartPositions(void){

	wizard[0].x = 248;
	wizard[0].y = 144;
	wizard[0].direction = DOWNRIGHT;
	wizard[1].x = 440;
	wizard[1].y = 270;
	wizard[1].direction = LEFT;
	wizard[2].x = 200;
	wizard[2].y = 272;
	wizard[2].direction = RIGHT;
	wizard[3].x = 392;
	wizard[3].y = 144;
	wizard[3].direction = DOWNLEFT;
	wizard[4].x = 320;
	wizard[4].y = 368;
	wizard[4].direction = UP;

	for (Uint8 n=0; n < 5; n++){
		wizardsorted[n] = &wizard[n];
	}

	for (Uint8 n = 0; n < numplayers; n++){
		wizard[n].status = 0;
		wizard[n].control.x = 0;
		wizard[n].control.y = 0;
		wizard[n].control.fire = 0;
		wizard[n].aidelay = rand()%6;
	}

	for (Uint8 n = 0; n < 10; n++){
		fireball[n].x = 320;
		fireball[n].y = 240;
		fireball[n].status = 0;
		fireball[n].frame = 0;
		fireball[n].speed.x = (rand()%3)+2;
		fireball[n].speed.y = (rand()%3)+2;
		bool neg = (rand()%2);
		if (neg == true) fireball[n].speed.x *= -1;
		neg = (rand()%2);
		if (neg == true) fireball[n].speed.y *= -1;
	}
}


// ******* mainloop functions *******

void Game::Events(void){

	Uint8 *keystate;
	SDL_PollEvent(&event);
	keystate = SDL_GetKeyState (0);

	if (event.type == SDL_QUIT){
		SDL_Quit();
		exit(0);
	}

	if (keystate[SDLK_ESCAPE]){ 	//quit when ESC is pressed
		stop = true;
		quit = true;
	}

	for (Uint8 n = 0; n < numplayers; n++) {
		wizard[n].updatecontroller();
		wizard[n].setspeeds();
	}
}



void Game::DisplayUpdate(void) {

		SortWizards();

		for (Sint8 n = numfireballs-1; n >= 0; n--) {
			fireball[n].blitrestore(screen, arena);
		}

		for (Sint8 n = numplayers-1; n >= 0; n--) {
			wizardsorted[n]->blitrestore(screen, arena);
		}

		for (Sint8 n = 0; n < numplayers; n++) {
			wizardsorted[n]->blit(screen);
		}


		for (Sint8 n = 0; n < numfireballs; n++) {
			if (fireball[n].status < 300) fireball[n].blit(screen);
		}

		SDL_Flip (screen);
}


//check if the match is over
void Game::CheckState(void){
	if (numdead >= numplayers-1){

		if (fireball[0].status < 200){
			for (Uint8 n = 0; n < numfireballs; n++){
				fireball[n].status = 200;
			}
		}
		else if (fireball[0].status == 300){
			stop = true;
		}
	}
}


void Game::GameUpdate(){

	CheckState ();

	//launch new fireballs
	if ((numfireballs < 10) && (numdead < numplayers-1)){
		fireballtimer++;
		if (fireballtimer > framerate.framespersecond*10){
			fireballtimer = 0;
			numfireballs++;
		}
	}



	for (Uint8 n = 0; n < numfireballs; n++) {

		//two fireballs collide
		for (Sint8 m = n+1; m < numfireballs; m++){

			double dist = sqrt((long double)(fireball[n].x-fireball[m].x)*(fireball[n].x-fireball[m].x)+(fireball[n].y-fireball[m].y)*(fireball[n].y-fireball[m].y));

			if ((dist < 28) && (fireball[m].status >= 100)) {
				fireball[n].collision = true;
				int zx = fireball[n].speed.x;
				int zy = fireball[n].speed.y;
				fireball[n].speed.x = fireball[m].speed.x;
				fireball[n].speed.y = fireball[m].speed.y;
				fireball[m].speed.x = zx;
				fireball[m].speed.y = zy;
				if (fireball[m].x > fireball[n].x){
					fireball[m].x++;
					fireball[n].x--;
				}
				else {
					fireball[m].x--;
					fireball[n].x++;
				}
				if (fireball[m].y > fireball[n].y){
					fireball[m].y++;
					fireball[n].y--;
				}
				else {
					fireball[m].y--;
					fireball[n].y++;
				}
			}
		}

		//fireball collides with wizard

		for (Sint8 m = 0; m < numplayers; m++){
			float dist = sqrt((fireball[n].x-wizard[m].x)*(fireball[n].x-wizard[m].x)+(fireball[n].y-wizard[m].y)*(fireball[n].y-wizard[m].y));
			if ((dist < 22) && (wizard[m].status < 400) && (fireball[n].status >= 100) && (fireball[n].status >= 100)){
				wizard[m].status = 400;
				numdead++;
			}
		}

		fireball[n].bordercollision();
		fireball[n].statusupdate();
	}

	for (Uint8 n = 0; n < numplayers; n++) {

		//Wizard hits Wizard
		if (wizard[n].status == 206){
			for (int m = 0; m < numplayers; m++){
				if ((n != m) && (faces(&wizard[n], &wizard[m]))){
					if (wizard[m].status < 300) {
						wizard[m].status = 300;
						wizard[m].direction = (wizard[n].direction + 4) % 8;
					}
					//fall correction (don't fall "into" the walls)
					if (wizard[m].x > 586) {
						if (wizard[m].direction == 5) wizard[m].direction = 4;
						if (wizard[m].direction == 6) wizard[m].direction = 2;
						if (wizard[m].direction == 7) wizard[m].direction = 0;
					}
					if (wizard[m].x < 54) {
						if (wizard[m].direction == 3) wizard[m].direction = 4;
						if (wizard[m].direction == 2) wizard[m].direction = 6;
						if (wizard[m].direction == 1) wizard[m].direction = 0;
					}
					if (wizard[m].y > 416) {
						if (wizard[m].direction == 1) wizard[m].direction = 2;
						if (wizard[m].direction == 0) wizard[m].direction = 4;
						if (wizard[m].direction == 7) wizard[m].direction = 6;
					}
					if (wizard[m].y < 32) {
						if (wizard[m].direction == 3) wizard[m].direction = 2;
						if (wizard[m].direction == 4) wizard[m].direction = 0;
						if (wizard[m].direction == 5) wizard[m].direction = 6;
					}
				}
			}
		}

		//Wizard hits Fireball
		if ((wizard[n].status > 204) && (wizard[n].status < 208)){
			for (int m = 0; m < numfireballs; m++){
				if (faces(&wizard[n], &fireball[m])){

					if ((fireball[m].status < 150) || (fireball[m].status > 153)){
						fireball[m].speed.x /= 2;
						fireball[m].speed.y /= 2;
						fireball[m].status = 150;

						switch (wizard[n].direction) {

							case UP:
								fireball[m].speed.y -= 7;
								break;

							case UPRIGHT:
								fireball[m].speed.x += 5;
								fireball[m].speed.y -= 5;
								break;

							case RIGHT:
								fireball[m].speed.x += 7;
								break;

							case DOWNRIGHT:
								fireball[m].speed.x += 5;
								fireball[m].speed.y += 5;
								break;

							case DOWN:
								fireball[m].speed.y += 7;
								break;

							case DOWNLEFT:
								fireball[m].speed.x -= 5;
								fireball[m].speed.y += 5;
								break;

							case LEFT:
								fireball[m].speed.x -= 7;
								break;

							case UPLEFT:
								fireball[m].speed.x -= 5;
								fireball[m].speed.y -= 5;
								break;
						}
					}
				}
			}
		}

		//Wizards collide:
		for (int m = 0; m < numplayers; m++){
			if ((n != m) && (collides(&wizard[n], &wizard[m]))){

				if ((wizard[n].status < 400) && (wizard[m].status < 400)){
					if ((wizard[n].x > wizard [m].x) && (wizard[n].speed.x < 0)){
						wizard[n].speed.x = 0;
					}
					else if ((wizard[n].x < wizard [m].x) && (wizard[n].speed.x > 0)){
						wizard[n].speed.x = 0;
					}

					if ((wizard[n].y > wizard [m].y) && (wizard[n].speed.y < 0)){
						wizard[n].speed.y = 0;
					}
					else if ((wizard[n].y < wizard [m].y) && (wizard[n].speed.y > 0)){
						wizard[n].speed.y = 0;
					}
				}
			}
		}

		//Wizard collides with Wall

		if ((wizard[n].x < 44) && (wizard[n].speed.x < 0)){
			wizard[n].speed.x = 0;
		}
		else if ((wizard[n].x > 596) && (wizard[n].speed.x > 0)){
			wizard[n].speed.x = 0;
		}

		if ((wizard[n].y < 22) && (wizard[n].speed.y < 0)){
			wizard[n].speed.y = 0;
		}
		else if ((wizard[n].y > 426) && (wizard[n].speed.y > 0)){
			wizard[n].speed.y = 0;
		}

		// calculate idletime
		if ((wizard[n].speed.x == 0) && (wizard[n].speed.y == 0) && (wizard[n].control.fire == false)) {
			wizard[n].idletime++;
		}
		else {
			wizard[n].idletime = 0;
		}

		wizard[n].statusupdate();
	}
}


void Game::PlaySounds(){

	for (Sint8 n = numplayers-1; n >= 0; n--) {

		if (wizard[n].status == 201){
			Mix_PlayChannel(soundchannel, swing, 0);
			soundchannel++;
			if (soundchannel > 6) soundchannel = 0;
		}
		if (wizard[n].status == 301){
			Mix_PlayChannel(soundchannel, hit, 0);
			soundchannel++;
			if (soundchannel > 6) soundchannel = 0;
		}

		if (wizard[n].status == 401){
			Mix_PlayChannel(soundchannel, die, 0);
			soundchannel++;
			if (soundchannel > 6) soundchannel = 0;
		}
	}

	Uint8 collision = 0;

	for (Uint8 n = 0; n < numfireballs; n++) {
		if (fireball[n].collision == true){
			collision++;
			fireball[n].collision = false;
		}
		if (fireball[n].status == 151){
			Mix_PlayChannel(soundchannel, shoot, 0);
			soundchannel++;
			if (soundchannel > 6) soundchannel = 0;
		}
		if (fireball[n].status == 1){
			Mix_PlayChannel(7, invoke, 0);
		}
	}

	if (collision > 0){
		Mix_PlayChannel(soundchannel, collide, 0);
		soundchannel++;
		if (soundchannel > 6) soundchannel = 0;
	}

}


void Game::AI(){

	for (Uint8 n = 0; n < numplayers; n++) {
		wizard[n].aidelay = (wizard[n].aidelay +1)%6;

		if ((wizard[n].ai == true) && (wizard[n].aidelay == 0)){

			wizard[n].control.x = 0;
			wizard[n].control.y = 0;
			wizard[n].control.fire = false;

			float x = 0; //Movement priorities will be added
			float y = 0;
			float fire = 0;

			// Try to avoid the corners:
			if (wizard[n].x > 480) x -= pow((wizard[n].x - 480)/160, 2)*(1.5-wizard[n].fear)*700;
			if (wizard[n].x < 160) x += pow((160 - wizard[n].x)/160, 2)*(1.5-wizard[n].fear)*700;
			if (wizard[n].y > 320) y -= pow((wizard[n].y - 320)/160, 2)*(1.5-wizard[n].fear)*700;
			if (wizard[n].y < 160) y += pow((160 - wizard[n].y)/160, 2)*(1.5-wizard[n].fear)*700;

			// Try to avoid the center:
			float centerdist = sqrt(pow((wizard[n].x-320),2)+pow((wizard[n].y-240),2));
			if (centerdist < 80){
				x += Sgn(wizard[n].x - 320) * (80 - centerdist)/80*(1.5-wizard[n].risk)*300;
				y += Sgn(wizard[n].y - 240) * (80 - centerdist)/80*(1.5-wizard[n].risk)*300;
			}

			// Move away from the fireballs:
			for (Uint8 m = 0; m < numfireballs; m++) {
				float dist = sqrt(pow((wizard[n].x-fireball[m].x),2)+pow((wizard[n].y-fireball[m].y),2));
				float xdist = sqrt(pow(wizard[n].x-fireball[m].x,2));
				float ydist = sqrt(pow(wizard[n].y-fireball[m].y,2));

				if (dist < 320) {
					if (xdist > 20) { //consider fireball position:
						x += Sgn(wizard[n].x - fireball[m].x) * pow((320 - dist)/320, 3)/numfireballs*(wizard[n].fear)*2500;
					}
					else { //consider fireball movement:
						x -= Sgn(fireball[m].speed.x) * pow((320 - dist)/320, 3)/numfireballs*(wizard[n].fear)*2500;
					}
					if (ydist > 18) { //consider fireball position:
						y += Sgn(wizard[n].y - fireball[m].y) * pow((320 - dist)/320, 3)/numfireballs*(wizard[n].fear)*2500;
					}
					else { //consider fireball movement:
						y -= Sgn(fireball[m].speed.y) * pow((320 - dist)/320, 3)/numfireballs*(wizard[n].fear)*2500;
					}
					fire -= pow((400 - dist),2)/200/numfireballs;
				}

			}


			for (Uint8 m = 0; m < numplayers; m++) {
				if (n != m) {
					float dist = sqrt(pow((wizard[n].x-wizard[m].x),2)+pow((wizard[n].y-wizard[m].y),2));

					//Move towards other wizards:
					if (wizard[m].status < 300) {
						x += Sgn(wizard[m].x - wizard[n].x) * 500/numplayers*wizard[n].aggressive;
						y += Sgn(wizard[m].y - wizard[n].y) * 500/numplayers*wizard[n].aggressive;
						if (faces(&wizard[n], &wizard[m])){
							fire += (200 + 500/numplayers)*wizard[n].aggressive;
						}
					}

					//Move away from Wizards that have been knocked down:
					else if (wizard[m].status < 400) {
						if (dist < 400) {
							x += Sgn(wizard[n].x - wizard[m].x) * pow((400 - dist)/400,2)*(1.5-wizard[n].aggressive)*300;
							y += Sgn(wizard[n].y - wizard[m].y) * pow((400 - dist)/400,2)*(1.5-wizard[n].aggressive)*300;
						}
					}
				}
			}

			//Add random movement to idle wizards:
			x += (rand()%3 - 1)*2*wizard[n].idletime;
			y -= (rand()%3 - 1)*2*wizard[n].idletime;

			//Calculate actual movement (threshold = 100)
			if (x > 100*wizard[n].idle) wizard[n].control.x = 1;
			else if (x < -100*wizard[n].idle) wizard[n].control.x = -1;

			if (y > 100*wizard[n].idle) wizard[n].control.y = 1;
			else if (y < -100*wizard[n].idle) wizard[n].control.y = -1;

			if (fire > 0) wizard[n].control.fire = true;

		}
	}
}


//finish animations before stopping the game
void Game::FinishAnimations(void){
	Uint8 frames = 0;
	do {
		Uint8 *keystate;
		SDL_PollEvent(&event);
		keystate = SDL_GetKeyState (0);

		if (event.type == SDL_QUIT){
			SDL_Quit();
			exit(0);
		}

		if (keystate[SDLK_ESCAPE]){
			quit = true;
			stop = true;
			frames = 100;
		}

		framerate.update();
		Sint16 numupdates = framerate.updates;

		for (Uint8 n = 0; n < numplayers; n++){
			wizard[n].blitrestore(screen, arena);
			if (wizard[n].status < 500){
				wizard[n].blit(screen);
			}
		}
        SDL_Flip (screen);
		for (numupdates; numupdates >= 0; numupdates--){
			for (Uint8 n = 0; n < numplayers; n++){
				if ((wizard[n].status < 500) && (wizard[n].status >= 200)){
					wizard[n].statusupdate();
				}
			}
			frames++;
		}

	} while (frames < 100);
}


void Game::Score(void) {
	for (Uint8 n = 0; n < numplayers; n++) {
		if (wizard[n].status < 400) wizard[n].points++;
		if (wizard[n].points == maxpoints) finished = true;
	}
}


void Game::ScoreBoard(TTF_Font *font, SDL_Surface *wizardicon[5], SDL_Surface *pointicon, SDL_Surface *emptypointicon) {

	SDL_Color white = {255, 255, 255};
	SDL_Color blue = {0, 0, 196};
	SDL_Surface *textsurface;
	textsurface = TTF_RenderText_Blended (font, "Scores", white);
	SDL_Rect trect;
	trect.w = textsurface->w;
	trect.h = textsurface->h;
	trect.x = (640 - trect.w)/2;
	trect.y = 32;
	SDL_BlitSurface (textsurface, NULL, screen, &trect);

	Uint16 boardwidth = 16 + wizardicon[0]->w + (pointicon->w + 5)*maxpoints;
	Uint16 boardx = (640 - boardwidth)/2;

	for (Uint8 n = 0; n < numplayers; n++){


		SDL_Rect wrect;
		wrect.w = wizardicon[n]->w;
		wrect.h = wizardicon[n]->h;
		wrect.x = boardx;
		wrect.y = 80 + n*(wizardicon[n]->h+10);

		SDL_BlitSurface (wizardicon[n], NULL, screen, &wrect);

		for (Uint8 m = 0; m < maxpoints; m++) {
			SDL_Rect prect;
			prect.w = pointicon->w;
			prect.h = pointicon->h;
			prect.x = boardx + 16 + wrect.w + m*(prect.w + 4);
			prect.y = wrect.y + (wrect.h - prect.h)/2;

			SDL_BlitSurface (emptypointicon, NULL, screen, &prect);
		}

		for (Uint8 m = 0; m < wizard[n].points; m++) {
			SDL_Rect prect;
			prect.w = pointicon->w;
			prect.h = pointicon->h;
			prect.x = boardx + 16 + wrect.w + m*(prect.w + 4);
			prect.y = wrect.y + (wrect.h - prect.h)/2;

			SDL_BlitSurface (pointicon, NULL, screen, &prect);
		}
	}

	SDL_Flip(screen);
	WaitForKeypress();
}

void Game::Winner(TTF_Font *font) {

	SDL_Rect rect;
	rect.x = 0;
	rect.w = 640;
	rect.y = 0;
	rect.h = 480;
	Sint8 winner = -1;

	SDL_Color white = {255, 255, 255};
	SDL_Surface *textsurface;
	textsurface = TTF_RenderText_Blended (font, "And the winner is...", white);

	SDL_FillRect (screen, &rect, 0x000000);

	for (Uint8 n = 0; n < numplayers; n++) {
		if (wizard[n].points == maxpoints){
			winner = n;
		}
	}

	if (winner > -1){
		rect.x = (640-textsurface->w)/2;
		rect.w = textsurface->w;
		rect.y = 32;
		rect.h = textsurface->h;
		SDL_BlitSurface (textsurface, NULL, screen, &rect);

		rect.x = 0;
		rect.w = 640;
		rect.y = 96;
		rect.h = 384;
		SDL_BlitSurface (wizard[winner].win, NULL, screen, &rect);

		SDL_Flip(screen);
		WaitForKeypress();
	}
}





// ******* auxiliary functions *******

void Game::WaitForKeypress(void) {
	bool keypress = false;
	while (keypress == false) {
		SDL_PumpEvents ();
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT){
			SDL_Quit();
			exit(0);
		}
		else if ((event.type == SDL_KEYDOWN) || (event.type == SDL_JOYBUTTONDOWN)) {
			keypress = true;
		}

	}
}


void Game::SortWizards(void){
	for (int i = 0; i < numplayers; ++i) {
		for (int j = 0; j < numplayers - i - 1; j++) {
			if (wizardsorted[j]->y > wizardsorted[j + 1]->y) {
				Wizard *tmp = wizardsorted[j];
				wizardsorted[j] = wizardsorted[j + 1];
				wizardsorted[j + 1] = tmp;
			}
		}
	}
}


float Game::Distance(Wizard *wizard, Fireball *fireball) {
	float dist = sqrt((wizard->x - fireball->x) * (wizard->x - fireball->x) + (wizard->y - fireball->y) * (wizard->y - fireball->y));
	return dist;
}


bool Game::collides(Wizard *wizard, Wizard *enemy){
	float dist = sqrt((wizard->x - enemy->x) * (wizard->x - enemy->x) + (wizard->y - enemy->y) * (wizard->y - enemy->y));
	if  (dist < 12){
		return true;
	}
	else {
		return false;
	}
}


//calculate if wizard faces wizard
bool Game::faces(Wizard *wizard, Wizard *enemy){

	int xface, yface;

	switch (wizard->direction){

		case UP:
			xface = wizard->x;
			yface = wizard->y - 10;
			break;

		case UPRIGHT:
			xface = wizard->x + 7;
			yface = wizard->y - 7;
			break;

		case RIGHT:
			xface = wizard->x + 10;
			yface = wizard->y;
			break;


		case DOWNRIGHT:
			xface = wizard->x + 7;
			yface = wizard->y + 7;
			break;

		case DOWN:
			xface = wizard->x;
			yface = wizard->y + 10;
			break;

		case DOWNLEFT:
			xface = wizard->x - 7;
			yface = wizard->y + 7;
			break;

		case LEFT:
			xface = wizard->x - 10;
			yface = wizard->y;
			break;

		case UPLEFT:
			xface = wizard->x - 7;
			yface = wizard->y - 7;
			break;
	}

	float dist = sqrt((xface - enemy->x) * (xface - enemy->x) + (yface - enemy->y) * (yface - enemy->y));
	if  (dist < 12){
		return true;
	}
	else {
		return false;
	}
}


//calculate if wizard faces fireball
bool Game::faces(Wizard *wizard, Fireball *fireball){

	int xface, yface;

	switch (wizard->direction){

		case UP:
			xface = wizard->x;
			yface = wizard->y - 30;
			break;

		case UPRIGHT:
			xface = wizard->x + 21;
			yface = wizard->y - 21;
			break;

		case RIGHT:
			xface = wizard->x + 30;
			yface = wizard->y;
			break;


		case DOWNRIGHT:
			xface = wizard->x + 21;
			yface = wizard->y + 21;
			break;

		case DOWN:
			xface = wizard->x;
			yface = wizard->y + 30;
			break;

		case DOWNLEFT:
			xface = wizard->x - 21;
			yface = wizard->y + 21;
			break;

		case LEFT:
			xface = wizard->x - 30;
			yface = wizard->y;
			break;

		case UPLEFT:
			xface = wizard->x - 21;
			yface = wizard->y - 21;
			break;
	}

	double dist = sqrt((long double)(xface - fireball->x) * (xface - fireball->x) + (yface - fireball->y) * (yface - fireball->y));
	if  (dist < 25){
		return true;
	}
	else {
		return false;
	}
}


Sint8 Game::Sgn(float a){
	Sint8 s;
	if (a < 0){
		s = -1;
	}
	else if (a > 0){
		s = 1;
	}
	else{
		s = 0;
	}
	return s;
}


void Game::FadeOut(void) {

	Sint16 position = 0;
	Sint16 height = 4;
	SDL_Rect rect1, rect2;

	rect1.x = 0;
	rect1.w = 640;
	rect2.x = 0;
	rect2.w = 640;

	framerate.update();

	do{
		Uint8 *keystate;
		SDL_PollEvent(&event);
		keystate = SDL_GetKeyState (0);

		if (event.type == SDL_QUIT){
			SDL_Quit();
			exit(0);
		}

		if (keystate[SDLK_ESCAPE]){ 	//quit when ESC is pressed
			quit = true;
			stop = true;
		}

		rect1.y = position;
		rect1.h = height;
		rect2.y = 480 - position - height;
		rect2.h = height;

		SDL_FillRect (screen, &rect1, 0x000000);
		SDL_FillRect (screen, &rect2, 0x000000);
		SDL_Flip (screen);

		position += height;

		framerate.update();
		Sint16 n = framerate.updates+1;
		height = 8*n;

		if (position+height > 240) height = 240-position;

	} while ((position < 240)&&(!quit));
}


void Game::FadeIn(void) {

	Sint16 position = 0;
	Sint16 height = 4;
	SDL_Rect rect1, rect2;

	rect1.x = 0;
	rect1.w = 640;
	rect2.x = 0;
	rect2.w = 640;

	framerate.update();

	do{
		Uint8 *keystate;
		SDL_PollEvent(&event);
		keystate = SDL_GetKeyState (0);

		if (event.type == SDL_QUIT){
			SDL_Quit();
			exit(0);
		}

		if (keystate[SDLK_ESCAPE]){ 	//quit when ESC is pressed
			quit = true;
			stop = true;
		}

		rect1.y = 240 + position;
		rect1.h = height;
		rect2.y = 240 - position - height;
		rect2.h = height;

		SDL_BlitSurface (arena, &rect1, screen, &rect1);
		SDL_BlitSurface (arena, &rect2, screen, &rect2);
		SDL_Flip (screen);

		position += height;

		framerate.update();
		Sint16 n = framerate.updates+1;
		height = 8*n;

		if (position+height > 240) height = 240-position;

	} while ((position < 240)&&(!quit));
}
