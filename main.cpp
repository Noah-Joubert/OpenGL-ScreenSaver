#ifndef MAIN_CPP
#define MAIN_CPP

#include "shader.h" // glad is included here
#include "stb_image_implementation.h" // for importing images
#include <GLFW/glfw3.h>
#include <iostream>

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

int main()
{
    init(); // init glfw

    /* ---------------------------- Create the window ---------------------------- */
    GLFWwindow* window;
    const int startWidth = 600, startHeight = 400;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(startWidth, startHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Inform OpenGL of are callback function to change the window size */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    /* Initialise GLAD so we can access OpenGL functions (must be done here) */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    Shader myShader("shaders/vertexShader.txt", "shaders/fragmentShader.txt"); // build the shader program
    
    /* ---------------------------- Build the VBO & VAO ---------------------------- */

    /* Load our first triangle into a Vertex Buffer Obect */
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3( 1.0f,  1.0f,  -1.0f), 
        glm::vec3( 0.0f,  0.1f, -0.0f),
        glm::vec3( 1.0f,  0.0f,  1.0f)
    };

    /* Gen the VAO and VBO and EBO */
    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO); // create the VAO
    glGenBuffers(1, &VBO); 
    glGenBuffers(1, &EBO); 

    glBindVertexArray(VAO); // bind the VAO, the the buffer, then configure vertex attributes

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // add the vertices to the VBO

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // configure attributes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float))); // configure attributes
    glEnableVertexAttribArray(0); 
    glEnableVertexAttribArray(1);   

    glBindVertexArray(0); 

    glEnable(GL_DEPTH_TEST); // enable depth testing

    /* ---------------------------- 3D ---------------------------- */
    // scale by 0.5 on each axis then rotate 90 degrees
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    unsigned int texture;
    glGenTextures(1, &texture); // gen the texture
    glBindTexture(GL_TEXTURE_2D, texture); // bind the texture

    stbi_set_flip_vertically_on_load(true); // flip the image as coordinates are reversed

    /* Import Giles */
    // load the image in 
    int width, height, nrChannels;
    unsigned char *data = stbi_load("obama.jpeg", &width, &height, &nrChannels, 0); 
    if (data)
    {
        // generate the texture to the binded texture variable
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data); // free the image from memory

    /* ---------------------------- Render Loop ---------------------------- */
    while (!glfwWindowShouldClose(window))
    {
        /* Handle user input */
        processInput(window);

        /* Render here */
        /* Clear the colour buffer with dark turqoise */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // (state setting)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // (state using)

        /* Show the image */
        myShader.use();
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO); // bind the VAO

        for (unsigned int i = 0; i < size(cubePositions); i ++) {
            glm::mat4 transform = glm::mat4(1.0f); // init an identity matrix
            transform = glm::translate(transform, cubePositions[i]); // translate first
            float angle = glfwGetTime() * 50; // use the current time as the angle
            transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.5, 0.5, 0.5)); // then rotate
            myShader.setMat4("transform", transform); // pass the uniform matrix
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0); // unbind the VAO

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* De-allocate memory */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    myShader.del();

    /* Terminate glfw */
    glfwTerminate();
    return 0;
};

#endif