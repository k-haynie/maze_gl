#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <random>

#include "cell.h"
#include "grid.h"
//#include "Kruskals.cpp"
//#include "PrimsSimp.cpp"
#include "RecursiveBacktracker.cpp"

#include "glad.h"
#include "glfw.h"
#include "shader_gl.h"
#include "camera.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

// because of OpenGL this has to be global
vector<vector<Cell*>> maze;
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;
int SQR_SIZE = 1;

// camera stuff
Camera camera(vec3(1.0f, 0.0f, 1.0f));

// prevent jumping in mouseover
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// render timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// generate a random int over an equal distribution
int randint(int high, int low = 0) {
	random_device rd;
	mt19937 generator(rd());
	uniform_int_distribution<int> distr(low, high);
	return distr(generator);
};

void asciiPrint() {
	int r = maze.size();
	int c = maze[0].size();

	// Print out the first line
	string dash = "+";
	for (int j = 0; j < c; j++) {
		dash = dash.append("---+");
	};
	cout << dash << endl;

	// Print out the actual board
	for (int i = 0; i < r; i++) {
		string line = "|";
		for (int j = 0; j < c; j++) {
			if (!maze[i][j]->e_conn) {
				line = line.append("   |");
			}
			else {
				line = line.append("    ");
			};
		};
		cout << line << endl;

		string dash = "+";
		for (int j = 0; j < c; j++) {
			if (!maze[i][j]->s_conn) {
				dash = dash.append("---+");
			}
			else
			{
				dash = dash.append("   +");
			}
		};
		cout << dash << endl;
	}
};

vector<vec3> vertex_gen() {
	// carve out the exit
	maze[maze.size() - 1][maze[0].size() - 1]->s_conn = true;
	vector <vec3> vertices;

	for (int i = -1; i < 2 * (float)maze[0].size(); i++) {
		vertices.push_back(vec3((i * SQR_SIZE) + 1, 0, 0));
	}
	for (int i = 1; i < 2 * (float)maze.size(); i++) {
		vertices.push_back(vec3(0, 0, (i * SQR_SIZE) + 1));
	}

	// draw the first item
	for (int i = 0; i - 1 < 2 * (int)maze.size(); i++) {
		for (int j = 0; j - 1 < 2 * (int)maze[0].size(); j++) {

			// in-between voxels
			if (i % 2 == 0 && j % 2 == 0) {
				vertices.push_back(vec3((j * SQR_SIZE), 0, (i * SQR_SIZE)));
			}
			// eastern cell
			if (i % 2 != 0 && j % 2 != 0 && !maze[(i - 1) / 2][(j - 1) / 2]->e_conn) {
				vertices.push_back(vec3((j * SQR_SIZE) + 1, 0, (i * SQR_SIZE)));
			}
			// southern cell
			if (i % 2 != 0 && j % 2 != 0 && !maze[(i - 1) / 2][(j - 1) / 2]->s_conn) {
				vertices.push_back(vec3((j * SQR_SIZE), 0, (i * SQR_SIZE) + 1));
			}
		}
	}
	return vertices;
}

class voxel {
public:
	mat4 position = mat4(1.0f);
	vec3 min;
	vec3 max;

	void set_position(mat4 pos) {
		position = pos;
		aabb_init();
	}

	void output() {
		cout << "CUBE AT: " << position[3][1] << "," << position[3][3] << endl;
		cout << "	x: " << min[0] << "," << max[0] << endl;
		cout << "	z: " << min[2] << "," << max[2] << endl;
		cout << endl;	
	}
private:

	// calculates the axis-aligned bounding box for each voxel. 
	// If the AABB is the exact same size as the object, the camera 
	//		can clip into the object, so here I initalize a size
	//		slightly larger than each voxel.

	void aabb_init() {
		for (int i = 0; i < 3; i++) {
			min[i] = position[3][i] - 0.5;// -0.65;
			max[i] = min[i] + 1;// +1.3;
		}

	}


};

vector<voxel*> objects;

voxel* collision_obj;

void setup() {
	int r, c;

	do {
		cout << "Width of maze (less than 26): ";
		cin >> c;
	} while (c > 100 || c < 1);

	do {
		cout << "Height of maze (less than 26): ";
		cin >> r;
	} while (r > 100 || r < 1);

	Grid grid;
	maze = grid.prepare_grid(r, c);
	grid.configure_cells(r, c, maze);

	RecBktr alg;
	alg.on(maze, randint(r-1), randint(c-1));
	//Kruskals krusk;
	//krusk.on(maze);
	asciiPrint();
}

// called whenever the window gets resized
void framebuffer_resize_callback(GLFWwindow* win, int width, int height) {
	glViewport(0, 0, width, height);
}

