#pragma once
#include "dependencies.h"

extern const unsigned int SCREEN_WIDTH;
extern const unsigned int SCREEN_HEIGHT;

struct Shader {

public:
	unsigned int ShaderProgrammID;

	//Compile Me A Shader Programm
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {

		std::ifstream VextexShaderFile;
		std::ifstream FragmentShaderFile;

		//ensure ifstream objects can throw exceptions:
		VextexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		FragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		std::string VertexShaderRAW;
		std::string FragmentShaderRAW;

		try {
			//Save Vertex Shader Source //This is just sample code to read from file
			VextexShaderFile.open(vertexShaderPath);
			std::stringstream VertexShaderStream;
			VertexShaderStream << VextexShaderFile.rdbuf();
			VertexShaderRAW = VertexShaderStream.str();
			VextexShaderFile.close();
		}
		catch (std::ifstream::failure e) {
			printf("Coult Not Open Vertex Shader Files\n");
		}
		try {
			//Save Fragment Shader Source //This is just sample code to read from file
			FragmentShaderFile.open(fragmentShaderPath);
			std::stringstream FragmentShaderStream;
			FragmentShaderStream << FragmentShaderFile.rdbuf();
			FragmentShaderRAW = FragmentShaderStream.str();
			FragmentShaderFile.close();
		}
		catch (std::ifstream::failure e) {
			printf("Coult Not Open Fragment Shader Files\n");
		}
		const char* VertexShaderSource = VertexShaderRAW.c_str();
		const char* FragmentShaderSource = FragmentShaderRAW.c_str();

		//Shader Bullshitery //
		unsigned int vertexShader; //Compiling Vertext Shader Bullshit
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &VertexShaderSource, NULL); //Asigning the shader variable to the compiled shader source from the beggining 
		glCompileShader(vertexShader);

		int CompileSuccessFlag; //checking is the shader is compiled


		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &CompileSuccessFlag);
		if (!CompileSuccessFlag) {
			printf("Could Not Compile Vertex Shader\n");
		}

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &FragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &CompileSuccessFlag);
		if (!CompileSuccessFlag) {
			printf("Could Not Compile Fragment Shader\n");
		}

		ShaderProgrammID = glCreateProgram();
		glAttachShader(ShaderProgrammID, vertexShader);
		glAttachShader(ShaderProgrammID, fragmentShader);

		glLinkProgram(ShaderProgrammID);

		glDeleteShader(vertexShader); //Delete the compiled shaders so that only the compiled program of compiled shaders remains
		glDeleteShader(fragmentShader);
		
		//Shader Bullshitery^^ //


		glUseProgram(ShaderProgrammID); //Assign every shader a "perspective" matrix 
		unsigned int CameraProjectionUniformLoc = glGetUniformLocation(ShaderProgrammID, "Projection");
		glm::mat4 DesiredProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(CameraProjectionUniformLoc, 1, GL_FALSE, glm::value_ptr(DesiredProjectionMatrix));
	}

	//Use the Compiled Shader for the code bellow
	void Use() {
		glUseProgram(ShaderProgrammID); //Use The Shitty Shaders, this line is technically useless since we use the shaders in the render loop, also it appears that the glUseProgram is also order based like the glBindBuffer function 
	}

	//Set uniform variable in shader 

	//Don't use glEnableVertexAttribArray(0); to set shader attributes, these function only work with uniforms
	void setBool(const std::string& name, bool value) const {
		glUseProgram(ShaderProgrammID);
		glUniform1i(glGetUniformLocation(ShaderProgrammID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const {
		glUseProgram(ShaderProgrammID);
		glUniform1i(glGetUniformLocation(ShaderProgrammID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const {
		glUseProgram(ShaderProgrammID);
		glUniform1f(glGetUniformLocation(ShaderProgrammID, name.c_str()), value);
	}

	void setMat4(const std::string& name, glm::mat4 value) const {
		glUseProgram(ShaderProgrammID);
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgrammID, name.c_str()),1,GL_FALSE, glm::value_ptr(value));
	}
};
