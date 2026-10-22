// ReSharper disable All
#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
private:
    GLFWwindow*& Currentwindow;
    bool Keys[1024] = { false };

    // ReSharper disable once IdentifierTypo
    double lastxpos;
    double lastypos;
    double xchange;
    double ychange;
    bool  bfirstmovement = true;


    // elements of camera 
    glm::vec3 Cameraposition;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldup = glm::vec3(0.f,1.f,0.f);

    double yaw;
    double pitch;

    float movementspeed;
    float turnspeed;



public:
    

    Camera(GLFWwindow*& currentwindow);


    //Note : this is an exact key call back please go and refer to the document
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void CursorCallback(GLFWwindow* window, double xpos, double ypos);

    void InitializeCamera(glm::vec3 position, glm::vec3 front, float movementspeed, float turnspeed, double yaw, double pitch);

    void Update();

    glm::mat4 GetViewMatrix();

    void HandleCursor();

    void HandleKeys(float deltatime);

    void RemoveCallbacks();

    ~Camera();

    


};

