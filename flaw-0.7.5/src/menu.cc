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


#include "menu.h"


Menu::Menu (Uint8 n, Uint16 s, bool i, bool c) {

	/* element max num is 20 */
	if (numelements <= MAX_MENUITEM_NUM) 
		numelements = n;
	else 
		numelements = MAX_MENUITEM_NUM;
	spacing = s;
	useicons = i;
	centered = c;
	selected = 0;
	usejoystick = false;

	for (Uint8 e = 0; e < MAX_MENUITEM_NUM; e++) 
	{
		if (e < n) 
			element[e].active = true;
		else 
			element[e].active = false;
		element[e].numoptions = 0;
		element[e].state = 0;
		element[e].switchable = false;
		for (Uint8 o = 0; o < MAX_OPTIONS_NUM; o++) {
			element[e].option[o].active = false;
		}
	}
}

void Menu::elementactive (Uint8 e, bool b) {
	if (e < numelements){
		element[e].active = b;
	}
	else printf ("Menu::elementactive: Invalid Element");
}

void Menu::optionactive (Uint8 e, Uint8 o, bool b) 
{
	if ((e < numelements) && (o < element[e].numoptions))
	{
		element[e].option[o].active = b;
	}
	else printf ("Menu::optionactive: Wrong values!");
}

void Menu::initoptions (Uint8 e, Uint8 n) 
{
	if ((e < numelements) && (n <= MAX_OPTIONS_NUM)) 
	{
		element[e].switchable = true;
		element[e].numoptions = n;
		for (Uint8 o = 0; o < n; o++)
		{
			element[e].option[o].active = true;
		}
	}
	else printf ("Menu::initoptions: Wrong values!");
}

void Menu::nameelement(Uint8 e, char *n) 
{
	if ((e < numelements) && (strlen(n) <= 25)) 
	{
		sprintf (element[e].name, n);
	}
	else printf ("Menu::namelement: Invalid Element or string too long!");
}

void Menu::nameoption(Uint8 e, Uint8 o, char *n) 
{
	if ((e < numelements) && (o < element[e].numoptions) && (strlen(n) <= 25)) 
	{
		sprintf (element[e].option[o].name, n);
	}
	else printf ("Menu::nameoption: Wrong values or string too long!");
}

void Menu::setfonts(TTF_Font *normalfont, TTF_Font *boldfont) {
	normal = normalfont;
	bold = boldfont;
}

void Menu::seticon(Uint8 e, SDL_Surface *s) {
	if (e < numelements){
		element[e].icon = s;
	}
	else printf ("Menu::seticons: Invalid Element");
}
/************************************************************************/
/* ÉèÖÃelementµÄstateÊÇo                                                                    */
/************************************************************************/
void Menu::setoption (Uint8 e, Uint8 o) 
{
	if (e < numelements) 
	{
		if (element[e].option[o].active == true) 
		{
			element[e].state = o;
		}
		else printf ("Menu::setoption: Invalid Option (out of range or inactive)");
	}
	else printf ("Menu::seticons: Invalid Element");
}

Uint8 Menu::getoption (Uint8 e) {
	if (e < numelements) {
		return element[e].state;
	}
	else {
		printf ("Menu::seticons: Invalid Element");
		return 0;
	}
}

void Menu::select (Uint8 e) {
	if (e < numelements) {
		selected = e;
	}
	else {
		printf ("Menu::select: Invalid Element");
	}
}

void Menu::setjoystick (SDL_Joystick *j) {
	if (!j) {
		printf ("Menu::setjoystick: No Joystick?");
	}
	else {
		joy = j;
		usejoystick = true;
	}
}

void Menu::nojoystick (void) {
		usejoystick = false;
}


Uint8 Menu::run (SDL_Surface *s, Uint16 x, Uint16 y, Uint16 w, Uint16 h) {

	SDL_Color colour_active = {255, 255, 255};
	SDL_Color colour_inactive = {128,128,128};
	SDL_Color colour_background = {0,0,0};

	SDL_Rect rect;
	rect.x = x;
	rect.w = w;
	rect.y = y;
	rect.h = h;

	bool done = false;
	while (done == false) 
	{

		SDL_FillRect (s, &rect, 0x000000);
		for (Uint8 e = 0; e < numelements; e++)
		{
			if (element[e].active) 
			{
				char txt[50];
				if (element[e].option[element[e].state].active) 
				{
					sprintf (txt, "%s %s", element[e].name, element[e].option[element[e].state].name);
				}else {
					sprintf (txt, "%s", element[e].name);
				}

				SDL_Surface *textsurface;
				SDL_Rect rect, iconrect;
				if (selected == e) {
					textsurface = TTF_RenderText_Shaded (bold, txt, colour_active, colour_background);
				}else {
					textsurface = TTF_RenderText_Shaded (normal, txt, colour_inactive, colour_background);
				}

				rect.w = textsurface->w;
				rect.h = textsurface->h;
				rect.y = y + e*spacing;

				if (!centered){
					rect.x = x;
					iconrect.x = x;
				}
				else {
					rect.x = (640 - textsurface->w)/2;
					iconrect.x = (640 - textsurface->w)/2;
				}


				if (useicons){
					if (!centered) {
						rect.x += element[e].icon->w + 3;
					}
					else {
						rect.x += element[e].icon->w/2;
						iconrect.x -= element[e].icon->w/2;
					}
					iconrect.w = textsurface->w;
					iconrect.h = textsurface->h;
					iconrect.y = y + e*spacing -6;
					SDL_BlitSurface (element[e].icon, NULL, s, &iconrect);
					SDL_BlitSurface (textsurface, NULL, s, &rect);
				}
				else{
					SDL_BlitSurface (textsurface, NULL, s, &rect);
				}

				//SDL_BlitSurface (textsurface, NULL, s, &rect);
			}
		}

		SDL_Flip (s);
		SDL_Event event;
		SDL_WaitEvent(&event);


		if (event.type == SDL_QUIT)
		{
			SDL_Quit();
			exit(0);
		}

		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				SDL_Quit();
				exit(0);
			}

			if (event.key.keysym.sym == SDLK_UP) {
				do {
					if (selected == 0) 
						selected = numelements-1;
					else 
						selected -= 1;
				} while (!element[selected].active);
			}else if (event.key.keysym.sym == SDLK_DOWN){
				do 
				{
					selected = (selected + 1) % numelements;
				} while (!element[selected].active);
			}else if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN){
				if (element[selected].switchable) {
					do {
						element[selected].state = (element[selected].state + 1) % element[selected].numoptions;
					} while (!element[selected].option[element[selected].state].active);
				}
				done = true;
			}
		}

		if (usejoystick){

			if (event.type == SDL_JOYAXISMOTION) {

				if (SDL_JoystickGetAxis(joy, 1) < -8000) {
					do {
						if (selected == 0) selected = numelements-1;
						else selected -= 1;
					} while (!element[selected].active);
				}

				else if (SDL_JoystickGetAxis(joy, 1) > 8000) {
					do {
						selected = (selected + 1) % numelements;
					} while (!element[selected].active);
				}
			}

			if (event.type == SDL_JOYBUTTONDOWN) {
				if ((SDL_JoystickGetButton(joy, 0)) || (SDL_JoystickGetButton(joy, 1))) {

					if (element[selected].switchable) {
						do {
							element[selected].state = (element[selected].state + 1) % element[selected].numoptions;
						} while (!element[selected].option[element[selected].state].active);
					}
					done = true;
				}
			}
		}
	}

	return selected;

}


