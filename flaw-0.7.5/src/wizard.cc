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

#include "wizard.h"
#include <cmath>

#ifdef __linux__
//#	define PATH "data/"
#	define PATH PACKAGE_DATA_DIR"/flaw/"
#else
#	define PATH "D:/data/"
#endif



void Wizard::load (const char *name, Uint8 gfx) {
	char filename[MAX_FILENAME_LEN];
	sprintf (filename, PATH"%d/%s/win.png", gfx, name);
	if (!(win = IMG_Load (filename))){
		cout << IMG_GetError();
		exit(1);
	}

	for (Sint8 d = 0; d < DIR_NUM; d++)
	{
		for (Sint8 f = 0; f < WALK_FRAME; f++)
		{
			char filename[MAX_FILENAME_LEN];
			sprintf (filename, PATH"%d/%s/walk_%d_%02d.png", gfx, name, d, f+1);
			if (!(walk[d][f] = IMG_Load (filename)))
			{
				cout << IMG_GetError();
				exit(1);
			}
		}
		for (Sint8 f = 0; f < HIT_FRAME; f++){
			char filename[MAX_FILENAME_LEN];
			int chk;

			chk = sprintf (filename, PATH"%d/%s/hit_%d_%02d.png", gfx, name, d, f+1);
			if (!(hit[d][f] = IMG_Load (filename))){
				cout << IMG_GetError();
			}
		}
		for (Sint8 f = 0; f < FALL_FRAME; f++){
			char filename[MAX_FILENAME_LEN];
			int chk;

			chk = sprintf (filename, PATH"%d/%s/fall_%d_%02d.png", gfx, name, d, f+1);
			if (!(fall[d][f] = IMG_Load (filename))){
				cout << IMG_GetError();
			}
		}
	}

	for (Sint8 f = 0; f < BURN_FRAME; f++)
	{
		char filename[MAX_FILENAME_LEN];
		int chk;

		chk = sprintf (filename, PATH"%d/%s/burn_%02d.png", gfx, name, f+1);
		if (!(burn[f] = IMG_Load (filename)))
		{
			cout << IMG_GetError();
		}
		if (gfx == 16) 
			SDL_SetAlpha(burn[f], SDL_SRCALPHA, 128);
	}

	w = walk[0][0]->w;
	h = walk[0][0]->h;
	rect.w = w;
	rect.h = h;
	store.w = rect.w+2;
	store.h = rect.h+2;
	frame = 0;
	status = 0;
	direction = 0;
	control.x = 0;
	control.y = 0;
	control.fire = false;
	blitted = false;
}



void Wizard::updatecontroller(void){

	Uint8 *keystate = SDL_GetKeyState (0);

	if (keyboard == true){

		if ((keystate [SDLK_UP]) || (keystate [SDLK_w])){
			control.y = -1;
		}else if ((keystate [SDLK_DOWN]) || (keystate [SDLK_s])){
			control.y = 1;
		}else {
			control.y = 0;
		}

		if ((keystate [SDLK_LEFT]) || (keystate [SDLK_a])){
			control.x = -1;
		}else if ((keystate [SDLK_RIGHT]) || (keystate [SDLK_d])){
			control.x = 1;
		}else {
			control.x = 0;
		}

		if ((keystate [SDLK_RCTRL]) || (keystate [SDLK_LCTRL]) || (keystate [SDLK_SPACE])){
			control.fire = true;
		}else {
			control.fire = false;
		}
	}else if (joystick == true){
        SDL_JoystickUpdate();
		if (SDL_JoystickGetAxis(joy, 0) < -8000){
			control.x = -1;
		}else if (SDL_JoystickGetAxis(joy, 0) > 8000){
			control.x = 1;
		}else {
			control.x = 0;
		}

		if (SDL_JoystickGetAxis(joy, 1) < -8000){
			control.y = -1;
		}else if (SDL_JoystickGetAxis(joy, 1) > 8000){
			control.y = 1;
		}else {
			control.y = 0;
		}

		if ((SDL_JoystickGetButton(joy, 0)) || (SDL_JoystickGetButton(joy, 1))){
			control.fire = true;
		}else {
			control.fire = false;
		}
	}
}


void Wizard::setspeeds(void)
{
	if (status < 300) 
	{
		if ((control.x != 0) && (control.y != 0)) 
		{
			speed.x = control.x*2;
			speed.y = control.y*2;
		}else {		
			speed.x = control.x*3;//在x方向上走的速度
			speed.y = control.y*2.5;//在y方向上走的速度
		}
	}

	if ((control.fire) && (status < 200))
	{
		status = 200;
	}
}


//Update Status and calculate the correct frame:

void Wizard::statusupdate(void) 
{
	if (status < 100)
	{
		if ((speed.x!= 0) || (speed.y != 0)) 
			status = 100;
	}else if (status < 200){
		if ((speed.x == 0) && (speed.y == 0)) 
			status = 0;
	}

	if (status >= 400)
	{			
		//BURN
		frame = (status - 400)/2;//调用两次才加一帧
		status++;
		if (status > 411)
		{
			status = 500;	//>=500就是died
		}
	}else if (status >= 390)
	{	
		//stand up
		frame = (status - 390)/2 + 4;
		status++;
		if (status > 397)
		{
			status = 0;
		}
	}else if (status >= 330){	//knocked out
		frame = 4;
		status++;
	}else if (status >= 300){	//FALL
		frame = (status - 300)/2;
		status++;
		if (status > 307){
			status = 330;
		}
	}else if (status >= 200){	//HIT
		frame = 1+(status - 200)/2;
		status++;
		if (status > 217)
		{
			status = 0;
		}
	}else if (status >= 100){	//WALK
		x+=speed.x;
		y+=speed.y;
		frame = (status - 100)/2;
		status++;
		if (status > 131){
			status = 100;
		}
		//Directions:
		if (speed.x > 0)
		{
			if (speed.y > 0)
			{
				direction = DOWNRIGHT;
			}else if (speed.y < 0){
				direction = UPRIGHT;
			}else {
				direction = RIGHT;
			}
		}else if (speed.x < 0){
			if (speed.y > 0){
				direction = DOWNLEFT;
			}else if (speed.y < 0){
				direction = UPLEFT;
			}else {
				direction = LEFT;
			}
		}else{
			if (speed.y > 0){
				direction = DOWN;
			}else if (speed.y < 0){
				direction = UP;
			}
		}
	}else if (status < 100){			//STAND
		frame = 0;
		status = 0;
	}

}


//Restore Background:

void Wizard::blitrestore (SDL_Surface *dst, SDL_Surface *backup) 
{
	if (blitted == true) 
	{
		SDL_BlitSurface (backup, &rect, dst, &rect);
		blitted = false;
	}
}


//blit:

void Wizard::blit (SDL_Surface *dst) 
{
	rect.x = x-w/2;
	rect.y = y-h/2;

	if (status < 500) //>=500是died
	{
		if (status >= 400) //>=400是burn
		{
			SDL_BlitSurface (burn[frame], NULL, dst, &rect);
			blitted = true;
		}else if (status >= 300){//站立，倒下，躺着
			SDL_BlitSurface (fall[direction][frame], NULL, dst, &rect);
			blitted = true;
		}else if (status >= 200){//击打
			SDL_BlitSurface (hit[direction][frame], NULL, dst, &rect);
			blitted = true;
		}else if (status >= 100){//移动
			SDL_BlitSurface (walk[direction][frame], NULL, dst, &rect);
			blitted = true;
		}else if (status < 100){//不动
			SDL_BlitSurface (walk[direction][0], NULL, dst, &rect);
			blitted = true;
		}
	}
}

