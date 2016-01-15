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

enum {UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWN, DOWNLEFT, LEFT, UPLEFT};

#include <iostream>
#include <cstdlib>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

using namespace std;

class Wizard {
	private:
		enum{
			MAX_FILENAME_LEN=150,
			DIR_NUM=8,
			WALK_FRAME=16,
			HIT_FRAME=10,
			FALL_FRAME=8,
			BURN_FRAME=6
		};
		SDL_Surface *walk[DIR_NUM][WALK_FRAME], *hit[DIR_NUM][HIT_FRAME], *fall[DIR_NUM][FALL_FRAME], *burn[BURN_FRAME];
		SDL_Rect rect, store;//what is store mean?
		bool blitted;
	public:
		SDL_Surface *win;

		Uint8 points;
		float x, y;
		Uint8 w, h;
		Uint8 aidelay;

		struct {
			float x, y;
		} speed;

		struct {
			Sint8 x, y;
			bool fire;
		} control;

		double aggressive, idle, risk, fear;

		SDL_Joystick *joy;
		bool joystick, keyboard, ai;
		
		/*
		** status=:
		** >400: burn
		*/
		Uint16 status, idletime; 
		Uint8 direction, frame;

		//Methoden:

		void load (const char *name, Uint8 gfx);
		void blit (SDL_Surface *dst);
		void blitrestore (SDL_Surface *dst, SDL_Surface *backup);
		void updatecontroller(void);
		void setspeeds(void);
		void statusupdate(void);
};









