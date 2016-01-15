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
#include "fireball.h"
#include "framerate.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>


class Game {
private:
	enum {MAX_FIREBALL_NUM = 10};
	
	public:
	
		Wizard wizard[5], *wizardsorted[5];
		Fireball fireball[MAX_FIREBALL_NUM];
		
		SDL_Surface *screen, *arena, *fireballfly[10], *fireballstart[9], *fireballend[8], *fireballglow[10];
		FrameRateControl framerate;	

		Mix_Chunk *swing, *hit, *collide, *die, *invoke, *shoot;	
		Mix_Music *music;
		
		Uint8 maxpoints, numplayers, numfireballs, numdead;
		Uint16 fireballtimer;
		bool stop, finished, quit, playmusic, playsounds, fullscreen;
		Uint8 soundchannel;
		Uint8 gfx;

		// Setup
		void DisplaySetup(void);
		void WizardSetup(void);
		void FireballSetup(void);
		void AudioSetup (void);
		void RandomizeAI(void);
		void SetStartPositions(void);
		
		void StartMusic(void);
		void StopMusic(void);

		//main loop
		void DisplayUpdate(void);
		void PlaySounds(void);	
		void Events(void);
		void GameUpdate(void);
		void AI(void);
		void CheckState(void);

		//end
		void FinishAnimations(void);
		void Score(void);
		void ScoreBoard(TTF_Font *font, SDL_Surface *wizardicon[5], SDL_Surface *pointicon, SDL_Surface *emptypointicon);
		void Winner(TTF_Font *font);

		//misc
		void FadeOut(void);
		void FadeIn (void);	
		float Distance(Wizard *wizard, Fireball *fireball);


		
	private:

		SDL_Event event;
		bool collides(Wizard *wizard, Wizard *enemy);
		bool faces(Wizard *wizard, Wizard *enemy);
		bool faces(Wizard *wizard, Fireball *fireball);
		void SortWizards(void);
		void WaitForKeypress(void);	
		Sint8 Sgn(float a);	

};

	
	
