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


#include <SDL/SDL.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdlib>
#include <SDL/SDL_ttf.h>

class Menu {
	
	public:
	
		Menu(Uint8 numelements, Uint16 spacing, bool useicons, bool centered);
		void elementactive (Uint8 element, bool active);
		void optionactive (Uint8 element, Uint8 option, bool active);//?
		void initoptions (Uint8 element, Uint8 numoptions);
		void nameelement(Uint8 element, char *name);	
		void nameoption(Uint8 element, Uint8 option, char *name);	
		void setfonts(TTF_Font *normalfont, TTF_Font *boldfont);
		void seticon(Uint8 element, SDL_Surface *surface);
		void setoption (Uint8 element, Uint8 state);
		void select (Uint8 element);
		void setjoystick(SDL_Joystick *joy);
		void nojoystick (void);
		
		Uint8 getoption (Uint8 element);	
		Uint8 run (SDL_Surface *s, Uint16 x, Uint16 y, Uint16 w, Uint16 h);	
			
		int getElementNum()
		{
			return numelements;
		}
		
		
	private:
		enum {
			MAX_MENUITEM_NUM = 20,
			MAX_OPTIONS_NUM = 20
		};
		/*  */
		Uint8 numelements;
	    Uint8 selected;
		Uint16 spacing;
		bool useicons, centered, usejoystick;
		TTF_Font *normal, *bold;
		SDL_Joystick *joy;
		
		struct {
			SDL_Surface *icon;
			char name[25];
			Uint8 numoptions, state;//y;ex. on,off
			
			bool active, switchable;//?;Is current option changable?
			struct {
				char name[25];
				bool active;
			} option[MAX_OPTIONS_NUM];//?
		} element[MAX_MENUITEM_NUM];//element is menu item?
};





