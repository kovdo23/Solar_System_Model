#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "shader.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <atomic> 

#include <iostream>

std::atomic<bool> startGame(false);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

unsigned int VBO, VAO;
unsigned int lightVAO;


void gameOver(GLFWwindow* window);
void resetGame(GLFWwindow* window);

void initbuffers(GLFWwindow* window);
void draw(GLFWwindow* window);
void checkcollision(GLFWwindow* window);

static unsigned int loadTexture(const char* path);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float fov = 45.0f;


float deltaTime = 0.0f;
float lastFrame = 0.0f;


Camera camera(glm::vec3(0.0f, 10.0f, 20.0f));

glm::vec3 lightPos(0.0f, 0.0f, 0.0f);


glm::vec3 cubePositions[] = {
    glm::vec3(4.0f,  0.0f, 0.0f),
    glm::vec3(6.0f,  0.0f, 0.0f),
    glm::vec3(8.0f,  0.0f, 0.0f),
    glm::vec3(10.0f,  0.0f, 0.0f),
    glm::vec3(12.0f,  0.0f, 0.0f),
    glm::vec3(14.0f,  0.0f, 0.0f),
    glm::vec3(16.0f,  0.0f, 0.0f),
    glm::vec3(18.0f,  0.0f, 0.0f),
    glm::vec3(0.0f,  10.0f, 20.0f),
};





