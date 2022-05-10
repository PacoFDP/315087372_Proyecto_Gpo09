
// Std. Includes
#include <string>
#include <cmath>
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
Camera camera(glm::vec3(0.0f, 4.0f, 20.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(0.5f, 0.5f, 2.5f);
float movelightPosX = 0.0f;
float movelightPosY = 5.0f;
float movelightPosZ = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;  

//Variable animacion
// --- ventilador
float rotacionVentilador = 0;
bool ventiladorEncendido = false;

// -- ventana
float posicionVentana = 0;
bool ventanaEncendido = false;
// -- Puerta
float rotacionPuerta = 0;
bool puertaEncendido = false;

//  -- Balon
float posicionBalonX=0;
float posicionBalonY=0;

bool rebote1 = true;
bool rebote2 = false;
bool rebote3 = false;
bool deslizamiento = false;

bool rebote1Izq = false;
bool rebote2Izq = false;
bool rebote3Izq = false;
bool deslizamientoIzq = false;

bool balonEncendido = false;
float h = 4; //posicion vertical de vertice
float k = 4; //posicion horizontal de vertice
float p = 1; // foco

float rotacionBalon = 0;


//Variables para el manejo de luz
bool lamparaEncedida = false;
float brilloLampara = 0;
bool focoEncendido = false;
float brilloFoco = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,7.0f, 0.0f),
    glm::vec3(-0.0f,4.05f, -0.0f),
};


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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

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

    // Shaders necesarios para pointlights
    /*Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
    Shader lampshader("Shaders/lamp.vs", "Shaders/lamp.frag");*/

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

    //Pantalla
    Model pantallaCristal((char*)"Models/Pantalla/pantallaCristal.obj");
    Model pantallaEstructura((char*)"Models/Pantalla/pantallaEstructura.obj");

    //Ventilador
    Model ventiladorEstructura((char*)"Models/Ventilador/VentiladorEstructura.obj");
    Model ventiladorHelices((char*)"Models/Ventilador/VentiladorHelices.obj");

    //Ventanas 
    Model VentanaEstructuraFija((char*)"Models/Ventana/VentanaEstructuraFija.obj");
    Model VentanaCristalFijo((char*)"Models/Ventana/VentanaCristalFijo.obj");
    Model VentanaEstructuraMovil((char*)"Models/Ventana/VentanaEstructuraMovil.obj");
    Model VentanaCristalMovil((char*)"Models/Ventana/VentanaCristalMovil.obj");

    //Puertas
    Model PuertaEstructuraFija((char*)"Models/Puerta/PuertaEstructuraFijo.obj");
    Model PuertaCristalFijo((char*)"Models/Puerta/PuertaCristalFijo.obj");
    Model PuertaEstructuraMovil((char*)"Models/Puerta/PuertaEstructuraMovil.obj");
    Model PuertaCristalMovil((char*)"Models/Puerta/PuertaCristalMovil.obj");

    // Balon
    Model Balon((char*)"Models/Balon/Balon.obj");
    
    //Exterior interior
    Model ExteriorInterior((char*)"Models/ExteriorInterior/ExteriorInterior.obj");



    
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

        //VERIFICAR ESTA LINEA
        
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, lightPos.x + movelightPosX, lightPos.y + movelightPosY, lightPos.z + movelightPosZ);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Set lights properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 2.0f, 2.0f, 2.0f);

        // Point light 2
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), brilloLampara, brilloLampara, brilloLampara);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), brilloLampara, brilloLampara, brilloLampara);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.35f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.44f);



        // Point light 2
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), brilloFoco, brilloFoco, brilloFoco);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), brilloFoco, brilloFoco, brilloFoco);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.35f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.44f);


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
        //model = glm::translate(model, glm::vec3(-12.5f, -3.0f, -10.0f));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        
        //  ------------------  SILLON  --------------------- 
        patasSillon.Draw(lightingShader);
        
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);
        model = glm::mat4(1);
        //model = glm::translate(model, glm::vec3(-12.5f, -3.0f, -10.0f));
        cojinesSillon.Draw(lightingShader);

        // ----------------  SILLA DE ESCRITORIO --------------------
        
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        estructura.Draw(lightingShader);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);
        model = glm::mat4(1);
        cojinesAsiento.Draw(lightingShader);
        cojinesRespaldo.Draw(lightingShader);

        // ----------------  CAMA --------------------
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        // Draw the loaded model
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);

        
        baseCama.Draw(lightingShader);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);
        model = glm::mat4(1);
        arribaCama.Draw(lightingShader);


        //------------------------- MESA DE SALA ----------------------------------
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        maderaMesa.Draw(lightingShader);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        teteraYVaso.Draw(lightingShader);


        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        charolaYPlato.Draw(lightingShader);


        //-------------------- LAMPARA -----------------------------------
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        focoLampara.Draw(lightingShader);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        estructuraLampara.Draw(lightingShader);

      
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        conoLampara.Draw(lightingShader);

       
        //------------------   Escritorio   --------------------------
        //Escritorio madera
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        escritorioMadera.Draw(lightingShader);

        //Escritorio marmol
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.9f, 0.9f, 0.9f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        escritorioMarmol.Draw(lightingShader);

        // --------------------  Pantalla  -------------------------------
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        pantallaCristal.Draw(lightingShader);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        pantallaEstructura.Draw(lightingShader);



        // ------------------------   Exterior
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.9f, 0.9f, 0.9f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        ExteriorInterior.Draw(lightingShader);

        //- --------------------- Ventilador -------------------
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 16.5f, -10.0f));
        //model = glm::rotate(model, glm::radians(rotacionVentilador), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        ventiladorEstructura.Draw(lightingShader);


        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 16.5f, -10.0f));
        model = glm::rotate(model, glm::radians(rotacionVentilador), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        ventiladorHelices.Draw(lightingShader);

        // -------------------  Ventana ---------------
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        VentanaEstructuraFija.Draw(lightingShader);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        VentanaCristalFijo.Draw(lightingShader);

                             // -- Parte movible
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posicionVentana, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        VentanaEstructuraMovil.Draw(lightingShader);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posicionVentana, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        VentanaCristalMovil.Draw(lightingShader);

        // ----------------------------   Puerta  ---
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        PuertaEstructuraFija.Draw(lightingShader);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        PuertaCristalFijo.Draw(lightingShader);

        // -- Parte movible
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(12.1, 0.0f,22.71f));
        model = glm::rotate(model, glm::radians(rotacionPuerta), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        PuertaEstructuraMovil.Draw(lightingShader);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(12.1, 0.0f,22.71f));
        model = glm::rotate(model, glm::radians(rotacionPuerta), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        PuertaCristalMovil.Draw(lightingShader);

        // -- Balon 
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posicionBalonX, posicionBalonY, 0.0f));
        //model = glm::rotate(model, glm::radians(rotacionBalon), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        Balon.Draw(lightingShader);

        //VERIFICAR TODO ESTO
        lampshader.Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        GLint modelLoc = glGetUniformLocation(lampshader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lampshader.Program, "view");
        GLint projLoc = glGetUniformLocation(lampshader.Program, "projection");

        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4(1);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //dibuja los puntos de luz
        for (GLuint i = 0; i < 2; i++)
        {
            model = glm::mat4(1);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        glBindVertexArray(0);




        //lampshader.Use();
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
        //glDrawArrays(GL_TRIANGLES, 0, 36);

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

    //-- actualizacion de rotacion ventilador
    if (ventiladorEncendido == true) {
        rotacionVentilador += 1.5;
    }

    // -- actualizacion de posicion en ventana
    if (ventanaEncendido == false) {
        if (posicionVentana >= -5.5) {
            posicionVentana -= 0.01;
        }
    }
    if (ventanaEncendido == true) {
        if (posicionVentana <= 0.0) {
            posicionVentana += 0.01;
        }
    }

    //   --------- movimiento de la puerta

    if (puertaEncendido == true) {
        if (rotacionPuerta <= 90) {
            rotacionPuerta += 0.1;
        }
    }
    if (puertaEncendido == false) {
        if (rotacionPuerta >= 0) {
            rotacionPuerta -= 0.1;
        }
    }
    // Balon en movimiento
    if (balonEncendido == true) {
        if (rebote1 == true) { // Realiza primera parabola der(rebote1)
            posicionBalonX += 0.1;
            posicionBalonY = (pow((posicionBalonX - h), 2) / -4 * p) + k; // Los valores de h,k y p son con los que se difinieron las varaibles
            if (posicionBalonX >= 8) { //Comienza segundo rebote
                rebote2 = true;
                rebote1 = false;
                h = 11; //posicion horizontal de vertice
                k = 2; //posicion vertical de vertice
                p = 1.1; // foco
            }
        }

        if (rebote2 == true) { //Realiza segunda parabola der(rebote2)
            posicionBalonX += 0.07;
            posicionBalonY = (pow((posicionBalonX - h), 2) / -4 * p) + k; // Loa valores h,k y p se definen en rebote1
            if (posicionBalonX >= 14) { //comienza rebote 3
                rebote3 = true;
                rebote2 = false;
                h = 15; //posicion horizontal de vertice
                k = 1; //posicion vertical de vertice
                p = 0.27; // foco
            }
        }

        if (rebote3 == true) { //Realiza tercera parabola(rebot3)
            posicionBalonX += 0.04;
            posicionBalonY = (pow((posicionBalonX - h), 2) / -4 * p) + k; // Los valores h,k, y p se definen en rebote2
            if (posicionBalonX >= 16) { // comienza el deslizamiento
                deslizamiento = true;
                rebote3 = false;
                posicionBalonY = 0; //asegurar que baje hasta el suelo y no flote por la presion con la que se calcula y
            }
        }

        if (deslizamiento == true) { // Realiza deslizamiento ya para deternerse
            posicionBalonX += 0.01;
            if (posicionBalonX >= 18) { //Si pasa de 18 se detiene deslizamiento y activa reboteIzq para que este listo en el momento que indique el usuario
                deslizamiento = false;
                rebote1Izq = true;
                h = 14; //posicion vertical de vertice
                k = 4; //posicion horizontal de vertice
                p = 1; // foco
            }
        } 

        
    }
    if (balonEncendido == false) { // Se regresa el balon 
        if (rebote1Izq == true) { //Realiza primera parabola pero ahora de regreso
            posicionBalonX -= 0.1;
            posicionBalonY = (pow((posicionBalonX - h), 2) / -4 * p) + k;
            if (posicionBalonX <= 10) { // Activa segunda parabola de regreso
                rebote2Izq = true;
                rebote1Izq = false;
                h = 7; //posicion vertical de vertice
                k = 2; //posicion horizontal de vertice
                p = 1.1; // foco
            }
        }
        if (rebote2Izq == true) { //Realiza segunda parabola
            posicionBalonX -= 0.07;
            posicionBalonY = (pow((posicionBalonX - h), 2) / -4 * p) + k;
            if (posicionBalonX <= 4) { //Activa tercera parabola de regreso
                rebote3Izq = true;
                rebote2Izq = false;
                h = 3; //posicion vertical de vertice
                k = 1; //posicion horizontal de vertice
                p = 0.27; // foco
            }
        }

        if (rebote3Izq == true) { //Realiza tercera parabola de regreso
            posicionBalonX -= 0.04;
            posicionBalonY = (pow((posicionBalonX - h), 2) / -4 * p) + k;
            if (posicionBalonX <= 2.0) { //activa deslizamiento de regreso hasta llegar a su posicion inicial
                deslizamientoIzq = true;
                rebote3Izq = false;
                posicionBalonY = 0; //asegurar que baje hasta el suelo y no flote por la presion con la que se calcula y
            }
        }

        if (deslizamientoIzq == true) { //realiza deslizamiento antes de detenerse
            posicionBalonX -= 0.01;
            if (posicionBalonX <= 0) { // Activa primer rebote de ida para funciona cuando el usuario presione V de nuevo
                deslizamientoIzq = false;
                rebote1 = true;
                h = 4; //posicion vertical de vertice
                k = 4; //posicion horizontal de vertice
                p = 1; // foco
            }
        }
        //if (rebote1Izq == true) {
        //    posicionBalonX -= 0.1;
        //    posicionBalonY = (pow((posicionBalonX - h), 2) / -4 * p) + k;
        //    if (posicionBalonX <= 8) {
        //        rebote2Izq = true;
        //        rebote1Izq = false;
        //        h = 5; //posicion vertical de vertice
        //        k = 2; //posicion horizontal de vertice
        //        p = 1.1; // foco
        //    }


        //}

        //if (rebote2Izq == true) {
        //    posicionBalonX += 0.07;
        //    posicionBalonY = (pow((posicionBalonX - h), 2) / -4 * p) + k;
        //    if (posicionBalonX <= 2) {
        //        rebote3Izq = true;
        //        rebote2Izq = false;
        //        h = 1; //posicion vertical de vertice
        //        k = 1; //posicion horizontal de vertice
        //        p = 0.27; // foco
        //    }
        //}

        //if (rebote3Izq == true) {
        //    posicionBalonX += 0.04;
        //    posicionBalonY = (pow((posicionBalonX - h), 2) / -4 * p) + k;
        //    if (posicionBalonX <= 0) {
        //        deslizamientoIzq = true;
        //        rebote3Izq = false;
        //        posicionBalonY = 0;
        //    }
        //}

        //if (deslizamientoIzq == true) {
        //    posicionBalonX += 0.01;
        //    if (posicionBalonX >= 19) {
        //        deslizamientoIzq = false;
        //        rebote1 = true;
        //    }
        //}
    }

    /*if (lamparaEncedida = true) {
        brilloLampara = 1.0f;
    }
    if (lamparaEncedida = false) {
        brilloLampara = 0.0f;
    }

    if (focoEncendido == true) {
        brilloFoco = 1.0;
    }
    if (focoEncendido == false) {
        brilloFoco = 0.0f;
    }*/
    
    
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

    if (keys[GLFW_KEY_X])
    {
        // Se activa o desactiva ventilador
        ventiladorEncendido = !ventiladorEncendido;
    }

    if (keys[GLFW_KEY_P])
    {
        // Se activa o desactiva ventilador
        ventanaEncendido = !ventanaEncendido;
    }
        
    if (keys[GLFW_KEY_O])
    {
        // Se activa o desactiva puerta
        puertaEncendido = !puertaEncendido;
    }

    if (keys[GLFW_KEY_V])
    {
        // Se activa o desactiva puerta
        balonEncendido = !balonEncendido;
    }

    //Encender lampara
    if (keys[GLFW_KEY_F])
    {
        // Se activa o desactiva puerta
        lamparaEncedida = !lamparaEncedida;
        printf("Lampara encendida");
    }
    //Encender foco habitacion
    if (keys[GLFW_KEY_R])
    {
        // Se activa o desactiva puerta
        focoEncendido = !focoEncendido;
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


