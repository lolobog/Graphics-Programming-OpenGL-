#define GLEW_STATIC
//You know about the triangle light issue
#include <glew.h>
#include <SDL_opengl.h>
#include<glm.hpp>
#include<gtc/type_ptr.hpp>
#include<SDL.h>
#include "mesh.h"
#include "Shader.h"
#include "Vertex.h"
#include <vector>
#include"LightBase.h"
#include "ObjectLoader.h"
#include "../Libraries/imgui-master/imgui.h"
#include "../Libraries/imgui-master/imgui_internal.h"
#include "../Libraries/imgui_sdl-master/imgui_sdl.h"
#include "../Libraries/imgui-master/backends/imgui_impl_sdl.h"
#include "../imgui-master/backends/imgui_impl_opengl3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#undef main SDL_main
using namespace std;


GLuint LoadTexture(string TextureLocation)
{
	GLuint textureID;
	int width, height, numComponents;
	unsigned char* ImageData = stbi_load(TextureLocation.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);

	if (ImageData == NULL)
	{
		cerr << "Texture loding failed for texture: " << TextureLocation << endl;
	}

	GLenum format=0;
	if (numComponents == 1)
		format = GL_RED;
	if (numComponents == 3)
		format = GL_RGB;
	if (numComponents == 4)
		format = GL_RGBA;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageData);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(ImageData);

	return textureID;
}




