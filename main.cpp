#include <iostream>
#include "GL/gl3w.h"
#include <GLFW/glfw3.h>
#include "Application.h"

#ifdef MAC
#define DPI_SCALE 2
#else
#define DPI_SCALE 1
#endif

#define VIEWPORT_INIT_WIDTH 640
#define VIEWPORT_INIT_HEIGHT 480

#define TARGET_FRAMERATE 60.0f

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		Application::instance().keyPressed(key);
	else if (action == GLFW_RELEASE)
		Application::instance().keyReleased(key);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Application::instance().mouseMove(int(xpos), int(ypos));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	int buttonId;
	
	switch(button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		buttonId = 0;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		buttonId = 1;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		buttonId = 2;
		break;
	}

	if(action == GLFW_PRESS)
		Application::instance().mousePress(buttonId);
	else if(action == GLFW_RELEASE)
		Application::instance().mouseRelease(buttonId);

}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    // EDIT: Multiply by 2 to to support macbook screen
	Application::instance().resize(width * DPI_SCALE, height * DPI_SCALE);
}

void render()
{
	Application::instance().render();
}


int main(int argc, char *argv[])
{
	GLFWwindow* window;
	double timePerFrame = 1.f / TARGET_FRAMERATE, timePreviousFrame, currentTime;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

    /* EDIT: Additions for mac support */
#ifdef MAC
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    /* End additions */

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(VIEWPORT_INIT_WIDTH * 2, VIEWPORT_INIT_HEIGHT * 2, "SRGGE", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}

	/* Set window initial position */
	glfwSetWindowPos(window, 100, 100);
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	/* Set callbacks */
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	// GL3W will take care of OpenGL extension functions
	gl3wInit();

	/* Init step of the game loop */
	Application::instance().init(window);

    //EDIT: Multiply by 2 to support macbook screen
	Application::instance().resize(VIEWPORT_INIT_WIDTH * DPI_SCALE, VIEWPORT_INIT_HEIGHT * DPI_SCALE);

	if(argc > 1)
	  Application::instance().loadMesh(argv[1]);

	timePreviousFrame = glfwGetTime();
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		if (currentTime - timePreviousFrame >= timePerFrame)
		{

			// cout << "FPS: " << (1.0 / (currentTime - timePreviousFrame)) << endl;

			/* Update & render steps of the game loop */
			if(!Application::instance().update(int(1000.0f * (currentTime - timePreviousFrame))))
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			render();
			timePreviousFrame = currentTime;
			
			/* Swap front and back buffers */
			glfwSwapBuffers(window);
		}

		/* Poll for and process events */
		glfwPollEvents();
	}

	// Shutdown GLFW
	glfwTerminate();

	return 0;
}





