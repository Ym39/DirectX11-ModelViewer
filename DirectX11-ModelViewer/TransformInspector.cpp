#include "TransformInspector.h"

void TransformInspector::Render(Transform& transform)
{
	float position[3] = { transform.GetPosition().x,transform.GetPosition().y ,transform.GetPosition().z };
	float rotation[3] = { transform.GetRotation().x,transform.GetRotation().y,transform.GetRotation().z };
	float scale[3] = { transform.GetScale().x,transform.GetScale().y,transform.GetScale().z };
	ImGui::Begin("Transform");

	ImGui::InputFloat3("Position", position);
	ImGui::InputFloat3("Rotation", rotation);
	ImGui::InputFloat3("Scale", scale);

	ImGui::End();
	transform.SetPosition(XMFLOAT3(position[0], position[1], position[2]));
	transform.SetRotation(XMFLOAT3(rotation[0], rotation[1], rotation[2]));
	transform.SetScale(XMFLOAT3(scale[0], scale[1], scale[2]));
}
