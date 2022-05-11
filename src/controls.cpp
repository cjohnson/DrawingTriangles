#include <iostream>

#include <GLFW/glfw3.h>
extern GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 view_matrix;
glm::mat4 projection_matrix;

glm::mat4 getViewMatrix()
{
    return view_matrix;
}

glm::mat4 getProjectionMatrix()
{
    return projection_matrix;
}

glm::vec3 position = glm::vec3(0, 0, 5);
float horizontalAngle = 3.14f;
float verticleAngle = 0.0f;
float initialFOV = 70.0f;

float speed = 8.0f;
float mouseSpeed = 0.002f;

double times = 0;
double frames = 0;
void computeMatricesFromInputs()
{
    static double lastTime = glfwGetTime();

    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    
    times += deltaTime;
    frames++;
    if(times >= 1.0) {    
        frames = 0;
        times = 0;
    }
    std::cout << "FPS:" << frames / times << std::endl;
    std::cout << "[" << position[0] << ", " << position[1] << ", " << position[2] << "], " << horizontalAngle << ", " << verticleAngle << std::endl;
    // screen clear
    std::cout << "\033[2J\033[1;1H";

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    glfwSetCursorPos(window, 1024/2, 768/2);

    horizontalAngle += mouseSpeed * float(1024/2 - xpos);
    verticleAngle   += mouseSpeed * float( 768/2 - ypos);

    if(verticleAngle >= M_PI / 2.0)
        verticleAngle = M_PI / 2.0;
    if(verticleAngle <= -M_PI / 2.0)
        verticleAngle = -M_PI / 2.0;

    glm::vec3 direction
    (
        cos(verticleAngle) * sin(horizontalAngle),
        sin(verticleAngle),
        cos(verticleAngle) * cos(horizontalAngle)
    );

    glm::vec3 right = glm::vec3
    (
        sin(horizontalAngle - 3.14f/2.0f),
        0,
        cos(horizontalAngle - 3.14f/2.0f)
    );

    glm::vec3 up = glm::cross(right, direction);

    // Move forward
    if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		position[0] = position[0] + sin(horizontalAngle) * (deltaTime * speed);
        position[2] = position[2] + cos(horizontalAngle) * (deltaTime * speed);
	}
    // Up
    if (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS){
		position[1] = position[1] + deltaTime * speed;
	}
    // Down
    if (glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS){
		position[1] = position[1] - deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		position[0] = position[0] - sin(horizontalAngle) * (deltaTime * speed);
        position[2] = position[2] - cos(horizontalAngle) * (deltaTime * speed);
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	float FoV = initialFOV;

    projection_matrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    view_matrix = glm::lookAt
    (
        position,
        position + direction,
        up
    );

    lastTime = currentTime;
}