#include "../include/ShaderModule.h"

using namespace RealTimeRendering;

namespace RealTimeRendering {
    // Hilfsfunktion zum Laden des Shader-Codes aus einer Datei
    void ShaderModule::loadShaderCodeFromFile(const std::string& fileLocation) {
        std::ifstream shaderInput(fileLocation);
        if (!shaderInput) {
            std::cerr << "Achtung: Zugriff auf Shader-Datei fehlgeschlagen! " << fileLocation << std::endl;
            componentID = 0;
            return;
        }

        shaderCodeText = std::string((std::istreambuf_iterator<char>(shaderInput)), std::istreambuf_iterator<char>());
    }

    // Hilfsfunktion zur Shader-Kompilierung
    void ShaderModule::compileShaderCode(GLenum shaderType) {
        componentID = glCreateShader(shaderType);
        const char* shaderSourcePointer = shaderCodeText.c_str();
        glShaderSource(componentID, 1, &shaderSourcePointer, NULL);
        glCompileShader(componentID);
    }

    // Hilfsfunktion zur Überprüfung des Kompilierungsstatus
    void ShaderModule::checkShaderCompilation() {
        GLint buildStatus;
        glGetShaderiv(componentID, GL_COMPILE_STATUS, &buildStatus);
        if (buildStatus == GL_FALSE) {
            char errorLog[1024];
            glGetShaderInfoLog(componentID, 1024, NULL, errorLog);
            std::cerr << "FEHLER: Fehler bei der Shader-Erstellung: " << errorLog << std::endl;
            glDeleteShader(componentID);
            componentID = 0;
        }
    }

    // Konstruktor für ShaderModule, lädt den Shadercode und kompiliert ihn
    ShaderModule::ShaderModule(GLenum shaderType, const std::string& fileLocation) : componentID(0) {
        loadShaderCodeFromFile(fileLocation);
        compileShaderCode(shaderType);
        checkShaderCompilation();
    }

    // Destruktor
    ShaderModule::~ShaderModule() {
        if (componentID != 0) {
            glDeleteShader(componentID);
        }
    }

    // Gibt die ID des Shader-Moduls zurück
    const GLuint ShaderModule::getComponentID() const {
        return componentID;
    }
}
