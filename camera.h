#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum camera_movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera {
private:
	float yaw = -90.f;
	float pitch = 0.f;
	float speed = 2.5f;
	float sensitivity = 0.1f;

	glm::vec3 position = glm::vec3(0.f, 0.f, 3.f);
	glm::vec3 front = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 flat_front = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 right = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 world_up = glm::vec3(0.f, 1.f, 0.f);

public:
	Camera(glm::vec3 position_, float speed_, float sensitivity_) {
		position = position_;
		speed = speed_;
		sensitivity = sensitivity_;

		updateVectors();
	}

	void processKeyboard(camera_movement direction, float delta_time) {
		float velocity = speed * delta_time;

		if (direction == FORWARD)
			position += flat_front * velocity;
		if (direction == BACKWARD)
			position -= flat_front * velocity;
		if (direction == LEFT)
			position -= right * velocity;
		if (direction == RIGHT)
			position += right * velocity;
		if (direction == UP)
			position.y += velocity;
		if (direction == DOWN)
			position.y -= velocity;
	}

	void processMouse(float xoffset, float yoffset) {
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		updateVectors();
	}

	glm::mat4 getViewMatrix() const {
		return glm::lookAt(position, position + front, up);
	}

	glm::vec3 getPosition() const {
		return position;
	}

private:
	void updateVectors() {
		glm::vec3 front_ = glm::vec3(0.f, 0.f, 0.f);
		front_.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front_.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		flat_front = glm::normalize(front_);

		front_.y = sin(glm::radians(pitch));
		front = glm::normalize(front_);

		right = glm::normalize(glm::cross(front, world_up));
		up = glm::normalize(glm::cross(right, front));
	}
};


