	#include "GameEngine.h"
#include <iostream>

namespace GE {

	//simple helper function to display program information
	//note it is seperate from the GameEngine class as it does not depend on it
	void display_info_message(const char* msg) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Basic Game Engine", msg, nullptr);
	}

	GameEngine::GameEngine() {

	}

	GameEngine::~GameEngine() {
	}

	bool GameEngine::init() {
		//Initialise SDL video subsystem
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "Unable to initialise SDL! SDL error: " << SDL_GetError() << std::endl;

			return false;
		}

		////Create new camera object
		//cam = new Camera(glm::vec3(0.0f, 0.0f, 5.0f),	//position
		//	glm::vec3(0.0f, 0.0f, 0.0f),	//look at
		//	glm::vec3(0.0f, 1.0f, 0.0f),	//up direction
		//	45.0f, 640.0f / 480.0f, 0.1f, 100.0f); //fov, aspect ratio based on window dimensions, near and far clip planes

		//Set the OpenGL version for the program
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		//Set the type of profile which is more core meaning  modern OpenGL
		//means no legacy features for backwards compatibility 
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		
		
		//Set the OpenGL version for the program
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		////Set the type of profile which is more core meaning  modern OpenGL
		////means no legacy features for backwards compatibility 
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//Create the window and frame features
		//In this case window has a fixed size and prepares window for OpenGL to render into
		window = SDL_CreateWindow("Game Engine Scene :)", 1920 / 4, 1080 / 4, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		//check window was created
		if (window == nullptr) {
			//Display an error if window wasn't dcreared
			//Could log the errors to a file or create more robust error handling
			//e.g. give specific errors a number
			std::cerr << "Unable to create window! SDL error: " << SDL_GetError() << std::endl;

			//return failure so caller can work out what to do
			return false;
		}

		glContext = SDL_GL_CreateContext(window);

		//creates the OpenGL context and links it to the wndow object
		//context represents OpenGL for program such as objects and drawing
		if (glContext == nullptr) {
			//display error and return failure
			std::cerr << "SDL could not create GL context! SDL Error: " << SDL_GetError() << std::endl;
			return false;
		}

		//Initialise GLEW. GLEW solves a problem with OpenGL on windows
		//as default API supported by windows is 1.1 (from 1997)
		//GLEW binds latest extensions that can be used
		GLenum status = glewInit();

		//check contedxt objecxt was created
		if (status != GLEW_OK) {
			std::cerr << "Error initialising GLEW! Error: " << glewGetErrorString(status) << std::endl;

			return false;
		}

		//Try to turn on VSync. Maybe this could be a parameter to the init method rather than hard coded
		if (SDL_GL_SetSwapInterval(true) != 0) {
			std::cerr << "Warning: Unable to set VSync! Error: " << SDL_GetError() << std::endl;

			return false;
		}

		//look down z axis into the screen
		dist = glm::vec3(0.0f, 0.0f, -100.0f);

		//camera setup
		cam = new Camera(glm::vec3(0.0f, 10.0f, 0.0f),//cam position
			glm::vec3(0.0f, 0.0f, 0.0f),//cam target
			glm::vec3(0.0f, 1.0f, 0.0f), //cam upDir
			45.0f, 640.0f / 480.0f, 0.01f, 1000.0f //fov, ar (aspect ratio), near, far clip planes
		);


		////create the TriangleRenderer object
		//triangle = new TriangleRenderer();
		////initialise the object
		//triangle->init();
		//triangle->setPos(0.0f, 0.0f, 0.0f);
		//triangle->setScale(1.0f, 1.0f, 1.0f);

		//quad
		quadtex = new Textures("./textures/path.jpg");
		quad = new QuadRenderer(cam);
		quad->init();
		quad->setTexture(quadtex);
		quad->setPos(0.0f, 0.0f, 0.0f);
		quad->setScale(500.0f, 500.0f, 0.0f);
		quad->setRot(90.0f, 0.0f, 0.0f);


		m = new Model();
		//load vertices from file
		bool result = m->loadFromFile("./model/broken_car.obj");
		//LOADED OK?
		if (!result) {
			std::cerr << "failed to load model" << std::endl;
		}
		tex = new Textures("./textures/Meshpart1_diff.png");
		//create a modelrenderer passing the model object just loaded
		mr = new ModelRenderer(cam);
		//initialise modelrenderer, reduce scale
		mr->init();
		mr->setPos(-20.0f, -5.0f, -20.0f);
		mr->setTexture(tex);
		mr->setRotation(0.0f, 0.0f, 0.0f);
		//mr->setScale(10.0f, 10.0f, 10.0f);
	
		
		//m2 = new Model();
		//bool result2 = m2->loadFromFile("./model/factory_building.obj");
		//if (!result2) {
		//	std::cerr << "failed to load model2" << std::endl;
		//}
		//tex2 = new Textures("./textures/old-build.png");
		//mr2 = new ModelRenderer(cam);
		////initialise modelrenderer, reduce scale
		//mr2->init();
		//mr2->setPos(-50.0f, 0.0f, 50.0f);
		//mr2->setTexture(tex2);
		//mr2->setRotation(0.0f, 90.0f, 0.0f);
		//mr2->setScale(0.15f, 0.4f, 0.02f);
		//
		//m3 = new Model();
		//bool result3 = m3->loadFromFile("./model/warehouse.obj");
		//if (!result3) {
		//	std::cerr << "failed to load model2" << std::endl;
		//}
		//tex3 = new Textures("./textures/warewhouse_Diff.jpg");
		//mr3 = new ModelRenderer(cam);
		////initialise modelrenderer, reduce scale
		//mr3->init();
		//mr3->setPos(0.0f, 30.0f, -150.0f);
		//mr3->setTexture(tex3);
		//mr3->setRotation(0.0f, 0.0f, 0.0f);
		////mr3->setScale(0.12f, 0.15f, 0.05f);
		//
		//m4 = new Model();
		//bool result4 = m4->loadFromFile("./model/factory_building.obj");
		//if (!result4) {
		//	std::cerr << "failed to load model4" << std::endl;
		//}
		//mr4 = new ModelRenderer(cam);
		////initialise modelrenderer, reduce scale
		//mr4->init();
		//mr4->setPos(70.0f, 0.0f, 50.0f);
		//mr4->setTexture(tex2);
		//mr4->setRotation(0.0f, 90.0f, 0.0f);
		//mr4->setScale(0.15f, 0.4f, 0.02f);

		m5 = new Model();
		//load vertices from file
		bool result5 = m5->loadFromFile("./model/mesh.obj");
		//LOADED OK?
		if (!result5) {
			std::cerr << "failed to load model" << std::endl;
		}
		tex5 = new Textures("./textures/tex01.jpg");
		//create a modelrenderer passing the model object just loaded
		mr5 = new ModelRenderer(cam);
		//initialise modelrenderer, reduce scale
		mr5->init();
		mr5->setPos(20.0f, 0.0f, -40.0f);
		mr5->setTexture(tex5);
		mr5->setRotation(0.0f, 0.0f, 0.0f);		
		
		
		m6 = new Model();
		//load vertices from file
		bool result6 = m6->loadFromFile("./model/space_frigate_6.obj");
		//LOADED OK?
		if (!result6) {
			std::cerr << "failed to load model" << std::endl;
		}
		tex6 = new Textures("./textures/space_frigate_6_color.jpg");
		//create a modelrenderer passing the model object just loaded
		mr6 = new ModelRenderer(cam);
		//initialise modelrenderer, reduce scale
		mr6->init();
		mr6->setPos(0.0f, 50.0f, 0.0f);
		mr6->setTexture(tex6);
		mr6->setRotation(10.0f, 0.0f, 0.0f);

		//skybox = new Skybox("./skybox/front.jpg", "./skybox/back.jpg", "./skybox/right.jpg", "./skybox/left.jpg", "./skybox/top.jpg", "./skybox/bottom.jpg");

		//terrain
		ttex = new Textures("./textures/old.jpg");
		mrt = new ModelRenderer(cam);
		mrt->init();
		terrain = new Terrain();
		mrt->setTexture(ttex);
		mrt->setPos(100.0f, -20.0f, 0.0f);

		//skydome
		stex = new Textures("./textures/cloudysky.png");
		sky = new SkyDome(570, 200);
		mrs = new ModelRenderer(cam);
		mrs->init();
		mrs->setTexture(stex);
		mrs->setScale(0.25, -0.25, 0.25);
		mrs->setPos(0, 0, 0);
		mrs->setRotation(0, 0, 0);

		//load a texture from the billboard
		bbTex = new Textures("./textures/big_tree.png");
		////by default billboard will be at 0,0,0 in front of cam
		////create the billboard renderer
		bbr = new BillboardRenderer(cam);
		bbr->init();
		bbr->setMaterial(bbTex);
		////scale billboard as only 1.0x1.0
		bbr->setScaleX(200.0f);
		bbr->setScaleY(180.0f);
		////set it in front of the model
		bbr->setPos(85.0f, -10.0f, -90.0f);
		

		bbTex2 = new Textures("./textures/tree.png");
		bbr2 = new BillboardRenderer(cam);
		bbr2->init();
		bbr2->setMaterial(bbTex2);
		bbr2->setScaleX(100.0f);
		bbr2->setScaleY(150.0f);
		bbr2->setPos(100, -10, 0);

		bbr3 = new BillboardRenderer(cam);
		bbr3->init();
		bbr3->setMaterial(bbTex2);
		bbr3->setScaleX(100.0f);
		bbr3->setScaleY(100.0f);
		bbr3->setPos(-40, -10, 120);

		bbr4 = new BillboardRenderer(cam);
		bbr4->init();
		bbr4->setMaterial(bbTex2);
		bbr4->setScaleX(100.0f);
		bbr4->setScaleY(150.0f);
		bbr4->setPos(-100, -10, 0);	
		
		bbTex5 = new Textures("./textures/bushes.png");
		bbr5 = new BillboardRenderer(cam);
		bbr5->init();
		bbr5->setMaterial(bbTex5);
		bbr5->setScaleX(60.0f);
		bbr5->setScaleY(30.0f);
		bbr5->setPos(-50, 0, -50);

		//GUIRenderer 
		gr = new GUIRenderer();
		//initialise it
		gr->init(w, h);
		//create a text obj for fps, FONT CREDIT: WWW.FONTSQUIRREL.COM
		FPSMsg = new GUIText(0, 0, "Hello World", "..\\Oswald-Regular.ttf");

		//map
		mmt = new Textures("./textures/path.jpg");
		mmi = new GUIImage(0, 0, mmt);
		mmr = new GUIRenderer();
		mmr->init(w / 3, h / 3);

		//hub
		mmt2 = new Textures("./textures/leaf_hub.png");
		mmi2 = new GUIImage(0, 0, mmt2);
		mmr2 = new GUIRenderer();
		mmr2->init(w, h);

		//player point 
		mmt3 = new Textures("./textures/player-mini-map-img.png");
		mmi3 = new GUIImage(0, 0, mmt3);
		mmi3->setPosX(50);
		mmi3->setPosY(50);
		mmr3 = new GUIRenderer();
		mmr3->init(w, h);

		//player speed increase meter
		mmt4 = new Textures("./textures/speed.png");
		mmi4 = new GUIImage(0, 0, mmt4);
		mmi4->setPosX(420);
		mmi4->setPosY(20);
		mmr4 = new GUIRenderer();
		mmr4->init(w, h);

		mmt5 = new Textures("./textures/player-mini-map-img2.png");
		mmi5 = new GUIImage(0, 0, mmt5);
		mmi5->setPosX(525);
		mmi5->setPosY(22);
		mmr5 = new GUIRenderer();
		mmr5->init(w, h);

		//frame buffer
		fb = new FrameBuffer(w, h);
		fb->init();
		qm2 = new Model();
		bool result7 = qm2->loadFromFile("./model/scene_quad.obj");
		if (!result7) {
			std::cerr << "failed to load quad model" << std::endl;
		}
		//create model renderer object to render the quad
		qmr2 = new ModelRenderer(cam);
		qmr2->init();
		qmr2->setPos(0.0f, 1.0f, 0.0f);
		qmr2->setScale(50.0f, 1.0f, 50.0f);
		qmr2->setTexture(fb->getTexture());

		//post process greyscale
		ppg = new PostProcessGreyscale(w, h);
		ppg->init();

		//instanced renderer
		ir = new InstancedRenderer();
		ir->init();
		mir = new Model();
		mir->loadFromFile("./model/fire.fbx");
		tir = new Textures("./textures/fire.jpeg");

		for (int count = 0; count < particles_num; count++) {
			// Generate random position, rotation, scale, and lifespan for each particle
			float rand_x = static_cast<float>(rand() % 20) - 30.0f; // Random x position between -30 and 20
			float rand_y = static_cast<float>(rand() % 20) + 0.0f; // Random y position between 0 and 20
			float rand_z = static_cast<float>(rand() % 10) - 30.0f; // Random z position between -30 and 10
			float rand_rot_x = static_cast<float>(rand() % 360); float rand_rot_y = static_cast<float>(rand() % 360); float rand_rot_z = static_cast<float>(rand() % 360); // Random x,y,z rotation between 0 and 359 degrees
			float rand_scale = static_cast<float>(rand() % 50) / 100.0f + 0.5f; // Random scale between 0.5 and 1.0
			
			// Modify the scale and lifespan to make the particles look like a raindrop flame
			rand_scale *= 0.3f; // Reduce the scale to make the particles smaller
			
			// Add some randomness to the rotation around the y-axis to make the particles spread out in a circular pattern
			rand_rot_y += static_cast<float>(rand() % 180) - 90.0f; // Random rotation around y-axis between -90 and 90 degrees

			// Modify the position to make the particles more raindrop flame-like
			float curve = 1.0f; // Create a curve
			rand_x *= curve * 0.5f; // Multiply the x position by a smaller curve to create a narrower shape
			rand_z *= curve * 0.5f; // Multiply the z position by a smaller curve to create a narrower shape
			rand_y += curve * curve * 10.0f; // Add a quadratic curve to the y position to create a flame-like shape
			rand_y *= curve; // Reduce the y position to make the top smaller

			// Adjust the scale along the x-axis to make the bottom thicker and the top thinner
			float x_scale = 1.0f - curve; // Calculate a normalized x scale value
			rand_scale *= x_scale * 0.5f + 0.5f; // Scale the particle along the x-axis to create a raindrop shape

			// Add the particle to the instances vector
			instances.push_back(InstancePosRotScaleLife{ rand_x, rand_y, rand_z, rand_rot_x, rand_rot_y, rand_rot_z, rand_scale, rand_scale, rand_scale });
		}
		// Update instancerenderer obj with instance data
		ir->setInstanceData(instances);

		//batch renderer
		br = new BatchRenderer();
		br->init();
		br->addModelToBatch("./model/factory_building.obj", "./textures/old-build.png",
			//location							rotation					scale
			glm::vec3(-50.0f, 0.0f, 50.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.15f, 0.4f, 0.02f));;

		br->addModelToBatch("./model/warehouse.obj", "./textures/warewhouse_Diff.jpg",
			//location							rotation					scale
			glm::vec3(0.0f, 30.0f, -150.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.f, 1.f, 1.f));;

		br->addModelToBatch("./model/factory_building.obj", "./textures/old-build.png",
			//location							rotation					scale
			glm::vec3(70.0f, 0.0f, 50.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.15f, 0.4f, 0.02f));;

		//br->addModelToBatch("./model/broken_car.obj", "./textures/Meshpart1_diff.png",
		//	//location							rotation					scale
		//	glm::vec3(-20.0f, -5.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.f,1.f,1.f));;
		//
		//br->addModelToBatch("./model/mesh.obj", "./textures/tex01.jpg",
		//	//location							rotation					scale
		//	glm::vec3(20.0f, 0.0f, -40.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.f,1.f,1.f));;		
		//
		//br->addModelToBatch("./model/space_frigate_6.obj", "./textures/space_frigate_6_color.jpg",
		//	//location							rotation					scale
		//	glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(1.f,1.f,1.f));;

		return true;
	}


	bool GameEngine::keep_running() {
		//need to do this to keep the event queue up to date
		SDL_PumpEvents();

		SDL_Event evt;

		//Check for quit event
		if (SDL_PeepEvents(&evt, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT)) {
			//If the user quits the program, then return false meaning; don't keep running
			return false;
		}

		//Got this far means no quit event so game can keep running
		return true;
	}

	void GameEngine::processinput()
	{
		//Camera and mouse speed should be influenced by deltatime for hardware independance
		//get current mouse x and y
		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);
		//calculate the difference between old mouse and new mouse position - note inversion on y as this is flipped
		float diffx = mouse_x - (w / 2);
		float diffy = (h / 2) - mouse_y;

		//update the yaw and pitch based on the mouse differential 
		cam->setYaw(cam->getYaw() + diffx * mouseSens);
		cam->setPitch(cam->getPitch() + diffy * mouseSens);

		//set mouse back to the centre
		SDL_WarpMouseInWindow(window, w / 2, h / 2);

		//calculate new camera facing direction from the change to yaw and pitch
		glm::vec3 direction;
		direction.x = cos(glm::radians(cam->getYaw())) * cos(glm::radians(cam->getPitch()));
		direction.y = sin(glm::radians(cam->getPitch()));
		direction.z = sin(glm::radians(cam->getYaw())) * cos(glm::radians(cam->getPitch()));
		cam->setTarget(glm::normalize(direction));

		//array to hold states of key presses we're interested in
		bool keyStates[6] = { 0,0,0,0,0,0 };
		//indeces to the above array to store respective key presses
		int UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, F = 4, G = 5;

		//poll for SDL event
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			//if there has been a key pressed, check if its UP, DOWN, LEFT AND RIGHT
			//mark the array to indiciate this key is held down
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
				case SDL_SCANCODE_W:
					keyStates[UP] = true;
					break;
				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_S:
					keyStates[DOWN] = true;
				break; case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_A:
					keyStates[LEFT] = true;
					break;
				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_D:
					keyStates[RIGHT] = true;
					break;
				case SDL_SCANCODE_F:
					keyStates[F] = true;
					break;
				case SDL_SCANCODE_G:
					keyStates[G] = true;
					break;
				}
			}
			//if key is not pressed anymore
			if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
				case SDL_SCANCODE_W:
					keyStates[UP] = false;
					break;
				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_S:
					keyStates[DOWN] = false;
					break;
				case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_A:
					keyStates[LEFT] = false;
					break;
				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_D:
					keyStates[RIGHT] = false;
					break;
				case SDL_SCANCODE_F:
					keyStates[F] = false;
					break;
				case SDL_SCANCODE_G:
					keyStates[G] = false;
					break;
				}
			}
		}

		glm::vec3 posLimit(200.0f, 200.0f, 200.0f);
		glm::vec3 camPos = cam->getPos();
		//mmi3->setPosX(camPos.x + 45);
		//mmi3->setPosY(camPos.z + 45);

		if ((cam->getPosX() <= posLimit.x && cam->getPosX() >= -posLimit.x) && (cam->getPosY() <= posLimit.y && cam->getPosY() >= -posLimit.y) && (cam->getPosZ() <= posLimit.z && cam->getPosZ() >= -posLimit.z)) {
			//std::cerr << cam->getPosX() << " " << cam->getPosY() << " " << cam->getPosZ() << std::endl;
			// handle camera movement based upon the relevant key press
			if (keyStates[UP]) {
				glm::vec3 newPos = cam->getPos() + cam->getTarget() * camSpeed;
				if (newPos.x < posLimit.x && newPos.y < posLimit.y && newPos.z < posLimit.z && mmi3->getY() >= 10) {
					cam->setPos(newPos);
					mmi3->setPosY(mmi3->getY() - mmplayerspeed);
					std::cerr << mmi3->getY() << std::endl;
				}
			}
			if (keyStates[DOWN]) {
				glm::vec3 newPos = cam->getPos() - cam->getTarget() * camSpeed;
				if (newPos.x < posLimit.x && newPos.y < posLimit.y && newPos.z < posLimit.z) {
					cam->setPos(newPos);
					mmi3->setPosY(mmi3->getY() + mmplayerspeed);
					std::cerr << mmi3->getY() << std::endl;
				}
			}
			if (keyStates[LEFT]) {
				// Take away cross product of the front facing and up vector from the camera position - strafe left
				glm::vec3 newPos = cam->getPos() - glm::normalize(glm::cross(cam->getTarget(), cam->getUpDir())) * camSpeed;
				if (newPos.x < posLimit.x && newPos.y < posLimit.y && newPos.z < posLimit.z && mmi3->getX() >= 10) {
					cam->setPos(newPos);

					mmi3->setPosX(mmi3->getX() - mmplayerspeed);
					std::cerr << mmi3->getX() << std::endl;
				}
			}
			if (keyStates[RIGHT]) {
				// take away cross product of the front facing and up vector from the camera position - strafe right
				glm::vec3 newPos = cam->getPos() + glm::normalize(glm::cross(cam->getTarget(), cam->getUpDir())) * camSpeed;
				if (newPos.x < posLimit.x && newPos.y < posLimit.y && newPos.z < posLimit.z) {
					cam->setPos(newPos);
					mmi3->setPosX(mmi3->getX() + mmplayerspeed);
					std::cerr << mmi3->getX() << std::endl;
				}
			}


			if (keyStates[F] && mmi5->getX() > 460) {
				camSpeed += 0.5f;
				mmi5->setPosX(mmi5->getX() - 20);
				std::cerr << "keystates F pressed" << std::endl;
				//std::cerr << mmi5->getX() << std::endl;
				std::cerr << camSpeed << "\n";
			}
			if (keyStates[G] && mmi5->getX() < 580) {
				camSpeed -= 0.5f;
				mmi5->setPosX(mmi5->getX() + 20);
				std::cerr << "keystates G pressed" << std::endl;
				//std::cerr << mmi5->getX() << std::endl;
				std::cerr << camSpeed << "\n";
			}
		}
		else {
			glm::vec3 newPos = cam->getPos();

			newPos.x = std::min(newPos.x, posLimit.x);
			newPos.x = std::max(newPos.x, -posLimit.x);

			newPos.y = std::min(newPos.y, posLimit.y);
			newPos.y = std::max(newPos.y, -posLimit.y);

			newPos.z = std::min(newPos.z, posLimit.z);
			newPos.z = std::max(newPos.z, -posLimit.z);

			cam->setPos(newPos);

			std::cerr << cam->getPosX() << " " << cam->getPosY() << " " << cam->getPosZ() << std::endl;
		}
		cam->updateCamMatrices();
	}

	void GameEngine::update() {

		//skybox spin
		//skybox->setRotation(0, skybox->getRotY() + 1.0f, 0);

		//mrs->setRotation(0.0f, mrs->getRotY() + 1.0f, 0.0f);
		//mr->setRotation(0.0f, mr->getRotY() + 1.25f, 0.0f);

		mr6->setRotation(mr6->getRotX() + 1.25f, 0.0f, 0.0f);


		for (int i = 0; i < instances.size(); i++) {
			// Generate a random displacement amount between -0.05 and 0.05 on the x and z axes
			float rand_dx = static_cast<float>(rand() % 11) / 100.0f - 0.05f;
			float rand_dz = static_cast<float>(rand() % 11) / 100.0f - 0.05f;

			// Apply the displacement to the particle's position, only changing X and Z coordinates
			instances[i].posX += rand_dx;
			instances[i].posZ += rand_dz;

			// Keep the particle within the bounds of the fire pit
			if (instances[i].posX < -30.0f) {
				instances[i].posX = -30.0f;
			}
			else if (instances[i].posX > 20.0f) {
				instances[i].posX = 20.0f;
			}
			if (instances[i].posZ < -30.0f) {
				instances[i].posZ = -30.0f;
			}
			else if (instances[i].posZ > 10.0f) {
				instances[i].posZ = 10.0f;
			}
		}

		ir->updateInstanced(instances);

		frame_count++;
		current_time = SDL_GetTicks();
		if (current_time - last_time > 1000) {
			std::string msg;
			msg = "FPS =" + std::to_string(frame_count);
			FPSMsg->setText(msg);
			frame_count = 0;
			last_time = current_time;
		}

		float deltaTime = (current_time - last_time) / 1000.0f;


		//rotate camera at location
		glm::mat4 cam_rot = glm::mat4(1.0f);

		//create a rotation matrix of 0.625 of a degree (per frame) on the Y axis
		cam_rot = glm::rotate(cam_rot, glm::radians(0.625f), glm::vec3(0.0f, 1.0f, 0.0f));

		//create a vec4 in order to multiply against rotation matrix; note, the last value is 0.0f as we want to rotate direction not translate
		glm::vec4 temp = glm::vec4(dist, 0.0f);
		//rotate the dist
		temp = temp * cam_rot;
		//create a new dist based on the rotation
		dist = glm::vec3(temp.x, temp.y, temp.z);

		//update camera
		cam->setTarget(dist);
	}

	void GameEngine::draw() {

		// Begin rendering to framebuffer
		fb->beginRender();
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw only wanted models inside the framebuffer
		mr6->draw(cam, m6);
		mrs->drawSkyDome(sky);

		fb->endRender(); //end frame buffer rendering, switches to default framebuffer
		
		ppg->apply(fb->getTexture());
						 
		// Clear default framebuffer as normal
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		//render what was rendered to the framebuffer
		//to a quad should look a bit like a mirror
		ppg->renderToScreen();

		// Draw the rest of the scene as usual
		mr->draw(cam, m);
		//mr2->draw(cam, m2);
		//mr3->draw(cam, m3);
		//mr4->draw(cam, m4);
		mr5->draw(cam, m5);
		mr6->draw(cam, m6);

		mrt->drawTerrain(terrain);

		mrs->drawSkyDome(sky);

		quad->draw();

		bbr->draw();
		bbr2->draw();
		bbr3->draw();
		bbr4->draw();
		bbr5->draw();

		qmr2->draw(cam, qm2); //draw quad model renderer

		//instance renderer
		ir->drawInstanced(cam, mir, tir);

		//batch renderer
		br->draw(cam);

		// Do GUI here
		gr->drawText(FPSMsg);
		mmr2->drawImage(mmi2, w, h);
		mmr->drawImage(mmi, w / 15, h / 15);
		mmr3->drawImage(mmi3, 10, 10);
		mmr4->drawImage(mmi4, w / 3, h / 9);
		mmr5->drawImage(mmi5, 10, 10);

		SDL_GL_SwapWindow(window);	//swaps the back buffer to the front butter to show frame on the screen 
	}


	void GameEngine::setwindowtitle(const char* new_title) {
		SDL_SetWindowTitle(window, new_title);
	}

	void GameEngine::shutdown() {

		//get the triangle object to release its resources
		//triangle->destroy();
		quad->destroy();

		//skybox->destroy();
		mr->destroy();
		m->~Model();		
		mr2->destroy();
		m2->~Model();		
		mr3->destroy();
		m3->~Model();	
		mr4->destroy();
		m4->~Model();
		gr->destroy();
		mmr->destroy();
		mmr2->destroy();
		mmr3->destroy();
		mmr4->destroy();
		mmr5->destroy();
		ir->destroy();
		br->~BatchRenderer();

		delete mmi, mmt, mmi2, mmt2, mmi3, mmt3;

		//delete particleSys;
		delete ir, mir, tir;

		//delete batch renderer
		delete br;

		delete mr, m, mr2, m2, mr3, m3, mr4, m4, mr5, m5, mr6, m6, cam;

		//destroy terrain
		delete mrt, terrain, ttex;

		//destroy skydome
		delete mrs, stex, sky;

		//free billboard objects
		delete bbr, bbTex, bbr, bbTex2, bbr3, bbr4, bbTex5, bbr5;

		SDL_DestroyWindow(window);

		window = nullptr;

		SDL_Quit();
	}
}