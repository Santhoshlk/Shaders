#include <iostream>
#include <string>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <cassert>

#include "VertexBuffers.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "BufferLayout.h" 
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Texture.h"

const unsigned int width = 1000, height = 500;

GLFWwindow* mainwindow = nullptr;

int bufferwidth, bufferheight;

unsigned int VBO, VAO, IBO;

int main(void)
{
    // extension name 


    // init glfw
    if (!glfwInit())
    {
        std::cout << "The Initialization of glfw failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    // set the prequistics of the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // now create the window
    mainwindow = glfwCreateWindow(width, height, "Main Window", NULL, NULL);

    if (!mainwindow)
    {
        std::cout << "The Window Initialization failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    // get the buffer width and height
    glfwGetFramebufferSize(mainwindow, &bufferwidth, &bufferheight);

    // set the window the current context
    glfwMakeContextCurrent(mainwindow);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "glew Initialization failed" << std::endl;
        glfwDestroyWindow(mainwindow);
        glfwTerminate();
        return 1;
    }


    // now create the viewport
    glViewport(0, 0, bufferwidth, bufferheight);
    {
        // create the vertex array
        VertexArray vao;

        // the data
        float vertices[] = {
          -0.25f,0.f, 1.0,0.0,0.0,//0
          0.25f,0.f,0.0,1.0,0.0,//1
          0.25f,1.f,0.0,0.0,1.0,//2
          -0.25,1.f,0.f,1.f,0.f//3
        };

        VertexBuffers buffer(vertices, 20 * sizeof(float));

        BufferLayout layout;


        layout.PushBuffers(3, 2, GL_FLOAT, GL_FALSE, 0);
        layout.PushBuffers(4, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float));


        vao.Bind();
        vao.addBuffer(buffer, layout);

        // the actual array data
        unsigned int indices[] = {
         0,1,2,
         2,3,0
        };

        IndexBuffer ibo(indices, 6 * sizeof(unsigned int));


        ShaderProgram program("FirstShader.txt");


        program.Bind();

        float r = 0.0;
        bool flag = false;

       int location =  glGetUniformLocation(program.getProgramId(), "u_alpha");
       
       if (location == -1)
       {
           std::cout << "Location is not valid" << std::endl;
       }

       

       Renderer renderer;

        while (!glfwWindowShouldClose(mainwindow))
        {
            // poll for events
            glfwPollEvents();

            renderer.Clear();

            program.Bind();
            vao.Bind();
            ibo.Bind();
            
            glUniform1f(location, r);
            renderer.BlendAlpha();
            renderer.Draw(vao, ibo, program);

            if (r <= 1.f)
            {
                if (!flag)
                {
                    r += 0.005f;
                }
                else
                {
                    if (r <= 0.f)
                    {
                        r = 0.f;
                        flag = false;
                    }
                    else
                    {
                        r -= 0.005f;
                    }
                }
            }
            else
            {
                r = 1.f;
                flag = true;
            }


            glfwSwapBuffers(mainwindow);
        }
    }
    // when its done remove glfw completely
    glfwTerminate();
    // done
}
