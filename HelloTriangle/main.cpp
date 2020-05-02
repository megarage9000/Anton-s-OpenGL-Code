#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// Setting up Visual Studio for OpenGL:
// https://www.wikihow.com/Set-Up-OpenGL-GLFW-GLEW-GLM-on-a-Project-with-Visual-Studio

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return -1;
	}

	// ---- Creating GLFW window ----
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// ---- Creating GLEW extension handler ----
	glewExperimental = GL_TRUE;
	glewInit();

	// ---- Version Information ----
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported: %s\n", version);

	// Tells GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth testing
	glDepthFunc(GL_LESS); // depth-interprets a smaller value as "closer"

	glfwTerminate();
	return 0;

}