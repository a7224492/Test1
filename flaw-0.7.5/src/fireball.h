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

#include <iostream>
#include <cstdlib>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

using namespace std;

class Fireball {
	
	private:

		SDL_Rect rect;
		bool blitted;
		
	public:
	
		SDL_Surface *fly[10], *start[9], *end[8], *glow[10];
		
		int x;
		int y;
		int w; 
		int h; 
		int status;
		int frame;
		bool collision;
		
		struct {
			Sint8 x;
			Sint8 y;
		} speed;
	
		void init (SDL_Surface *flysprites[10], SDL_Surface *startsprites[9], SDL_Surface *endsprites[8], SDL_Surface *glowsprites[10]);
		void statusupdate(void);
		void bordercollision(void);
		
		void blit (SDL_Surface *dst);
		void blitrestore(SDL_Surface *dst, SDL_Surface *backup);
};
