#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include <SDL_opengl.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "TriangleRenderer.h"
#include "Camera.h"
#include "ModelRenderer.h"
#include "Model.h"
#include "Textures.h"
#include "Skybox.h"
#include "Terrain.h"
#include "SkyDome.h"
#include "BillboardRenderer.h"
#include "QuadRenderer.h"
#include "GUIRenderer.h"
#include "FrameBuffer.h"
#include "PostProcessGrey.h"
#include "InstancedRenderer.h"
#include "BatchRenderer.h"


namespace GE {
	//Basic Game Engine class
	class GameEngine {
	public:
		GameEngine();	//constructor

		virtual ~GameEngine();	//deconstructor

		bool init();	//Object initialisation
		bool keep_running();	//indicates if user has closed window (game)
		void processinput();	//prcess key presses
		void update();	//Updates game logic
		void draw();	//Renders game frames
		void shutdown();	//Called when game is ending
							//To release objects created during the game

		//extra methods
		void setwindowtitle(const char*);
		bool fullscreen = false;
		int w = 640, h = 480;	//window w and h
		int windowflags;	 //hold info on how to display window

	private:
		//Private data
		//SDL window data to represent the frame
		SDL_Window* window;

		//SDL GL content to represent OpenGL in the program
		SDL_GLContext glContext;

		//add code for distance/direction to look from camera
		glm::vec3 dist;

		//Camera
		Camera* cam;

		////TriangleRenderer object variable
		//TriangleRenderer* triangle;

		//quadrenderer object variable
		QuadRenderer* quad;
		Textures* quadtex;
		ModelRenderer* qmr;

		//Model objects
		Model* m;
		Textures* tex;
		
		ModelRenderer* mr;		
		
		Model* m2;
		Textures* tex2;
		Textures* normTex2;
		ModelRenderer* mr2;			
		
		Model* m3;
		Textures* tex3;
		Textures* normTex3;
		ModelRenderer* mr3;				
		
		Model* m4;
		Textures* tex4;
		ModelRenderer* mr4;				
		
		Model* m5;
		Textures* tex5;
		Textures* normTex5;
		ModelRenderer* mr5;	

		Model* m6;
		Textures* tex6;
		Textures* normTex6;
		ModelRenderer* mr6;		

		//skybox
		Skybox* skybox;

		//terrain
		Terrain* terrain;
		ModelRenderer* mrt;
		Textures* ttex;
		Textures* normTtex;

		//skydome
		SkyDome* sky;
		Textures* stex;
		ModelRenderer* mrs;

		//billboard objects
		Textures* bbTex;
		BillboardRenderer* bbr;		
		Textures* bbTex2;
		BillboardRenderer* bbr2;		
		BillboardRenderer* bbr3;
		BillboardRenderer* bbr4;
		Textures* bbTex5;
		BillboardRenderer* bbr5;

		//GUI and fonts
		GUIRenderer* gr;
		GUIText* FPSMsg;

		GUIRenderer* mmr;
		Textures* mmt;
		GUIImage* mmi;		
		
		GUIRenderer* mmr2;
		Textures* mmt2;
		GUIImage* mmi2;	

		GUIRenderer* mmr3;
		Textures* mmt3;
		GUIImage* mmi3;

		GUIRenderer* mmr4;
		Textures* mmt4;
		GUIImage* mmi4;		
		
		GUIRenderer* mmr5;
		Textures* mmt5;
		GUIImage* mmi5;

		FrameBuffer* fb;
		//declare model and model renderer for quad
		//onto which framebuffer for texture to be renderered
		Model* qm2;
		ModelRenderer* qmr2;

		PostProcessGreyscale* ppg;

		//instancerenderer
		InstancedRenderer* ir;
		Model* mir;
		Textures* tir;

		// Create a vector to store the locations, rotations, scales, and lifespans of the particles
		std::vector<InstancePosRotScaleLife> instances;

		// Define the properties of the particles
		int particles_num = 1000;
		const float max_lifetime = 10.0f; // Maximum lifespan of particles in seconds
		
		//batchrenderer
		BatchRenderer* br;

		//These variables store the time at two points in the program
		Uint32 last_time = SDL_GetTicks(), current_time = 0;

		//Frame counter
		int frame_count = 0;

		float camSpeed = 2.0f;
		float mouseSens = 0.1f;
		float mmplayerspeed = camSpeed * 0.2f;

	};
	//helper function
	void display_info_message(const char*);
}