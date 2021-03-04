#include "Camera.h"

Camera::Camera()
{
	mPositionX = 0.0f;
	mPositionY = 0.0f;
	mPositionZ = 0.0f;

	mRotationX = 0.0f;
	mRotationY = 0.0f;
	mRotationZ = 0.0f;
}

Camera::Camera(const Camera&)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	/*mPositionX = x;
	mPositionY = y;
	mPositionZ = z;*/
	mTransform.SetPosition(XMFLOAT3(x,y,z));
}

void Camera::SetRotation(float x, float y, float z)
{
	/*mRotationX = x;
	mRotationY = y;
	mRotationZ = z;*/
	mTransform.SetRotation(XMFLOAT3(x, y, z));
}

XMFLOAT3 Camera::GetPosition()
{
	//return XMFLOAT3(mPositionX,mPositionY,mPositionZ);
	return mTransform.GetPosition();
}

XMFLOAT3 Camera::GetRotation()
{
	//return XMFLOAT3(mRotationX,mRotationY,mRotationZ);
	return mTransform.GetRotation();
}

void Camera::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position.x = mPositionX;
	position.y = mPositionY;
	position.z = mPositionZ;

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&mTransform.GetPosition());

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = mTransform.GetRotation().x * 0.0174532925f;
	yaw = mTransform.GetRotation().y * 0.0174532925f;
	roll = mTransform.GetRotation().z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	mViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = mViewMatrix;
}
