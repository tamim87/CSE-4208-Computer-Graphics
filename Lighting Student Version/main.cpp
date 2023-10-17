//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);

void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void room(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
//void rightwall(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void table(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);


// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
Camera camera(glm::vec3(0.0f, 1.1f, 5.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(3.50f,  5.50f,  -1.0f), /// Spot
    glm::vec3(0.5f,  3.5f,  0.0f),   /// Point
    glm::vec3(3.0f,  15.5f,  1.5f), /// Sun
    //glm::vec3(-1.5f,  -1.5f,  0.0f) //point 2
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    1.0f, 1.0f, 1.0f,     // ambient
    1.0f, 1.0f, 1.0f,      // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    1.0f, 1.0f, 1.0f,     // ambient
    1.0f, 1.0f, 1.0f,      // diffuse
    1.0f, 1.0f, 1.0f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);


PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    1.0f, 1.0f, 1.0f,     // ambient
    1.0f, 1.0f, 1.0f,      // diffuse
    1.0f, 1.0f, 1.0f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);

PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    1.0f, 1.0f, 1.0f,     // ambient
    1.0f, 1.0f, 1.0f,      // diffuse
    1.0f, 1.0f, 1.0f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);

/*
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    1.0f, 1.0f, 1.0f,     // ambient
    1.0f, 1.0f, 1.0f,      // diffuse
    1.0f, 1.0f, 1.0f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);
*/


// light settings
bool onOffPointToggle = true;
bool onOffPoint2Toggle = true;
bool onOffSpotToggle = true;
bool onOffDirectToggle = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    

    const int parts = 50;
    const float pi = 3.1415926535;
    const float angle = pi * 2.0f / parts;
    float points[200000]{}, radius = 1.0f;

    int ind = 0;
    points[ind++] = 0.0f;
    points[ind++] = 0.0f;
    points[ind++] = 0.0f;
    for (int i = 1; i <= parts; i++) {
        points[ind++] = radius * sin(angle * i);
        points[ind++] = -radius * cos(angle * i);
        points[ind++] = 0.0f;
    }

    for (float r = radius-0.005f, z = 0.005f; r > 0.0f; r -= 0.005f, z+=0.005f)
    {
        for (int i = 1; i <= parts + 1; i++) {
            points[ind++] = (r+0.005) * sin(angle * i);
            points[ind++] = -(r + 0.005) * cos(angle * i);
            points[ind++] = z-0.005f;

            points[ind++] = r * sin(angle * i);
            points[ind++] = -r * cos(angle * i);
            points[ind++] = z;
        }
    }
    for (float r = radius - 0.005f, z = -0.005f; r > 0.0f; r -= 0.005f, z -= 0.005f)
    {
        for (int i = 1; i <= parts + 1; i++) {
            points[ind++] = (r + 0.005) * sin(angle * i);
            points[ind++] = -(r + 0.005) * cos(angle * i);
            points[ind++] = z + 0.005f;

            points[ind++] = r * sin(angle * i);
            points[ind++] = -r * cos(angle * i);
            points[ind++] = z;
        }
    }

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /// Sphere
    unsigned int VBOCL, shpareVAO;
    glGenVertexArrays(1, &shpareVAO);
    glGenBuffers(1, &VBOCL);
    glBindVertexArray(shpareVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBOCL);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
       // pointlight4.setUpPointLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //bed(cubeVAO, lightingShader, model,0);
        glm::mat4 tmp = model, tempt;
        tempt = glm::translate(identityMatrix, glm::vec3(-6, -1, -7.5));
        tmp = tmp * tempt;
        room(cubeVAO, lightingShader, tmp);
        
        //rightwall(cubeVAO, lightingShader, model);
        
        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.

        //glBindVertexArray(lightCubeVAO);
        glBindVertexArray(shpareVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            tempt = glm::translate(identityMatrix, glm::vec3(-20, -1, -30));
            ourShader.setMat4("model", model* tempt);
            if (i == 0)
            {
                if(pointlight1.isOn())
                    ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
                else
                ourShader.setVec3("color", glm::vec3(0.2f, 0.2f, 0.2f));
            }
            if (i == 1)
            {
                if (pointlight2.isOn())
                    ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
                else
                    ourShader.setVec3("color", glm::vec3(0.2f, 0.2f, 0.2f));
            }
            if (i == 2)
            {
                if (pointlight3.isOn())
                    ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
                else
                    ourShader.setVec3("color", glm::vec3(0.2f, 0.2f, 0.2f));
            }
            if (i == 3)
            {
                if (pointlight3.isOn())
                    ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
                else
                    ourShader.setVec3("color", glm::vec3(0.2f, 0.2f, 0.2f));
            }


            //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, ind);
        }
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setVec3("material.emissive", glm::vec3(r/8, g/8, b/8));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

