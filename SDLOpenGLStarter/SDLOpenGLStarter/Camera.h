#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>

namespace GE {
    class Camera {
    public:
        // Constructor
        Camera(glm::vec3 cam_pos, glm::vec3 cam_target, glm::vec3 cam_upDir, float fov, float ar, float nearClip, float farClip) :
            pos(cam_pos), target(cam_target), up(cam_upDir), fovy(fov), aspectR(ar), nearClip(nearClip), farClip(farClip) {
            updateCamMatrices();
            //get active GL window
            SDL_Window* window = SDL_GL_GetCurrentWindow();

            //get width and height of pixels in window
            int w, h;
            SDL_GetWindowSize(window, &w, &h);

            //set mouse to the center of the window
            SDL_WarpMouseInWindow(window, w / 2, h / 2);
        }

        // Accessor methods
        float getPosX() const { return pos.x; }
        float getPosY() const { return pos.y; }
        float getPosZ() const { return pos.z; }
        glm::vec3 getPos() const { return pos; }
        const glm::vec3& getTarget() const { return target; }
        const glm::vec3& getUpDir() const { return up; }
        float getPitch() const { return pitch; }
        float getYaw() const { return yaw; }
        glm::mat4 getViewMatrix() const { return viewMat; }
        glm::mat4 getProjectionMatrix() const { return projectionMat; }

        // Mutator methods
        void setPosX(float newX) {
            pos.x = newX;
            updateCamMatrices();
        }

        void setPosY(float newY) {
            pos.y = newY;
            updateCamMatrices();
        }

        void setPosZ(float newZ) {
            pos.z = newZ;
            updateCamMatrices();
        }

        void setPos(float newX, float newY, float newZ) {
            pos = glm::vec3(newX, newY, newZ);
            updateCamMatrices();
        }

        void setPos(const glm::vec3& newPos) {
            pos = newPos;
            updateCamMatrices();
        }

        void setTarget(const glm::vec3& newTarget) {
            target = newTarget;
            updateCamMatrices();
        }

        void setUpDir(const glm::vec3& newUp) {
            up = newUp;
            updateCamMatrices();
        }

        void setPitch(float newPitch) {
            // limit the pitch value between -70 and 70 degrees
            pitch = glm::clamp(newPitch, -70.0f, 70.0f);
        }

        void setYaw(float newYaw) {
            yaw = newYaw;
        }

        void setFov(float newFov) {
            fovy = newFov;
            updateCamMatrices();
        }

        void setAspectRatio(float newAR) {
            aspectR = newAR;
            updateCamMatrices();
        }

        void setNearClip(float newNearClip) {
            nearClip = newNearClip;
            updateCamMatrices();
        }

        void setFarClip(float newFarClip) {
            farClip = newFarClip;
            updateCamMatrices();
        }

        // Update the view and projection matrices
        void updateCamMatrices() {
            // Compute the view matrix
            viewMat = glm::lookAt(pos, pos + target, up);

            // Compute the projection matrix
            projectionMat = glm::perspective(glm::radians(fovy), aspectR, nearClip, farClip);
        }

    private:
        glm::vec3 pos;
        glm::vec3 target;
        glm::vec3 up;

        // Camera projection variables
        float fovy;
        float aspectR;
        float nearClip;
        float farClip;

        //camera pitch and yaw values
        float pitch = 0.0f;
        float yaw = -90.0f; //to ensure ther camera points along the -z axis to start with

        // View and projection matrices
        glm::mat4 viewMat;
        glm::mat4 projectionMat;
    };
}