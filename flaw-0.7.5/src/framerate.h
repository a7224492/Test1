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

class FrameRateControl {
	
	private:
		Uint32 ticks, prevticks, tickspassed;
		int period;
		
		
	public:
	
		int framespersecond;	
		int updates;
		
		void init(void);
		void update(void);
		void setfps(int fps);
			
};
