#include "../include/ShaderPipeline.h"

#include <iostream>
#include <GL/glew.h>
#include <assert.h>

using namespace RealTimeRendering;

ShaderPipeline::ShaderPipeline() {
	graphicsID = glCreateProgram();
	if (graphicsID == 0) {
		std::cerr << "Kritischer Fehler: Erstellung des Grafikprogramms fehlgeschlagen." << std::endl;
	} else {
		std::cout << "Grafikprogramm erfolgreich erstellt. ID: " << graphicsID << std::endl;
	}
}
ShaderPipeline::~ShaderPipeline() {
	if (graphicsID != 0) {
		// Delete the program and log a message in German
		glDeleteProgram(graphicsID);
		std::cout << "Grafikprogramm gelöscht. ID: " << graphicsID << std::endl;
	}
}

void ShaderPipeline::integrateModule(ShaderModule shaderModule) {
	GLuint shaderID = shaderModule.getComponentID();  // Retrieve the shader component ID
	if (shaderID) {  // Check if the shader ID is valid
		glAttachShader(graphicsID, shaderID);  // Attach the shader to the pipeline
		std::cout << "Shader erfolgreich an die Grafikpipeline angeschlossen." << std::endl;
	} else {
		std::cerr << "Fehler: Ungültige Shader-ID. Attachment fehlgeschlagen." << std::endl;
	}
}


void ShaderPipeline::completeIntegration() {
	std::cout << "Verknüpfung des Shader-Programms starten..." << std::endl;

	glLinkProgram(graphicsID);
	GLint linkStatus;
	glGetProgramiv(graphicsID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		GLint logLength;
		glGetProgramiv(graphicsID, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* logBuffer = new GLchar[logLength];

		GLsizei actualLength;
		glGetProgramInfoLog(graphicsID, logLength, &actualLength, logBuffer);
		std::cerr << "Fehler: Shader-Modul-Verknüpfung fehlgeschlagen! " << logBuffer << std::endl;
		delete[] logBuffer;
	}
	else {
		std::cout << "Shader-Programm erfolgreich verknüpft." << std::endl;
	}
}

void RealTimeRendering::ShaderPipeline::defineFeedbackVariants(const char** variant) {
	const GLuint count = 3;
	const GLenum bufferMode = GL_INTERLEAVED_ATTRIBS;
	glTransformFeedbackVaryings(graphicsID, count, variant, bufferMode);
}

void ShaderPipeline::engageGraphics() {
	glUseProgram(graphicsID);
}

const GLuint ShaderPipeline::getGraphicsID() const {
	return graphicsID;
}

GLint ShaderPipeline::fetchAttributeSpot(const std::string& name) {
	const char* attributeName = name.c_str();
	GLint location = glGetAttribLocation(graphicsID, attributeName);
	return location;}

void ShaderPipeline::applyUniform(GLint spot, glm::mat3 const& param) {
	glUseProgram(graphicsID);
	const bool transpose = false;
	const GLsizei count = 1;
	const GLfloat* matrixPtr = reinterpret_cast<const GLfloat*>(&param);
	glUniformMatrix3fv(spot, count, transpose, matrixPtr);
}

void ShaderPipeline::applyUniform(GLint spot, glm::mat4 const& param) {
	glUseProgram(graphicsID);
	const bool transpose = false;
	const GLsizei count = 1;
	const GLfloat* matrixPtr = reinterpret_cast<const GLfloat*>(&param);
	glUniformMatrix4fv(spot, count, transpose, matrixPtr);
}

GLint ShaderPipeline::locateUniformSpot(std::string name) {
	const char* uniformName = name.c_str();
	GLint location = glGetUniformLocation(graphicsID, uniformName);
	return location;
}