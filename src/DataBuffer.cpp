#include "../include/DataBuffer.h"

#include <iostream>

using namespace RealTimeRendering;

// Benutzerdefinierte Konstanten für OpenGL-Werte
#define MY_BUFFER_USAGE_DRAW GL_STATIC_DRAW
#define MY_BUFFER_USAGE_READ GL_STATIC_READ
#define MY_FLOAT_TYPE GL_FLOAT
#define MY_FALSE GL_FALSE

// Konstruktor für DataBuffer, initialisiert den Puffer mit dem angegebenen Zieltyp
DataBuffer::DataBuffer(GLenum bufferTarget) : m_bufferTarget(bufferTarget) {
	glGenBuffers(1, (GLuint*)&m_vertexBuffer);
}

// Destruktor für DataBuffer, löscht den OpenGL-Puffer
DataBuffer::~DataBuffer() {
	glDeleteBuffers(1, (GLuint*)&m_vertexBuffer);
}

// Gibt die ID des Vertex-Puffers zurück
GLuint DataBuffer::getVertexBuffer() {
	return m_vertexBuffer;
}

// Bindet den Vertex-Puffer an das angegebene Ziel
void DataBuffer::bindVertexBuffer() {
	glBindBuffer(m_bufferTarget, m_vertexBuffer);
}

// Trennt den Vertex-Puffer
void DataBuffer::unbindVertexBuffer() {
	glBindBuffer(m_bufferTarget, 0);
}

// Lädt neue Daten in den Puffer
void DataBuffer::bufferSubData(GLsizeiptr dataSize, const GLvoid* newData) {
	glBufferSubData(m_bufferTarget, 0, dataSize, newData);
}

// Lädt Daten in den Puffer und gibt an, wie der Puffer verwendet wird
void DataBuffer::bufferUploadData(GLsizeiptr dataSize, const GLvoid* bufferData) {
	std::cout << "Hochladen von Pufferdaten der Größe: " << dataSize << " zum Puffer." << std::endl;
	glBufferData(m_bufferTarget, dataSize, bufferData, MY_BUFFER_USAGE_DRAW);
}

// Liest Daten aus dem Puffer
void DataBuffer::bufferDownloadData(GLsizeiptr dataSize, const GLvoid* bufferData) {
	glBufferData(m_bufferTarget, dataSize, bufferData, MY_BUFFER_USAGE_READ);
}

// Aktiviert das Vertex-Attribut für das Rendern
void DataBuffer::activateVertexAttributes(GLint attributeIndex) {
	glEnableVertexAttribArray(attributeIndex);
}

// Konfiguriert das Vertex-Attribut im Puffer
void DataBuffer::configureVertexAttributes(GLint attributeIndex, GLint attributeSize, GLsizei strideLength, const GLvoid* attributePointer) {
	glVertexAttribPointer(attributeIndex, attributeSize, MY_FLOAT_TYPE, MY_FALSE, strideLength, attributePointer);
}

// Hilfsfunktionen für das Speichern und Abrufen von Pufferdaten
void DataBuffer::storeBufferData(GLsizeiptr dataSize, const GLvoid* bufferData) {
	glBufferData(m_bufferTarget, dataSize, bufferData, MY_BUFFER_USAGE_DRAW);
}

void DataBuffer::retrieveBufferData(GLsizeiptr dataSize, const GLvoid* bufferData) {
	glBufferData(m_bufferTarget, dataSize, bufferData, MY_BUFFER_USAGE_READ);
}