#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <cmath>

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

/**
Define possible options for camera movement. This is used so as not to be bound
to specific inputs.
*/
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

/**
Abstract camera class that processes input and calculates the corresponding 
Eular Angles, Vectors and Matrices to be used in OpenGL
*/
class Camera
{
public:

	Camera();

	const glm::vec3& m_position() const;
	/**
	Set the camera position.

	@param position Camera position.
	*/
	void m_setPosition(const glm::vec3& position);
	void m_offsetPosition(Camera_Movement movement, GLfloat deltaTime);

	GLfloat m_fieldOfView() const;
	/**
	Set the vertical viewing angle of the camera, in degrees.

	@param fliedOfView FOV. The value must be between 0 and 180.
	*/
	void m_setFieldOfView(float fieldOfView);

	/**
	The closest visible distance from the camera.
	*/
	GLfloat m_nearPlane() const;

	/**
	The farthest visible distance from the camera
	*/
	GLfloat m_farPlane() const;

	/**
	Set the near and far plane distances.

	@param nearPlane Minimum visible distance from the camera. Must be > 0.
	@param farPlane Maximum visible distance from the camera. Must be > nearPlane.
	*/
	void m_setNearAndFarPlanes(GLfloat nearPlane, GLfloat farPlane);

	/**
	Get a rotation matrix expressing the direction the camera is looking to.
	It does not include translation, i.e. the camera's position.
	*/
	glm::mat4 m_getOrientation() const;

	/**
	Offset the camera orientation.

	The verticle angle is constrained between -85° and 85° to avoid gimbal lock.

	@param upAngle Angle (in degrees) to offset upwards. Negative values are downwards.
	@param rightAngle Angle (in degrees) to offset rightwards. Negative values are leftwards.
	*/
	void m_offsetOrientation(GLfloat upAngle, GLfloat rightAngle);

	/**
	Get the current viewport aspect ratio.
	
	@result Viewport aspect ratio.
	*/
	GLfloat m_getViewportAspectRatio() const;

	/**
	Get the current viewport aspect ratio, i.e. ratio between the window's current width and height.

	@param width Window widtht.
	@param height Window height.
	*/
	void m_setViewportAspectRatio(GLfloat width, GLfloat height);

	/**
	Get a unit vector representing the direction the camera is facing.
	*/
	glm::vec3 m_forward() const;

	/**
	Get a unit vector representing the direction to the right of the camera.
	*/
	glm::vec3 m_right() const;

	/**
	Get a unit vector representing the direction out of the top of the camera.
	*/
	glm::vec3 m_up() const;

	/**
	Projection matrix.
	*/
	glm::mat4 m_projection() const;

	/**
	View matrix.

	This matrix is used to go from World Space (all vertices defined relatively to the 
	center of the world) to Camera Space (all vertices defined relatively to the camera).
	*/
	glm::mat4 m_view() const;


	/**
	Model View Projection matrix.

	This is the matrix to be used in the vertex shader.
	*/
	glm::mat4 m_MVP(const glm::mat4& model) const;

private:

	glm::vec3 _position;
	GLfloat _pitch;
	GLfloat _yaw;
	GLfloat _fieldOfView;
	GLfloat _nearPlane;
	GLfloat _farPlane;
	GLfloat _viewportAspectRatio;

	void m_normalizeAngles();
};

#endif // !CAMERA_H

