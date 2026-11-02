#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <array>
#include <cmath>


#include "Mesh.h" 
#include "ShaderProgram.h"
#include "Renderer.h"
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

const float degtoradians = 0.01745f;

GLFWwindow* mainwindow = nullptr;

int bufferwidth, bufferheight;


float now = 0.f, deltatime = 0.f, lasttime =0.f;

//** ImGui GLOBAL CONSTANTS**//
float font_size = 1.5f;
float elements_size = 1.5f;
//** End **//

std::vector<std::unique_ptr<Mesh>> meshobjs;
std::vector<std::unique_ptr<ShaderProgram>> Shaders;


void Print(const std::string& Message)
{
    // using imgui
    ImGui::SameLine();
    ImGui::Text(Message.c_str());
}

glm::mat4 perspeciveProjection(float anglefov,float width,float height,float znear, float zfar)
{
    glm::mat4 arr(0.f);


    float Halfangle = anglefov / 2.f;
   float f = 1.f/ std::tanf(Halfangle);

   float aspectratio = height / width;

   arr[0][0] = aspectratio * f;
   arr[1][1] = f;

   float l = zfar / (zfar - znear);
   arr[2][2] = -l;
   arr[3][2] = -l * znear;
   arr[2][3] = -1.f;

   return arr;
}

nt main(void)
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

        std::unique_ptr<Mesh> obj1 = std::make_unique<Mesh>();

        BufferLayout layout;
        layout.PushBuffers(0, 3, GL_FLOAT, false, 0);
        layout.PushBuffers(1, 3, GL_FLOAT, false, 3 * sizeof(float));

        obj1->AddBuffer(vertices, 24 * sizeof(float),layout);
        obj1->AddIndices(indices, 12 * sizeof(unsigned int));

        meshobjs.push_back(std::move(obj1));


        // shader program

        std::unique_ptr<ShaderProgram> shader = std::make_unique<ShaderProgram>("PyramidShader.txt");

        Shaders.push_back(std::move(shader));

        Shaders[0]->Bind();

        // u can create ur translate etc
        glm::mat4 r(1.f);

        r = glm::rotate(r,glm::radians(45.f), glm::vec3(1.f, 1.f, 0.f));

        glm::mat4 t(1.f);

        t = glm::translate(t, glm::vec3(0.25f,0.f,0.f));

        glm::mat4 model = t * r;

        // glm::mat4 proj = glm::perspective(glm::radians(60.f),(float)width/(float)height,0.1f,100.f);

        camera.InitializeCamera(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, -1.f), 5.f, 0.25f, -90.f, 0.f);


     auto proj = perspeciveProjection(degtoradians* 45.f,width,height,0.1f,100.f);


        Renderer renderer;

        while (!glfwWindowShouldClose(mainwindow))
        {
            now = glfwGetTime();
            deltatime = now - lasttime;
            glfwPollEvents();
            
            renderer.Clear();
         
            camera.HandleCursor();
            camera.HandleKeys(deltatime);
          
            glm::mat4 view = camera.GetViewMatrix();
            Shaders[0]->SetUniformMat4("u_proj",proj);
            Shaders[0]->SetUniformMat4("u_view", view);

            renderer.Draw(meshobjs[0]->getVertexArray(),meshobjs[0]->getIndexBuffer(),*Shaders[0]);
            lasttime = now;

            
            glfwSwapBuffers(mainwindow);
        }
        
    }
    // delete all no need now as they are smart pointers
    glfwTerminate();

}
