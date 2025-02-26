#ifndef SHADERPIPELINE_H
#define SHADERPIPELINE_H


#include "ShaderModule.h"
#include <iostream>
#include <vector>
#include "glm/glm.hpp"


namespace RealTimeRendering
{
	class ShaderPipeline
	{
	public:
		ShaderPipeline();
		~ShaderPipeline();

		// Uniform application methods first to change the flow
		void applyUniform(GLint spot, glm::mat3 const& param);
		void applyUniform(GLint spot, glm::mat4 const& param);
		template <class T>
		void applyUniform(std::string name, const T& t) {
			GLint uniformSpot = locateUniformSpot(name);
			if (uniformSpot != -1) { // Check if the location is valid
				setUniformValue(uniformSpot, t);
			} else {
				std::cerr << "Error: Uniform '" << name << "' nicht gefunden." << std::endl;
			}
		}

		// Integration and graphics engagement grouped together
		void integrateModule(ShaderModule shaderModule);
		void engageGraphics();

		// Retrieval and definition methods
		const GLuint getGraphicsID() const;
		GLint fetchAttributeSpot(const std::string& name);
		GLint locateUniformSpot(std::string name);

		// Finalization methods at the end
		void completeIntegration();
		void defineFeedbackVariants(const char** variant);

	private:
		GLuint graphicsID;
		template <class T>
		void setUniformValue(GLint spot, const T& input) {
			applyUniform(spot, input);
		}
	};

}



#endif