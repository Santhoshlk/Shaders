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


//**External Linked Libs End**//


const unsigned int width = 1500, height = 750;

const float degtoradians = 0.01745f;

GLFWwindow* mainwindow = nullptr;

int bufferwidth, bufferheight;


float now = 0.f, deltatime = 0.f, lasttime =0.f;


std::vector<std::unique_ptr<Mesh>> meshobjs;
std::vector<std::unique_ptr<ShaderProgram>> Shaders;



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

void PopulateAveragesOfNormals(float vertices[],unsigned int indices[],int UniqueIndices,int v_count, int i_count,int sides)
{
    // which holds all our normals
    std::vector<glm::vec3> v(UniqueIndices);
    std::vector<glm::vec3> normals(UniqueIndices,glm::vec3(0.f));
   
   // first fill all the vertices
    // 8 as x , y , z , u , v , n1 , n2 , n3
    int k = 0;
    for (int i = 0;i<v_count;i+=8)
    {
        v[k].x = vertices[i];
        v[k].y = vertices[i + 1];
        v[k].z = vertices[i + 2];
        k++;
    }

    for (int i = 0;i<i_count;i+=3)
    {
        unsigned int v0 = indices[i];
        unsigned int v1 = indices[i + 1];
        unsigned int v2 = indices[i + 2];

       normals[v0]+= glm::normalize(glm::cross(v[v1] - v[v0], v[v2] - v[v0]));
       normals[v1] += glm::normalize(glm::cross(v[v1] - v[v0], v[v2] - v[v0]));
       normals[v2] += glm::normalize(glm::cross(v[v1] - v[v0], v[v2] - v[v0]));
    }

    // now u need to average out them by the sides
    for (int i = 0;i<normals.size();i++)
    {
        normals[i] = glm::normalize(normals[i]);
    }

    // shader handles the normalization of this so u can just use this to find and populate the normals in vertices
    k = 0;
    for (int i = 5;i<v_count;i+=8)
    {
        // just go and populate the normals
        vertices[i] = normals[k].x;
        vertices[i + 1] = normals[k].y;
        vertices[i + 2] = normals[k].z;
        k++;
    }

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

        std::array<float, 32> vertices = {
            // x      y      z      u     v      nx    ny    nz
            -1.0f, -1.0f,  0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   // 0: left
             0.0f,  1.0f,  0.0f,   0.5f, 1.0f,   0.0f, 0.0f, 0.0f,   // 1: top
             0.0f, -1.0f,  1.0f,   0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   // 2: front
             1.0f, -1.0f,  0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   // 3: right
        };

        std::array<unsigned int, 12> indices = {
            0, 1, 2,   // Face 1: left, top, front
            2, 1, 3,   // Face 2: front, top, right
            3, 1, 0,   // Face 3: right, top, left
            0, 2, 3    // Face 4 (bottom): left, front, right
        };

        std::unique_ptr<Mesh> obj1 = std::make_unique<Mesh>();

        BufferLayout layout;
        layout.PushBuffers(0, 3, GL_FLOAT, false, 0);
        layout.PushBuffers(1, 2, GL_FLOAT, false, 3 * sizeof(float));
        layout.PushBuffers(2, 3, GL_FLOAT, false, 5 * sizeof(float));


        // now process the avg normals
        PopulateAveragesOfNormals(vertices.data(), indices.data(), 4, 32, 12, 3);



        obj1->AddBuffer(vertices.data(), 60 * sizeof(float),layout);
        obj1->AddIndices(indices.data(), 12 * sizeof(unsigned int));

        meshobjs.push_back(std::move(obj1));


        // shader program

        std::unique_ptr<ShaderProgram> shader = std::make_unique<ShaderProgram>("PyramidTexture.txt");

        Shaders.push_back(std::move(shader));

        Shaders[0]->Bind();

        // u can create ur translate etc
        glm::mat4 r(1.f);

        r = glm::rotate_slow(r,degtoradians*45.f,glm::vec3(0.f,1.f,0.f));

        glm::mat4 t(1.f);

        // t = glm::translate(t, glm::vec3(0.25f,0.f,0.f));

       
        // glm::mat4 proj = glm::perspective(glm::radians(60.f),(float)width/(float)height,0.1f,100.f);

        camera.InitializeCamera(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, -1.f), 5.f, 0.25f, -90.f, 0.f);


     auto proj = perspeciveProjection(degtoradians* 45.f,width,height,0.1f,100.f);

        // Here Do the textures
     std::unique_ptr<Texture> MarbleTexture = std::make_unique<Texture>(R"(D:\Rendering\OpenGL\Prism\Prism\assets\brick.png)",GL_NEAREST_MIPMAP_NEAREST,GL_REPEAT);

     MarbleTexture->Bind(0);
     Shaders[0]->SetUniform1i("u_slot",0);
     // u Have bound texture u have loaded it and bound it and send it 
     glm::vec4 Color(1.f, 1.f, 1.f, 1.f);
     glm::vec3 Direction(1.f, 0.f, 0.f);


     LightBase light_Directional(Direction,1.f,Color,1.f);
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
            r = glm::rotate_slow(r, degtoradians * 0.5f, glm::vec3(0.f, 1.f, 0.f));
            glm::mat4 model = t * r;

            Shaders[0]->SetUniformMat4("u_proj",proj);
            Shaders[0]->SetUniformMat4("u_view", view);
            Shaders[0]->SetUniformMat4("u_model",model);

            renderer.Draw(meshobjs[0]->getVertexArray(),meshobjs[0]->getIndexBuffer(),*Shaders[0]);
            lasttime = now;

            
            glfwSwapBuffers(mainwindow);
        }

        MarbleTexture.reset();
        
    }
    // clean them before contex it dead
   
    meshobjs.clear();
    Shaders.clear();
    glfwTerminate();

}
