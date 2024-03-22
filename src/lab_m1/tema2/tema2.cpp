#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/tema2/transform3D.h"
#include <core/engine.h>

using namespace std;
using namespace m1;

float angle;

float angle_tanc1;

float projectileSpeed = 0.0f;

tema2::tema2()
{
}

struct Building {
    glm::vec3 position;
    glm::vec3 scale;

    Building(const glm::vec3& pos, const glm::vec3& scl) : position(pos), scale(scl) {}
};
std::vector<Building> buildings;


tema2::~tema2()
{
}

bool tema2::CheckCollision(const glm::vec3& tankPosition, const glm::vec3& tankScale, const glm::vec3& buildingPosition, const glm::vec3& buildingScale, float collisionDistance) {
    
    float tankMinX = tankPosition.x - (tankScale.x + collisionDistance) / 2.0f;
    float tankMaxX = tankPosition.x + (tankScale.x + collisionDistance) / 2.0f;
    float tankMinY = tankPosition.y - (tankScale.y + collisionDistance) / 2.0f;
    float tankMaxY = tankPosition.y + (tankScale.y + collisionDistance) / 2.0f;
    float tankMinZ = tankPosition.z - (tankScale.z + collisionDistance) / 2.0f;
    float tankMaxZ = tankPosition.z + (tankScale.z + collisionDistance) / 2.0f;

    
    float buildingMinX = buildingPosition.x - (buildingScale.x + collisionDistance) / 2.0f;
    float buildingMaxX = buildingPosition.x + (buildingScale.x + collisionDistance) / 2.0f;
    float buildingMinY = buildingPosition.y - (buildingScale.y + collisionDistance) / 2.0f;
    float buildingMaxY = buildingPosition.y + (buildingScale.y + collisionDistance) / 2.0f;
    float buildingMinZ = buildingPosition.z - (buildingScale.z + collisionDistance) / 2.0f;
    float buildingMaxZ = buildingPosition.z + (buildingScale.z + collisionDistance) / 2.0f;

    
    bool collisionX = tankMaxX >= buildingMinX && tankMinX <= buildingMaxX;
    bool collisionY = tankMaxY >= buildingMinY && tankMinY <= buildingMaxY;
    bool collisionZ = tankMaxZ >= buildingMinZ && tankMinZ <= buildingMaxZ;

    
    return collisionX && collisionY && collisionZ;
}

void tema2::Init()
{

    buildings.emplace_back(glm::vec3(5, 0.3f, -18), glm::vec3(3.5f, 3.5f, 3.5f));
    buildings.emplace_back(glm::vec3(-1, 0.3f, -8.f), glm::vec3(2, 2, 2));
    buildings.emplace_back(glm::vec3(6, 0.3f, -21), glm::vec3(5, 5, 5));
    buildings.emplace_back(glm::vec3(-8, 0.3f, -25), glm::vec3(3, 3, 3));


    {
        Mesh* mesh = new Mesh("soare");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "soare.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        lightPosition = glm::vec3(0, 10, 0);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }

    {
        Mesh* mesh1 = new Mesh("tanc");
        mesh1->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "tanc.obj");
        meshes[mesh1->GetMeshID()] = mesh1;
    }

    {
        Mesh* mesh2 = new Mesh("roti");
        mesh2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "roti.obj");
        meshes[mesh2->GetMeshID()] = mesh2;
    }

    {
        Mesh* mesh3 = new Mesh("suport_turela");
        mesh3->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "suport_turela.obj");
        meshes[mesh3->GetMeshID()] = mesh3;
    }

    {
        Mesh* mesh4 = new Mesh("turela");
        mesh4->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "turela.obj");
        meshes[mesh4->GetMeshID()] = mesh4;
    }

    {
        Mesh* mesh5 = new Mesh("cladire");
        mesh5->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cladire.obj");
        meshes[mesh5->GetMeshID()] = mesh5;
    }

    {
        Mesh* mesh6 = new Mesh("cladirecasa");
        mesh6->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cladirecasa.obj");
        meshes[mesh6->GetMeshID()] = mesh6;
    }

    
    translateX = 0;
    translateY = 0.3f;
    translateZ = 1;

    translateX_tanc1 = 11;
    translateY_tanc1 = 0.3f;
    translateZ_tanc1 = -2;

    translateX_proiectil = translateX;
    translateY_proiectil = translateY + 0.4f;
    translateZ_proiectil = translateZ;

    scaleX_jucator = 1.5f;
    scaleY_jucator = 1.5f;
    scaleZ_jucator = 1.5f;


    scaleX = 1.5f;
    scaleY = 1.5f;
    scaleZ = 1.5f;

    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    angle_turela = 0.f;
    angleZ = 0.f;

    angle = 0.f;
    angle_tanc1 = 0.f;

    posX = cos(angle);
    posZ = sin(angle);

    angleJump = 0.f;

    startPos = glm::vec3(1.f, 1.f, 1.f);
    endPos = glm::vec3(1.f + JUMP_DISTANCE, 1.f, 1.f);

    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

}