int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


    Shader lightingShader("Shaders/shader.vs", "Shaders/shader.fs");
    Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.fs");


    initbuffers(window);
   


    unsigned int galaxis = loadTexture("imgs/galaxis.jpg");
    unsigned int merkur = loadTexture("imgs/merkur.jpg");
    unsigned int venusz = loadTexture("imgs/venusz.jpg");
    unsigned int fold = loadTexture("imgs/fold.jpg");
    unsigned int mars = loadTexture("imgs/mars.jpg");
    unsigned int jupiter = loadTexture("imgs/jupiter.jpg");
    unsigned int szaturnusz = loadTexture("imgs/szaturnusz.jpg");
    unsigned int uranusz = loadTexture("imgs/uranusz.jpg");
    unsigned int neptunusz = loadTexture("imgs/neptunusz.jpg");

    lightingShader.use();



    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        lightingShader.use();
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);


        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);

        float distanceFromSun1 = glm::length(cubePositions[0]);
        float distanceFromSun2 = glm::length(cubePositions[1]);
        float distanceFromSun3 = glm::length(cubePositions[2]);
        float distanceFromSun4 = glm::length(cubePositions[3]);
        float distanceFromSun5 = glm::length(cubePositions[4]);
        float distanceFromSun6 = glm::length(cubePositions[5]);
        float distanceFromSun7 = glm::length(cubePositions[6]);
        float distanceFromSun8 = glm::length(cubePositions[7]);


        glBindVertexArray(VAO);
        //#1 cube
        glActiveTexture(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, merkur);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 c1 = glm::mat4(1.0f);
        c1 = glm::translate(c1, glm::vec3(cos((float)glfwGetTime() * 0.5) * distanceFromSun1, cubePositions[0].y, sin((float)glfwGetTime() * 0.5) * distanceFromSun1));
        c1 = glm::scale(c1, glm::vec3(0.15f));
        c1 = glm::rotate(c1, (float)glfwGetTime() * glm::radians(13.0f), glm::vec3(1.0f, 0.3f, 0.5f));

        cubePositions[0] = glm::vec3(cos((float)glfwGetTime() * 0.5) * distanceFromSun1, cubePositions[0].y, sin((float)glfwGetTime() * 0.5) * distanceFromSun1);

        lightingShader.setMat4("model", c1);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //#2 cube

        glActiveTexture(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, venusz);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 c2 = glm::mat4(1.0f);
        c2 = glm::translate(c2, glm::vec3(cos((float)glfwGetTime() * 0.4) * distanceFromSun2, cubePositions[1].y, sin((float)glfwGetTime() * 0.4) * distanceFromSun2));
        c2 = glm::scale(c2, glm::vec3(0.4f));
        c2 = glm::rotate(c2, (float)glfwGetTime() * glm::radians(5.0f), glm::vec3(1.0f, 0.3f, 0.5f));

        cubePositions[1] = glm::vec3(cos((float)glfwGetTime() * 0.4) * distanceFromSun2, cubePositions[1].y, sin((float)glfwGetTime() * 0.4) * distanceFromSun2);

        lightingShader.setMat4("model", c2);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //#3 cube

        glActiveTexture(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, fold);
        glEnable(GL_DEPTH_TEST);


        glm::mat4 c3 = glm::mat4(1.0f);
        c3 = glm::translate(c3, glm::vec3(cos((float)glfwGetTime() * 0.35) * distanceFromSun3, cubePositions[2].y, sin((float)glfwGetTime() * 0.35) * distanceFromSun3));
        c3 = glm::scale(c3, glm::vec3(0.4f));
        c3 = glm::rotate(c3, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));

        cubePositions[2] = glm::vec3(cos((float)glfwGetTime() * 0.35) * distanceFromSun3, cubePositions[2].y, sin((float)glfwGetTime() * 0.35) * distanceFromSun3);

        lightingShader.setMat4("model", c3);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //#4 cube

        glActiveTexture(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, mars);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 c4 = glm::mat4(1.0f);
        c4 = glm::translate(c4, glm::vec3(cos((float)glfwGetTime() * 0.3) * distanceFromSun4, cubePositions[3].y, sin((float)glfwGetTime() * 0.3) * distanceFromSun4));
        c4 = glm::scale(c4, glm::vec3(0.4f));
        c4 = glm::rotate(c4, (float)glfwGetTime() * glm::radians(19.0f), glm::vec3(1.0f, 0.3f, 0.5f));

        cubePositions[3] = glm::vec3(cos((float)glfwGetTime() * 0.3) * distanceFromSun4, cubePositions[3].y, sin((float)glfwGetTime() * 0.3) * distanceFromSun4);

        lightingShader.setMat4("model", c4);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //#5 cube

        glActiveTexture(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, jupiter);
        glEnable(GL_DEPTH_TEST);
        glm::mat4 c5 = glm::mat4(1.0f);
        c5 = glm::translate(c5, glm::vec3(cos((float)glfwGetTime() * 0.08) * distanceFromSun5, cubePositions[4].y, sin((float)glfwGetTime() * 0.08) * distanceFromSun5));
        c5 = glm::scale(c5, glm::vec3(0.7f));
        c5 = glm::rotate(c5, (float)glfwGetTime() * glm::radians(32.0f), glm::vec3(1.0f, 0.3f, 0.5f));

        cubePositions[4] = glm::vec3(cos((float)glfwGetTime() * 0.08) * distanceFromSun5, cubePositions[4].y, sin((float)glfwGetTime() * 0.08) * distanceFromSun5);

        lightingShader.setMat4("model", c5);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //#6 cube

        glActiveTexture(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, szaturnusz);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 c6 = glm::mat4(1.0f);
        c6 = glm::translate(c6, glm::vec3(cos((float)glfwGetTime() * 0.05) * distanceFromSun6, cubePositions[5].y, sin((float)glfwGetTime() * 0.05) * distanceFromSun6));
        c6 = glm::scale(c6, glm::vec3(0.5f));
        c6 = glm::rotate(c6, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(1.0f, 0.3f, 0.5f));

        cubePositions[5] = glm::vec3(cos((float)glfwGetTime() * 0.05) * distanceFromSun6, cubePositions[5].y, sin((float)glfwGetTime() * 0.05) * distanceFromSun6);

        lightingShader.setMat4("model", c6);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //#7 cube

        glActiveTexture(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, uranusz);
        glEnable(GL_DEPTH_TEST);
        glm::mat4 c7 = glm::mat4(1.0f);
        c7 = glm::translate(c7, glm::vec3(cos((float)glfwGetTime() * 0.03) * distanceFromSun7, cubePositions[6].y, sin((float)glfwGetTime() * 0.03) * distanceFromSun7));
        c7 = glm::scale(c7, glm::vec3(0.4f));
        c7 = glm::rotate(c7, (float)glfwGetTime() * glm::radians(-24.0f), glm::vec3(1.0f, 0.3f, 0.5f));

        cubePositions[6] = glm::vec3(cos((float)glfwGetTime() * 0.03) * distanceFromSun7, cubePositions[6].y, sin((float)glfwGetTime() * 0.03) * distanceFromSun7);

        lightingShader.setMat4("model", c7);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //#8 cube

        glActiveTexture(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, neptunusz);
        glEnable(GL_DEPTH_TEST);


        lightingShader.use();


        glm::mat4 c8 = glm::mat4(1.0f);
        c8 = glm::translate(c8, glm::vec3(cos((float)glfwGetTime() * 0.009) * distanceFromSun8, cubePositions[7].y, sin((float)glfwGetTime() * 0.009) * distanceFromSun8));
        c8 = glm::scale(c8, glm::vec3(0.45f));
        c8 = glm::rotate(c8, (float)glfwGetTime() * glm::radians(25.0f), glm::vec3(1.0f, 0.3f, 0.5f));

        cubePositions[7] = glm::vec3(cos((float)glfwGetTime() * 0.009) * distanceFromSun8, cubePositions[7].y, sin((float)glfwGetTime() * 0.009) * distanceFromSun8);

        lightingShader.setMat4("model", c8);
        glDrawArrays(GL_TRIANGLES, 0, 36);



        lightingShader.setVec3("objectColor", 0.0f, 0.0f, 0.5f);


        //#9 cube

        glActiveTexture(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, galaxis);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 c9 = glm::mat4(1.0f);
        c9 = glm::translate(c9, cubePositions[8]);
        c9 = glm::scale(c9, glm::vec3(100.0f));

        lightingShader.setMat4("model", c9);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        cubePositions[8] = camera.Position;


        checkcollision(window);



       // lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);

        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.022f);
        lightingShader.setFloat("light.quadratic", 0.0019f);


        lightingShader.setVec3("light.position", camera.Position);
        lightingShader.setVec3("light.direction", camera.Front);
       
        lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(10.0f)));
        lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(15.0f)));


        lightingShader.setVec3("material.ambient", 0.3f, 0.3f, 0.3f);
        lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        lightingShader.setFloat("material.shininess", 20.0f);


        lightingShader.setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("light.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("light.specular", 0.5f, 0.5f, 0.5f);





        glm::mat4 model = glm::mat4(1.0f);

        lightPos.x = 0.0f;
        lightPos.y = 0.0f;
        lightPos.z = 0.0f;

        
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(3.0f));
        lampShader.setMat4("model", model);
        

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);


    glfwTerminate();
    return 0;
}

