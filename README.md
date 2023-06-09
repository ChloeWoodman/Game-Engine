# Progress
Milestone 1
At 14/02/2023 my game scene could read models with texture and output fps which are statistics to show the preformance of the game, camera also rotates and moves however skybox is not working yet
![image](https://user-images.githubusercontent.com/113985493/219033173-ffcad720-4d2a-46e1-b610-7ff9ddecba91.png)

Performance profiler for GPU, CPU and memory also show regular results; GPU is ranging from 4.9% to to 16.3% depending on camera rotation, overall it is very smooth and secure. Memory starts off large as the scene is loading with highs off 56MB but quickly stablizes into 30MB, which ensures memory is stable and not leaking; finally, CPU is pretty regular as well with short usage - at the beginning of the scenes initialisation there are highs of 8% of processors, then quickly goes to ranging from 0-2%.
![image](https://user-images.githubusercontent.com/113985493/219034676-9c277b03-1466-4fb5-a318-9f818275f6e8.png)
CPU is also 99.9% kernel which is just the window of the sceene, this is expected because we are rendering our scene directly onto the window.
We can see here that by far the biggest parts of CPU usage inside our game scene is the main functions and the draw funciton inside GameEngine class. This is also expected because main is utilising and constantly running the rendering commands to draw our scene and draw is the funciton that holds the drawing information for the scene.
![image](https://user-images.githubusercontent.com/113985493/219035991-803fe816-8e32-47df-b1ae-bc104de98fcd.png)


![image](https://user-images.githubusercontent.com/113985493/219034948-6f60e0a7-0f75-412b-8bb0-efb1485c310b.png)
GPU is also pretty regular with GPU stats pretty evenly distributed across the scene, to which a lot of it is via DrawIndexed or CopySubResourceRegion.

Progress demo 26/02/2023
A lot has changed since the 14th. Now my scene has functioning skydome, skybox, 3 models (1 hidden due to scaling issues), 3 billboards, terrains and heightmaps, also rendering a quad that I am using as a plane.
Due to importing multiple models, memory has now reeached up to a steady 267MB, at the beginning there was a memory rise of course due to the starting program, then a surge of up to 400MB as assets were importing though it quickly stabilized, which means there are no memory leaks and the scene memory is consistent. 
I attempted to use performance profiler however visual studio was having output issues, so this screenshot I took while running the program will display my statitsics instead. This is using skydome by the way.
At the bottom you can see CPU usage as well, with a range of 0-4% for CPU usage, there has been a 2% usage increase in CPU after incorporating all the extra features, which I would argue is actually incredibly great results all things considered.
![image](https://user-images.githubusercontent.com/113985493/221415579-8ef123d8-7dd4-4965-850a-0132796475a9.png)
![image](https://user-images.githubusercontent.com/113985493/221415781-dccf532a-b71c-45f7-839d-654792e35cbb.png)


23/03/2023
This scene has changed in design, I have imported 3 large models which boosted the memory up by a lot - 440MB memory usage for initialized which slowly settles to 300MB afterwards. The CPU usage did have a slight increase also of starting up being 15% CPU usage but slowly settles to ranging between 0-7% usage, a 3% difference from last time but considering this is due to the models I believe it makes sense; the scene now encorporates lighting into it also which explains the changes in colour, though this may be worked upon further. Below I have attached preformance profiling to output these findings to prove that the code indeed only takes up this memory because of models (which are called in via gameEngine.cpp and being run all the time from the main function). As you can see, after initilization however memory stays the same with no increases, which shows there is no memory leak inside of the code.
![image](https://user-images.githubusercontent.com/113985493/227258292-ad047c3c-be7d-400c-985b-c5c200a127b0.png)
![image](https://user-images.githubusercontent.com/113985493/227258680-2516c8cb-ba7b-46a8-aa52-038642309254.png)
As we can see the functions that take up the most usage is in fact main which is running the program, game engine which imports everything into the scene and loading from file for the models - which we know my models are particularly large.

I will now test the game engine with my GUI I have implemented (I have made a minimap and hud) and output any changes in preformance below.
![image](https://user-images.githubusercontent.com/113985493/227259739-f4ec6052-4445-4ec4-941d-7ab687d069bf.png)
![image](https://user-images.githubusercontent.com/113985493/227260312-1777593b-5410-4dbd-a088-52327bbcb85b.png)
![image](https://user-images.githubusercontent.com/113985493/227261412-af0b5fd1-8716-4fe0-8175-91db793ba2c8.png)
As you can see CPU is the same, and memory has only increased by 1-2%.

04/04/2023
Lighting has changed ever so slightly compared to last time, it is now being utlised by a true or false uniform variable, so only thing I have authorised to have lighting will have lighting (for example models), meanwhile others do not (like my skydome and terrain, though you cannot see terrain in this screenshot because it is behind the right building model.
Alongside this, I have now created a framebuffer that mirrors anything that I input the draw function of inside of it, to which transforms into a texture that is attached to a quad on the floor to simulate water.
![image](https://user-images.githubusercontent.com/113985493/229646690-95bea73f-2ecc-4b05-8ded-b0cd4de5799b.png)
After doing preformance profiling for GPU and CPU, the gathered information is that a consistent 25% GPU has been in usage which is pretty calm all things considered, with CPU starting at highs of 18% during initialisation and quickly stabling to match ranges of 0-6% usage, which is about the same as last time so there isn't much to say about the changes all things considered. 
![image](https://user-images.githubusercontent.com/113985493/229646925-c6e386a8-7331-4d7b-b1f2-956f608154d9.png)
![image](https://user-images.githubusercontent.com/113985493/229647149-985464c6-5b52-4ce4-9650-4e5daab4bd55.png)
![image](https://user-images.githubusercontent.com/113985493/229647167-82c680eb-5893-46a8-b30f-5401684b70a8.png)
With memory, the intialisation was a slow start with highs of 309 MB, but quickly settles into a constant 237MB which to me highlights how there is no memory leak; strangly, it seems like after these changes the memory usage has actually decreased, most likely because I replaced one high poly model with a lower poly model between last test and this test, so hey, I'll take it!
![image](https://user-images.githubusercontent.com/113985493/229647701-2820da43-b896-4ad0-be02-2f383a8043d7.png)

03/05/2023
Finalized scene, this one now imports particle system using 1000 instances and batch rendering - the CPU has an intilaisation range of 13% and slowly settles into 0-13% range afterwards - this is reduced due to the large building models now being called in via batch rendering instead of the model renderer, thus helping the scene boost in with less CPU requirements. GPU is 88% which I believe could be done via the use of instances since it is 1000 particles, it is also due to the specs on my laptop is not particularly great.
![image](https://user-images.githubusercontent.com/113985493/236021461-9c41b05b-af0a-424a-ac27-57fa5349ea65.png)
As for memory usage, the intialisation was a fast increase and speeds of about 30 seconds to load with highs of 325MB due to the newly implemented instances rendering, and slowly setlles down into 253.9MB with no increase that insures there is no memory leak inside of the code; the memory usage is about a 20MB difference compared to last time before the implementation of batch and instances rendering, however I am pretty happy with this due to the fact the instances is still rendering in 1000 new particles - which use models, thus the fact it is only increasing memory by 20MB is pretty impressive. 
![image](https://user-images.githubusercontent.com/113985493/236023704-1f2ec898-5bd4-475f-ab7c-ac9dde0df1e3.png)

# Project works on Linux using Code Blocks
![image](https://user-images.githubusercontent.com/113985493/227385919-492c7f0d-2775-438f-bbb5-dc23770e9292.png) [dated 4/4/2023)

final project for Linux:
![image](https://user-images.githubusercontent.com/113985493/236647450-dbe4c569-5d0a-4400-80c6-3050265ad9a6.png)

# Testing
Due to the fact I have skybox and skydome working, I will now test them (using the same image for correct results) via quality and memory usage to decide which one is best for my game scene. For this I will comment out all other rendering inside game engine and run only skybox/skydome, then I will compare memory usage and picture quality to make an informed decision on which one to pick.
First is skydome.
![image](https://user-images.githubusercontent.com/113985493/221416180-e8378c90-3bf2-4a76-b4af-7ed4b7cfd81d.png)
As we can see,  memory quickly rose at the beginning to load in skydome, with highs of 66MB at one point, but after initialization, quickly went back down to a consistent 59-60MB. The image quality of skydome is very crisp, it is clear to me the reason memory is high is due to image quality looking better.
![image](https://user-images.githubusercontent.com/113985493/221416274-5f498ea3-3b16-4f49-bc27-ad5a1e177886.png)

Now for skybox
Note, this is using the same image for all sides, thus the output looks very stripey. 
![image](https://user-images.githubusercontent.com/113985493/221416481-02aefeac-fdbf-4f0f-b8ef-bf93d001558c.png)
Here we see memory highs of 41MB which slowly decrease into a steady 31MB confirming that skybox is about half the memory size as skydome. However this is due to a lack of image quality, the skybox is more pixelated at less aesthetically pleasing than a skydome, which means the compressed image files help keep skybox's size at a minimum.
![image](https://user-images.githubusercontent.com/113985493/221416556-7017df26-e0e0-49c2-876a-aa2ccdf3b647.png)

For me personally, I am okay with the 30MB difference in the game scene, to gain a clearer image - skydome allows for one image to be wrapped around for a scene's sky rather than editing in and making 6 images go perfectly with each other for  a sky that is less quality. For my own personal convience and aesthetics I personally choose skydome.


# Final game scene illustration
Due to my theme being dystopian/post apocolyptic, my scene will be abandoned and reclaimed by nature. For the final scene I decided to use the Midjourney Bot inside the BCU chat using the /prompt to generate illustrations to base my scene off. These are the outputs I personally enjoyed:
![image](https://user-images.githubusercontent.com/113985493/221444616-99ab73f5-84d3-4940-ab48-1fe85a90ff3b.png)
The scene will be very green with an abandoned feeling with overgrown plants against old buildings. I intend to create this scene by using abandoned building models inside scene, maybe some extra models too for affect like broken cars and big tree models. Billboards will consist of plants that are growing around the floor and growing upwards or just dangling plants from objects for a more overgrown effect. I might want to add water puddles inside the scene yet I am undecided. Unlike these images I may also have mountain terrains in the horizon for dramatic effect, however eveything else is rather consistent. I also intend on having some lighting inside my scene for a more dramatic effect, kind of like sun shining through the buildings to show nature taking control over what once was human. 

# Model references

"broken car" (https://skfb.ly/oq9TY) by Belsbury is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
"3D Warehouse" by HuNtEr_3DdD (https://www.turbosquid.com/3d-models/3d-warehouse-building-architecture-1601421).
"Old rusty bike" (https://skfb.ly/69ZLP) by alban is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
"Abandoned Factory Building" by Abandoned World (https://www.turbosquid.com/3d-models/3d-abandoned-factory-building/1116492).
"fire" (https://skfb.ly/6V9OK) by Edgar_koh is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

# Textures
![image](https://user-images.githubusercontent.com/113985493/217689932-878a1475-4978-4222-a0f5-e60125e15339.png)
https://photos.com/featured/moody-sky-eyedias.html?product=greeting-card
https://www.google.com/url?sa=i&url=http%3A%2F%2Fwww.stickpng.com%2Fimg%2Fnature%2Fbushes-and-branches%2Fgreen-bush&psig=AOvVaw096JU5pnE0LDacLFDl9-BR&ust=1679098961655000&source=images&cd=vfe&ved=0CA8QjRxqFwoTCJCvxJrZ4f0CFQAAAAAdAAAAABAQ
![big_tree](https://user-images.githubusercontent.com/113985493/225783256-95e0a7f9-d856-44c6-8e25-a25727bb223e.png)
![leaf_hub](https://user-images.githubusercontent.com/113985493/225783446-634bfd9f-c3a4-48ab-8af2-bbc449122c32.png)
![image](https://user-images.githubusercontent.com/113985493/217690262-927136f2-9e17-4d48-b93a-39f62cd6b448.png)
https://www.freepik.com/premium-photo/old-castle-wild-mossy-stone-wall-background_8130426.htm
![old-build](https://user-images.githubusercontent.com/113985493/225783480-71ae45a5-0621-478c-9fe6-7667a80a1f58.png)
fire texture https://www.google.com/url?sa=i&url=https%3A%2F%2Fwww.123rf.com%2Fphoto_27372628_blaze-fire-flame-texture-background.html&psig=AOvVaw29FFtkqaOjRDNFiNB2Rz6g&ust=1682340938990000&source=images&cd=vfe&ved=0CBAQjRxqFwoTCNCTlMeGwP4CFQAAAAAdAAAAABAJ

