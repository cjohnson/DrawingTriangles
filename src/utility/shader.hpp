#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>

/**
 * @brief                 Transcribes a file from file memory to a std::string object
 * @throws                std::runtime_exception 
 * 
 * @param input_file_path File path in file memory
 * @return std::string    File contents in a string object
 */
std::string transcribe_file(const char* input_file_path);

/**
 * @brief             Compiles a shader from the string text formats to the graphics pipeline format.
 * @throws            std::runtime_exception
 * 
 * @param shader_id   The OpenGL-issued id of the shader
 * @param shader_code The string text format of the shader
 */
void compile_shader(GLuint* shader_id, char const* shader_code);

/**
 * @brief                    Load and compile shaders from a text file into a graphics pipeline useful format.
 * 
 * @param vertex_file_path   Path to the vertex shader file
 * @param fragment_file_path Path to the fragment shader file
 * @return GLuint            Handle of the resulting graphics pipeline shader
 */
GLuint load_shaders_from_file(const char* vertex_file_path, const char* fragment_file_path)
{
    // Create the shader reference objects
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Create the transcribed string objects
    std::string vertex_shader_code = transcribe_file(vertex_file_path);
    std::string fragment_shader_code = transcribe_file(fragment_file_path);

    GLint result = GL_FALSE;
    int error_log_length;

    // Compile Shaders
    compile_shader(&vertex_shader_id, vertex_shader_code.c_str());
    compile_shader(&fragment_shader_id, fragment_shader_code.c_str());

	// Link the program
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);

	// Check the program
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &error_log_length);
	if (error_log_length > 0){
		std::vector<char> program_error_message(error_log_length+1);
		glGetProgramInfoLog(program_id, error_log_length, NULL, &program_error_message[0]);
		throw std::runtime_error("Failed to compile shader program: " + std::string(&program_error_message[0]));
	}
	
	glDetachShader(program_id, vertex_shader_id);
	glDetachShader(program_id, fragment_shader_id);
	
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return program_id;
}

#define TRANSCRIBE_FILE_ERROR_STRING "Error: failed to transcribe file. Are you using the correct target directory?"
std::string transcribe_file(const char* input_file_path) 
{
    // Resource declaration
    std::string file_contents;
    std::ifstream input_file_stream(input_file_path, std::ios::in);

    // File read error checking
    if(!input_file_stream.is_open())
    {
        input_file_stream.close();
        throw std::runtime_error(TRANSCRIBE_FILE_ERROR_STRING);
    }

    // Populate the stringstream with results from the filestream.
    std::stringstream sstr;
    sstr << input_file_stream.rdbuf();

    // Close open resources (stream)
    input_file_stream.close();

    return sstr.str();
}

void compile_shader(GLuint* shader_id, char const* shader_code)
{
    // Compile the text format shader to the graphics pipeline shader.
    glShaderSource(*shader_id, 1, &shader_code, NULL);
    glCompileShader(*shader_id);

    // Read the result and status of the compilation process
    GLint result;
    int error_log_length;
    glGetShaderiv(*shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(*shader_id, GL_INFO_LOG_LENGTH, &error_log_length);

    // Handle compilation errors
    if(error_log_length > 0)
    {
        std::vector<char> shader_error_message(error_log_length+1);
        glGetShaderInfoLog(*shader_id, error_log_length, NULL, &shader_error_message[0]);
        throw std::runtime_error("Failed to compile shader: " + std::string(&shader_error_message[0]));
    }
}