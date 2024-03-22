#pragma once

#include "components/simple_scene.h"

#define SPEEDUP_RATIO		(3.f)
#define CUBES_DISTANCE		(3.f)
#define JUMP_DISTANCE		(4.f)

namespace m1
{
    class tema2 : public gfxc::SimpleScene
    {
     public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        tema2();
        ~tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        bool CheckCollision(const glm::vec3& tankPosition, const glm::vec3& tankScale, const glm::vec3& buildingPosition, const glm::vec3& buildingScale, float collisionDistance);

        void RenderScene();

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));

        

     protected:

        glm::mat4 modelMatrix;
        glm::vec3 endPos, midPos, startPos;
        float translateX, translateY, translateZ;

        float translateX_tanc1, translateY_tanc1, translateZ_tanc1;
        float translateX_proiectil, translateY_proiectil, translateZ_proiectil;


        float scaleX, scaleY, scaleZ;
        float scaleX_jucator, scaleY_jucator, scaleZ_jucator;
        float angularStepOX, angularStepOY, angularStepOZ;
        GLenum polygonMode;

        GLfloat translateZSine, translateXSine, translateYSine;
        GLfloat angle_turela, angleZ;
        GLfloat posX, posZ, angle;
        GLfloat angleJump;

        glm::vec3 lightPosition;

        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        ViewportArea miniViewportArea;

        
    };
}   // namespace m1

