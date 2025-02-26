#ifndef DATABUFFER_H
#define DATABUFFER_H

#include <GL/glew.h>

namespace RealTimeRendering {
    class DataBuffer
    {
    public:
        // Konstruktor und Destruktor
        DataBuffer(GLenum bufferTarget);
        ~DataBuffer();

        // Funktionen zum Binden und Entbinden
        void bindVertexBuffer();
        void unbindVertexBuffer();

        // Funktionen zum Verwalten von Pufferdaten
        void bufferUploadData(GLsizeiptr dataSize, const GLvoid* bufferData);
        void bufferDownloadData(GLsizeiptr dataSize, const GLvoid* bufferData);
        void bufferSubData(GLsizeiptr dataSize, const GLvoid* newData);

        // Funktionen zum Setzen von Attributen
        void activateVertexAttributes(GLint attributeIndex);
        void configureVertexAttributes(GLint attributeIndex, GLint attributeSize, GLsizei strideLength, const GLvoid* attributePointer);

        // Hilfsfunktionen für das Speichern und Abrufen von Pufferdaten
        void storeBufferData(GLsizeiptr dataSize, const GLvoid* bufferData);
        void retrieveBufferData(GLsizeiptr dataSize, const GLvoid* bufferData);

        // Getter-Funktion
        GLuint getVertexBuffer();

    private:
        // Private Mitglieder (nun am Ende für eine andere Struktur)
        GLuint m_vertexBuffer;
        GLenum m_bufferTarget;
    };

}

#endif // DATABUFFER_H
