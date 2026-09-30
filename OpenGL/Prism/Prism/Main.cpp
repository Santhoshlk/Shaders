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

      
    
       texture.Bind(2);

       program.SetUniform1i("u_TexSlot",2);
       program.SetUniformMat4("u_mvp", mvp);


       //Dear ImGui Setup
       IMGUI_CHECKVERSION();
       ImGui::CreateContext();
      ImGuiIO & io = ImGui::GetIO();

      // actual style 
      ImGuiStyle& Style =  ImGui::GetStyle();
      Style.ScaleAllSizes(elements_size);
      Style.FontScaleMain = font_size;


      ImGui::StyleColorsDark();

      //ImGui bools
      static bool showdemo = true;



     // actual window initialization
      ImGui_ImplGlfw_InitForOpenGL(mainwindow, true);
      const char* glsl_version = "#version 460 core";
      ImGui_ImplOpenGL3_Init(glsl_version);


        while (!glfwWindowShouldClose(mainwindow))
        {
            // poll for events
            glfwPollEvents();
            renderer.Clear();
            program.Bind();
            vao.Bind();
            ibo.Bind();

            renderer.BlendAlpha();
            renderer.Draw(vao, ibo, program);

            //frame creation
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (showdemo)
            {
                ImGui::ShowDemoWindow(&showdemo);
            }

             // window creation with static inits
            {
                static bool showwindow = true;
                static bool state = false;
                static bool state_dark = false;
                static bool checkbox = false;  
                static int radiobutton = 0;

                static int counter = 0;

                if (showwindow)
                {
                    ImGui::Begin("My new custom Window", &showwindow, ImGuiWindowFlags_NoScrollbar);

                    if (ImGui::Button("Random##Light"))
                    {
                        if (state)
                        {
                            state = false;
                        }
                        else
                        {
                            state = true;
                        }

                    }

                    if (state)
                    {
                        Print("Nice Thanks for clicking light!");
                    }

                    if (ImGui::Button("Random##dark"))
                    {
                        if (state_dark)
                        {
                            state_dark = false;
                        }
                        else
                        {
                            state_dark = true;
                        }
                    }

                    if (state_dark)
                    {
                        Print("Nice Thanks for clicking dark!");
                    }

                    ImGui::Checkbox("CheckBox", &checkbox);

                    checkbox ? Print("True") : Print("false");


                    //ImGui auto assumes next line
                    ImGui::RadioButton("Button a", &radiobutton, 0); ImGui::SameLine();
                    ImGui::RadioButton("Button b", &radiobutton, 1); ImGui::SameLine();
                    ImGui::RadioButton("Button c", &radiobutton, 2); ImGui::SameLine();

                    switch (radiobutton)
                    {

                    case 0:
                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Pressed a");
                        break;
                    case 1:
                        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Pressed b");
                        break;
                    case 2:
                        ImGui::TextColored(ImVec4(0, 0, 1, 1), "Pressed c");
                        break;

                    default:
                        break;
                    }
                    //HyperLinks
                        //!)TextLinkUrl
                    ImGui::TextLinkOpenURL("Github SanthoshLk", "https://github.com/Santhoshlk");

                    if(ImGui::TextLink("Print Label"))
                    {
                        Print("Label");
                    }


                    // style buttons
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 0.75));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0, 0, 0.5));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1,0,0,1));

                    ImGui::Button("Click!");

                    ImGui::PopStyleColor(3);

                    // arrow button with hold to repeat
                    ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat,true);
                    ImGui::Text("Hold to Repeat");
                    ImGui::SameLine();

                    if (ImGui::ArrowButton("##left",ImGuiDir_Left))
                    {
                        counter--;
                    }
                    ImGui::SameLine();
                    if (ImGui::ArrowButton("##Right",ImGuiDir_Right))
                    {
                        counter++;
                    }
                    ImGui::PopItemFlag();
                    ImGui::SameLine();
                    ImGui::Text("Counter :%d", counter);

                    // sliders
                    // int float double drag
                    static int count = 2;
                    static float speed = 0.5f;

                    ImGui::SliderInt("Int Slider",&count,0,10);
                    ImGui::SliderFloat("Slider Float", &speed, 0.f, 13.5f, "%.2f");

                    // any static ones
                    static double inc = 5;
                    static double max = 13.5;
                    static double min = 0;
                    ImGui::SliderScalar("Slider Double",ImGuiDataType_Double,&inc,&min,&max,"%.2f");



                    //draggers
                    static float dragger_float = 0.5f;
                    static int dragger_int = 4;
                    static float float2[] = {0.f,1.f};
                    static ImU64 Sized = 2;


                    // this is unbounded
                    ImGui::DragFloat("Drag Float", &dragger_float);

                    ImGui::DragInt("Drag Int",&dragger_int,2,0,10,"%d",ImGuiSliderFlags_AlwaysClamp);

                    ImGui::DragFloat2("Drag float 2 ",float2,0.5f);
                    
                    // any type
                    ImGui::DragScalar("Drag Scalar",ImGuiDataType_U64,&Sized);


                    // Tooltip
                    ImGui::Button("Save Data");

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Saves the data to nvme!");
                    
                    //Rich Tooltip
                    static bool tooltip_check = false;
                    ImGui::Checkbox("Tooltip",&tooltip_check);

                    if (ImGui::BeginItemTooltip())
                    {
                        ImGui::Text("Tooltip check working");
                        ImGui::Separator();
                        ImGui::TextColored(ImVec4(0.f,1.f,0.f,1.f),"GPU OK");
                        ImGui::EndTooltip();
                    }

                    // 
                    ImGui::TextDisabled("Text Tooltip");
                    if (ImGui::BeginItemTooltip())
                    {
                        // this allows 40 character per line on a box
                        ImGui::PushTextWrapPos(ImGui::GetFontSize()*40.f);
                        ImGui::TextUnformatted("Long explanation that wraps instead of running off screen. "
                            "Adding a lot more text here so it goes way past thirty five "
                            "characters per line and you can actually see the difference."
                            );
                        ImGui::PopTextWrapPos();
                        ImGui::EndTooltip();
                    }

                    //Inputs
               // here i am doing a fixed size so no need to do dynamic stack allocation
                    static char name[64] = "Umbra";
                    ImGui::InputText("name", name, sizeof(char)*64);

                    ImGui::Text(name);

                   // default hint
                    static char search[64] = "";    
                    ImGui::InputTextWithHint("##Search","Search from the assets u know",search,sizeof(char)*64);

                    // multiline text
                    static char multiline[128] = "";
                    ImGui::InputTextMultiline("##multiline",multiline,sizeof(char)*128,ImVec2(100.f,100.f));

                    // input int,float,scalar
                    static int input = 0;
                    ImGui::InputInt("Input Int",&input,2);

                    static float input_float = 0.f;
                    ImGui::InputFloat("Input Float", &input_float, 2.5f,0,"%.2f");

                    // color picker and editor

                    static float color_rgb[3] = { 0.f,1.f,0.f };
                    ImGui::ColorEdit3("RGB Color",color_rgb,ImGuiColorEditFlags_DisplayRGB);

                    static float color_rgba[4] = { 0.f,1.f,0.f,1.f };
                    ImGui::ColorEdit4("RGBA Color", color_rgba, ImGuiColorEditFlags_AlphaBar);

                    // Picker: the full square/wheel drawn inline
                    static ImVec4 clearColor = ImVec4(0.1f, 0.1f, 0.12f, 1.0f);
                    ImGui::ColorPicker4("clear", (float*)&clearColor,
                        ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview);

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
