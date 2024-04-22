#ifndef _VECTOR_CAMERA_INCLUDE
#define _VECTOR_CAMERA_INCLUDE


#include <glm/glm.hpp>


// VectorCamera contains the properies of the camera the scene is using
// It is responsible for computing the associated GL matrices


class VectorCamera
{

public:
	VectorCamera();
	~VectorCamera();
	
	void init(const glm::vec3 &initPosition, float initDirection = 180.f);
	
	const glm::vec3 &getPosition() const;
	void setPosition(const glm::vec3 &pos);
	float getDirection() const;
	void setDirection(float direction);

public:
	void resizeCameraViewport(int width, int height);
	void rotateCamera(float rotation);
	void changePitch(float rotation);
	void moveForward(float distance);
	void strafe(float distance);
	void sendToOpenGL();
	
	glm::mat4 &getProjectionMatrix();
	glm::mat4 &getViewMatrix();

private:
	void computeViewMatrix();

private:
	glm::vec3 position;					// Camera parameters
	float angleDirection, anglePitch;
	float rangeDistanceCamera[2];
	glm::mat4 projection, view;	// OpenGL matrices

};


#endif // _VECTOR_CAMERA_INCLUDE

