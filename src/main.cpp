#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "utility/shader.hpp"
#include "controls.hpp"

int main() 
{
    // Initialize GLFW
    if( !glfwInit() )
    {
        fprintf(stderr, "Failed to initialize GLFW!\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "DrawingTriangles", NULL, NULL);
    if(window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window! If you have an Intel GPU, they are not 3.3 compatible.\n");
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize OpenGL context!\n");
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwSwapInterval(0);

    glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = load_shaders_from_file("../shader/SimpleVertexShader.vert", "../shader/SimpleFragmentShader.frag");

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    glm::mat4 View = glm::lookAt
    (
        glm::vec3(3, 3, -3),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );

    glm::mat4 Model = glm::mat4(1.0f);

    glm::mat4 MVP = Projection * View * Model;

    static const GLfloat g_vertex_buffer_data[] = { 
		-1.0/2.0,-1.0/2.0,-1.0/2.0,
		-1.0/2.0,-1.0/2.0, 1.0/2.0,
		-1.0/2.0, 1.0/2.0, 1.0/2.0,
		 1.0/2.0, 1.0/2.0,-1.0/2.0,
		-1.0/2.0,-1.0/2.0,-1.0/2.0,
		-1.0/2.0, 1.0/2.0,-1.0/2.0,
		 1.0/2.0,-1.0/2.0, 1.0/2.0,
		-1.0/2.0,-1.0/2.0,-1.0/2.0,
		 1.0/2.0,-1.0/2.0,-1.0/2.0,
		 1.0/2.0, 1.0/2.0,-1.0/2.0,
		 1.0/2.0,-1.0/2.0,-1.0/2.0,
		-1.0/2.0,-1.0/2.0,-1.0/2.0,
		-1.0/2.0,-1.0/2.0,-1.0/2.0,
		-1.0/2.0, 1.0/2.0, 1.0/2.0,
		-1.0/2.0, 1.0/2.0,-1.0/2.0,
		 1.0/2.0,-1.0/2.0, 1.0/2.0,
		-1.0/2.0,-1.0/2.0, 1.0/2.0,
		-1.0/2.0,-1.0/2.0,-1.0/2.0,
		-1.0/2.0, 1.0/2.0, 1.0/2.0,
		-1.0/2.0,-1.0/2.0, 1.0/2.0,
		 1.0/2.0,-1.0/2.0, 1.0/2.0,
		 1.0/2.0, 1.0/2.0, 1.0/2.0,
		 1.0/2.0,-1.0/2.0,-1.0/2.0,
		 1.0/2.0, 1.0/2.0,-1.0/2.0,
		 1.0/2.0,-1.0/2.0,-1.0/2.0,
		 1.0/2.0, 1.0/2.0, 1.0/2.0,
		 1.0/2.0,-1.0/2.0, 1.0/2.0,
		 1.0/2.0, 1.0/2.0, 1.0/2.0,
		 1.0/2.0, 1.0/2.0,-1.0/2.0,
		-1.0/2.0, 1.0/2.0,-1.0/2.0,
		 1.0/2.0, 1.0/2.0, 1.0/2.0,
		-1.0/2.0, 1.0/2.0,-1.0/2.0,
		-1.0/2.0, 1.0/2.0, 1.0/2.0,
		 1.0/2.0, 1.0/2.0, 1.0/2.0,
		-1.0/2.0, 1.0/2.0, 1.0/2.0,
		 1.0/2.0,-1.0/2.0, 1.0/2.0
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = { 
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        computeMatricesFromInputs();
        glm::mat4 projection_matrix = getProjectionMatrix();
        glm::mat4 view_matrix = getViewMatrix();
        glm::mat4 model_matrix = glm::mat4(1.0);
        glm::mat4 MVP = projection_matrix * view_matrix * model_matrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer
        (
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer
        (
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0);
}