int main()
{
	//Initialization
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8 );
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32 );
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1 );
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16 );

	SDL_Renderer* renderer = nullptr;
	SDL_Window* window = nullptr;
	int screenHeight = 1080;
	int screenWidth = 1440;
	int menuHeight = 20;

	//Window Creation
	window = SDL_CreateWindow("My window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext GLContext = SDL_GL_CreateContext(window);


	

	if (!window)
	{
		printf("WINDOW initialisation failed: %s/n");
		printf("Press any key to continue/n");
		getchar();
	}

	renderer= SDL_CreateRenderer(
		window,
		-1,
		0
	);

	if (!renderer)
	{
		printf("WINDOW initialisation failed: %s/n");
		printf("Press any key to continue/n");
		getchar();
	}

	//imGui Initialize
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_CreateContext(window));
	ImGui_ImplOpenGL3_Init();
	
	
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImColor(16, 16, 16);
	style.Colors[ImGuiCol_ChildBg] = ImColor(24, 24, 24);
	style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
	style.Colors[ImGuiCol_Header]= ImColor(40, 40, 40);
	style.Colors[ImGuiCol_HeaderActive] = ImColor(40, 40, 40);
	style.Colors[ImGuiCol_HeaderHovered] = ImColor(40, 40, 40);
	style.Colors[ImGuiCol_MenuBarBg] = ImColor(40, 40, 40);
	style.Colors[ImGuiCol_TabActive] = ImColor(40, 40, 40);
	style.Colors[ImGuiCol_TitleBgActive]= ImColor(50, 50,50);
	style.Colors[ImGuiCol_SliderGrab]= ImColor(50, 50, 50);
	style.Colors[ImGuiCol_SliderGrabActive] = ImColor(60, 60, 60);
	style.Colors[ImGuiCol_Button]= ImColor(40, 40, 40);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(50, 50, 50);

	io.Fonts->AddFontFromFileTTF(".\\Assets\\ComicSans.ttf",16.0f);

	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	//Initializing GLEW
	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//Checking if GLEW is initialized correctly
	if (status != GLEW_OK)
	{
		cout << "GLEW failed to initialize!" << endl;
	}

	//Creating a square using indices

	vector <Vertex> SquareVerticies;
	SquareVerticies.push_back(Vertex (vec3(-0.5f, 0.5f, 1.0f),vec2(0,0),vec3(1,0,0))); //top left
	SquareVerticies.push_back(Vertex(vec3(0.5f, 0.5f, 1.0f), vec2(1, 0), vec3(0, 1, 0))); //top right
	SquareVerticies.push_back(Vertex (vec3(0.5f, -0.5f, 1.0f), vec2(1, 1), vec3(0, 0, 1))); //bottom right
	SquareVerticies.push_back(Vertex (vec3(-0.5f, -0.5f, 1.0f), vec2(0, 1), vec3(1, 1, 1))); //bottom left
	unsigned int SquareIndecies[]
	{
		0,1,2,0,2,3,
	};

	
	

	LightBase* light=new LightBase() ;
	light->GetTransform().SetPosition(vec3(5, 15, -15));

	LightBase* secondLight = new LightBase();
	secondLight->GetTransform().SetPosition(vec3(15, 15, 0));
	bool enableSecondLight = false;

	




	
		Camera cam;
		vec3 camera1=vec3(0, 10, -40);  
		vec3 camera2=vec3(40, 10, 0);
		vec3 camera3=vec3(-40, 10, 0);
		vec3 camera4=vec3(0, 10, 40);	


		Shader* basicShader = new Shader( "./Shaders/Resources/Basic", cam);  //"./Shaders/Resources/Basic", cam
		Shader* depthShader = new Shader("./Shaders/Resources/DepthShader", cam);
		Shader* gouraudShader = new Shader("./Shaders/Resources/Gouraud", cam);

		GLuint DiffuseTextureID=LoadTexture("./Textures/brickwall.jpg");
		GLuint NormalTextureID = LoadTexture("./Textures/brickwall_normal.jpg");
		Mesh Square1(&SquareVerticies[0], SquareVerticies.size(), &SquareIndecies[0], 6);
		Square1.m_transform.SetScale(vec3(60));
		Square1.m_transform.SetRotation(vec3(glm::radians(90.0f), 0.0f, 0.0f));
		Square1.m_transform.SetPosition(vec3(0, 60, 0));

		
		Mesh SpriteSheet(&SquareVerticies[0], SquareVerticies.size(), &SquareIndecies[0], 6);
		
		//string* AmbiantLoc = new string("");
		//string* DiffuseLoc = new string("");
		//string* SpecLoc = new string("");
		//string* NormalLoc = new string("");

		string AmbiantLoc ;
		string DiffuseLoc ;
		string SpecLoc ;
		string NormalLoc ;

		vector<uint>Indecies;

		vector<Vertex>LoadedVerts = OBJLoader::LoadOBJ("blocks_01.obj","./Objects",AmbiantLoc,DiffuseLoc,SpecLoc,NormalLoc,Indecies);
		
		GLuint AmbiantTexID = LoadTexture("./Objects/" + AmbiantLoc);
		GLuint DiffuseTexID = LoadTexture("./Objects/"+ DiffuseLoc);
		GLuint SpecularTexID = LoadTexture("./Objects/"+ SpecLoc);
		GLuint NormalTexID = LoadTexture("./Objects/" + NormalLoc);

		Mesh LoadedObj(&LoadedVerts[0], LoadedVerts.size(), &Indecies[0], Indecies.size());
		LoadedObj.m_transform.SetScale(vec3(0.35f));

		vector<uint>Indecies2;
		vector<Vertex>LoadedVerts2 = OBJLoader::LoadOBJ("blocks_01.obj", "./Objects", AmbiantLoc, DiffuseLoc, SpecLoc, NormalLoc, Indecies2);
		
		Mesh Cube(&LoadedVerts2[0], LoadedVerts2.size(), &Indecies2[0], Indecies2.size());
		Cube.m_transform.SetScale(vec3(0.35f));
		
		


		//create depth texutre


		GLuint depthMapFBO;
		glGenFramebuffers(1, &depthMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

		GLuint shadowMapID;
		int ShadowWidth = 2048;
		int ShadowHeight = 2048;

		glGenTextures(1, &shadowMapID);
		glBindTexture(GL_TEXTURE_2D, shadowMapID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowWidth, ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[]{ 1,1,1,1 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cerr << "ERROR Frame buffer is incomplete" << endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);







		
	//Window Loop
	while (true)
	{
		glViewport(0, 0, ShadowWidth, ShadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
	


		depthShader->Bind();

		GLfloat near_plane = 1.0f, far_plane = 100.f;
		glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(light->GetTransform().GetPosition(), vec3(0), vec3(0, 1, 0));
		glm::mat4 secondLightView = glm::lookAt(secondLight->GetTransform().GetPosition(), vec3(0), vec3(0, 1, 0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		glm::mat4 secondLightSpaceMatrix = lightProjection *secondLightView;
		depthShader->UpdateForShadow(LoadedObj.m_transform, lightSpaceMatrix);
		glCullFace(GL_FRONT);
		LoadedObj.Draw();

		
		

		glCullFace(GL_BACK);

		//Imgui Menu
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
	
		ImGui::ShowDemoWindow();
		{
			
			ImGui::Begin("Menu");
			{
				ImGui::PushStyleColor(ImGuiCol_Border, ImColor(0, 0, 0, 255).Value);
				ImGui::BeginChild("##LeftSide",ImVec2(240,ImGui::GetContentRegionAvail().y),true);
				{
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Scene Objects");
					ImGui::Text("Cube");
					ImGui::SliderFloat("Cube X", LoadedObj.m_transform.xRef, -50.0f, 50.0f);
					ImGui::SliderFloat("Cube Y", LoadedObj.m_transform.yRef, -50.0f, 50.0f);
					ImGui::SliderFloat("Cube Z", LoadedObj.m_transform.zRef, -50.0f, 50.0f);
					ImGui::Text("Sprite Sheet");
					ImGui::SliderFloat("Sprite Sheet X", SpriteSheet.m_transform.xRef, -50.0f, 50.0f);
					ImGui::SliderFloat("Sprite Sheet Y", SpriteSheet.m_transform.yRef, -50.0f, 50.0f);
					ImGui::SliderFloat("Sprite Sheet Z", SpriteSheet.m_transform.zRef, -50.0f, 50.0f);
					ImGui::Text("Platform");
					ImGui::SliderFloat("Platform X", Square1.m_transform.xRef, -50.0f, 50.0f);
					ImGui::SliderFloat("Platform Y", Square1.m_transform.yRef, -50.0f, 50.0f);
					ImGui::SliderFloat("Platform Z", Square1.m_transform.zRef, -50.0f, 50.0f);
					ImGui::Text("Light"); 
					ImGui::SliderFloat("Light X", light->GetTransform().xRef, -50.0f, 50.0f);
					ImGui::SliderFloat("Light Y", light->GetTransform().yRef, -50.0f, 50.0f);
					ImGui::SliderFloat("Light Z", light->GetTransform().zRef, -50.0f, 50.0f);
					ImGui::Text("Second Light");
					ImGui::Checkbox("Enable Second Light(might break shadow)", &enableSecondLight);
					ImGui::SliderFloat("2nd Light X", secondLight->GetTransform().xRef, -50.0f, 50.0f);
					ImGui::SliderFloat("2nd Light Y", secondLight->GetTransform().yRef, -50.0f, 50.0f);
					ImGui::SliderFloat("2nd Light Z", secondLight->GetTransform().zRef, -50.0f, 50.0f);
				}
				ImGui::EndChild();
				
				{
					ImGui::SameLine();
					ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
					ImGui::SameLine();
				}

				ImGui::BeginChild("##RightSide", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
				{
					ImGui::Text("Cameras");
					if (ImGui::SmallButton("Camera Front")) 
					{
						cam.GetTransform().SetPosition(camera1);
						
					}
					if (ImGui::SmallButton("Camera Left"))
					{
						cam.GetTransform().SetPosition(camera2);
						
					}
					if (ImGui::SmallButton("Camera Right"))
					{
						cam.GetTransform().SetPosition(camera3);
						
					}
					if (ImGui::SmallButton("Camera Back"))
					{
						cam.GetTransform().SetPosition(camera4);
						
					}
				}
				ImGui::EndChild();
				ImGui::PopStyleColor();
			}
			ImGui::End();
			
		}


		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			const Uint8* keyState = SDL_GetKeyboardState(NULL);
			if (keyState[SDL_SCANCODE_W])
			{
				cam.GetTransform().SetPosition(cam.GetTransform().GetPosition() + normalize(cam.GetUpVector()));
			}
			if (keyState[SDL_SCANCODE_S])
			{
				cam.GetTransform().SetPosition(cam.GetTransform().GetPosition() - normalize(cam.GetUpVector()));
			}
			if (keyState[SDL_SCANCODE_D])
			{
				cam.GetTransform().SetPosition(cam.GetTransform().GetPosition() - normalize(cam.GetRightVector()));
			}
			if (keyState[SDL_SCANCODE_A])
			{
				cam.GetTransform().SetPosition(cam.GetTransform().GetPosition() + normalize(cam.GetRightVector()));
			}

			if (event.type == SDL_MOUSEWHEEL)
			{
				if (event.wheel.y > 0)
				{
					cam.GetTransform().SetPosition(cam.GetTransform().GetPosition() + normalize(cam.GetForwardVector()));
				}
				else
					if (event.wheel.y < 0)
					{
						cam.GetTransform().SetPosition(cam.GetTransform().GetPosition() - normalize(cam.GetForwardVector()));
					}
			}


			
				
		}
		
		//reset

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glClear(GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		basicShader->Bind();
		
		//Created object
		glActiveTexture(GL_TEXTURE0);
		GLuint TextureLoc = glGetUniformLocation(basicShader->GetProgram(), "texture_diffuse");
		glUniform1i(TextureLoc, 0);
		glBindTexture(GL_TEXTURE_2D, DiffuseTexID); //DiffuseTextureID
		

		glActiveTexture(GL_TEXTURE1);
		TextureLoc = glGetUniformLocation(basicShader->GetProgram(), "texture_normal");
		glUniform1i(TextureLoc, 1);
		glBindTexture(GL_TEXTURE_2D, NormalTexID); //NormalTexID
		basicShader->Update(Square1.m_transform, *light,lightSpaceMatrix);
		if (enableSecondLight)
		basicShader->Update(Square1.m_transform, *secondLight, secondLightSpaceMatrix);

		glActiveTexture(GL_TEXTURE2);
		TextureLoc = glGetUniformLocation(basicShader->GetProgram(), "texture_Shadow");
		glUniform1i(TextureLoc, 2);
		glBindTexture(GL_TEXTURE_2D, shadowMapID); 
	
		SpriteSheet.Draw();
		glCullFace(GL_FRONT);

		Square1.Draw();

		//SpriteSheet.Draw();

		basicShader->Update(SpriteSheet.m_transform, *light, lightSpaceMatrix);
		if (enableSecondLight)
		basicShader->Update(Square1.m_transform, *secondLight, secondLightSpaceMatrix);
		basicShader->Update(LoadedObj.m_transform, *light,lightSpaceMatrix);
		//basicShader->Update(Cube.m_transform, *light, lightSpaceMatrix);
		
		

		LoadedObj.Draw();

		

		glCullFace(GL_BACK);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*	gouraudShader->Bind();
		gouraudShader->Update(Cube.m_transform, *light, lightSpaceMatrix);
		Cube.Draw();*/

		glBindFramebuffer(GL_FRAMEBUFFER, 0);



		light->Draw(&cam);

		if(enableSecondLight)
			secondLight->Draw(&cam);
		
		
		//SDL_Delay(16);
		
		
	
	
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
		
	}


	
	

	//Clean up of all the objects
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();



	return 0;
}