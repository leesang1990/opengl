 #include <iostream>
 
 // GLEW
 #define GLEW_STATIC
 #include <GL/glew.h>
 
 // GLFW
 #include <GLFW/glfw3.h>
 
 // Other Libs
 #include <SOIL.h>
 // GLM Mathematics
 #include <glm.hpp>
 #include <gtc/matrix_transform.hpp>
 #include <gtc/type_ptr.hpp>
 
 // Other includes
 #include "Shader.h"
 
 #ifndef UseCamera2
 
 static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
 static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
 static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
 static GLfloat deltaTime = 0.0f;   // 当前帧遇上一帧的时间差
 static GLfloat lastFrame = 0.0f;   // 上一帧的时间
 
 static bool firstMouse = true;
 
 static bool keys[1024];
 static GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
 static GLfloat pitch = 0.0f;
 static GLfloat fov = 45.0f;
 
 
 // Window dimensions
 static const GLuint WIDTH = 800, HEIGHT = 600;
 
 static GLfloat lastX = WIDTH / 2.0;
 static GLfloat lastY = HEIGHT / 2.0;
 
 // The MAIN function, from here we start the application and run the game loop
 int UseCamera1()
 {
 	// Init GLFW
 	glfwInit();
 	// Set all the required options for GLFW
 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
 
 	// Create a GLFWwindow object that we can use for GLFW's functions
 	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
 	glfwMakeContextCurrent(window);
 
 	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode)
 	{
 		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
 			glfwSetWindowShouldClose(window, GL_TRUE);
 		if (action == GLFW_PRESS)
 			keys[key] = true;
 		else if (action == GLFW_RELEASE)
 			keys[key] = false;
 	});
 
 	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
 	{
 		if (firstMouse)
 		{
 			lastX = xpos;
 			lastY = ypos;
 			firstMouse = false;
 		}
 
 		GLfloat xoffset = xpos - lastX;
 		GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
 		lastX = xpos;
 		lastY = ypos;
 
 		GLfloat sensitivity = 0.05;	// Change this value to your liking
 		xoffset *= sensitivity;
 		yoffset *= sensitivity;
 
 		yaw += xoffset;
 		pitch += yoffset;
 
 		// Make sure that when pitch is out of bounds, screen doesn't get flipped
 		if (pitch > 89.0f)
 			pitch = 89.0f;
 		if (pitch < -89.0f)
 			pitch = -89.0f;
 
 		glm::vec3 front;
 		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
 		front.y = sin(glm::radians(pitch));
 		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
 		cameraFront = glm::normalize(front);
 	});
 
 	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
 	{
 		if (fov >= 1.0f && fov <= 45.0f)
 			fov -= yoffset;
 		if (fov <= 1.0f)
 			fov = 1.0f;
 		if (fov >= 45.0f)
 			fov = 45.0f;
 	});
 
 	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
 	glewExperimental = GL_TRUE;
 	// Initialize GLEW to setup the OpenGL Function pointers
 	glewInit();
 
 	// Define the viewport dimensions
 	glViewport(0, 0, WIDTH, HEIGHT);
 
 	// Setup OpenGL options
 	glEnable(GL_DEPTH_TEST);
 
 
 	// Build and compile our shader program
 	Shader ourShader("shaders/camera.vs", "shaders/camera.fs");
 
 
 	// Set up vertex data (and buffer(s)) and attribute pointers
 	GLfloat vertices[] = {
 		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
 		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
 		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
 		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
 		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
 		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
 
 		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
 		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
 		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
 		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
 		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
 		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
 
 		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
 		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
 		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
 		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
 		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
 		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
 
 		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
 		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
 		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
 		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
 		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
 		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
 
 		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
 		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
 		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
 		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
 		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
 		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
 
 		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
 		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
 		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
 		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
 		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
 		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
 	};
 	// World space positions of our cubes
 	glm::vec3 cubePositions[] = {
 		glm::vec3(0.0f, 0.0f, 0.0f),
 		glm::vec3(2.0f, 5.0f, -15.0f),
 		glm::vec3(-1.5f, -2.2f, -2.5f),
 		glm::vec3(-3.8f, -2.0f, -12.3f),
 		glm::vec3(2.4f, -0.4f, -3.5f),
 		glm::vec3(-1.7f, 3.0f, -7.5f),
 		glm::vec3(1.3f, -2.0f, -2.5f),
 		glm::vec3(1.5f, 2.0f, -2.5f),
 		glm::vec3(1.5f, 0.2f, -1.5f),
 		glm::vec3(-1.3f, 1.0f, -1.5f)
 	};
 
 	GLuint VBO, VAO;
 	glGenVertexArrays(1, &VAO);
 	glGenBuffers(1, &VBO);
 
 	glBindVertexArray(VAO);
 	glBindBuffer(GL_ARRAY_BUFFER, VBO);
 
 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
 	// Position attribute
 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
 	glEnableVertexAttribArray(0);
 	// TexCoord attribute
 	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
 	glEnableVertexAttribArray(2);
 
 	glBindVertexArray(0); // Unbind VAO
 
 
 	// Load and create a texture 
 	GLuint texture1;
 	GLuint texture2;
 	// ====================
 	// Texture 1
 	// ====================
 	glGenTextures(1, &texture1);
 	glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
 	// Set our texture parameters
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 	// Set texture filtering
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 	// Load, create texture and generate mipmaps
 	int width, height;
 	unsigned char* image = SOIL_load_image(CURRENT_DIR("container.jpg"), &width, &height, 0, SOIL_LOAD_RGB);
 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
 	glGenerateMipmap(GL_TEXTURE_2D);
 	SOIL_free_image_data(image);
 	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
 	// ===================
 	// Texture 2
 	// ===================
 	glGenTextures(1, &texture2);
 	glBindTexture(GL_TEXTURE_2D, texture2);
 	// Set our texture parameters
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 	// Set texture filtering
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 	// Load, create texture and generate mipmaps
 	image = SOIL_load_image(CURRENT_DIR("awesomeface.png"), &width, &height, 0, SOIL_LOAD_RGB);
 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
 	glGenerateMipmap(GL_TEXTURE_2D);
 	SOIL_free_image_data(image);
 	glBindTexture(GL_TEXTURE_2D, 0);
 
 	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 	// Game loop
 	while (!glfwWindowShouldClose(window))
 	{
 		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
 		glfwPollEvents();
 
 		GLfloat currentFrame = glfwGetTime();
 		deltaTime = currentFrame - lastFrame;
 		lastFrame = currentFrame;
 		[]()
 		{
 			// 摄像机控制
 			GLfloat cameraSpeed = 5.0f * deltaTime;
 			if (keys[GLFW_KEY_W])
 				cameraPos += cameraSpeed * cameraFront;
 			if (keys[GLFW_KEY_S])
 				cameraPos -= cameraSpeed * cameraFront;
 			if (keys[GLFW_KEY_A])
 				cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
 			if (keys[GLFW_KEY_D])
 				cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
 		}();
 
 		// Render
 		// Clear the colorbuffer
 		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
 
 		// Bind Textures using texture units
 		glActiveTexture(GL_TEXTURE0);
 		glBindTexture(GL_TEXTURE_2D, texture1);
 		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
 		glActiveTexture(GL_TEXTURE1);
 		glBindTexture(GL_TEXTURE_2D, texture2);
 		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);
 
 		// Activate shader
 		ourShader.Use();
 
 		// Create transformations
 		glm::mat4 view;
 		glm::mat4 projection;
 #if 0
 		GLfloat radius = 10.0f;
 		GLfloat camX = sin(glfwGetTime()) * radius;
 		GLfloat camZ = cos(glfwGetTime()) * radius;
 		view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
 #else
 		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
 #endif
 		projection = glm::perspective(fov, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
 		// Get their uniform location
 		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
 		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
 		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
 		// Pass the matrices to the shader
 		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
 		// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
 		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
 
 		glBindVertexArray(VAO);
 		for (GLuint i = 0; i < 10; i++)
 		{
 			// Calculate the model matrix for each object and pass it to shader before drawing
 			glm::mat4 model;
 			model = glm::translate(model, cubePositions[i]);
 			GLfloat angle = 20.0f * i;
 			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
 			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
 
 			glDrawArrays(GL_TRIANGLES, 0, 36);
 		}
 		glBindVertexArray(0);
 
 		// Swap the screen buffers
 		glfwSwapBuffers(window);
 	}
 	// Properly de-allocate all resources once they've outlived their purpose
 	glDeleteVertexArrays(1, &VAO);
 	glDeleteBuffers(1, &VBO);
 	// Terminate GLFW, clearing any resources allocated by GLFW.
 	glfwTerminate();
 	return 0;
 }
 
 #endif
 
 
 
 
