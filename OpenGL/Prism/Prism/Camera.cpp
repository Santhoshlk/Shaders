#include "Camera.h"
#include <iostream>

Camera::Camera(GLFWwindow*& currentwindow) : Currentwindow(currentwindow)
{
    //Handle all the callbacks in the constructor
    glfwSetWindowUserPointer(Currentwindow, this);

    glfwSetKeyCallback(Currentwindow,KeyCallback);

    glfwSetCursorPosCallback(Currentwindow, CursorCallback);
}

void Camera::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // u need the camera
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        // u should escape
        glfwSetWindowShouldClose(window,GL_TRUE);
    }
    if (key >=0 && key<1024)
    {
        if (action == GLFW_PRESS && !camera->Keys[key])
        {
            std::cout << "Pressed Key :" <<key<< std::endl;
            camera->Keys[key] = true;
        }
        else if (action == GLFW_RELEASE && camera->Keys[key])
        {
            std::cout << "Released Key :" <<key<<std::endl;
            camera->Keys[key] = false;
        }


    }




}

void Camera::CursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    // u need the camera
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    if (camera->bfirstmovement)
    {
        camera->lastxpos = xpos;
        camera->lastypos = ypos;

        camera->bfirstmovement = false;

    }


    camera->xchange = xpos - camera->lastxpos;
    camera->ychange = ypos - camera->lastypos;

    camera->lastxpos = xpos;
    camera->lastypos = ypos;

    std::cout << "X change :" << camera->xchange << " Y Change :" << camera->ychange << std::endl;
}

void Camera::RemoveCallbacks()
{
    glfwSetKeyCallback(Currentwindow, nullptr);
    glfwSetCursorPosCallback(Currentwindow, nullptr);
}

Camera::~Camera()
{
    glfwSetKeyCallback(Currentwindow, nullptr);
    // dont set ptr to null
}
