#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint width = 1000, height = 500;

GLFWwindow* mainwindow = nullptr;

GLint bufferwidth, bufferheight;

int main(void)
{
    // init glfw
    if (!glfwInit())
    {
        std::cout << "The Initialization of glfw failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    // set the prequistics of the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);

    // now create the window
    mainwindow = glfwCreateWindow(width,height,"Main Window",NULL,NULL);

    if (!mainwindow)
    {
        std::cout << "The Window Initialization failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    // get the buffer width and height
    glfwGetFramebufferSize(mainwindow,&bufferwidth,&bufferheight);
    
    // set the window the current context
    glfwMakeContextCurrent(mainwindow);

    glewExperimental = GL_TRUE;

    if (glewInit()!=GLEW_OK)
    {
        std::cout << "glew Initialization failed" << std::endl;
        glfwDestroyWindow(mainwindow);
        glfwTerminate();
        return 1;
    }

    // now create the viewport
    glViewport(0,0,bufferwidth,bufferheight);

    while (!glfwWindowShouldClose(mainwindow))
    {
        // poll for events
        glfwPollEvents();

        glClearColor(0.f, 0.f, 1.f, 1.f);
         // what to clear
        glClear(GL_COLOR_BUFFER_BIT);


        // now swap the buffer
        glfwSwapBuffers(mainwindow);
    }

    // when its done remove glfw completely
    glfwTerminate();
}
