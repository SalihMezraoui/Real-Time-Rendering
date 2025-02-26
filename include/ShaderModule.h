#pragma once
#ifndef SHADERMODULE_H
#define SHADERMODULE_H

#include <GL/glew.h>
#include <iostream>

#include <fstream>
#include <streambuf>
#include <string>
#include <cerrno>

namespace RealTimeRendering
{
	class ShaderModule {


	public:
		// Der Konstruktor, der den Shader-Typ und den Dateipfad übernimmt
		ShaderModule(GLenum shaderType, const std::string& fileLocation);

		// Destruktor kommt zuerst, löscht die Shader-Komponente
		~ShaderModule();

		// Funktion zum Abrufen der Shader-Komponente-ID
		const GLuint getComponentID() const;
	private:

		GLuint componentID; // Die private Variable, die die Shader-ID speichert
		std::string shaderCodeText;  // Stores the source code of the shader

		// Hilfsfunktion zum Laden des Shader-Codes aus einer Datei
		void loadShaderCodeFromFile(const std::string &fileLocation);

		// Hilfsfunktion zur Shader-Kompilierung
		void compileShaderCode(GLenum shaderType);

		// Hilfsfunktion zur Überprüfung des Kompilierungsstatus
		void checkShaderCompilation();





	};
}

#endif // SHADERMODULE_H