// process any input
void input_process(GLFWwindow* wind) {
	if (glfwGetKey(wind, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(wind, true);
	}
	if (glfwGetKey(wind, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(wind, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(wind, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(wind, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
};

// process mouse movement
void mouse_callback(GLFWwindow* wind, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouse(xoffset, yoffset);
}

// set up a texture for a skybox
unsigned int skybox(vector<string> faces) {
	unsigned int ID;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			cout << "Skybox failed to load" << endl;
		}
		stbi_image_free(data);
	}
	// set parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return ID;
}

// set up collision coordinates
void collision_detect() {
	for (voxel* v: objects) {
		// feed the camera coordinates for each of the cubes for collision detection
		camera.coll_x_min.push_back(v->min.x);
		camera.coll_x_max.push_back(v->max.x);
		camera.coll_z_min.push_back(v->min.z);
		camera.coll_z_max.push_back(v->max.z);
		}
};

int glfw_setup() {
	// set up variables
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// create a window, make sure it shows
	GLFWwindow* wind = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MazeView", NULL, NULL);
	if (wind == NULL) {
		cout << "Window creation failed." << endl;
		glfwTerminate();
		return -1;
	}
	// set up the window, link callbacks, and capture the mouse
	glfwMakeContextCurrent(wind);
	glfwSetFramebufferSizeCallback(wind, framebuffer_resize_callback);
	glfwSetCursorPosCallback(wind, mouse_callback);
	glfwSetInputMode(wind, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// verify GLAD works
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "GLAD linking failed." << endl;
		return -1;
	}

	// set up frag and vertex shaders, viewport, enable depth testing
	Shader shaderGL("shader.vs", "shader.fs");
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);


	// CREATE AN OBJECT TO DISPLAY
	// define a cube composed of 12 triangles
	float cube_dem[] = { // lower triangle first, then top triangle
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
	};
	
	// get vertices to move the objects to
	vector<vec3> vertices = vertex_gen();

	// create buffers and array objects
	unsigned int bufObj, ArrObj;
	glGenVertexArrays(1, &ArrObj);
	glGenBuffers(1, &bufObj);

	// bind the array Object, then the buffer object, and finally intialize data
	glBindVertexArray(ArrObj);
	glBindBuffer(GL_ARRAY_BUFFER, bufObj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_dem), cube_dem, GL_STATIC_DRAW);

	// set up positional channel
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// set up texture channel
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// load a texture image
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// verify the texture loaded
	int width, height, nrChannels;
	unsigned char* data = stbi_load("C:/Users/astro/Pictures/Programming/crate.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "ERROR: TEXTURE FAILED TO LOAD" << endl;
	}
	stbi_image_free(data);

	float ln = 1.0 + (2.0 * (int)maze.size());
	float wd = 1.0 + (2.0 * (int)maze[0].size());

	// set up a floor - different coordinates mean different repeating?
	float floorVertices[] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		wd, 0.0f, 0.0f, wd/2, 0.0f,
		wd, 0.0f, ln, wd/2, ln/2,
		 
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,  ln, 0.0f, ln/2,
		wd, 0.0f, ln, wd/2, ln/2,


	};

	unsigned int floorVBO, floorVAO;
	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &floorVBO);
	glBindVertexArray(floorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), &floorVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// load a texture image
	unsigned int grass;
	glGenTextures(1, &grass);
	glBindTexture(GL_TEXTURE_2D, grass);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// verify the texture loaded
	int grasswidth, grassheight, grassnrChannels;
	unsigned char* grassdata = stbi_load("../../maze_img/grass.jpg", &grasswidth, &grassheight, &grassnrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, grasswidth, grassheight, 0, GL_RGB, GL_UNSIGNED_BYTE, grassdata);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "ERROR: TEXTURE FAILED TO LOAD" << endl;
	}
	stbi_image_free(grassdata);

	// set up a skybox
	vector<string> faces = {
		"../../maze_img/right.jpg",
		"../../maze_img/left.jpg",
		"../../maze_img/top.jpg",
		"../../maze_img/bottom.jpg",
		"../../maze_img/front.jpg",
		"../../maze_img/back.jpg"
	};

	unsigned int skyboxTexture = skybox(faces);
	unsigned int skyVAO, skyVBO;

	float skyboxVert[] = { 
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	glGenVertexArrays(1, &skyVAO);
	glGenBuffers(1, &skyVBO);
	glBindVertexArray(skyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVert), &skyboxVert, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// configure shaders
	shaderGL.use();
	shaderGL.setInt("texture", 0);

	Shader skyShader("skybox.vs", "skybox.fs");
	skyShader.use();
	skyShader.setInt("skybox", 0);

	// wireframe mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// glLineWidth(2);

	bool first = true;

	while (!glfwWindowShouldClose(wind)) {
		// handle necessary stuff
		input_process(wind);

		// set a clear color, clear the previous frame's colors and buffer bit
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set up camera speed unmodified by graphics processing rate
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// use the frag and vertex shaders
		shaderGL.use();

		// change the visible window
		shaderGL.setMat4("view", camera.GetViewMatrix());

		// change the near/far rendered geometry 
		mat4 projection = perspective(radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		shaderGL.setMat4("projection", projection);


		// tell the GPU to use the initialized data; bind the desired texture
		glBindVertexArray(ArrObj);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

 		// for each intended coordinate, draw a new cube
		for (int i = 0; i < vertices.size(); i++) {
			voxel* cube = new voxel;
			mat4 transform = mat4(1.0f);
			cube->set_position(translate(transform, vertices[i]));
			shaderGL.setMat4("model", cube->position);
			if (first) {
				objects.push_back(cube);
			}
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		if (first) {
			collision_detect();
			cout << camera.coll_x_min.size() << " " << camera.coll_x_max.size() << endl;
			first = false;
		}
		
		// unbind VAO
		glBindVertexArray(0);

		glBindVertexArray(floorVAO);
		glBindTexture(GL_TEXTURE_2D, grass);
		mat4 transform = translate(mat4(1.0f), vec3(-0.5f, -0.5f, -0.5f));
		shaderGL.setMat4("model", transform);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		
		// draw the skybox
		glDepthFunc(GL_LEQUAL);
		skyShader.use();
		skyShader.setMat4("view", mat4(mat3(camera.GetViewMatrix())));
		skyShader.setMat4("projection", projection);
		glBindVertexArray(skyVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
		
		glfwSwapBuffers(wind);
		// to prevent excessive computation, wait for next frame interval
		glfwSwapInterval(1);
		glfwPollEvents();
	}
	objects.clear();
	glfwTerminate();
}

int main() {
	setup();
	glfw_setup();
	return 0;
};
// https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
// Research the implementation of AABB collision detection