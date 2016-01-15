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

#include "fireball.h"


void Fireball::init (SDL_Surface *flysprites[10], SDL_Surface *startsprites[9], SDL_Surface *endsprites[8], SDL_Surface *glowsprites[10]){
	
	for (Sint8 f = 0; f < 10; f++){
		fly[f] = flysprites[f];
	}
	
	for (Sint8 f = 0; f < 10; f++){
		glow[f] = glowsprites[f];
	}	
	
	for (Sint8 f = 0; f < 9; f++){
		start[f] = startsprites[f];
	}
	
	for (Sint8 f = 0; f < 8; f++){
		end[f] = endsprites[f];
	}
	
	w = fly[0]->w;
	h = fly[0]->h;
	rect.w = w;
	rect.h = h;
	
	blitted = false;
}


void Fireball::statusupdate (void){
	
	if ((status >= 200) && (status < 300)){	//END	
		x+=speed.x;
		y+=speed.y;
		frame = (status - 200)/2;		
		status++;
		if (status > 215){      
			status = 300;
		}
	}

	else if (status >= 150){				//GLOW
		x+=speed.x;
		y+=speed.y;
		frame = (status - 150)/4;		
		status++;
		if (status > 189){      
			status = 100;
		}		
	}
	
	else if (status >= 100){				//FLY	
		x+=speed.x;
		y+=speed.y;
		frame = (status - 100)/2;		
		status++;
		if (status > 119){      
			status = 100;
		}
	}
	
	else {									//START
		frame = status/4;		
		status++;
		if (status > 33){
			status = 100;
		}
	}
}


void Fireball::bordercollision (void) {
	if (x > 596) {
		if (speed.x > 0) speed.x *= -1;
		collision = true;
	}
	
	else if (x < 44) {
		if (speed.x < 0) speed.x *= -1;
		collision = true;
	}
	
	if (y > 434) {
		if (speed.y > 0) speed.y *= -1;
		collision = true;
	}
	
	else if (y < 28) {
		if (speed.y < 0) speed.y *= -1;
		collision = true;
	}
}



//blit:

void Fireball::blit (SDL_Surface *dst) {
	
	rect.x = x-w/2;
	rect.y = y-h/2;
	
	if (status < 300){	
		if (status >= 200){
			SDL_BlitSurface (end[frame], NULL, dst, &rect);
			blitted = true;
		}	
		
		else if (status >= 150){
			SDL_BlitSurface (glow[frame], NULL, dst, &rect);
			blitted = true;
		}	
		
		else if (status >= 100){
			SDL_BlitSurface (fly[frame], NULL, dst, &rect);
			blitted = true;
		}
		
		else {
			SDL_BlitSurface (start[frame], NULL, dst, &rect);
			blitted = true;
		}
	}
}


//restore background:

void Fireball::blitrestore(SDL_Surface *dst, SDL_Surface *backup){
	if (blitted == true) {
		SDL_BlitSurface (backup, &rect, dst, &rect);
		blitted = false;
	}
}



