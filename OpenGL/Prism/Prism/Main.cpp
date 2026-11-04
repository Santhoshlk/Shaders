#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <array>
#include <cmath>


#include "Texture.h"
#include "Mesh.h" 
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Camera.h"
#include "LightBase.h"


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

   // here opengl uses right hand thumb rule so birds eye is -z so math changes to thisSS

   arr[0][0] =  f* aspectratio;
   arr[1][1] = f;

   float l = (zfar + znear) / (zfar - znear);
   arr[2][2] = -l;
   arr[3][2] = -2.f * znear * zfar / (zfar - znear);
   arr[2][3] = -1.f;

   return arr;
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

     float vertices[] = {
                 
         // Face 1: left, top, front
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,   0.5f, 1.0f,
         0.0f, -1.0f, 1.0f,   1.0f, 0.0f,
         // Face 2: front, top, right
          0.0f, -1.0f, 1.0f,   0.0f, 0.0f,
          0.0f,  1.0f, 0.0f,   0.5f, 1.0f,
          1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
          // Face 3: right, top, left
           1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
           0.0f,  1.0f, 0.0f,   0.5f, 1.0f,
          -1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
          // Face 4 (bottom): left, front, right
          -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
           0.0f, -1.0f, 1.0f,   0.5f, 1.0f,
           1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
     };

        unsigned int indices[] = {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            9, 10, 11
        };

        std::unique_ptr<Mesh> obj1 = std::make_unique<Mesh>();

        BufferLayout layout;
        layout.PushBuffers(0, 3, GL_FLOAT, false, 0);
        layout.PushBuffers(1, 2, GL_FLOAT, false, 3 * sizeof(float));

        obj1->AddBuffer(vertices, 60 * sizeof(float),layout);
        obj1->AddIndices(indices, 12 * sizeof(unsigned int));

        meshobjs.push_back(std::move(obj1));


        // shader program

        std::unique_ptr<ShaderProgram> shader = std::make_unique<ShaderProgram>("PyramidTexture.txt");

        Shaders.push_back(std::move(shader));

        Shaders[0]->Bind();

        // u can create ur translate etc
        glm::mat4 r(1.f);

        r = glm::rotate(r,glm::radians(45.f), glm::vec3(1.f, 1.f, 0.f));

        glm::mat4 t(1.f);

        t = glm::translate(t, glm::vec3(0.25f,0.f,0.f));

        glm::mat4 model(1.f);

        // glm::mat4 proj = glm::perspective(glm::radians(60.f),(float)width/(float)height,0.1f,100.f);

        camera.InitializeCamera(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, -1.f), 5.f, 0.25f, -90.f, 0.f);


     auto proj = perspeciveProjection(degtoradians* 45.f,width,height,0.1f,100.f);

        // Here Do the textures
     std::unique_ptr<Texture> MarbleTexture = std::make_unique<Texture>(R"(D:\Rendering\OpenGL\Prism\Prism\assets\brick.png)",GL_NEAREST_MIPMAP_NEAREST,GL_REPEAT);

     MarbleTexture->Bind(0);
     Shaders[0]->SetUniform1i("u_slot",0);
     // u Have bound texture u have loaded it and bound it and send it 
     glm::vec4 Color(1.f, 1.f, 1.f, 1.f);

     LightBase light_Directional(Color,255.f);
     light_Directional.UseLight(*Shaders[0]);


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
            Shaders[0]->SetUniformMat4("u_model",model);

            renderer.Draw(meshobjs[0]->getVertexArray(),meshobjs[0]->getIndexBuffer(),*Shaders[0]);
            lasttime = now;

            
            glfwSwapBuffers(mainwindow);
        }
        
    }
    // clean them before contex it dead
    meshobjs.clear();
    Shaders.clear();
    glfwTerminate();

}