//BED

//void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
//{
//    float baseHeight = 0.3;
//    float width = 1;
//    float length = 2;
//    float pillowWidth = 0.3;
//    float pillowLength = 0.15;
//    float blanketWidth = 0.8;
//    float blanketLength = 0.7;
//    float headHeight = 0.6;
//
//    //base
//    glm::mat4 model = glm::mat4(1.0f);
//    glm::mat4 translate = glm::mat4(1.0f);
//    glm::mat4 translate2 = glm::mat4(1.0f);
//    glm::mat4 scale = glm::mat4(1.0f);
//    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
//    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
//    model = alTogether * scale * translate;
//    drawCube(cubeVAO, lightingShader, model, 0.345, 0.171, 0.475);
//
//    //foam
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3(0, baseHeight, 0));
//    scale = glm::scale(model, glm::vec3(width, 0.06, length));
//    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
//    model = alTogether * translate2 * scale * translate;
//    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);
//
//    //pillow 1
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3((width / 2) - (0.1 + pillowWidth / 2), baseHeight + 1 * 0.06, (length / 2) - (0.025 + pillowWidth / 2)));
//    scale = glm::scale(model, glm::vec3(pillowWidth, 0.04, pillowLength));
//    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
//    model = alTogether * translate2 * scale * translate;
//    drawCube(cubeVAO, lightingShader, model, .3, .1, .8);
//
//    //pillow 2
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3((-width / 2) + (0.1 + pillowWidth / 2), baseHeight + 1 * 0.06, (length / 2) - (0.025 + pillowWidth / 2)));
//    scale = glm::scale(model, glm::vec3(pillowWidth, 0.04, pillowLength));
//    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
//    model = alTogether * translate2 * scale * translate;
//    drawCube(cubeVAO, lightingShader, model, .3, .1, .8);
//
//    //blanket
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3(0, baseHeight + 1 * 0.06, -(length / 2 - 0.025) + blanketLength / 2));
//    scale = glm::scale(model, glm::vec3(blanketWidth, 0.015, blanketLength));
//    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
//    model = alTogether * translate2 * scale * translate;
//    drawCube(cubeVAO, lightingShader, model, 0.541, 0.169, 0.886);
//
//    //head
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3(0, 0, (length / 2 - 0.02 / 2) + 0.02));
//    scale = glm::scale(model, glm::vec3(width, headHeight, 0.02));
//    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
//    model = alTogether * translate2 * scale * translate;
//    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
//
//}

//TABLE

//void table(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
//{
//
//    float hight = 0.04;
//    float length = 0.5;
//    float width = 0.3;
//    float supporthight = 0.5;
//    float supportwidth = 0.04;
//    float supportlength = 0.04;
//
//    // base
//    glm::mat4 model = glm::mat4(1.0f);
//    glm::mat4 translate = glm::mat4(1.0f);
//    glm::mat4 translate2 = glm::mat4(1.0f);
//    glm::mat4 scale = glm::mat4(1.0f);
//    scale = glm::scale(model, glm::vec3(length, hight, length));
//    translate = glm::translate(model, glm::vec3(0.0, supporthight, 0.0));
//    model = alTogether * scale * translate;
//    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
//
//    //leg1
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3(0, 0.02, 0));
//    scale = glm::scale(model, glm::vec3(supportlength, -supporthight, supportwidth));
//    translate = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
//    model = alTogether * translate2 * scale * translate;
//    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);
//
//    //leg2
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3(0, 0.02, length-0.05));
//    scale = glm::scale(model, glm::vec3(supportlength, -supporthight, supportwidth));
//    translate = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
//    model = alTogether * translate2 * scale * translate;
//    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);
//
//    //leg3
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3(length-supportwidth, 0.02, length - 0.05));
//    scale = glm::scale(model, glm::vec3(supportlength, -supporthight, supportwidth));
//    translate = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
//    model = alTogether * translate2 * scale * translate;
//    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);
//
//    //leg4
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3(length - supportwidth, 0.02, supportlength));
//    scale = glm::scale(model, glm::vec3(supportlength, -supporthight, supportwidth));
//    translate = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
//    model = alTogether * translate2 * scale * translate;
//    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);
//
//}

