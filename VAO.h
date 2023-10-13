#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include <string>
#include <vector>

using namespace std;

/* This class encapsulates VAOs to streamline rendering */
class MyVAO {
    unsigned int VAO, VBO;

    unsigned int stride = 0; // the stride between each vertex in the VBO
    unsigned int numVertices = 0; 

public:
    MyVAO() {
        /* Gen the VAO and VBO */
        glGenVertexArrays(1, &VAO); // create the VAO
        glGenBuffers(1, &VBO); // create the VBO

        glBindVertexArray(VAO); // bind the VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind VBO to the VAO

        glBindVertexArray(0); // unbind the VAO
    }
    void addAttrib(unsigned int attribSizes[], unsigned int numAttributes) {
        /* Attributes are added one at a time with IDs in {1, ..} */
        /* attribSize is the size of the attributes (eg. 3 for a vec3) */
        /* Attributes may only be added after data */
        
        /* First bind the VAO and VBO to configure attributes */
        glBindVertexArray(VAO); 

        unsigned int startIndex = 0; // index from which the attribute starts
        for (int id = 0; id < numAttributes; id++) {
            unsigned int attribSize = attribSizes[id];

            glVertexAttribPointer(id, attribSize, GL_FLOAT, GL_FALSE, stride, (void*)startIndex);
            glEnableVertexAttribArray(id);

            startIndex += attribSize * sizeof(float);
        }
        
        /* Unbind the VAO */
        glBindVertexArray(0);
    }
    void addData(float vertices[], unsigned int numVerticesIn, unsigned int strideIn) {
        glBindVertexArray(VAO); // bind the VAO

        numVertices = numVerticesIn;
        stride = strideIn;

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * stride * numVertices, &vertices[0], GL_STATIC_DRAW);

        glBindVertexArray(0); // unbind the VAO
    }
    void draw() {
        glBindVertexArray(VAO); // bind the VAO
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
        glBindVertexArray(0); // unbind the VAO
    }
    void del() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

#endif