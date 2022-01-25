#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "PxPhysicsAPI.h"

#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Callback.h"
#include "Utilities.h"
#include "Grid.h"

float deltaTime = 0.0, currentFrame, lastFrame = 0.f;
float diffTime = 0.0, currentTime, lastTime = 0.f;
int fpsToShow = 0;
unsigned long long counter = 0;

// GLOBAL PHYSICS VARIABLES.
static physx::PxDefaultAllocator pAllocator;
static physx::PxDefaultErrorCallback pError;

static physx::PxFoundation* pFoundation = nullptr;
static physx::PxPhysics* pPhysics = nullptr;

physx::PxMaterial* pMaterial = nullptr;

bool pPhysicsStart = false;
constexpr float pPhysicsDeleteThreshold = 1000.f;

physx::PxRigidDynamic* createSphereProjectileFromCamera(Camera* camera);
glm::mat4 getGlmTransformMatrixFromPhysX(physx::PxTransform t);

int main()
{
    //init Nvidia PhysX API.
    pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, pAllocator, pError);
    if (!pFoundation)
    {
        printf("ERROR: PhysX foundation failed.\n");
        std::exit(EXIT_FAILURE);
    }

    pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *pFoundation, physx::PxTolerancesScale());
    if (!pPhysics)
    {
        printf("ERROR: PhysX physics failed.\n");
        std::exit(EXIT_FAILURE);
    }

    //PhysX simulation time accumulator.
    double pAccumulator = 0.0;
    double pPhysicsStepSize = 1.0 / 60.0;

    //Scene and scene descriptor.
    physx::PxScene* pScene = nullptr;
    
    physx::PxSceneDesc pSceneDesc(pPhysics->getTolerancesScale());
    pSceneDesc.gravity = physx::PxVec3(0.f, -9.8f, 0.f);
    pSceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(15);
    pSceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

    pScene = pPhysics->createScene(pSceneDesc);

    //Creating common material.
    pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

    //Create rigid static actor. (Plane)
    physx::PxTransform pPlaneRelativeTransform = physx::PxTransform(physx::PxVec3(0.f), physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.f, 0.f, 1.f)));
    physx::PxTransform pPlaneGlobalTransform = physx::PxTransform(physx::PxVec3(0.f),physx::PxQuat(0.f,physx::PxVec3(0.f)));
    physx::PxRigidStatic* pPlaneActor = pPhysics->createRigidStatic(pPlaneGlobalTransform);
    physx::PxShape* pPlaneShape = physx::PxRigidActorExt::createExclusiveShape(*pPlaneActor, physx::PxPlaneGeometry(), *pMaterial);
    pPlaneShape->setLocalPose(pPlaneRelativeTransform);
    pScene->addActor(*pPlaneActor);

    //Rigidbody dynamic container for tracking physics objects.
    std::vector<physx::PxRigidDynamic*> rigidbodyDynamic;
    //Projectile dynamic container for tracking camera projectiles.
    std::vector < physx::PxRigidDynamic*> projectileDynamic;
    //To obstruct creating vast numbers of projectiles we will use lock mechanism.
    bool blockProjectileGeneration = false;

    //Create rigid dynamic actor. (Box)
    unsigned int stackHeight = 10u, stackWidth = 10u;
    for (size_t i = 0; i < stackHeight; i++)
    {
        for (size_t j = 0; j < stackWidth; j++)
        {
            physx::PxTransform pBoxTransform = physx::PxTransform(j * 2.f, i * 2.f + 5.f, 0.f);
            physx::PxBoxGeometry PBoxGeometry(physx::PxVec3(1.f));
            physx::PxRigidDynamic* pBoxActor = pPhysics->createRigidDynamic(pBoxTransform);
            physx::PxShape* pBoxShape = physx::PxRigidActorExt::createExclusiveShape(*pBoxActor, PBoxGeometry, *pMaterial);
            physx::PxRigidBodyExt::updateMassAndInertia(*pBoxActor, physx::PxReal(1.f));
            rigidbodyDynamic.push_back(pBoxActor);
            pScene->addActor(*pBoxActor);
        }
    }

    //Create kinematic actor using sphere. (To simulate camera's effect on other dynamics)
    physx::PxTransform pInýtTransform = physx::PxTransform(physx::PxVec3(0.f));
    physx::PxSphereGeometry pSphereGeometry(physx::PxReal(0.3f));
    physx::PxRigidDynamic* pCameraActor = pPhysics->createRigidDynamic(pInýtTransform);
    physx::PxShape* pSphereShape = physx::PxRigidActorExt::createExclusiveShape(*pCameraActor, pSphereGeometry, *pMaterial);
    pCameraActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
    pScene->addActor(*pCameraActor);

    const int screenWidth = 2560, screenHeight = 1440;
    const float near = 0.1f, far = 1000.f;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Window", NULL, NULL);
    if (window == NULL)
    {
        printf("ERROR: GLFW could not create a window.\n");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, windowSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("ERROR: Failed to initialise GLAD.\n");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);

    Camera camera(window);
    camera.setCameraSpeed(15.f);
    Grid grid;

    glm::mat4 model = glm::mat4(1.f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)(screenWidth) / (float)(screenHeight), near, far);

    glm::vec3 viewPos = glm::vec3(0.f);

    Model sphere("resources/sphere.obj");

    Shader mShader("shader/main.vert","shader/main.frag");
    Shader gShader("shader/grid.vert", "shader/grid.frag");

    unsigned int container = loadTextureFromFile("resources/container.jpg");
    unsigned int red = loadTextureFromFile("resources/plastic.png");

    //Always use texture unit 0.
    mShader.use();
    mShader.setInt("texture_diffuse0", 0);
    glActiveTexture(GL_TEXTURE0);

    while (!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        currentTime = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        diffTime = currentTime - lastTime;
        lastFrame = currentFrame;

        if (diffTime >= 1.0)
        {
            fpsToShow = counter;
            counter = 0;
            lastTime = currentTime;
            glfwSetWindowTitle(window, std::to_string(fpsToShow).c_str());
        }

        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_X))
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_E))
            pPhysicsStart = true;

        if (glfwGetKey(window,GLFW_KEY_SPACE) && !blockProjectileGeneration)
        {
            blockProjectileGeneration = true;

            physx::PxRigidDynamic* projectileActor = createSphereProjectileFromCamera(&camera);
            projectileDynamic.push_back(projectileActor);
            pScene->addActor(*projectileActor);

        }
        else if (!glfwGetKey(window, GLFW_KEY_SPACE)) //If it is not pressed then 
        {
            blockProjectileGeneration = false;
        }

        camera.update();
        view = camera.getViewMatrix();
        viewPos = camera.getCameraPosition();

        //Update Nvidia PhysX API.
        //CAUTION: PhysX is so sensitive to both very small, large and non constant time steps.
        //Updating simulation with deltatime may cause artifacts like jittering and undefined behavior.
        if (pPhysicsStart)
        {
            pAccumulator += (double)deltaTime;
            if (pAccumulator >= pPhysicsStepSize)
            {
                pScene->simulate(pPhysicsStepSize);
                pScene->fetchResults(true);

                pAccumulator = 0.0;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Render dynamic rigidbody representation.
        mShader.use();
        mShader.setMat4("view", view);
        mShader.setMat4("projection", projection);
        glBindTexture(GL_TEXTURE_2D, container);

        //Query rigidbody world transform.
        for (size_t i = 0; i < rigidbodyDynamic.size(); i++)
        {
            model = glm::mat4(1.f);

            //Query rigidbody actor transform.
            physx::PxTransform transform = rigidbodyDynamic.at(i)->getGlobalPose();
            //Track object distance from view position in order to delete them if they exceed designated threshold.
            glm::vec3 locationRelativeToViewPos(glm::vec3(transform.p.x, transform.p.y, transform.p.z) - viewPos);
            if (glm::length(viewPos) > pPhysicsDeleteThreshold)
            {
                if (rigidbodyDynamic.at(i)->isReleasable())
                {
                    rigidbodyDynamic.at(i)->release();
                    rigidbodyDynamic.erase(rigidbodyDynamic.begin() + i);
                }
            }

            //Apply transformation to graphics.
            model *= getGlmTransformMatrixFromPhysX(transform);

            mShader.setMat4("model", model);
            renderCube();
        }

        //Query projectile rigidbody world transform.
        for (size_t i = 0; i < projectileDynamic.size(); i++)
        {
            model = glm::mat4(1.f);

            //Query rigidbody actor transform.
            physx::PxTransform transform = projectileDynamic.at(i)->getGlobalPose();
            //Track object distance from view position in order to delete them if they exceed designated threshold.
            glm::vec3 locationRelativeToViewPos(glm::vec3(transform.p.x, transform.p.y, transform.p.z) - viewPos);
            if (glm::length(viewPos) > pPhysicsDeleteThreshold)
            {
                if (projectileDynamic.at(i)->isReleasable())
                {
                    projectileDynamic.at(i)->release();
                    projectileDynamic.erase(projectileDynamic.begin() + i);
                }
            }

            //Apply transformation to graphics.
            model *= getGlmTransformMatrixFromPhysX(transform);

            mShader.setMat4("model", model);
            sphere.Draw(mShader);
        }

        //Render plane representation.
        model = glm::mat4(1.f);
        model = glm::scale(model, glm::vec3(1000.f, 0.f, 1000.f));
        mShader.setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, red);
        renderCube();

        //In every frame it is essential to update kinematic dynamic actor which refers camera.

        pInýtTransform.p = physx::PxVec3(viewPos.x, viewPos.y, viewPos.z);
        pCameraActor->setKinematicTarget(pInýtTransform);

        gShader.use();
        model = glm::mat4(1.f);
        gShader.setMat4("model", model);
        gShader.setMat4("view", view);
        gShader.setMat4("projection", projection);
        grid.draw(gShader);

        //------------------SWAP BUFFERS------------------
        glfwSwapBuffers(window);
        counter++;
    }

    //shutdown Nvidia PhysX API as reverse order of creation.
    pScene->release();
    pPhysics->release();
    pFoundation->release();

    glfwDestroyWindow(window);
    glfwTerminate();
}

