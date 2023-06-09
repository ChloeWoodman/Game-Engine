#define SDL_MAIN_HANDLED
#include "GameEngine.h"
#include <sstream>

using namespace GE;

//Program starts
int main(int argc, char* argv[]) {
	//Create a game engine object
	GameEngine ge;

	//Initialise it
	if (!ge.init()) {
		display_info_message("Could not start SDL. Check console output for more details.");

		return -1;
	}

	//These variables store the time at two points in the program
	Uint32 last_time = SDL_GetTicks(), current_time = 0;

	//Frame counter
	int frame_count = 0;

	//Main loop
	while (ge.keep_running()) {
		//Update game state
		ge.update();

		//handle input
		ge.processinput();

		//draw
		ge.draw();

		//frame counter increase
		frame_count++;

		//gets current time now
		current_time = SDL_GetTicks();

		//the difference between the two times 1000
		if (current_time - last_time > 1000) {
			//variable to store a string
			std::ostringstream msg;

			//create a message based on value in frame_count
			msg << "FPS = " << frame_count;

			//display message in window title
			ge.setwindowtitle(msg.str().c_str());

			//reset frame counter
			frame_count = 0;

			//update time variable for next count
			last_time = current_time;
		}
	}

	//Destory the game engine
	ge.shutdown();

	return 0;
}
//End of program