void table(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{

    float hight = 0.04;
    float length = 0.5;
    float width = 0.3;
    float supporthight = 0.5;
    float supportwidth = 0.04;
    float supportlength = 0.04;

    // base
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(length, hight, length));
    translate = glm::translate(model, glm::vec3(0.0, supporthight, 0.0));
    model = alTogether * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);

    //leg1
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, 0.02, 0));
    scale = glm::scale(model, glm::vec3(supportlength, -supporthight, supportwidth));
    translate = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);

    //leg2
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, 0.02, length - 0.05));
    scale = glm::scale(model, glm::vec3(supportlength, -supporthight, supportwidth));
    translate = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);

    //leg3
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(length - supportwidth, 0.02, length - 0.05));
    scale = glm::scale(model, glm::vec3(supportlength, -supporthight, supportwidth));
    translate = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);

    //leg4
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(length - supportwidth, 0.02, supportlength));
    scale = glm::scale(model, glm::vec3(supportlength, -supporthight, supportwidth));
    translate = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);

}

//ROOM

void room(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    /// floor
    float baseHeight = 0.01;
    float width = 30.0;
    float length = 20.5;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.3, 0.7, 1);

    /*/// right wall
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(width, 0, 0));
    scale = glm::scale(model, glm::vec3(baseHeight, width, length));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);*/


    /*/// left wall
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, 0, 0));
    scale = glm::scale(model, glm::vec3(baseHeight, width, length));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);*/


    /// back wall
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, 0, 0));
    scale = glm::scale(model, glm::vec3(width, width, baseHeight));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);


    //road
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, baseHeight, length / 1.6));
    scale = glm::scale(model, glm::vec3(width, baseHeight, length/4));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.2, 0.2, 0.184);


    //road divider
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, baseHeight + baseHeight, length / 1.6 + (length / (4 * 2))));
    scale = glm::scale(model, glm::vec3(width, baseHeight, .09));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.949, 0.949, 0.949);

    //bus-body
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0 + 2, baseHeight+.6, length / 1.6 + 1));
    scale = glm::scale(model, glm::vec3(width / 3, baseHeight + 5, (length / 4) - 2));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.71, 0.647, 0.502);

    ////bus-window
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0 + 2 + 1, baseHeight + 1 + 2, length / 1.50));
    scale = glm::scale(model, glm::vec3(4 / 2.3, 10 / 5, baseHeight + .6));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.239, 0.227, 0.227);

    ////bus-window
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0 + 2 + 1 +3, baseHeight + 1 + 2, length / 1.5));
    scale = glm::scale(model, glm::vec3(4 / 2.3, 10 / 5, baseHeight + .6));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.239, 0.227, 0.227);

    ////bus-window
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0 + 2 + 1 + 6, baseHeight + 1 + 2, length / 1.50));
    scale = glm::scale(model, glm::vec3(4 / 2.3, 10 / 5, baseHeight + .6));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.239, 0.227, 0.227);





    //back1
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(12, baseHeight,4));
    scale = glm::scale(model, glm::vec3(4, 10, baseHeight+.8));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.553, 0.804, 0.91);



    //back2
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(12+4, baseHeight, 4));
    scale = glm::scale(model, glm::vec3(4, 10, baseHeight+.8));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.447, 0.757, 0.89);
    
    //back3
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(12 + 4 + 4, baseHeight, 4));
    scale = glm::scale(model, glm::vec3(4, 10, baseHeight+.8));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.325, 0.706, 0.871);

    //back4
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(12 + 4 + 4+4, baseHeight, 4));
    scale = glm::scale(model, glm::vec3(4, 10, baseHeight+.8));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.188, 0.667, 0.871);

    //cover
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(12 - 1, baseHeight+10, 4-1));
    scale = glm::scale(model, glm::vec3(16+2, baseHeight+.6, 8));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.616, 0.667, 0.69);



    //bench
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(12 - 1 + 2, baseHeight + 10  - 6 , 4 - 1 + 1.8));
    scale = glm::scale(model, glm::vec3(16 + 2 - 4, baseHeight + .2 + .2, 8 - 5));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.616, 0.667, 0.69);


    float tx = 2.5;
    float ty = 2.5;
    float tz = 2.5;

    
    //table
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(tx+-.20, ty+baseHeight, tz+0.0));
    scale = glm::scale(model, glm::vec3(3.6, 0.2, 2.6 + 4));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.616, 0.667, 0.69);


    //leg1
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(tx+0.0, ty+baseHeight, tz+0.0));
    scale = glm::scale(model, glm::vec3(0.2, -1.5, 0.2));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.616, 0.667, 0.69);


    //leg2
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(tx+1.35, ty+baseHeight, tz+0.0));
    scale = glm::scale(model, glm::vec3(0.2, -1.5, 0.2));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.616, 0.667, 0.69);


    //leg3
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(tx+1.35, ty+baseHeight, tz+1.2 + 2));
    scale = glm::scale(model, glm::vec3(0.2, -1.5, 0.2));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.616, 0.667, 0.69);


    //leg4
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(tx+0, ty+baseHeight, tz+1.2 + 2));
    scale = glm::scale(model, glm::vec3(0.2, -1.5, 0.2));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.616, 0.667, 0.69);










    //// Bed
    //model = glm::mat4(1.0f);
    //translate = glm::mat4(1.0f);
    //translate = glm::translate(model, glm::vec3(1.2, 0, 1.0));
    //glm::mat4 rotateYMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = alTogether * translate * rotateYMatrix * glm::mat4(1.0f);
    //bed(cubeVAO, lightingShader, model);

    //model = glm::mat4(1.0f);
    //translate = glm::mat4(1.0f);
    //translate = glm::translate(model, glm::vec3(length-1.6, 0, 1.0));
    //rotateYMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = alTogether * translate * rotateYMatrix * glm::mat4(1.0f);
    //bed(cubeVAO, lightingShader, model);

    //model = glm::mat4(1.0f);
    //translate = glm::mat4(1.0f);
    //translate = glm::translate(model, glm::vec3(1.2, 0, width/2));
    //rotateYMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = alTogether * translate * rotateYMatrix * glm::mat4(1.0f);
    //bed(cubeVAO, lightingShader, model);

    //model = glm::mat4(1.0f);
    //translate = glm::mat4(1.0f);
    //translate = glm::translate(model, glm::vec3(length - 1.6, 0, width/2));
    //rotateYMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = alTogether * translate * rotateYMatrix * glm::mat4(1.0f);
    //bed(cubeVAO, lightingShader, model);

    //// table
    //model = glm::mat4(1.0f);
    //translate = glm::mat4(1.0f);
    //translate = glm::translate(model, glm::vec3(length / 2 - width/6, 0.5, 0.1));
    //rotateYMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = alTogether * translate * rotateYMatrix * glm::mat4(1.0f);
    //table(cubeVAO, lightingShader, model);

    //model = glm::mat4(1.0f);
    //translate = glm::mat4(1.0f);
    //translate = glm::translate(model, glm::vec3(length / 2 + width / 8, 0.5, 0.1));
    //rotateYMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = alTogether * translate * rotateYMatrix * glm::mat4(1.0f);
    //table(cubeVAO, lightingShader, model);

    //model = glm::mat4(1.0f);
    //translate = glm::mat4(1.0f);
    //translate = glm::translate(model, glm::vec3(0.1, 0.5, width/4));
    //rotateYMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = alTogether * translate * rotateYMatrix * glm::mat4(1.0f);
    //table(cubeVAO, lightingShader, model);

    //model = glm::mat4(1.0f);
    //translate = glm::mat4(1.0f);
    //translate = glm::translate(model, glm::vec3(length-1.0, 0.5, width / 4));
    //rotateYMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = alTogether * translate * rotateYMatrix * glm::mat4(1.0f);
    //table(cubeVAO, lightingShader, model);
    
}


