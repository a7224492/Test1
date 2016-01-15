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

#include "framerate.h"
#include <SDL/SDL.h>

void FrameRateControl::update(void){
	
	ticks = SDL_GetTicks ();
	tickspassed = ticks - prevticks;
	updates = 0;
	
	while (tickspassed > period) {
		tickspassed -= period;
		updates++;
	}
	
	SDL_Delay (period - tickspassed);
	prevticks = SDL_GetTicks ();
}



void FrameRateControl::setfps(int fps){
	framespersecond = fps;
	period = 20;
}

void FrameRateControl::init(void){
	prevticks = SDL_GetTicks ();
}

	
