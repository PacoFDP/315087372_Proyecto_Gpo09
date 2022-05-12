//  -- SECCION DE LIBRERIAS --
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


// -- CARACTERISTICAS VENTANA DE EJECUCIÓN --
const GLuint WIDTH = 900, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// -- DECLARACIÓN DE FUNCIONES --
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// -- CARACTERISTICAS DE LA CAMARA --
Camera camera(glm::vec3(0.0f, 4.0f, 20.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// -- ATRIBUTOS DE LA ILUMINACIÓN GLOBAL --
glm::vec3 lightPos(0.5f, 5.5f, 2.5f);
float movelightPosX = 0.0f;
float movelightPosY = 5.0f;
float movelightPosZ = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;  

// -- SECCION VARAIBLES DE ANIMACIONES --

// Animación ventilador
float rotacionVentilador = 0;
bool ventiladorEncendido = false;

// Animación ventana
float posicionVentana = 0;
bool ventanaEncendido = false;

// Animación puerta
float rotacionPuerta = 0;
bool puertaEncendido = false;

// Animación balon
bool balonEncendido = false;
float posicionBalonX=0;
float posicionBalonY=0;

// -rebotes de izquierda a derecha
bool rebote1 = false;
bool rebote2 = false;
bool rebote3 = false;
bool deslizamiento = false;

//-rebotes de derecha a izquierda
bool rebote1Izq = false;
bool rebote2Izq = false;
bool rebote3Izq = false;
bool deslizamientoIzq = false;

//-caracteristicas parabola
float h = 4; 
float k = 4; 
float p = 1; 

// pateo de niños
float rotacionPatadaP1 = 0;
float rotacionPatadaP2 = 0;
bool patea1 = true;
bool patea2 = false;

// Animación sillas
bool activaSilla = false;
float posicionSilla = 0;
float posicionSilla2 = 0;
bool activaSilla2 = false;

// Animacion niño
bool ninoEncendido = false;
bool segmento0 = true;
bool segmento1 = false;
bool segmento2 = false;
bool segmento3 = false;
bool segmento4 = false;
bool segmento5 = false;
bool segmento6 = false;
bool segmento7 = false;
bool segmento8 = false;


float posicionCorredorX = -27.0;
float posicionCorredorY = 2.8;
float posicionCorredorZ = -35.0;
float rotacionCorredor = 270;
float pasoDerecho = 0;
bool direccionPaso = true;
bool paro = false;


// -- FUNCION PRINCIPAL
int main()
{
    // Se iniciar GLFW
    glfwInit();
    // Se configuran todas las opciones requeridas por glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Creacion del objeto GLFWwindow que puede ser usado por las funciones GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Se establecen las funciones de devolución de llamada requeridas
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // Configuracion necesaria que GLEW pueda recuperar punteros de funciones y extensiones
    glewExperimental = GL_TRUE;
    // Inicialice GLEW para configurar los punteros de la función OpenGL
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define las dimensiones del viewport 
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Opciones de openGL
    glEnable(GL_DEPTH_TEST);

    // -- SECCION SHADERS --
    // Se indican los shaders a utilizar
    Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");
    Shader lampshader("Shaders/Proyecto/lamp.vs", "Shaders/Proyecto/lamp.frag");
    Shader lightingShader("Shaders/Proyecto/lighting.vs", "Shaders/Proyecto/lighting.frag");

    // -- SECCION CARGA DE MODELOS --
    // Necesario que modelo a cargar sea .obj
    //Modelo sillon
    Model cojinesSillon((char*)"Models/Sillon/cojinesSillon.obj");
    Model patasSillon((char*)"Models/Sillon/patasSillon.obj");
    
    //Modelos sillas
    Model cojinesAsiento((char*)"Models/Silla/cojinAsiento.obj");
    Model cojinesRespaldo((char*)"Models/Silla/cojinRespaldo.obj");
    Model estructura((char*)"Models/Silla/estructuraSilla.obj");
    Model cojinesAsiento2((char*)"Models/Silla/cojinAsiento2.obj");
    Model cojinesRespaldo2((char*)"Models/Silla/cojinRespaldo2.obj");
    Model estructura2((char*)"Models/Silla/estructuraSilla2.obj");

    //Modelo cama
    Model baseCama((char*)"Models/Bed/baseCama.obj");
    Model arribaCama((char*)"Models/Bed/arribaCama.obj");

    //Modelo mesa sala
    Model maderaMesa((char*)"Models/MesaSala/MaderaMesa.obj");
    Model teteraYVaso((char*)"Models/MesaSala/TeteraYVaso.obj");
    Model charolaYPlato((char*)"Models/MesaSala/CharolaYPlato.obj"); 

    //Modelo lampara
    Model estructuraLampara((char*)"Models/Lampara/EstructuraMetal.obj");
    Model conoLampara((char*)"Models/Lampara/ConoLampara.obj");
    Model focoLampara((char*)"Models/Lampara/FocoLampara.obj");

    //Modelo escritorio
    Model escritorioMadera((char*)"Models/Escritorio/EscritorioMadera.obj");
    Model escritorioMarmol((char*)"Models/Escritorio/EscritorioMarmol.obj");

    //Modelo pantalla
    Model pantallaCristal((char*)"Models/Pantalla/pantallaCristal.obj");
    Model pantallaEstructura((char*)"Models/Pantalla/pantallaEstructura.obj");

    //Modelo ventilador
    Model ventiladorEstructura((char*)"Models/Ventilador/VentiladorEstructura.obj");
    Model ventiladorHelices((char*)"Models/Ventilador/VentiladorHelices.obj");

    //Modelo ventana
    Model VentanaEstructuraFija((char*)"Models/Ventana/VentanaEstructuraFija.obj");
    Model VentanaCristalFijo((char*)"Models/Ventana/VentanaCristalFijo.obj");
    Model VentanaEstructuraMovil((char*)"Models/Ventana/VentanaEstructuraMovil.obj");
    Model VentanaCristalMovil((char*)"Models/Ventana/VentanaCristalMovil.obj");

    //Modelo puerta
    Model PuertaEstructuraFija((char*)"Models/Puerta/PuertaEstructuraFijo.obj");
    Model PuertaCristalFijo((char*)"Models/Puerta/PuertaCristalFijo.obj");
    Model PuertaEstructuraMovil((char*)"Models/Puerta/PuertaEstructuraMovil.obj");
    Model PuertaCristalMovil((char*)"Models/Puerta/PuertaCristalMovil.obj");

    // Modelo balon
    Model Balon((char*)"Models/Balon/Balon.obj");

    //Modelo niño
    Model cuerpoPersona1((char*)"Models/Personas/cuerpoPersona1.obj");
    Model piernaPersona1((char*)"Models/Personas/piernaPersona1.obj");
    
    //Modelo exterior e interior
    Model ExteriorInterior((char*)"Models/ExteriorInterior/ExteriorInterior.obj");

    //Modelo niño corriendo
    Model cuerpoCorredor((char*)"Models/Corredor/cuerpoCorredor.obj");
    Model piernaDerechaCorredor((char*)"Models/Corredor/piernaDerechaCorredor.obj");
    Model piernaIzquierdaCorredor((char*)"Models/Corredor/piernaIzquierdaCorredor.obj");


    // Tipo de proyección
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

    // Se configura el VAO y VBO
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Se define la posicion de los atributos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Atributo normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Configuracion carga de texturas
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
        // Configuracion del tiempo de frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Revisa y llama los eventos de las funciones previamente definidas
        glfwPollEvents();
        DoMovement();

        // Limpiar el colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Inicia el shader 
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, lightPos.x + movelightPosX, lightPos.y + movelightPosY, lightPos.z + movelightPosZ);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Configuración de la iluminacion general
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.7f, 0.7f, 0.7f);


        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // -- SECCION DIBUJADO DE MODELOS --
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.0215f, 0.1745f, 0.0215f);
        // Sillon
        // iluminacion y caracteristicas patas sillon
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        patasSillon.Draw(lightingShader);
        
        // iluminacion y caracteristicas cojines sillon
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        cojinesSillon.Draw(lightingShader);

        // Sillas escritorio
        // iluminacion y caracteristicas  estructura silla escritorio 1
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posicionSilla, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        estructura.Draw(lightingShader);

        // iluminacion y caracteristicas  cojines asiento y respaldo silla escritorio 1
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posicionSilla, 0.0f, 0.0f));
        cojinesAsiento.Draw(lightingShader);
        cojinesRespaldo.Draw(lightingShader);

        // iluminacion y caracteristicas  estructura silla escritorio 2
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posicionSilla2, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        estructura2.Draw(lightingShader);

        //iluminacion y caracteristicas  cojines asiento y respaldo silla escritorio 2
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posicionSilla2, 0.0f, 0.0f));
        cojinesAsiento2.Draw(lightingShader);
        cojinesRespaldo2.Draw(lightingShader);

        // Cama
        //iluminacion y caracteristicas base de cama 
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        baseCama.Draw(lightingShader);

        //iluminacion y caracteristicas para superior cama
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        arribaCama.Draw(lightingShader);


        // Mesa de sala
        //iluminacion y caracteristicas para madera mesa sala
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        maderaMesa.Draw(lightingShader);

        //iluminacion y caracteristicas para teetra y vaso
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        teteraYVaso.Draw(lightingShader);

        //iluminacion y caracteristicas para charola,plato y alfombra
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        charolaYPlato.Draw(lightingShader);


        // Lampara
        //iluminacion y caracteristicas para foco de lampara
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        focoLampara.Draw(lightingShader);

        //iluminacion y caracteristicas para estructura de metal lampara
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        estructuraLampara.Draw(lightingShader);

        //iluminacion y caracteristicas para cono de plastico lampara
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        conoLampara.Draw(lightingShader);

       
        // Escritorio
        //iluminacion y caracteristicas para madera escritorio
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        escritorioMadera.Draw(lightingShader);

        //iluminacion y caracteristicas para marmol de escritorio
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.9f, 0.9f, 0.9f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        escritorioMarmol.Draw(lightingShader);

        // Pantalla
        //iluminacion y caracteristicas para crital de pantalla
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        pantallaCristal.Draw(lightingShader);

        //iluminacion y caracteristicas para estructura de pantalla
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        pantallaEstructura.Draw(lightingShader);

        // Exterior e interior
        //iluminacion y caracteristicas para elementos de exterior e interior
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.9f, 0.9f, 0.9f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        ExteriorInterior.Draw(lightingShader);

        // Ventilador 
        //iluminacion y caracteristicas para modelo estructura ventilador
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 16.5f, -10.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        ventiladorEstructura.Draw(lightingShader);

        //iluminacion y caracteristicas para helices del ventilador
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.00f, 0.00f, 0.00f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 16.5f, -10.0f));
        model = glm::rotate(model, glm::radians(rotacionVentilador), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        ventiladorHelices.Draw(lightingShader);

        // Ventana
        //iluminacion y caracteristicas para estrutura de la venta fija
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        VentanaEstructuraFija.Draw(lightingShader);

        //iluminacion y caracteristicas para crital de ventana fija
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        VentanaCristalFijo.Draw(lightingShader);

        //iluminacion y caracteristicas para estrutura de la venta movil
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posicionVentana, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        VentanaEstructuraMovil.Draw(lightingShader);

        //iluminacion y caracteristicas para crital de ventana movil
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posicionVentana, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        VentanaCristalMovil.Draw(lightingShader);

        //Puerta
        //iluminacion y caracteristicas para estructura de puerta fija
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        PuertaEstructuraFija.Draw(lightingShader);

        //iluminacion y caracteristicas para crital de puerta fija
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        PuertaCristalFijo.Draw(lightingShader);

        //iluminacion y caracteristicas para estructura de puerta movil
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.50f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(12.1, 0.0f,22.71f));
        model = glm::rotate(model, glm::radians(rotacionPuerta), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        PuertaEstructuraMovil.Draw(lightingShader);

        //iluminacion y caracteristicas para crital de puerta fija
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(12.1, 0.0f,22.71f));
        model = glm::rotate(model, glm::radians(rotacionPuerta), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        PuertaCristalMovil.Draw(lightingShader);

        // Balon
        //iluminacion y caracteristicas para balon
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posicionBalonX, posicionBalonY-0.2, -122.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        Balon.Draw(lightingShader);

        //Persona lado izquierdo del balon
        //iluminacion y caracteristicas cuerpo de personaje
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-26.0f, 2.8f, -57.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        cuerpoPersona1.Draw(lightingShader);

        //iluminacion y caracteristicas pierna de personaje
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-26.0f, 2.8f, -57.0f));
        model = glm::rotate(model, glm::radians(rotacionPatadaP1), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        piernaPersona1.Draw(lightingShader);

        //Persona lado derecho del balon
        //iluminacion y caracteristicas cuerpo de personaje
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.5f, 2.8f, -56.5f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        cuerpoPersona1.Draw(lightingShader);

        //iluminacion y caracteristicas pierna de personaje
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.5f, 2.8f, -56.5f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotacionPatadaP2), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        piernaPersona1.Draw(lightingShader);


        //Persona corriendo en la casa
        //iluminacion y caracteristicas cuerpo de personaje corriendo
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posicionCorredorX, posicionCorredorY, posicionCorredorZ));
        model = glm::rotate(model, glm::radians(rotacionCorredor), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        cuerpoCorredor.Draw(lightingShader);

        //iluminacion y caracteristicas pierna derecha de personaje corredor
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posicionCorredorX, posicionCorredorY, posicionCorredorZ));
        model = glm::rotate(model, glm::radians(rotacionCorredor), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(pasoDerecho), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        piernaDerechaCorredor.Draw(lightingShader);

        //iluminacion y caracteristicas pierna izquierda de personaje corredor
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.633f, 0.727811f, 0.633f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posicionCorredorX, posicionCorredorY, posicionCorredorZ));
        model = glm::rotate(model, glm::radians(rotacionCorredor), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-pasoDerecho), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        piernaIzquierdaCorredor.Draw(lightingShader);

        glBindVertexArray(0);

        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(lightPos.x + movelightPosX, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, lightPos.y + movelightPosY, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, lightPos.z + movelightPosZ));

        model = glm::scale(model, glm::vec3(0.05f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// En esta funcion se definen los movimientos de la posicion de la camara apartir de la entradas por teclado
void DoMovement()
{
    // Si se presiona W o UP la camara de mueve hacia adelante
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    //Si se presiona S o DOWN la camara se mueve hacia atrás
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    // Si se presiona A o IZQUIERDA la cama se mueve hacia la izquierda
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    // Si se presiona D o DERECHA la camara se mueve hacia la dercha
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    // Actualizacion de rotacion ventilador
    if (ventiladorEncendido == true) {
        rotacionVentilador += 1.5;
    }

    // Actualizacion de posicion en ventana
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

    // Actualizacion rotacion de la puerta

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
    // Actualizacion movimiento balon
    if (balonEncendido == true) {
        if (patea1 == true) { // personaje de la izquierda patea
            rotacionPatadaP1 += 0.51;
            if (rotacionPatadaP1 >= 30) { // comienza primer rebote
                rebote1 = true;
                patea1 = false;
            }
            
        }
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
                patea2 = true;
                h = 14; //posicion vertical de vertice
                k = 4; //posicion horizontal de vertice
                p = 1; // foco
            }
        } 

        
    }
    if (patea1 == false) { // regresa pierna de personaje a posicion inicial
        if (rotacionPatadaP1 >= 0) {
            rotacionPatadaP1 -= 0.51;
        }
    }
    if (balonEncendido == false) { // balon se mueve de derecha a izquierda
        if (patea2 == true) { // patea el personaje de la izquierda
            rotacionPatadaP2 += 0.51;
            if (rotacionPatadaP2 >= 30) { //activa primer rebote de izq a der
                rebote1Izq = true;
                patea2 = false;
            }
        }
        
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

            if (posicionBalonX <= 0) { // Activa primer rebote de ida para funcionamiento cuando el usuario presione V de nuevo
                deslizamientoIzq = false;
                patea1 = true;
                h = 4; //posicion vertical de vertice
                k = 4; //posicion horizontal de vertice
                p = 1; // foco
            }
        }
        if (patea2 == false) { // regresa pie de person derecho a  posicion inicial
            if (rotacionPatadaP2 >= 0) {
                rotacionPatadaP2 -= 0.51;
            }
        }
        
    }

    // Actualizacion posicion silla 1
    if (activaSilla == true) {
        if (posicionSilla >= -2) {
            posicionSilla -= 0.1;
        }
    }
    if (activaSilla == false) {
        if (posicionSilla <= 0) {
            posicionSilla += 0.1;
        }
    }
    // Actualizacion posicion silla 2
    if (activaSilla2 == true) {
        if (posicionSilla2 >= -2) {
            posicionSilla2 -= 0.1;
        }
    }
    if (activaSilla2 == false) {
        if (posicionSilla2 <= 0) {
            posicionSilla2 += 0.1;
        }
    }
    
    //Activa niño corriendo
    if (ninoEncendido == true) {
        if (segmento0 == true) {
            posicionCorredorZ += 0.15;
            if (posicionCorredorZ >= 65.5) {
                segmento0 = false;
                rotacionCorredor = 0;
                segmento1 = true;
            }
        }
        if (segmento1 == true) {
            posicionCorredorX += 0.15;
            if (posicionCorredorX >= 9.5) {
                segmento1 = false;
                segmento2 = true;
                rotacionCorredor = 90;
            }
        }
        if (segmento2 == true) {
            posicionCorredorZ -= 0.15;
            if (posicionCorredorZ <= 28.5) {
                segmento2 = false;
                puertaEncendido = true; 
                segmento3 = true;       
            }
        }
        if (segmento3 == true) {
            paro = true;
            if (rotacionPuerta >= 90) {
                paro = false;
                posicionCorredorZ -= 0.15;
                if (posicionCorredorZ <= 16.5) {
                    segmento3 = false;
                    segmento4 = true;
                    rotacionCorredor = 180;
                }
            }
        }
        if (segmento4 == true) {
            posicionCorredorX -= 0.1;
            if (posicionCorredorX <= 0.5) {
                segmento4 = false;
                segmento5 = true;
                rotacionCorredor = 90;
            }
        }
        if (segmento5 == true) {
            posicionCorredorZ -= 0.1;
            if (posicionCorredorZ <= -16) {
                segmento5 = false;
                segmento6 = true;
            }

        }
        if (segmento6) {
            posicionCorredorZ -= 0.007;
            posicionCorredorY += 0.007;
            puertaEncendido = false;
            if (posicionCorredorY >= 4.0) {
                segmento6 = false;
                segmento7 = true;
            }

        }
        if (segmento7 == true) {
            posicionCorredorZ -= 0.15;
            if (posicionCorredorZ <= -30) {
                posicionCorredorY = 2.8;
            }
            if (posicionCorredorZ <= -35.0) {
                segmento7 = false;
                segmento8 = true;
                rotacionCorredor = 180;
            }
                
            
        }
        
        if (segmento8 == true) {
            posicionCorredorX -= 0.15;
            if (posicionCorredorX <= -27) {
                segmento8 = false;
                segmento0 = true;
                rotacionCorredor = 270;
            }
        }
        if (paro == false) {
            if (direccionPaso == true) {
                pasoDerecho += 0.9;
                if (pasoDerecho >= 30) {
                    direccionPaso = false;
                }
            }
            if (direccionPaso == false) {
                pasoDerecho -= 0.9;
                if (pasoDerecho <= -30) {
                    direccionPaso = true;
                }
            }
        }
        else {
            pasoDerecho = 0;
        }
        
    } 
    
}

