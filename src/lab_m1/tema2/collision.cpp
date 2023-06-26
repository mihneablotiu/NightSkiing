#include "track.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/tema2/collision.h"

/* The function used to determine the area of a triangle
* given the three points that create the triangle */
float collision::triangleArea(
	glm::vec3 firstPoint,
	glm::vec3 secondPoint,
	glm::vec3 thirdPoint
	) {

	glm::vec2 firstSecond = glm::vec2(secondPoint.x - firstPoint.x, secondPoint.z - firstPoint.z);
	glm::vec2 firstThird = glm::vec2(thirdPoint.x - firstPoint.x, thirdPoint.z - firstPoint.z);

	float crossProduct = (firstSecond.x * firstThird.y) - (firstSecond.y * firstThird.x);

	return (fabs(crossProduct) / 2);
}

/* The function that checks if a point is inside a triangle
* made by three other points / vertexes */
bool collision::insideTriangle(
	glm::vec3 point,
	glm::vec3 firstVertex,
	glm::vec3 secondVertex,
	glm::vec3 thirdVertex) {

	float triangleArea = collision::triangleArea(firstVertex, secondVertex, thirdVertex);

	float areaSum = 0;

	areaSum += collision::triangleArea(firstVertex, secondVertex, point);
	areaSum += collision::triangleArea(firstVertex, thirdVertex, point);
	areaSum += collision::triangleArea(secondVertex, thirdVertex, point);

	if (fabs(triangleArea - areaSum) < 0.01f) {
		return true;
	}

	return false;
}

/* The function that checks if the player's car collides
with any other car by the circle collision formula */
bool collision::determineDinamicCollision(
	int carNumber,
	float lengthOfTheCar,
	std::vector<glm::vec3> translationPositions,
	glm::vec3 carPosition) {

	float minimumCollisionDistance = 2 * lengthOfTheCar;

	glm::vec3 currentPosition = translationPositions.at(carNumber);
	float collisionDistanceX = pow((carPosition.x - currentPosition.x), 2);
	float collisionDistanceY = pow((carPosition.y - currentPosition.y), 2);
	float collisionDistanceZ = pow((carPosition.z - currentPosition.z), 2);

	if (sqrt(collisionDistanceX + collisionDistanceY + collisionDistanceZ) <= minimumCollisionDistance) {
		return true;
	}

	return false;
}