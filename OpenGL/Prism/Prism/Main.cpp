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


//**External Linked Libs Start**//
#include <imgui_impl_opengl3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ImGUI/imgui.h"
#include "ImGUI/Backend/imgui_impl_glfw.h"
//**External Linked Libs End**//


const unsigned int width = 1500, height = 750;

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


       //Dear ImGuI Setup
         // Setup Dear ImGui context
       const char* glsl_version = "#version 460 core";
       IMGUI_CHECKVERSION();
       ImGui::CreateContext();
       ImGuiIO& io = ImGui::GetIO(); (void)io;
       io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
       io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 
    
       ImGui_ImplGlfw_InitForOpenGL(mainwindow, true);
       ImGui_ImplOpenGL3_Init(glsl_version);

       // Setup Dear ImGui style
       ImGui::StyleColorsDark();


      //ImGUI bools
       bool show_demo_window = true;
       bool show_another_window = false;
       ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

     
       glm::mat4 proj = glm::ortho(-2.f,2.f,-1.f,1.f,-1.f,1.f);
       
       glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(-0.5f,0.f,0.f));

       glm::mat4 model = glm::translate(glm::mat4(1.f),glm::vec3(0.5f,-0.25,0.f));
       glm::mat4 mvp = proj * view * model;

      
    
       texture.Bind(2);

       program.SetUniform1i("u_TexSlot",2);

       program.SetUniformMat4("u_mvp", mvp);
        while (!glfwWindowShouldClose(mainwindow))
        {
            // poll for events
            glfwPollEvents();
            renderer.Clear();

            //ImGUI Frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            program.Bind();
            vao.Bind();
            ibo.Bind();

            renderer.BlendAlpha();
            renderer.Draw(vao, ibo, program);

            //ImGUI Window Render
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }




            //ImGUI Render
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(mainwindow);
        }
    }
    //ImGUI Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
     
    glfwTerminate();
  
}