//Funcion llamada cuando el usuario presiona y suelta una tecla
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
    //Mueve iluminacion en eje X
    if (keys[GLFW_KEY_H])
    {
        //En direccion negativa de X
        movelightPosX -= 0.1f;
    }
    if (keys[GLFW_KEY_L])
    {
        //En direccion positiva de X
        movelightPosX += 0.1f;
    }

    //Mueve iluminacion en eje Z
    if (keys[GLFW_KEY_U])
    {
        //En direccion positiva de Z
        movelightPosZ += 0.1f;
    }
    if (keys[GLFW_KEY_J])
    {
        //En direccion negativa de Z
        movelightPosZ -= 0.1f;
    }

    //Mueve iluminacion en eje Y
    if (keys[GLFW_KEY_M])
    {
        //En direccion positiva de Y
        movelightPosY += 0.1f;
    }
    if (keys[GLFW_KEY_N])
    {
        //En direccion negativa de Y
        movelightPosY -= 0.1f;
    }

    // Se activa o desactiva ventilador
    if (keys[GLFW_KEY_X])
    {
        ventiladorEncendido = !ventiladorEncendido;
    }

    //Se abre o cierra la ventana
    if (keys[GLFW_KEY_P])
    {
        ventanaEncendido = !ventanaEncendido;
    }
    
    //Se abre o cierra puerta 
    if (keys[GLFW_KEY_O])
    {
        puertaEncendido = !puertaEncendido;
    }

    // Se golpea el balon de izquierda a derecha o de derecha a izquierda
    if (keys[GLFW_KEY_V])
    {
        balonEncendido = !balonEncendido;
    }


    //Activa movimiento de silla 1 (derecha)
    if (keys[GLFW_KEY_1])
    {
        activaSilla = !activaSilla;
    }

    //Activa movimiento de silla 2 (izquierda)
    if (keys[GLFW_KEY_2])
    {
        activaSilla2 = !activaSilla2;
    }

    //Activa niño corriendo
    if (keys[GLFW_KEY_9])
    {
        ninoEncendido = !ninoEncendido;
    }
}

// Funcion para detectar posicion del mouse
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}


