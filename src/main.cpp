#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include "../include/ShaderModule.h"
#include "../include/ShaderPipeline.h"
#include "../include/DataBuffer.h"
#include "../include/VertexBuffer.h"


using namespace RealTimeRendering;

int windowWidth = 800;
int windowHeight = 600;
int numIterations = 8;
int numVertices = 1000;
int numTriangles = 500;

struct VertexData
{
	glm::vec3 position;
	float length;
	glm::vec3 normal;
	VertexData(float x, float y, float z, float len)
		: position(x, y, z), length(len), normal(0.0f, 1.0f, 0.0f) {}

};

int calculateofTriangles(int numIterations) {
	return 4 * std::pow(3, numIterations) - 3;
}

void calculateofVertices(int numIterations) {
	numTriangles = calculateofTriangles(numIterations);
	numVertices = numTriangles * 3;
}

bool initializeGLFWAndGLEW(int windowWidth, int windowHeight, GLFWwindow*& window) {

	const GLubyte* renderer;
	const GLubyte* version;

	//�berpr�fe ob die Initializierung gelungen ist
	if (!glfwInit()) {
		std::cerr << "Fehler: GLFW Initialisierung fehlgeschlagen!" << std::endl;
		return 1;
	}

	//es wird GL v4.6 verwendet
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, "Prozeduraler Baum", NULL, NULL);

	if (!window) {
		std::cerr << "Fehler: Fenstererstellung fehlgeschlagen!" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Fehler: GLEW INITIALISIERUNG fehlgeschlagen" << std::endl;
		glfwTerminate();
		return false;
	}

	renderer = glGetString(GL_RENDERER);
	version = glGetString(GL_VERSION);
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL Version: " << version << std::endl;

	return true;
}

