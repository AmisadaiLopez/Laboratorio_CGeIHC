//López Hernández Miriam Amisadai
//320260366
//Fecha de entrega: 09/04/2026
//Practica 8

// Std. Includes
#include <string>

//Agregando una nueva libreria 
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
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
float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;

//agregamos variables para la animación del sol y la luna
//constante para convertir grados a radianes
const float PI = 3.1415926f;

//posiciones iniciales del sol y la luna
/*glm::vec3 sunPos(-1.6f, 10.0f, -0.75f);
glm::vec3 moonPos(1.6f, 10.0f, -0.75f);*/

glm::vec3 sunPos(0.0f, 0.0f, -2.0f);
glm::vec3 moonPos(0.0f, 0.0f, 2.0f);

//ángulos iniciales para la animación
float sunAngle = glm::radians(180.0f);
float moonAngle = glm::radians(180.0f);

//velocidades de rotación
float sunRotation = 0.0f;
float moonRotation = 0.0f;


bool animSun = false;
bool animMoon = false;

//parámetros para la órbita del sol y la luna
float orbitRadius = 9.0f;
float orbitCenterX = 0.0f;
float orbitCenterY = -1.5f;
float sunZ = -2.0f;
float moonZ = 2.0f;



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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Materiales e Iluminacion - Miriam Lopez - Practica 8", nullptr, nullptr);

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
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Load models
    Model red_dog((char*)"Models/RedDog.obj");
	//agregamos nuevo modelo
    Model house((char*)"Models/House.obj");

    //carga de modelos 
    Model grass((char*)"Models/grass.obj");
    Model doghouse((char*)"Models/doghouse.obj");
    Model tree((char*)"Models/Tree.obj");
    Model arbusto((char*)"Models/arbusto.obj");
    Model plato((char*)"Models/Dog Food and Water Bowl.obj");

	//Modelos del sol y la luna
	Model sun((char*)"Models/sol/13913_Sun_v2_l3.obj");
	Model moon((char*)"Models/luna/Moon.obj");

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

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
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

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Check and call events
        glfwPollEvents();
        DoMovement();

		//agregamos la animación del sol y la luna, haciendo que se muevan en una órbita circular alrededor de la casa.
        
		//agregamos la orbita del sol para que se mueva de manera sincronizada con la luna,
        //pero con una velocidad diferente para que no estén siempre en la misma posición
        if (animSun)
        {
            sunAngle -= 0.45f * deltaTime;      //se va ocultando
            sunRotation += 45.0f * deltaTime;   //gira sobre si mismo
        }

        sunPos.x = orbitCenterX + cos(sunAngle) * orbitRadius;
        sunPos.y = orbitCenterY + sin(sunAngle) * orbitRadius;
        sunPos.z = sunZ;

        if (sunPos.y < -2.7f)
        {
            animSun = false;
        }

		//agregamos la órbita de la luna para que se mueva de manera sincronizada con el sol, 
        // pero con una velocidad diferente para que no estén siempre en la misma posición
        if (animMoon)
        {
			moonAngle -= 0.30f * deltaTime;     //se va ocultando
			moonRotation += 20.0f * deltaTime;  //gira sobre si mismo
        }

        moonPos.x = orbitCenterX + cos(moonAngle) * orbitRadius;
        moonPos.y = orbitCenterY + sin(moonAngle) * orbitRadius;
        moonPos.z = moonZ;

        if (moonPos.y < -2.7f)
        {
            animMoon = false;
        }

        // Color del cielo
        if (sunPos.y > -1.0f)
        {
            glClearColor(0.53f, 0.81f, 0.98f, 1.0f); //color para el dia
        }
        else if (moonPos.y > -1.0f)
        {
            glClearColor(0.02f, 0.02f, 0.10f, 1.0f); //color para la noche
        }
        else
        {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        



        /*
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, lightPos.x + movelightPos, lightPos.y + movelightPos, lightPos.z + movelightPos);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Set lights properties
        /*glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.2f, 0.7f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.3f, 0.6f, 0.4f);//

        
        //agregamos una nueva luz para el modelo de la casa
       	glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.7f, 0.7f, 0.7f);*/
        

        lightingShader.Use();

        // luz original que ya tenias
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"),
            lightPos.x + movelightPos,
            lightPos.y + movelightPos,
            lightPos.z + movelightPos);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"),
            camera.GetPosition().x,
            camera.GetPosition().y,
            camera.GetPosition().z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.20f, 0.20f, 0.20f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.30f, 0.30f, 0.30f);

		//agregamos la luz del sol 
        glUniform3f(glGetUniformLocation(lightingShader.Program, "sunLight.position"),
            sunPos.x, sunPos.y, sunPos.z);

        if (sunPos.y > -1.0f)
        {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "sunLight.ambient"), 0.25f, 0.20f, 0.10f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "sunLight.diffuse"), 0.95f, 0.75f, 0.35f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "sunLight.specular"), 1.00f, 0.90f, 0.60f);
        }
        else
        {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "sunLight.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "sunLight.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "sunLight.specular"), 0.0f, 0.0f, 0.0f);
        }

		//agregamos la luz de la luna
        glUniform3f(glGetUniformLocation(lightingShader.Program, "moonLight.position"),
            moonPos.x, moonPos.y, moonPos.z);

        if (moonPos.y > -1.0f)
        {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "moonLight.ambient"), 0.05f, 0.05f, 0.10f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "moonLight.diffuse"), 0.30f, 0.35f, 0.55f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "moonLight.specular"), 0.55f, 0.60f, 0.80f);
        }
        else
        {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "moonLight.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "moonLight.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "moonLight.specular"), 0.0f, 0.0f, 0.0f);
        }





        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.3f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.6f, 0.6f, 0.6f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);





        // Draw the loaded model
        glm::mat4 model(1);
        //model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        model = glm::translate(model, glm::vec3(0.0f, -0.15f, 1.0f));
        model = glm::scale(model, glm::vec3(1.15f, 1.15f, 1.15f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
       
        //glDrawArrays(GL_TRIANGLES, 0, 36);

		//agregando el modelo del perro rojo
		red_dog.Draw(lightingShader);

		//llamamos al modelo de la casa
		//house.Draw(lightingShader);


        //carga de modelos para la practica 
        
        //Dibujar el pasto
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-5.0f, -1.45f, -10.0f));
        model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        grass.Draw(lightingShader);

        // Dibujar la casa 
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.6f, -0.75f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        house.Draw(lightingShader);

        //dibujar la casa del perro desplazada a la izquierda
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.8f, -0.8f, 0.0f));
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
        model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        doghouse.Draw(lightingShader);

        //dibujar el plato del perro desplazado a la izquierda
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.0f, -0.6f, 2.0f));
        model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        plato.Draw(lightingShader);

        //dibujar el arbol desplazado a la derecha
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(5.0f, -0.6f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 1.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tree.Draw(lightingShader);

        //dibujar el arbol desplazado a la izquierda
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.5f, -0.7f, -0.5f));
        model = glm::scale(model, glm::vec3(0.5f, 1.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tree.Draw(lightingShader);

        //dibujar el arbusto desplazado a la derecha
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(5.0f, -0.6f, 1.2f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbusto.Draw(lightingShader);

        //dibujar el arbusto desplazado a la izquierda
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.0f, -0.6f, 1.5f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbusto.Draw(lightingShader);


        

        glBindVertexArray(0);

		//Dibujar el sol y la luna
        shader.Use();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        //Sol
        if (sunPos.y > -1.0f)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, sunPos);
            model = glm::rotate(model, glm::radians(sunRotation), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.003f));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            sun.Draw(shader);
        }

        //Luna
        if (moonPos.y > -1.0f)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, moonPos);
            model = glm::rotate(model, glm::radians(moonRotation), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.003f));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            moon.Draw(shader);
        }
        

		// Dibujar la lámpara que representa la luz que ya se tenia antes, para que se vea dónde está ubicada
        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos + movelightPos);
        model = glm::scale(model, glm::vec3(0.3f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

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

    if (keys[GLFW_KEY_SPACE])
    {
        camera.ProcessKeyboard(UP, deltaTime);
    }

    if (keys[GLFW_KEY_LEFT_CONTROL])
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
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

    if (keys[GLFW_KEY_O])
    {
       
        movelightPos += 0.1f;
    }

    /*if (keys[GLFW_KEY_L])
    {
        
        movelightPos -= 0.1f;
    }*/

    if (keys[GLFW_KEY_L])
    {
        movelightPos -= 0.1f;
    }

    // activar sol con N
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        sunAngle = glm::radians(110.0f);
        animSun = true;
    }

    // activar luna con M
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        moonAngle = glm::radians(110.0f);
        animMoon = true;
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


