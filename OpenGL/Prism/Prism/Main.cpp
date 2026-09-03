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

const unsigned int width = 1000, height = 500;

GLFWwindow* mainwindow = nullptr;

int bufferwidth, bufferheight;

unsigned int VBO, VAO,IBO;

// create a struct for storing the string of vertex and fragment shaders
struct Shaders
{
    std::string vs;
    std::string fs;
};

enum class ShaderType : uint8_t
{
    none,
    vertex,
    fragment
};
Shaders ParseFile(const std::string& infile)
{

    //create an object of ifstream
    std::ifstream inf(infile);

    if (!inf)
    {
        std::cout << "I am not able to open the file" << std::endl;
        return { "","" };
    }

    // next create a array of sstream
    std::stringstream s[2];

    ShaderType st = ShaderType::none;
    std::string line;

    while (std::getline(inf, line))
    {
        // get the line check for #shader
        if (line.find("#shader") != std::string::npos)
        {
            // check if it is vertes
            if (line.find("vertex") != std::string::npos)
                st = ShaderType::vertex;
            if (line.find("fragment") != std::string::npos)
                st = ShaderType::fragment;
        }
        else
        {
            if (st == ShaderType::vertex)
            {
                s[0] << line << "\n";
            }
            else if (st == ShaderType::fragment)
            {
                s[1] << line << "\n";
            }
        }

    }

    // now just return the struct
    return { s[0].str(),s[1].str() };
}



// create two functions to create the shader program and compile shaders

unsigned int CompileShader(unsigned int ShaderType, const std::string& Shader)
{
    unsigned int ShaderId = glCreateShader(ShaderType);

    const char* source = Shader.c_str();
    // u send the shader code into the shader
    int sourcelength = (int)strlen(source);
    glShaderSource(ShaderId, 1, &source, &sourcelength);

    // now u actully compile shader
    glCompileShader(ShaderId);

    int status;
    // shader error handling
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &status);

    if (!status)
    {
        // now that means an error happened

        int InfoLength;
        glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &InfoLength);

        // need to display message
        // have stack allocation
        char* message = (char*)(alloca(InfoLength));

        glGetShaderInfoLog(ShaderId, InfoLength, &InfoLength, message);

        // now just print

        if (ShaderType == GL_VERTEX_SHADER)
        {
            std::cout << "U Have and error in ur vertex shader" << std::endl;
            std::cout << message << std::endl;
        }
        else
        {
            std::cout << "U Have and error in ur fragment shader" << std::endl;
            std::cout << message << std::endl;
        }
        return 0;
    }


    return ShaderId;

}

unsigned int ShaderProgram(const std::string& VertexShader, const std::string& FragmentShader)
{
    // create the program
    unsigned int ProgramId = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);

    if (!vs || !fs)
    {
        return 0;
    }

    // u need to attach Shaders to Program
    glAttachShader(ProgramId, vs);
    glAttachShader(ProgramId, fs);

    glLinkProgram(ProgramId);
    glValidateProgram(ProgramId);


    int result;
    glGetProgramiv(ProgramId, GL_LINK_STATUS, &result);

    if (!result)
    {
        int length;
        glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length);

        glGetProgramInfoLog(ProgramId, length, &length, message);
        std::cout << "There is an error in Program linking stage" << std::endl;
        std::cout << message << std::endl;
        return 0;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return ProgramId;

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
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);





    // the data
    float vertices[] = {
      -0.25f,0.f, 1.0,0.0,0.0,//0
      0.25f,0.f,0.0,1.0,0.0,//1
      0.25f,1.f,0.0,0.0,1.0//2
    };

    VertexBuffers buffer(vertices, 15 * sizeof(float));


    // tell how to traverse the attribute 1
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<const void*>(0));

    // tell how to traverse attribute 2
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void*>(2 * sizeof(float)));


    // the actual array data
    unsigned int indices[] = {
     0,1,2
    };

    IndexBuffer ibo(indices, 3 * sizeof(unsigned int));

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);




    Shaders source = ParseFile(std::string("FirstShader.txt"));



    unsigned int programid = ShaderProgram(source.vs, source.fs);
    if (!programid)
    {
        return 1;
    }
    // here we are using the same program and vao so just bind it once


    // u can retrive the location of the uniform variable
    //int location = glGetUniformLocation(programid,"u_color");

    //assert("Either u are not using the uniform variable or the name u wrote is wrong" && location!=-1 );


    // now ur shaders are known and compiled use here
    float r = 0.0;
    bool flag = false;


    while (!glfwWindowShouldClose(mainwindow))
    {
        // poll for events
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programid);
        glBindVertexArray(VAO);
        ibo.Bind();
        //glUniform4f(location, r, 0.0, 0.0, 1.0);
        // draw the elements
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        //if (!flag)
        //{
        //   if (r >= 1.f) flag = true;
        //    r += 0.001f;

        //}
        //else
        //{
        //    if (r <= 0.f) flag = false;
        //    r -= 0.001f;
        //}



        // now swap the buffer
        glfwSwapBuffers(mainwindow);
    }

    // just remove all
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(programid);
  }
    // when its done remove glfw completely
    glfwTerminate();
    // done
}
