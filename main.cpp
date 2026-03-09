#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, double& speed) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		speed = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		speed = 5;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		speed = 10;
	}

}

int CreateShader(unsigned int& shader, const char* shaderSource, GLenum shaderType) {
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 1;
	}

	return 0;
}

void DrawATriangle() {
	float vertices[] = {
		 -0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f, 0.5f, 0.0f
	};

	// 着色器
	unsigned int vertexShader;
	const char* vertexShaderSource = "#version 330 core\n" "layout (location = 0) in vec3 aPos;\n" "void main()\n" "{\n" " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" "}\0";
	{
		auto err = CreateShader(vertexShader, vertexShaderSource, GL_VERTEX_SHADER);
		if (err) {
			std::cout << "CreateVertexShader failed" << std::endl;
			return;
		}
	}

	unsigned int fragmentShader;
	const char* fragmentShaderSource = "#version 330 core\n" "out vec4 FragColor;\n" "void main()\n" "{\n" " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" "}\0";
	{
		auto err = CreateShader(fragmentShader, fragmentShaderSource, GL_FRAGMENT_SHADER);
		if (err) {
			std::cout << "CreateFragmentShader failed" << std::endl;
			return;
		}
	}

	// 创建着色器程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// glGetProgramiv

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // ?

	// 使用程序
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

}

int main() {
	// 初始化 GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// 创建窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// 初始化 GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 视口设置
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	double speed = 1;

	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		auto time = glfwGetTime();

		auto r = (sin(time * speed) * 0.5f) + 0.5f;
		auto g = (sin(time * speed + 2.0943) * 0.5f) + 0.5f;
		auto b = (sin(time * speed + 4.1886) * 0.5f) + 0.5f;

		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		processInput(window, speed);

		DrawATriangle();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();

	return 0;
}