void checkcollision(GLFWwindow* window) {
    if (glm::distance(camera.Position, cubePositions[0]) < 0.15f) {
        std::cout << "Game Over! Nekiutkoztel A Merkurnak." << std::endl;
        gameOver(window);
    }
    if (glm::distance(camera.Position, cubePositions[1]) < 0.4f) {
        std::cout << "Nekiutkoztel a Venusznak." << std::endl;
        gameOver(window);
    }
    if (glm::distance(camera.Position, cubePositions[2]) < 0.4f) {
        std::cout << "Nekiutkoztel a Foldnek." << std::endl;
        gameOver(window);
    }
    if (glm::distance(camera.Position, cubePositions[3]) < 0.4f) {
        std::cout << "Nekiutkoztel a Marsnak." << std::endl;
        gameOver(window);
    }
    if (glm::distance(camera.Position, cubePositions[4]) < 0.7f) {
        std::cout << "Nekiutkoztel a Jupiternek." << std::endl;
        gameOver(window);
    }
    if (glm::distance(camera.Position, cubePositions[5]) < 0.5f) {
        std::cout << "Nekiutkoztel a Szaturnusznak." << std::endl;
        gameOver(window);
    }
    if (glm::distance(camera.Position, cubePositions[6]) < 0.4f) {
        std::cout << "Nekiutkoztel az Urnánusznak." << std::endl;
        gameOver(window);
    }
    if (glm::distance(camera.Position, cubePositions[7]) < 0.45f) {
        std::cout << "Nekiutkoztel a Neptunusznak." << std::endl;
        gameOver(window);
    }

    if (glm::distance(camera.Position, lightPos) < 2.0f) {
        std::cout << "Nekiutkoztel a napnak." << std::endl;
        gameOver(window);
    }
}

void initbuffers(GLFWwindow* window) {
    float vertices[] = {
           -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
           0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
           -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
           -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

           -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
           0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
           -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
           -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

           -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
           -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
           -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
           -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
           -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
           -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

           0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
           0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
           0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
           0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
           0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
           0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

           -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
           0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
           0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
           0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
           -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
           -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

           -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
           -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
           -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };



    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    // we only need to bind to the VBO, the container's VBO's data already contains the correct data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attributes (only position data for our lamp)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}







void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


unsigned int loadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // set the texture wrapping/filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}


void resetGame(GLFWwindow* window) {

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    camera.Position = glm::vec3(0.0f, 10.0f, 20.0f);
    bool firstMouse = true;
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

   
    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

    cubePositions[0] = glm::vec3(4.0f, 0.0f, 0.0f),
    cubePositions[1] = glm::vec3(6.0f, 0.0f, 0.0f),
        cubePositions[2] = glm::vec3(8.0f, 0.0f, 0.0f),
        cubePositions[3] = glm::vec3(10.0f, 0.0f, 0.0f),
        cubePositions[4] = glm::vec3(12.0f, 0.0f, 0.0f),
        cubePositions[5] = glm::vec3(14.0f, 0.0f, 0.0f),
        cubePositions[6] = glm::vec3(16.0f, 0.0f, 0.0f),
        cubePositions[7] = glm::vec3(18.0f, 0.0f, 0.0f),
        cubePositions[8] = glm::vec3(0.0f, 10.0f, 20.0f);

}


void gameOver(GLFWwindow* window) {
    std::cout << "Nyomj Entert az ujrakezdeshez vagy Esc-et a kilepeshez." << std::endl;



    while (!(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) && !glfwWindowShouldClose(window)) {
        glfwPollEvents();
        Shader lightingShader("Shaders/shader.vs", "Shaders/shader.fs");
        Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.fs");

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    while (true) {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            resetGame(window);
            return;
        }
        else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
            return;
        }
    }
}


