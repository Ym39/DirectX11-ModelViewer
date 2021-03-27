#include "CameraTransform.h"

void CameraTransformUI::Render(Camera& camera)
{
	XMFLOAT3 position = camera.GetPosition();
	XMFLOAT3 rotation = camera.GetRotation();
	float positionArray[3] = {position.x,position.y,position.z};
	float rotationArray[3] = { rotation.x,rotation.y,rotation.z };
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Position", positionArray, 1.0f, -FLT_MAX,FLT_MAX,"%.1f");
	ImGui::DragFloat3("Rotation", rotationArray, 1.0f, -FLT_MAX, FLT_MAX, "%.1f");
	ImGui::End();

	camera.SetPosition(positionArray[0], positionArray[1], positionArray[2]);
	camera.SetRotation(rotationArray[0], rotationArray[1], rotationArray[2]);
}
