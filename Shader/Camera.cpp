#include "Camera.h"

static const float maxVerticalAngle = 85.0f; // Must be less than 90 to avoid gimbal lock
static const GLfloat cameraSpeed = 0.01;
static const GLfloat cameraSensitivity = 0.05;

Camera::Camera():
	_position(0.0, 0.0, 0.0),
	_pitch(0.0),
	_yaw(0.0),
	_fieldOfView(45.0),
	_nearPlane(0.01),
	_farPlane(100.0),
	_viewportAspectRatio(4.0F/3.0F)
{
}

const glm::vec3 & Camera::m_position() const
{
	return _position;
}

void Camera::m_setPosition(const glm::vec3 & position)
{
	_position = position;
}

void Camera::m_offsetPosition(Camera_Movement movement, GLfloat deltaTime)
{
	/* This method is used to compute the displacement of the camera's position.
	displacement = moveSpeed * secondElapsed * moveDirection.
	*/
	switch (movement)
	{
	case FORWARD:
		_position += cameraSpeed * deltaTime * m_forward();
		break;
	case RIGHT:
		_position += cameraSpeed * deltaTime * m_right();
		break;
	case BACKWARD:
		_position += cameraSpeed * deltaTime * -m_forward();
		break;
	case LEFT:
		_position += cameraSpeed * deltaTime * -m_right();
		break;
	case UP:
		_position += cameraSpeed * deltaTime * glm::vec3(0, 1, 0);
		break;
	case DOWN:
		_position += cameraSpeed * deltaTime * glm::vec3(0, -1, 0);
		break;
	}
}

GLfloat Camera::m_fieldOfView() const
{
	return _fieldOfView;
}

void Camera::m_setFieldOfView(float fieldOfView)
{
	_fieldOfView = fieldOfView;
}

GLfloat Camera::m_nearPlane() const
{
	return _nearPlane;
}

GLfloat Camera::m_farPlane() const
{
	return _farPlane;
}

void Camera::m_setNearAndFarPlanes(GLfloat nearPlane, GLfloat farPlane)
{
	_nearPlane = nearPlane;
	_farPlane = farPlane;
}

glm::mat4 Camera::m_getOrientation() const
{
	glm::mat4 orientation;

	orientation = glm::rotate(orientation, glm::radians(_yaw), glm::vec3(1, 0, 0));
	orientation = glm::rotate(orientation, glm::radians(_pitch), glm::vec3(0, 1, 0));

	return orientation;
}

void Camera::m_offsetOrientation(GLfloat upAngle, GLfloat rightAngle)
{
	_yaw += upAngle * cameraSensitivity;
	_pitch += rightAngle * cameraSensitivity;

	m_normalizeAngles();
}

GLfloat Camera::m_getViewportAspectRatio() const
{
	return _viewportAspectRatio;
}

void Camera::m_setViewportAspectRatio(GLfloat width, GLfloat height)
{
	_viewportAspectRatio = width / height;
}

glm::vec3 Camera::m_forward() const
{
	glm::vec3 forward = glm::inverse(m_getOrientation())*glm::vec4(0, 0, -1, 1);
	return forward;
}

glm::vec3 Camera::m_right() const
{
	glm::vec3 right = glm::inverse(m_getOrientation()) * glm::vec4(1, 0, 0, 1);
	return right;
}

glm::vec3 Camera::m_up() const
{
	/* Knowing that the up direction is always (0,1,0) after the camera rotation
	(i.e., the camera direction) has been applied, we can multiply this 
	vector by the inverse rotation giving us the up direction before the camera 
	rotation was applied. 
	Since (0,1,0) is a unit vector, it is not necessary to normalize the result. */
	glm::vec3 up = glm::inverse(m_getOrientation()) * glm::vec4(0, 1, 0, 1);
	return up;
}

glm::mat4 Camera::m_projection() const
{
	glm::mat4 projection = glm::perspective(glm::radians(_fieldOfView), _viewportAspectRatio, _nearPlane, _farPlane);
	return projection;
}

glm::mat4 Camera::m_view() const
{
	/* We are using '-_position' because instead of moving the camera 
	forward we are pulling the whole 3D scene backward. */
	return m_getOrientation() * glm::translate(glm::mat4(), -_position);
}

glm::mat4 Camera::m_MVP(const glm::mat4& model) const
{
	return m_projection() * m_view() * model;
}

glm::mat4 Camera::m_matrix() const
{
	return m_projection() * m_view();
}

void Camera::m_normalizeAngles()
{
	_pitch = fmodf(_pitch, 360.0f);

	// fmodf can return negative values, but this will make them all positive
	if (_pitch < 0.0f)
	{
		_pitch += 360.0f;
	}

	if (_yaw > maxVerticalAngle)
	{
		_yaw = maxVerticalAngle;
	}
	else if (_yaw < -maxVerticalAngle)
	{
		_yaw = -maxVerticalAngle;
	}
}
