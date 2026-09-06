#include "ShaderProgram.h"

unsigned int ShaderProgram::CompileShader(unsigned int ShaderType, const std::string& Shader)
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

unsigned int ShaderProgram::CreateShaderProgram(const std::string& VertexShader, const std::string& FragmentShader)
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

ShaderProgram::ShaderProgram(const std::string& filetoParse)
{
    Shaders source = ParseFile(filetoParse);
   m_ProgramId =  CreateShaderProgram(source.vs, source.fs);
}




void ShaderProgram::Bind()
{
    glUseProgram(m_ProgramId);
}

void ShaderProgram::UnBind()
{
    glUseProgram(0);
}

const unsigned int& ShaderProgram::getProgramId() const
{
    return m_ProgramId;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_ProgramId);
}

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