int main()
{
	//erzeuge das Fenster in dem die Scene gezeichnet wird
	GLFWwindow* window = NULL;


	if (!initializeGLFWAndGLEW(windowWidth, windowHeight, window)) {
		return 1;
	}

	VertexData vert[] =
	{
		VertexData(+0.0f, +0.5f, 0.0f,1.0f),
		VertexData(-0.5f, -0.5f, 0.0f,1.0f),
		VertexData(+0.5f, -0.5f, 0.0f,1.0f)

	};

	// Berechnung der Anzahl der Eckpunkte
	calculateofVertices(numIterations);

	//generiere die Buffer in denen die Dreiecke gespeichert werden
	DataBuffer mybufferTriangle(GL_ARRAY_BUFFER);
	mybufferTriangle.bindVertexBuffer();
	mybufferTriangle.bufferUploadData(sizeof(vert) * numVertices, nullptr);
	mybufferTriangle.bufferSubData(sizeof(vert), vert);
	//Buffer f�r das Transformfeedback
	DataBuffer mybufferFeedback(GL_ARRAY_BUFFER);
	mybufferFeedback.bindVertexBuffer();
	mybufferFeedback.bufferDownloadData(sizeof(vert) * numVertices, nullptr);

	//shader f�r das Transformfeedback
	ShaderModule geoVertexShader(GL_VERTEX_SHADER, "../shaders/TreeVertexShader.vert");
	ShaderModule geoGeometyShader(GL_GEOMETRY_SHADER, "../shaders/TreeGeometryShader.geo");
	ShaderPipeline geometryProgram;
	geometryProgram.integrateModule(geoVertexShader);
	geometryProgram.integrateModule(geoGeometyShader);
	const GLchar* varyings[] = { "out_position","out_length","out_normal" };
	geometryProgram.defineFeedbackVariants(varyings);
	geometryProgram.completeIntegration();

	//shader zum zeichnen des fertigen Baums
	ShaderModule renderVertexShader(GL_VERTEX_SHADER, "../shaders/VertexShader.vert");
	ShaderModule renderFragmentShader(GL_FRAGMENT_SHADER, "../shaders/FragmentShader.frag");
	ShaderPipeline renderProgram;
	renderProgram.integrateModule(renderVertexShader);
	renderProgram.integrateModule(renderFragmentShader);
	renderProgram.completeIntegration();

	/* create a vertex array object */
	VertexBuffer vertexArr;
	vertexArr.bind();
	vertexArr.attachBuffer(mybufferTriangle.getVertexBuffer());
	GLint pos = geometryProgram.fetchAttributeSpot("position");
	vertexArr.enableAttribute(pos);
	vertexArr.setAttributePointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);
	GLint length = geometryProgram.fetchAttributeSpot("length");
	vertexArr.enableAttribute(length);
	vertexArr.setAttributePointer(length, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(sizeof(float) * 3));

	GLint normal = geometryProgram.fetchAttributeSpot("normal");
	vertexArr.enableAttribute(normal);
	vertexArr.setAttributePointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(sizeof(float) * 4));

	VertexBuffer renderVertex;
	renderVertex.bind();
	renderVertex.attachBuffer(mybufferFeedback.getVertexBuffer());
	GLint renderpos = renderProgram.fetchAttributeSpot("position");
	renderVertex.enableAttribute(renderpos);
	renderVertex.setAttributePointer(renderpos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);
	GLint renderlength = renderProgram.fetchAttributeSpot("length");
	renderVertex.enableAttribute(renderlength);
	renderVertex.setAttributePointer(renderlength, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(sizeof(float) * 3));
	GLint rendernormal = renderProgram.fetchAttributeSpot("normal");
	renderVertex.enableAttribute(rendernormal);
	renderVertex.setAttributePointer(rendernormal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(sizeof(float) * 4));

	//Setzen eines Zeigers auf das vertexArr und auf den Buffer um ihn sp�ter auszutauschen
	std::cout << &vertexArr << std::endl;
	VertexBuffer* currentVertexArray;
	currentVertexArray = &vertexArr;
	std::cout << &mybufferFeedback << std::endl;
	DataBuffer* currentFeedbackbuffer = &mybufferFeedback;

	std::cout << &renderVertex << std::endl;
	VertexBuffer* lastVertexArray = &renderVertex;
	std::cout << &mybufferTriangle << std::endl;
	DataBuffer* lastFeedbackbuffer = &mybufferTriangle;

	//Erstellung des Baumes
	for (int i = 0; i < numIterations; ++i)
	{
		currentVertexArray->bind();
		geometryProgram.engageGraphics();
		currentFeedbackbuffer->bindVertexBuffer();
		glEnable(GL_RASTERIZER_DISCARD);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, currentFeedbackbuffer->getVertexBuffer());
		glBeginTransformFeedback(GL_TRIANGLES);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
		glEndTransformFeedback();
		glDisable(GL_RASTERIZER_DISCARD);
		glFlush();

		// Nach dem Tausch wird das VertexArray mit neuen Eckpunkten gef�llt.
		VertexBuffer* swapVertexArray = currentVertexArray;
		currentVertexArray = lastVertexArray;
		lastVertexArray = swapVertexArray;

		//Buffer zum tauschen
		DataBuffer* swapFeedbackBuffer = currentFeedbackbuffer;
		currentFeedbackbuffer = lastFeedbackbuffer;
		lastFeedbackbuffer = swapFeedbackBuffer;

	}

	// Hintergrundfarbe
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 camaraPos(00, 00, 20);

	float rotationX = 0.0f;
	float rotationY = 0.0f;
	float aspect = (float)windowWidth / (float)windowHeight;
	while (!glfwWindowShouldClose(window))
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		renderVertex.bind();
		renderProgram.engageGraphics();

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
		glfwPollEvents();

		glfwSwapBuffers(window);
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, 1);
		if (glfwGetKey(window, GLFW_KEY_A))
			camaraPos.z += -10.0f * elapsed_seconds;
		if (glfwGetKey(window, GLFW_KEY_D))
			camaraPos.z += 10.0f * elapsed_seconds;
		if (glfwGetKey(window, GLFW_KEY_W))
			rotationX += 10.f * elapsed_seconds;
		if (glfwGetKey(window, GLFW_KEY_S))
			rotationX -= 10.f * elapsed_seconds;

		// modelmatrix
		glm::mat4 tree = glm::mat4(1.0f);
		tree = glm::rotate(tree, rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
		tree = glm::rotate(tree, rotationY, glm::vec3(0.0f, 1.0f, 0.0f));

		//viewmatrix
		glm::mat4 view = glm::lookAt(camaraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//perspektive
		glm::mat4 proj = glm::perspective(50.0f, aspect, 0.1f, 100.0f);

		//setunifrom�s matrix
		std::string s = "";
		GLint treei = renderProgram.locateUniformSpot("model");
		renderProgram.applyUniform("model", tree);
		GLint viewi = renderProgram.locateUniformSpot("view");
		renderProgram.applyUniform("view", view);
		GLint proji = renderProgram.locateUniformSpot("proj");
		renderProgram.applyUniform("proj", proj);

	}

	glfwTerminate();
	return 0;
}

