#ifndef MAIN_CPP
#define MAIN_CPP

#include "shader.h" // glad is included here
#include "VAO.h"
#include "stb_image_implementation.h" // for importing images
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

/* Callback function for when the user resizes the window */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

/* Process the user input (non-callback) */
void processInput(GLFWwindow *window) {
    /* Close the window when we press esc */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);
}

/* Init GLFW and Glad */
void init() {
    /* ---------------------------- Init GLFW & GLAD ---------------------------- */

    /* Initialize the library */
    if (!glfwInit()) return;

    /* This configures GLFW. The first arg is the option, the segment number is our selection */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Using the core profile means we get access to fewer features. 
    Mac OS X needs this statement for backwards compatability */
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   
}

/* Build the display window */
GLFWwindow* buildWindow() {
    GLFWwindow* window;

    const int startWidth = 600, startHeight = 400;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(startWidth, startHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Inform OpenGL of are callback function to change the window size */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    /* Initialise GLAD so we can access OpenGL functions (must be done here) */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return nullptr;
    }

    return window;
}

float* genSineCurve(unsigned int points, unsigned int &vertices) {
    // returns a set of triangle vertices that span the area under a sine curve
    const int rectangles = points  - 1;
    const int triangles = rectangles * 2;
    vertices = triangles * 3;
    
    const float x_width = 3.0f;

    const int attributes = 3; 
    const int x_stretch = 15; // factor we stretch the x-axis

    float * vertexArray = new float[vertices * attributes];

    for (int i = 0; i < rectangles; i ++) {
        // our rectangle goes from (x_1, -1), (x_2, -1), (x_1, y_1), (x_2, y_2)
        float x_1 = -x_width + 2.0f * x_width * i / points;
        float x_2 = -x_width + 2.0f * x_width * (i + 1) / points;
        float y_1 = sin(x_1 * x_stretch) / 5, y_2 = sin(x_2 * x_stretch) / 5;

        // (x_1, -1), (x_2, -1), (x_1, y_1)
        vertexArray[18*i] = x_1;
        vertexArray[18*i + 1] = y_1;
        vertexArray[18*i + 2] = 0.0f;

        vertexArray[18*i + 3] = x_1;
        vertexArray[18*i + 4] = -1.0f;
        vertexArray[18*i + 5] = 0.0f;

        vertexArray[18*i + 6] = x_2;
        vertexArray[18*i + 7] = -1.0f;
        vertexArray[18*i + 8] = 0.0f;

        // (x_2, y_2), (x_1, y_1), (x_2, -1)
        vertexArray[18*i + 9] = x_2;
        vertexArray[18*i + 10] = y_2;
        vertexArray[18*i + 11] = 0.0f;

        vertexArray[18*i + 12] = x_1;
        vertexArray[18*i + 13] = y_1;
        vertexArray[18*i + 14] = 0.0f;
        
        vertexArray[18*i + 15] = x_2;
        vertexArray[18*i + 16] = -1.0f;
        vertexArray[18*i + 17] = 0.0f;
    }

    return vertexArray;

}

int main()
{
    init(); // init glfw

    GLFWwindow* window = buildWindow(); // build the window

    glEnable(GL_DEPTH_TEST); // enable depth testing

    Shader myShader("shaders/vertexShader.txt", "shaders/fragmentShader.txt"); // build the shader program

    /* Load the sine curve into a VAO */
    unsigned int samplePoints = 100000;
    unsigned int numVertices;
    float * sineCurve = genSineCurve(samplePoints, numVertices);
    
    unsigned int attributes[] = {3};
    unsigned int stride = 3 * sizeof(float);
    MyVAO myVao;
    myVao.addData(sineCurve, numVertices, stride);
    myVao.addAttrib(attributes, 1);

    /* Define attributes for each sine curve */
    struct Curve {
        float colour = 0.0f;
        float colorStep = 0.01f;
        float speed = 0.01f;

        glm::vec3 trans;
        glm::vec3 transStep;

        Curve (float pos) { 
            speed = pos / 500;
            transStep = glm::vec3(speed, speed, speed);
            trans = glm::vec3(0.0f, -1 + 2.0f * pos, 1.0f * pos);
        }

        void step() {
            // update the step
            if (((trans.x > 1) && (transStep.x > 0)) || ((trans.x < -1) && (transStep.x < 0))) {
                transStep.x = -transStep.x;
            } 

            // make the step
            trans.x += transStep.x;
        }
    };
    const int numCurves = 9;
    vector<Curve> curves;
    for (int i = 0; i < numCurves; i ++) {
        Curve c((float)i / numCurves);

        curves.emplace_back(c);
    }

    /* ---------------------------- Render Loop ---------------------------- */
    while (!glfwWindowShouldClose(window))
    {
        /* Handle user input */
        processInput(window);

        /* Clear the colour buffer with dark turqoise */
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // (state setting)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // (state using)

        /* Create the transformation matrix */
        myShader.use();
        for (Curve &curve: curves) {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), curve.trans);

            myShader.setMat4("transform", transform);
            myShader.setFloat("color", curve.colour);

            curve.step();

            myVao.draw();
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* De-allocate memory */
    myVao.del();
    myShader.del();

    /* Terminate glfw */
    glfwTerminate();
    return 0;
};

#endif