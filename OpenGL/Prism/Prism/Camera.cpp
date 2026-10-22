// ReSharper disable All
#include "Camera.h"
#include <iostream>
#include <algorithm>

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
          
            camera->Keys[key] = true;
        }
        else if (action == GLFW_RELEASE && camera->Keys[key])
        {
           
            camera->Keys[key] = false;
        }


    }




}

void Camera::CursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    
    if (camera->bfirstmovement)
    {
        camera->lastxpos = xpos;
        camera->lastypos = ypos;
        camera->bfirstmovement = false;
    }

    // find the changes

    camera->xchange = xpos - camera->lastxpos;
    camera->ychange = ypos - camera->lastypos;

    camera->lastxpos = xpos;
    camera->lastypos = ypos;
}

void Camera::InitializeCamera(glm::vec3 position, glm::vec3 front, float movementspeed, float turnspeed, double yaw, double pitch)
{
    this->Cameraposition = position;
    this->front = front;
    this->movementspeed = movementspeed;
    this->turnspeed = turnspeed;
    this->yaw = yaw;
    this->pitch = pitch;
}
void Camera::Update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    right= glm::normalize(glm::cross(front, worldup));
    up = glm::normalize(glm::cross(right, front));
    
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Cameraposition, Cameraposition + front, up);
}

void Camera::HandleCursor()
{
    yaw += xchange * static_cast<double>(turnspeed);
    pitch += ychange * static_cast<double>(turnspeed);

    xchange = 0.0;
    ychange = 0.0;

    pitch = std::clamp(pitch, -89.0, 89.0);

    Update();
}

void Camera::HandleKeys(float deltatime)
{

    Update();

    // next u write the logic to handle keys
    // in opengl front is z right is x and up is y
    if (Keys[GLFW_KEY_W])
    {
        float dist = movementspeed * deltatime;
        Cameraposition += front * dist;
    }
    if (Keys[GLFW_KEY_S])
    {
        float dist = movementspeed * deltatime;
        Cameraposition -= front * dist;
    }
    if (Keys[GLFW_KEY_A])
    {
        float dist = movementspeed * deltatime;
        Cameraposition -= right * dist;
    }
    if (Keys[GLFW_KEY_D])
    {
        float dist = movementspeed * deltatime;
        Cameraposition += right * dist;
        
    }

}

void Camera::RemoveCallbacks()
{
    glfwSetKeyCallback(Currentwindow, nullptr);
    glfwSetCursorPosCallback(Currentwindow, nullptr);
}

Camera::~Camera()
{
    RemoveCallbacks();
    // dont set ptr to null
}
