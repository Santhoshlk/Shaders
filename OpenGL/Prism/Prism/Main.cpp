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
          // create the vertex array
          VertexArray vao;
        
          constexpr int v_size =28;
          // the data
          std::array<float,v_size> vertices = {
            -0.5f,0.f,0.f,0.f,1.f,0.f,0.f,//0
            0.5f,0.f,1.f,0.f,0.f,1.f,0.f,//1
            0.5f,1.f,1.f,1.f,0.f,0.f,1.f,//2
            -0.5,1.f,0.f,1.f,0.f,1.f,0.f//3
          };
        
          VertexBuffers buffer(vertices.data(), vertices.size() * sizeof(float));
        
          BufferLayout layout;
        
        
          layout.PushBuffers(0, 2, GL_FLOAT, GL_FALSE, 0);
          layout.PushBuffers(1, 2, GL_FLOAT, GL_FALSE,2*sizeof(float));
          layout.PushBuffers(2, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float));
        
        
          vao.Bind();
          vao.addBuffer(buffer, layout);
        
          constexpr int ibo_size = 6;
          // the actual array data
          std::array<unsigned int , ibo_size> indices = {
           0,1,2,
           2,3,0
          };
        
          IndexBuffer ibo(indices.data(), indices.size() * sizeof(unsigned int));
        
        
          ShaderProgram program("FirstShader.txt");
        
        
          program.Bind();
        
        
         // create a texture obj
         Texture texture(std::string(R"(C:\onedrivenew\Desktop\Rendering\ExternalResources\Cockatiel.png)"));
         texture.Bind(2);
        
         Renderer renderer;
        
        
        
         glm::mat4 proj = glm::ortho(-2.f,2.f,-1.f,1.f,-1.f,1.f);
         
         glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(-0.5f,0.f,0.f));
        
         glm::mat4 model = glm::translate(glm::mat4(1.f),glm::vec3(0.5f,-0.25,0.f));
         glm::mat4 mvp = proj * view * model;
        
        //ImGuiSetup
         IMGUI_CHECKVERSION();
         ImGui::CreateContext();
         ImGuiStyle& Style =ImGui::GetStyle();
         Style.ScaleAllSizes(elements_size);
         Style.FontScaleMain = font_size;
        
         ImGuiIO& Io =  ImGui::GetIO();
        
          //no need of io
         ImGui_ImplGlfw_InitForOpenGL(mainwindow, true);
         const char* glsl = "#version 460 core";
         ImGui_ImplOpenGL3_Init(glsl);
        
        
        
         texture.Bind(2);
        
         program.SetUniform1i("u_TexSlot",2);
        
         
         Test::Test* CurrentTest = nullptr;
        
         Test::TestMenu* TestMenu = new Test::TestMenu(CurrentTest);
        
        
        // so u have to register
         TestMenu->RegisterTests<Test::TestClearColor>("Clear_Color");



        program.SetUniformMat4("u_mvp", mvp);

        while (!glfwWindowShouldClose(mainwindow))
        {
            // poll for events
            glfwPollEvents();
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            renderer.Clear();

            {
                static bool showWindow = true;
            
                if (showWindow)
                {
                    ImGui::Begin("Test Menu Window", &showWindow);
                    
                    // pressing of all buttons check if its on test menu
                    if (CurrentTest)
                    {
                        CurrentTest->OnUpdate();
                        CurrentTest->OnRender();
                        CurrentTest->OnImGuiRender();
                    }
                    
                    if (CurrentTest!= TestMenu && ImGui::Button("Back"))
                    {
                        delete CurrentTest;
                        CurrentTest = TestMenu;
                    }
            
                    ImGui::End();
                }
            
            
            }



            
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(mainwindow);
        }
    }
    // cleanup 
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

}
