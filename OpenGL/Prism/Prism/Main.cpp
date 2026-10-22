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
#include "Camera.h"


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


float now = 0.f, deltatime = 0.f, lasttime =0.f;

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

    // camera Creation 
    
    Camera camera(mainwindow);




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
        VertexArray vao;
        vao.Bind();

        float vertices[] = {
     -1.0f, -1.0f, 0.0f ,1.f,0.f,0.f,   // 0: base left
      0.0f, -1.0f, 1.0f,0.f,1.f,0.f,// 1: base front (out in Z)
      1.0f, -1.0f, 0.0f,0.f,0.f,1.f,   // 2: base right
      0.0f,  1.0f, 0.0f ,1.f,0.f,1.f   // 3: top
        };

        unsigned int indices[] = {
            0, 3, 1,
            1, 3, 2,
            2, 3, 0,
            0, 1, 2
        };



        VertexBuffers vbo(vertices,24*sizeof(float));
        vbo.Bind();

        // buffer layout

        BufferLayout layout;
        layout.PushBuffers(0,3,GL_FLOAT,false,0);
        layout.PushBuffers(1, 3, GL_FLOAT, false, 3 * sizeof(float));

        vao.addBuffer(vbo,layout);

        // ibo
        IndexBuffer ibo(indices, 12 * sizeof(unsigned int));

        // shader program

        ShaderProgram shader("PyramidShader.txt");

        shader.Bind();

        // u can create ur translate etc
        glm::mat4 r(1.f);

        r = glm::rotate(r,glm::radians(45.f), glm::vec3(1.f, 1.f, 0.f));

        glm::mat4 t(1.f);

        t = glm::translate(t, glm::vec3(0.25f,0.f,0.f));

        glm::mat4 model = t * r;

        glm::mat4 proj = glm::ortho(-4.f,4.f,-2.f,2.f,0.1f,100.f);

        camera.InitializeCamera(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, -1.f), 5.f, 0.25f, 90.f, 0.f);


        Renderer renderer;

        while (!glfwWindowShouldClose(mainwindow))
        {
            now = glfwGetTime();
            deltatime = now - lasttime;
            glfwPollEvents();
            
            renderer.Clear();
            //Do the actual rendering here
            // update the camera turn and movement here
            camera.HandleCursor();
            camera.HandleKeys(deltatime);
          
            glm::mat4 view = camera.GetViewMatrix();
            shader.SetUniformMat4("u_mvp",proj*view);

            renderer.Draw(vao,ibo,shader);
            lasttime = now;

            
            glfwSwapBuffers(mainwindow);
        }
        
    }
    glfwTerminate();

}
