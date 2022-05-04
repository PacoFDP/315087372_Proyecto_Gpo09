
// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(0.5f, 0.5f, 2.5f);
float movelightPosX = 0.0f;
float movelightPosY = 0.0f;
float movelightPosZ = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Iluminacion", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");
    Shader lampshader("Shaders/Practica 8/lamp.vs", "Shaders/Practica 8/lamp.frag");
    Shader lightingShader("Shaders/Practica 8/lighting.vs", "Shaders/Practica 8/lighting.frag");

    //comentario para corregir fecha commit

    // Load models

    //MODELOS SILLON
    Model cojinesSillon((char*)"Models/Sillon/cojinesSillon.obj");
    Model patasSillon((char*)"Models/Sillon/patasSillon.obj");
    
    //MODELOS SILLA
    Model cojinesAsiento((char*)"Models/Silla/cojinAsiento.obj");
    Model cojinesRespaldo((char*)"Models/Silla/cojinRespaldo.obj");
    Model estructura((char*)"Models/Silla/estructuraSilla.obj");

    //CAMA
    Model baseCama((char*)"Models/Bed/baseCama.obj");
    Model arribaCama((char*)"Models/Bed/arribaCama.obj");

    //MESA SALA
    Model maderaMesa((char*)"Models/MesaSala/MaderaMesa.obj");
    Model teteraYVaso((char*)"Models/MesaSala/TeteraYVaso.obj");
    Model charolaYPlato((char*)"Models/MesaSala/CharolaYPlato.obj"); 

    //LAMPARA
    Model estructuraLampara((char*)"Models/Lampara/EstructuraMetal.obj");
    Model conoLampara((char*)"Models/Lampara/ConoLampara.obj");
    Model focoLampara((char*)"Models/Lampara/FocoLampara.obj");

    //Escritorio
    Model escritorioMadera((char*)"Models/Escritorio/EscritorioMadera.obj");
    Model escritorioMarmol((char*)"Models/Escritorio/EscritorioMarmol.obj");

    
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("images/goku.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, lightPos.x + movelightPosX, lightPos.y + movelightPosY, lightPos.z + movelightPosZ);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Set lights properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 2.0f, 2.0f, 2.0f);




        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.0215f, 0.1745f, 0.0215f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        // Draw the loaded model
        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(-12.5f, -3.0f, -10.0f));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        
        //  ------------------  SILLON  --------------------- 
        patasSillon.Draw(lightingShader);
        
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-12.5f, -3.0f, -10.0f));
        cojinesSillon.Draw(lightingShader);

        // ----------------  SILLA DE ESCRITORIO --------------------
        
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, 4.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        estructura.Draw(lightingShader);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, 4.0f));
        cojinesAsiento.Draw(lightingShader);
        cojinesRespaldo.Draw(lightingShader);

        // ----------------  CAMA --------------------
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        // Draw the loaded model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-9.5f, -3.0f, 10.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);

        
        baseCama.Draw(lightingShader);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-9.5f, -3.0f, 10.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        arribaCama.Draw(lightingShader);


        //------------------------- MESA DE SALA ----------------------------------
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-5.5f, -2.5f, -12.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        maderaMesa.Draw(lightingShader);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-6.0f, -2.5f, -12.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        teteraYVaso.Draw(lightingShader);


        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-6.0f, -2.5f, -12.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        charolaYPlato.Draw(lightingShader);


        //-------------------- LAMPARA -----------------------------------
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(8.0f, 2.3f, 2.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        focoLampara.Draw(lightingShader);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(8.0f, 2.3f, 2.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        estructuraLampara.Draw(lightingShader);

      
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(8.0f, 2.3f, 2.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        conoLampara.Draw(lightingShader);

       
        //------------------   Escritorio   --------------------------
        //Escritorio madera
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(7.0f, -1.2f, 7.0f));
        //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        escritorioMadera.Draw(lightingShader);

        //Escritorio marmol
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.9f, 0.9f, 0.9f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(7.0f, -1.2f, 7.0f));
        //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        escritorioMarmol.Draw(lightingShader);

        glBindVertexArray(0);




        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        model = glm::mat4(1.0f);
        //model = glm::translate(model, lightPos + movelightPos);
        model = glm::translate(model, glm::vec3(lightPos.x + movelightPosX, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, lightPos.y + movelightPosY, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, lightPos.z + movelightPosZ));

        model = glm::scale(model, glm::vec3(0.05f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /*model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.0f, 0.0f, -5.0F));

        model = glm::scale(model, glm::vec3(0.9f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);*/

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
    //EJE X
    if (keys[GLFW_KEY_H])
    {
        //activanim = true;
        movelightPosX -= 0.1f;
    }
    if (keys[GLFW_KEY_L])
    {
        //activanim = true;
        movelightPosX += 0.1f;
    }

    // EJE Z
    if (keys[GLFW_KEY_U])
    {
        //activanim = true;
        movelightPosZ += 0.1f;
    }
    if (keys[GLFW_KEY_J])
    {
        //activanim = true;
        movelightPosZ -= 0.1f;
    }

    // EJE Y
    if (keys[GLFW_KEY_M])
    {
        //activanim = true;
        movelightPosY += 0.1f;
    }
    if (keys[GLFW_KEY_N])
    {
        //activanim = true;
        movelightPosY -= 0.1f;
    }

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}


