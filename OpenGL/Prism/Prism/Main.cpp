#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <array>

#include "VertexBuffers.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "BufferLayout.h" 
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Texture.h"
#include "TestClearColor.h"
#include "TestMenu.h"


//**External Linked Libs Start**//

//~Begin Interface glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
//~End Interface glm

//~Begin Interface ImGui

#include "ImGUI/imgui.h"
#include "ImGuI/Backend/imgui_impl_glfw.h"
#include "ImGUi/Backend/imgui_impl_opengl3.h"
//**External Linked Libs End**//


const unsigned int width = 1500, height = 750;

GLFWwindow* mainwindow = nullptr;

int bufferwidth, bufferheight;

unsigned int VBO, VAO, IBO;

//** ImGui GLOBAL CONSTANTS**//
float font_size = 1.5f;
float elements_size = 1.5f;
//** End **//



void Print(const std::string& Message)
{
    // using imgui
    ImGui::SameLine();
    ImGui::Text(Message.c_str());
}



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

    // set the prerequisites of the window
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
        std::cout << "GLEW Initialization failed" << std::endl;
        glfwDestroyWindow(mainwindow);
        glfwTerminate();
        return 1;
    }


    // now create the viewport
    glViewport(0, 0, bufferwidth, bufferheight);
    {
        // Batch Rendering test
        VertexArray vao;

        // get the data
        std::array<float,16> vertices = {
            // quad 1 (left)
            -1.8f, -0.7f,
            -0.4f, -0.7f,
            -0.4f,  0.7f,
            -1.8f,  0.7f,

            // quad 2 (right)
             0.4f, -0.7f,
             1.8f, -0.7f,
             1.8f,  0.7f,
             0.4f,  0.7f
        };

        VertexBuffers vbo(vertices.data(),vertices.size() * sizeof(float));

         // now u need to tell the layout

        BufferLayout layout;
 
        layout.PushBuffers(0,2,GL_FLOAT,false,0);

        vao.addBuffer(vbo,layout);

        std::array<unsigned int,12> indices = {
            0,1,2,
            2,3,0,
            4,5,6,
            6,7,4
        };


        // do the ibo
        IndexBuffer ibo(indices.data(), indices.size() * sizeof(unsigned int));

        //Create the program
        ShaderProgram program("BatchRendering.txt");

        Renderer renderer;

        glm::mat4 proj = glm::ortho(-2.f, 2.f, -1.f, 1.f, -1.f, 1.f);
        glm::mat4 mvp = proj;


        program.SetUniformMat4("u_mvp",mvp);

        while (!glfwWindowShouldClose(mainwindow))
        {
            // poll for events
            glfwPollEvents();
            renderer.Clear();

           
            renderer.Draw(vao,ibo,program);

            //
            // ImGui::Render();
            // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(mainwindow);
        }
    }
    // // cleanup 
    // ImGui_ImplOpenGL3_Shutdown();
    // ImGui_ImplGlfw_Shutdown();
    // ImGui::DestroyContext();
    glfwTerminate();
  
}
