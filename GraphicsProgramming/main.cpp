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

	//Window Creation
	SDL_Window* window = SDL_CreateWindow("My window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext GLContext = SDL_GL_CreateContext(window);

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
	//Square points
	//float SquareVerticies[]


	
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
	light->GetTransform().SetPosition(vec3(0, 0, -2));


	




	
		Camera cam;
		Shader* basicShader = new Shader( "Shaders/Resources/Basic", cam);
		GLuint DiffuseTextureID=LoadTexture("Textures/brickwall.jpg");
		GLuint NormalTextureID = LoadTexture("Textures/brickwall_normal.jpg");
		Mesh Square1(&SquareVerticies[0], SquareVerticies.size(), &SquareIndecies[0], 6);

		
		//string* AmbiantLoc = new string("");
		//string* DiffuseLoc = new string("");
		//string* SpecLoc = new string("");
		//string* NormalLoc = new string("");

		string AmbiantLoc ;
		string DiffuseLoc ;
		string SpecLoc ;
		string NormalLoc ;

		vector<uint>Indecies;

		vector<Vertex>LoadedVerts = OBJLoader::LoadOBJ("blocks_01.obj","Objects",AmbiantLoc,DiffuseLoc,SpecLoc,NormalLoc,Indecies);
		
		GLuint AmbiantTexID = LoadTexture("Objects/" + AmbiantLoc);
		GLuint DiffuseTexID = LoadTexture("Objects/"+ DiffuseLoc);
		GLuint SpecularTexID = LoadTexture("Objects/"+ SpecLoc);
		GLuint NormalTexID = LoadTexture("Objects/" + NormalLoc);

		Mesh LoadedObj(&LoadedVerts[0], LoadedVerts.size(), &Indecies[0], Indecies.size());

		
		glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
		glViewport(0, 0, 800, 600);



	//Window Loop
	while (true)
	{
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			const Uint8* keyState = SDL_GetKeyboardState(NULL);
			if (keyState[SDL_SCANCODE_W])
			{
				cam.GetTransform().SetPosition(cam.GetTransform().GetPosition() + vec3(0, -0.1, 0));
			}
			if (keyState[SDL_SCANCODE_S])
			{
				cam.GetTransform().SetPosition(cam.GetTransform().GetPosition() + vec3(0, 0.1, 0));
			}
			if (keyState[SDL_SCANCODE_D])
			{
				cam.GetTransform().SetPosition(cam.GetTransform().GetPosition() + vec3(0.1, 0, 0));
			}
			if (keyState[SDL_SCANCODE_A])
			{
				cam.GetTransform().SetPosition(cam.GetTransform().GetPosition() + vec3(-0.1, 0, 0));
			}
		


			
				
		}
		

		basicShader->Bind();
		glActiveTexture(GL_TEXTURE0);
		GLuint TextureLoc = glGetUniformLocation(basicShader->GetProgram(), "texture_diffuse");
		glUniform1i(TextureLoc, 0);
		glBindTexture(GL_TEXTURE_2D, DiffuseTextureID);
		

		glActiveTexture(GL_TEXTURE1);
		TextureLoc = glGetUniformLocation(basicShader->GetProgram(), "texture_normal");
		glUniform1i(TextureLoc, 1);
		glBindTexture(GL_TEXTURE_2D,NormalTextureID);
		basicShader->Update(Square1.m_transform, *light);
		//Square1.Draw();

		LoadedObj.Draw();


		light->Draw(&cam);
		

		
		SDL_Delay(16);
		SDL_GL_SwapWindow(window);
		
	}


	//Clean up of all the objects
	SDL_GL_DeleteContext(GLContext);
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();



	return 0;
}