physx::PxRigidDynamic* createSphereProjectileFromCamera(Camera* camera)
{
    float distanceCoefficient = 10.f;
    float velocityCoefficient = 100.f;

    glm::vec3 viewPos = camera->getCameraPosition();
    glm::vec3 viewFront = camera->getCameraFront();
    glm::vec3 initPos = viewPos + distanceCoefficient * viewFront;
    physx::PxVec3 velocity = physx::PxVec3(viewFront.x,viewFront.y,viewFront.z) * velocityCoefficient;

    physx::PxTransform t = physx::PxTransform(physx::PxVec3(initPos.x, initPos.y, initPos.z));
    physx::PxSphereGeometry g = physx::PxSphereGeometry(1.f);
    physx::PxRigidDynamic* actor = pPhysics->createRigidDynamic(t);
    physx::PxRigidActorExt::createExclusiveShape(*actor, g, *pMaterial);
    physx::PxRigidBodyExt::updateMassAndInertia(*actor, physx::PxReal(1.f));
    actor->setLinearVelocity(velocity);

    return actor;
}

glm::mat4 getGlmTransformMatrixFromPhysX(physx::PxTransform t)
{
    physx::PxMat44 transformationMatrix = physx::PxMat44(t);
    const float* fp16Format = transformationMatrix.front();
    glm::mat4 glmFormat = glm::make_mat4(fp16Format);

    return glmFormat;
}