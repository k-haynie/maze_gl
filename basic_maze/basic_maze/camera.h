#ifndef CAMERA_H
#define CAMERA_H

#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

using namespace std;
using namespace glm;

/*
- better collision detector without break and with vectors
- on-screen representation of maze
- individual lighting channels for each cube?
- polish up file structure, create a repo

*/
// drawn from the amazing tutorial on learnopengl.com
// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h

enum CAMERA_MOVEMENT {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
 // Yaw defines initial view direction
const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 2.0f;
const float SNSTV = 0.1f;

class Camera {
public:
	vec3 Position;
	vec3 Front;
	vec3 Up;
	vec3 Right;
	vec3 WorldUp;
	bool init = false;

	float Yaw;
	float Pitch;

	float MvtSpeed;
	float MouseSnstv;

	// prevent collision
	bool coll = false;
	vector<float> coll_x_min;
	vector<float> coll_x_max;
	vector<float> coll_z_min;
	vector<float> coll_z_max;

	// vector construction
	Camera(vec3 position = vec3(1.0f, 0.0f, 1.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(vec3(0.0f, 0.0f, 0.0f)), MvtSpeed(SPEED), MouseSnstv(SNSTV) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
		bool init = true;
	}

	// scalar construction - best practices, not actually utilized
	Camera (float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(vec3(0.0f, 0.0f, 0.0f)), MvtSpeed(SPEED), MouseSnstv(SNSTV) {
		Position = vec3(posX, posY, posZ);
		WorldUp = vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
		bool init = true;
	}

	mat4 GetViewMatrix() { 
		return lookAt(Position, Position + Front, Up); }

	void initTarget(vec3 target) {
		Front = target;
		updateCameraVectors();
	}


	// on each itr, check each set of limiting coords
	// if none of them are colliding, than move.
	// otherwise, lock the appropriate axis

	void ProcessKeyboard(CAMERA_MOVEMENT direction, float deltaTime) {
		float velocity = MvtSpeed * deltaTime;
		bool x_coll = false;
		bool z_coll = false;

		vec3 Proposed = vec3(Position.x, Position.y, Position.z);

		if (direction == FORWARD) {
			Proposed += Front * velocity;
		}
		if (direction == BACKWARD) {
			Proposed -= Front * velocity;
		}
		if (direction == LEFT) {
			Proposed -= Right * velocity;
		}
		if (direction == RIGHT) {
			Proposed += Right * velocity;
		}

		// iterate over, check for collisions
		/*
		for (int i = 0; i < coll_x_min.size(); i++) {
			if (Proposed.x > coll_x_min[i] && Proposed.x < coll_x_max[i]) {
				cout << "Camera: " << Position.x << " I: " << i << endl;
				cout << "X: " << coll_x_min[i] << " " << coll_x_max[i] << endl;
				cout << "x collision" << endl;
				x_coll = true;
			}

			if (Proposed.z > coll_z_min[i] && Proposed.z < coll_z_max[i]) {
				cout << "z collision" << endl;
				z_coll = true;
			}
		}*/

		// update camera position if no collisions were found
		if (!x_coll) {
			Position.x = Proposed.x;
		}
		if (!z_coll) {
			Position.z = Proposed.z;
		}

		Position.y = Proposed.y;
		// Position.y = 0.0f;
	}

	void ProcessMouse(float xoffset, float yoffset, GLboolean constrainPitch = true) {
		xoffset *= MouseSnstv;
		yoffset *= MouseSnstv;

		Yaw += xoffset;
		Pitch += yoffset;

		// prevent screen from flipping
		if (constrainPitch) {
			if (Pitch > 89.0f) {
				Pitch = 89.0f;
			}
			if (Pitch < -89.0f) {
				Pitch = -89.0f;
			}
		}

		updateCameraVectors();
	}

private:
	void updateCameraVectors() {
		vec3 front;
		// change the vectors
		front.x = cos(radians(Yaw)) * cos(radians(Pitch));
		front.y = sin(radians(Pitch));
		front.z = sin(radians(Yaw)) * cos(radians(Pitch));

		// normalize the vectors
		Front = normalize(front);
		Right = normalize(cross(Front, WorldUp));
		Up = normalize(cross(Right, Front));
	}
};

#endif