#include <iostream>
#include <string>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint width = 1000, height = 500;

GLFWwindow* mainwindow = nullptr;

GLint bufferwidth, bufferheight;

GLuint VBO,VAO;


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
    glGetShaderiv(ShaderId,GL_COMPILE_STATUS,&status);

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

    // u need to attach Shaders to Program
    glAttachShader(ProgramId,vs);
    glAttachShader(ProgramId,fs);

    glLinkProgram(ProgramId);
    glValidateProgram(ProgramId);
    glDeleteShader(vs);
    glDeleteShader(fs);

    int result;
    glGetProgramiv(ProgramId, GL_LINK_STATUS,&result);

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

    return ProgramId;
   
}


static const std::string VertexShader = R"(
#version 460 core

layout(location = 0) in vec2 Pos;

void main()
{
   gl_Position = vec4(vec3(Pos,0.f),1.f);
}
)";

static const std::string FragmentShader = R"(
#version 460 core

out vec4 Color;

void main()
{
   Color = vec4(0.f,0.f,1.f,1.f);
}
)";




void VertexSpecification()
{

    // create the vertex array
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // tell how to traverse the data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*2,reinterpret_cast<const void*>(0));

    // the data
    float vertices[6] = {
      -0.5f,0.f,
      0.5f,0.f,
      0.f,0.5f
    };
    // u are currently binding this buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);
}

int main(void)
{
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

    
    VertexSpecification();

    unsigned int programid = ShaderProgram(VertexShader, FragmentShader);
    while (!glfwWindowShouldClose(mainwindow))
    {
        // poll for events
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programid);
        glBindVertexArray(VAO);
        // now u can actually use draw calls
        glDrawArrays(GL_TRIANGLES,0,3);

        // now swap the buffer
        glfwSwapBuffers(mainwindow);
    }

    // just remove all
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programid);

    // when its done remove glfw completely
    glfwTerminate();
    // done
}
