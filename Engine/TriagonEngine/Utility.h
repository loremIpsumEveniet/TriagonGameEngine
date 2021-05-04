#pragma once
#include "dependencies.h"

///Below are some utility functions related to matrix math

glm::mat4 Translate(glm::vec3 Transform) {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, Transform);

	return trans;
}

glm::mat4 SetRotation(float EulerX, float EulerY, float EulerZ) { //This is terrible and does not work
	glm::mat4 transX = glm::mat4(1.0f); //Create Identity Matrix(s)
	glm::mat4 transY = glm::mat4(1.0f);
	glm::mat4 transZ = glm::mat4(1.0f);

	transX = glm::rotate(transX, glm::radians(EulerX), glm::vec3(1.0, 0.0, 0.0));
	transY = glm::rotate(transY, glm::radians(EulerY), glm::vec3(0.0, 1.0, 0.0));
	transZ = glm::rotate(transZ, glm::radians(EulerZ), glm::vec3(0.0, 0.0, 1.0));

	return transX * transY * transZ;
}
