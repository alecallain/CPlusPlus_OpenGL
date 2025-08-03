// This file contains the 'main' function.Program execution begins and ends here.

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "utils.h"
#include "Shader.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// Camera configuration
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Camera movement speed
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// Set mouse items
float fov = 45.0f; // Field of view
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400, lastY = 300;
const float sensitivity = 0.1f; // Adjust mouse sensitivity;
bool firstMouse = true; // First mouse movement

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Configuring GLFW window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Modeling", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Accounts for window resizing

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Disable mouse cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Listen for mouse events
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

    // Enables non intersecting faces of objects
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("3.3.vertex.glsl", "3.3.fragment.glsl");
    Shader lightingShader("light.vertex.glsl", "light.fragment.glsl");

    //// Set up shaders
    //unsigned int vertexShader, fragmentShader;
    //int success;
    //char infoLog[512];

    //// Vertex Shader
    //vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //glCompileShader(vertexShader);
    //glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    //if (!success)
    //{
    //    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //    std::cout << "ERROR: Shader Vertex compilation failed\n" << infoLog << std::endl;
    //}

    //// Fragment Shader
    //fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //glCompileShader(fragmentShader);
    //glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    //if (!success) 
    //{
    //    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //    std::cout << "ERROR: Shader Fragment compilation failed\n" << infoLog << std::endl;
    //}

    //// link shaders
    //unsigned int shaderProgram = glCreateProgram();
    //glAttachShader(shaderProgram, fragmentShader);
    //glAttachShader(shaderProgram, vertexShader);
    //glLinkProgram(shaderProgram);
    //glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    //if (!success) 
    //{
    //    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //    std::cout << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
    //}

    //// Once binded to the program, not needed anymore
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);

    // Set up vertex data
    //float vertices[] = { // One triangle
    //    // positions         // colors
    //     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    //    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
    //     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    //};

    //float vertices[] = { // one rectangle
    //    // positions          // colors           // texture coordinates
    //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    //    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    //};

    // Set up texture coordinates
    //float textureCoordinates[] = {
    //    0.0f, 0.0f, // lower left corner
    //    1.0f, 0.0f, // lower right corner
    //    0.5f, 1.0f // top center
    //};

    //float vertices[] = { // multiple triangles
    //    0.5f, 0.5f, 0.0f, // top right
    //    0.5f, -0.5f, 0.0f, // bottom right
    //    -0.5f, -0.5f, 0.0f, // bottom left
    //    -0.5f, 0.5f, 0.0f //top left
    //};

	// vertices for a rectangle with texture coordinates
    //float vertices[] = {
    //    // positions          // texture coords
    //     0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
    //     0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
    //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
    //    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    //};

	// vertices for a cube with texture coordinates
    /*float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };*/

	// vertex positions for a cube
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
    
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
    
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
    
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
    
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    //unsigned int indices[] = {
    //    0, 1, 3, // first triangle
    //    1, 2, 3  // second triangle
    //};

    // multiple cube position variations in scene
    /*glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };*/

    // Vertex Buffer Objects, Vertex Array Objects and Element Buffer Objects
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);

    // Copy vertices array in vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind vertex array object
    glBindVertexArray(VAO);

    // Copy index arry in element buffer
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex position attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // adding light source
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set color attribute pointers
    /*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/

    // Set texture coordinate attribute pointers
    /*glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/

    // Set texture parameters if using Clamp to Border option
    //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Bind texture
    //unsigned int texture1, texture2;
    //unsigned char* data;
    //glGenTextures(1, &texture1);
    //glBindTexture(GL_TEXTURE_2D, texture1);

    //// Set texture parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //// Generate Mipmap & Texture Filtering
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true);
    //data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    //if (data) {
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else {
    //    std::cout << "Failed to load texture" << std::endl;
    //}

    //// Free image memory
    //stbi_image_free(data);

    //// Setup second texture
    //glGenTextures(1, &texture2);
    //glBindTexture(GL_TEXTURE_2D, texture2);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    //if (data)
    //{
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else {
    //    std::cout << "Failed to load texture" << std::endl;
    //}

    //stbi_image_free(data);

    // Set up before utilizing uniforms
    //ourShader.use();
    //glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // Manual setup
    //ourShader.setInt("texture1", 0);
    //ourShader.setInt("texture2", 1);

    // adding color to objects
	//glm::vec3 lightColor(0.33f, 0.42f, 0.18f); // white light
	//glm::vec3 objectColor(1.0f, 0.5f, 0.31f); // coral color
	//glm::vec3 colorResult = lightColor * objectColor;

    // Set up projection matrix (best practice since it rarely changes if not using camera)
    /*glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);*/

    // Adding transformation (practice)
    /*glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;*/

    // Adding transformation to container
    /*glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));*/

    // can safely unbind
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    // Uncomment to draw in wireframes
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // if only using a single shader, activate shader before loop
    //glUseProgram(shaderProgram);

    // Main render loop
    while (!glfwWindowShouldClose(window))
    {
        // Keep track of delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);
        
        // rendering
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind texture
        /*glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);*/

        // Create transformation
        /*glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));*/

        // run program and draw object
        //glUseProgram(shaderProgram);
		ourShader.use();
        ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        //ourShader.use();
        //unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// Set up model and view matrices
        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // rotates cube over time
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

        /*glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        ourShader.setMat4("view", view);*/
        
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        // Add the camera position
        glm::mat4 view = glm::mat4(1.0f);
        //glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

        // Set up camera direction
        //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        //glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

        // Set up camera right axis
        //glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        //glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

        // Set up up axis
        //glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

        // Draw circle for camera
		/*const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;*/

		// Create view matrix
		//view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

        // retrieve the matrix uniform locations
        //unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        //unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");

        // pass them to the shaders
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        /*glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
        ourShader.setInt("texture2", 1);*/

        //// update uniform color
        //double timeValue = glfwGetTime();
        //float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glm::mat4 model = glm::mat4(1.0f);
        //float angle = (float)glfwGetTime() * 25.0f;
        //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, 36); // Draw cube with 36 vertices

        //for (unsigned int i = 0; i < 10; i++) // Draw multiple cubes
        //{
        //    // calculate the model matrix for each object and pass it to shader before drawing
        //    glm::mat4 model = glm::mat4(1.0f);
        //    model = glm::translate(model, cubePositions[i]);
        //    float angle = (float)glfwGetTime() * 25.0f;
        //    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //    ourShader.setMat4("model", model);
        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

        // draw light source
		lightingShader.use();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightingShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36); // Draw light source cube

        // refresh
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // deallocates all resources
    glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);

    // Officially end program
    glfwTerminate();
    return 0;
}

// window resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// processes relevant keys
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront; // Move camera forward
	}

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront; // Move camera backward
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; // Move camera left
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; // Move camera right
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
		lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // y-coordinates range from bottom to top
	lastX = xpos;
    lastY = ypos;
	
    xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// constrain pitch to prevent camera flipping
    if (pitch > 89.0f) {
		pitch = 89.0f;
    }

	if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    // Camera direction vector
    glm::vec3 direction;
    /*direction.x = cos(glm::radians(yaw));
    direction.y = sin(glm::radians(yaw));*/

    direction.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;

    if (fov < 1.0f) {
        fov = 1.0f;
    }
        
    if (fov > 45.0f) {
        fov = 45.0f;
    }
}
