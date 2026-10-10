#pragma once
#include <GLFW/glfw3.h>


class Camera
{
private:
    GLFWwindow*& Currentwindow;
    bool Keys[1024] = { false };

    double lastxpos;
    double lastypos;
    double xchange;
    double ychange;
    bool  bfirstmovement = true;


public:
    

    Camera(GLFWwindow*& currentwindow);


    //Note : this is an exact key call back please go and refer to the document
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void CursorCallback(GLFWwindow* window, double xpos, double ypos);



    void RemoveCallbacks();

    ~Camera();

    


};

