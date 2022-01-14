#define GLEW_STATIC

#include <glew.h>
#include <SDL_opengl.h>
#include<glm.hpp>
#include<gtc/type_ptr.hpp>
#include<SDL.h>
#include "mesh.h"
#include "Shader.h"





#undef main SDL_main
using namespace std;






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

	//Triangle points
	float Verticies[]{
		0.0f,0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		-0.5f,-0.5f,0.0f };

	//Creating a buffer and passing the data of our points into it
	GLuint VertexBufferObject = 0;
	glGenBuffers(1, &VertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), Verticies, GL_STATIC_DRAW);
	//Create an array of vertixes 
	GLuint VertexArrayObject = 0;
	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);
	glEnableVertexAttribArray(0);
	//Bind the buffer and Vertex array together and tell it how to read the data 
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);


		Mesh Tri1(Verticies, 3);
		Camera cam;
		Shader* basicShader = new Shader( "Shaders/Resources/Basic", cam);
		
		

		



		
		glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
		glViewport(0, 0, 800, 600);



	//Window Loop
	while (true)
	{
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(VertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, 3);



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
		basicShader->Update(Tri1.m_transform);
		Tri1.Draw();

		
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