void tema2::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    glViewport(0, 0, resolution.x, resolution.y);
}



void tema2::RenderScene() {

    /// Tancul jucatorului

    modelMatrix = glm::mat4(1);
    
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrix *= transform3D::Scale(scaleX_jucator, scaleY_jucator, scaleZ_jucator);
    RenderMesh(meshes["tanc"], shaders["LabShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
   
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrix *= transform3D::Scale(scaleX_jucator, scaleY_jucator, scaleZ_jucator);
    RenderMesh(meshes["roti"], shaders["LabShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
   
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrix *= transform3D::RotateOY(angle_turela);
    modelMatrix *= transform3D::Scale(scaleX_jucator, scaleY_jucator, scaleZ_jucator);
    modelMatrix *= transform3D::Translate(0.08f, -0.1f, 0.5f);
    RenderMesh(meshes["suport_turela"], shaders["LabShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
   
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrix *= transform3D::RotateOY(angle_turela);
    modelMatrix *= transform3D::Scale(scaleX_jucator, scaleY_jucator, scaleZ_jucator);
    RenderMesh(meshes["turela"], shaders["LabShader"], modelMatrix);


    // Tanc inamic 1
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOY(-45);
    modelMatrix *= transform3D::RotateOY(angle_tanc1);
    modelMatrix *= transform3D::Translate(translateX_tanc1, translateY_tanc1, translateZ_tanc1);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["tanc"], shaders["LabShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOY(-45);
    modelMatrix *= transform3D::RotateOY(angle_tanc1);
    modelMatrix *= transform3D::Translate(translateX_tanc1, translateY_tanc1, translateZ_tanc1);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["roti"], shaders["LabShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOY(-45);
    modelMatrix *= transform3D::RotateOY(angle_tanc1);
    modelMatrix *= transform3D::Translate(translateX_tanc1, translateY_tanc1, translateZ_tanc1);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    modelMatrix *= transform3D::Translate(0.08f, -0.1f, 0.5f);
    RenderMesh(meshes["suport_turela"], shaders["LabShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOY(-45);
    modelMatrix *= transform3D::RotateOY(angle_tanc1);
    modelMatrix *= transform3D::Translate(translateX_tanc1, translateY_tanc1, translateZ_tanc1);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["turela"], shaders["LabShader"], modelMatrix);



    /// proiectil

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::Translate(translateX_proiectil, translateY_proiectil, translateZ_proiectil);
    modelMatrix *= transform3D::RotateOY(angle_turela);
    modelMatrix *= transform3D::Scale(0.2f, 0.2f, 0.2f);
    RenderMesh(meshes["sphere"], shaders["LabShader"], modelMatrix);

    ///// Cladiri

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOY(-45);
    modelMatrix *= transform3D::Translate(5, 0.3f, -18);
    modelMatrix *= transform3D::Scale(3.5f, 3.5f, 3.5f);
    RenderMesh(meshes["cladire"], shaders["LabShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOY(45);
    modelMatrix *= transform3D::Translate(-1, 0.3f, -8.f);
    modelMatrix *= transform3D::Scale(2, 2, 2);
    RenderMesh(meshes["cladire"], shaders["LabShader"], modelMatrix);


    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(6, 0.3f, -21);
    modelMatrix *= transform3D::Scale(5, 5, 5);
    RenderMesh(meshes["cladire"], shaders["LabShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-8, 0.3f, -25);
    modelMatrix *= transform3D::Scale(3, 3, 3);
    RenderMesh(meshes["cladirecasa"], shaders["LabShader"], modelMatrix);


    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, lightPosition);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(8));
        RenderMesh(meshes["soare"], shaders["LabShader"], modelMatrix);
    }


    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.01f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1000));
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix);
    }
}




void tema2::Update(float deltaTimeSeconds)
{
    if (projectileSpeed == 0.0f)
    {
        translateX_proiectil = translateX;
        translateZ_proiectil = translateZ;
    }

    for (const auto& building : buildings) {
        if (CheckCollision(glm::vec3(translateX, translateY, translateZ), glm::vec3(scaleX_jucator, scaleY_jucator, scaleZ_jucator),
            building.position, building.scale, 2.0f)) {
            scaleX_jucator = scaleX_jucator - deltaTimeSeconds;
            scaleY_jucator = scaleY_jucator - deltaTimeSeconds;
            scaleZ_jucator = scaleZ_jucator - deltaTimeSeconds;
            std::cout << "Coliziune cu cladire!" << std::endl;
        }
    }

    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);


    float oscillationSpeed = 0.2f;
    angle_tanc1 -= oscillationSpeed * deltaTimeSeconds;


    float oscillationAmplitude = 0.01f;
    translateZ_tanc1 = sin(angle_tanc1) * oscillationAmplitude;

    if (projectileSpeed != 0.0f)
    {
        translateX_proiectil += cos(glm::radians(angle_turela)) * projectileSpeed * deltaTimeSeconds;
        translateZ_proiectil += sin(glm::radians(angle_turela)) * projectileSpeed * deltaTimeSeconds;
    }

    RenderScene();
    DrawCoordinateSystem();

}




void tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    
    glUseProgram(shader->program);

    GLint locLightPos = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(locLightPos, 1, glm::value_ptr(lightPosition));

    GLint locMaterial = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(locMaterial, materialShininess);

    GLint locMaterialKd = glGetUniformLocation(shader->program, "material_kd");  
    glUniform1f(locMaterialKd, materialKd);

    GLint locMaterialKs = glGetUniformLocation(shader->program, "material_ks");  
    glUniform1f(locMaterialKs, materialKs);

    GLint locObject = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(locObject, 1, glm::value_ptr(color));

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}




void tema2::FrameEnd()
{
}



void tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float speed = 0.2f;

        if (window->KeyHold(GLFW_KEY_W))
        {
            //translateX += speed * deltaTime * cos(angle);
            //translateY += speed * deltaTime * sin(angle);
            translateZ -= deltaTime;
            //translateZ_proiectil -= deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_S))
        {
            //translateX -= speed * deltaTime * cos(angle);
            //translateY -= speed * deltaTime * sin(angle);
            translateZ += deltaTime;
            //translateZ_proiectil += deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_D))
        {
            angle += deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_A))
        {
            angle -= deltaTime;
        }


        
    }

}


void tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_P)
    {
        projectileSpeed = 3.0f;
    }
}


void tema2::OnKeyRelease(int key, int mods)
{ 
}


void tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    glm::ivec2 resolution = window->GetResolution();

    float xNormalized = (2.0f * mouseX) / resolution.x - 1.0f;
    float yNormalized = 1.0f - (2.0f * mouseY) / resolution.y;

    glm::vec3 cursorWorldPosition = glm::vec3(xNormalized, yNormalized, 0.0f);

    float angleToCursor = atan2(cursorWorldPosition.z - translateZ, cursorWorldPosition.x - translateX);

    float sensitivity = 0.005f;  
    angle_turela = glm::degrees(angleToCursor) * sensitivity;
    angle_turela = (angle_turela < 0) ? angle_turela + 360 : angle_turela;
}




void tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema2::OnWindowResize(int width, int height)
{
}