//void rightwall(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
//{
//    float baseHeight = 8.5;
//    float width = 8.0;
//    float length = 0.01;
//
//    glm::mat4 model = glm::mat4(1.0f);
//    glm::mat4 translate = glm::mat4(1.0f);
//    glm::mat4 translate2 = glm::mat4(1.0f);
//    glm::mat4 scale = glm::mat4(1.0f);
//    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
//    translate = glm::translate(model, glm::vec3(-0.2, 0, -0.5));
//    model = alTogether * scale * translate;
//    drawCube(cubeVAO, lightingShader, model, 0.3, 0.7, 1);
//}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    /*if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }*/
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001;
    ////if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;


    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);
    }
    /*if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }*/
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    /*if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }*/
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);
    }
    /*if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }*/

    /*if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }*/

    /*if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }*/
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        pointlight2.turnOn();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        pointlight2.turnOff();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        pointlight3.turnOn();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        pointlight3.turnOff();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        pointlight1.turnOn();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        pointlight1.turnOff();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        pointlight4.turnOn();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        pointlight4.turnOff();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }



    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if(pointlight1.isOn())
            pointlight1.turnAmbientOn();
        if(pointlight2.isOn())
            pointlight2.turnAmbientOn();
        if (pointlight3.isOn())
            pointlight3.turnAmbientOn();
        if (pointlight4.isOn())
            pointlight4.turnAmbientOn();

        //pointlight4.turnDiffuseOn();
        //diffuseToggle = !diffuseToggle;
    //}
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnAmbientOff();
        if (pointlight2.isOn())
            pointlight2.turnAmbientOff();
        if (pointlight3.isOn())
            pointlight3.turnAmbientOff();
        if (pointlight4.isOn())
            pointlight4.turnAmbientOff();
        
        //pointlight4.turnDiffuseOff();
        //diffuseToggle = !diffuseToggle;
    //}
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnDiffuseOn();
        if (pointlight2.isOn())
            pointlight2.turnDiffuseOn();
        if (pointlight3.isOn())
            pointlight3.turnDiffuseOn();
        if (pointlight4.isOn())
            pointlight4.turnDiffuseOn();
        
        //pointlight4.turnAmbientOn();
        //diffuseToggle = !diffuseToggle;
        //}
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnDiffuseOff();
        if (pointlight2.isOn())
            pointlight2.turnDiffuseOff();
        if (pointlight3.isOn())
            pointlight3.turnDiffuseOff();
        if (pointlight4.isOn())
            pointlight4.turnDiffuseOff();
        
        //diffuseToggle = !diffuseToggle;
        //}
    }


    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnSpecularOn();
        if (pointlight2.isOn())
            pointlight2.turnSpecularOn();
        if (pointlight3.isOn())
            pointlight3.turnSpecularOn();
        if (pointlight4.isOn())
            pointlight4.turnSpecularOn();
        

        //pointlight4.turnSpecularOn();
        //diffuseToggle = !diffuseToggle;
        //}
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        /*cout << "1 " << pointlight1.isOn() << endl;
        cout << pointlight2.isOn() << endl;
        cout << pointlight3.isOn() << endl;*/
        if (pointlight1.isOn())
            pointlight1.turnSpecularOff();
        if (pointlight2.isOn())
            pointlight2.turnSpecularOff();
        if (pointlight3.isOn())
            pointlight3.turnSpecularOff();
        if (pointlight4.isOn())
            pointlight4.turnSpecularOff();

        //pointlight4.turnSpecularOff();
        //diffuseToggle = !diffuseToggle;
        //}
    }
   // if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
   // {
   //     if (onOffPointToggle)
   //     {
   //         pointlight1.turnOff();
   //         
   //         onOffPointToggle = false;
   //     }
   //     else
   //     {
   //         pointlight1.turnOn();
   //       
   //         onOffPointToggle = true;
   //     }
   //    // pointlight3.turnOff();
   //    // pointlight4.turnOff();

   // }
   // 

   // if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
   // {
   //     
   //     if (onOffSpotToggle)
   //     {
   //        
   //         pointlight2.turnOff();
   //         onOffSpotToggle = false;
   //     }
   //     else
   //     {
   //         pointlight2.turnOn();
   //         onOffSpotToggle = true;
   //     }
   // }

   // if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
   // {

   //     if (onOffDirectToggle)
   //     {

   //         pointlight3.turnOff();
   //         onOffDirectToggle = false;
   //     }
   //     else
   //     {
   //         pointlight3.turnOn();
   //         onOffDirectToggle = true;
   //     }
   // }
   // 
   // if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
   // {
   //     pointlight1.turnAmbientOn();
   //     pointlight2.turnAmbientOn();
   //    // pointlight3.turnAmbientOn();
   //    // pointlight4.turnAmbientOn();
   // }
   // if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
   // {
   //     pointlight1.turnAmbientOff();
   //     pointlight2.turnAmbientOff();
   //   //  pointlight3.turnAmbientOff();
   //   //  pointlight4.turnAmbientOff();
   // }
   // if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
   // {
   //     pointlight1.turnDiffuseOn();
   //     pointlight2.turnDiffuseOn();
   //  //   pointlight3.turnDiffuseOn();
   // //    pointlight4.turnDiffuseOn();
   // }
   // if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
   // {
   //     pointlight1.turnDiffuseOff();
   //     pointlight2.turnDiffuseOff();
   ////     pointlight3.turnDiffuseOff();
   // //    pointlight4.turnDiffuseOff();
   // }
   // if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
   // {
   //     pointlight1.turnSpecularOn();
   //     pointlight2.turnSpecularOn();
   // //    pointlight3.turnSpecularOn();
   // //    pointlight4.turnSpecularOn();
   // }
   // if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
   // {
   //     pointlight1.turnSpecularOff();
   //     pointlight2.turnSpecularOff();
   ////     pointlight3.turnSpecularOff();
   // //    pointlight4.turnDiffuseOff();
   // }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
