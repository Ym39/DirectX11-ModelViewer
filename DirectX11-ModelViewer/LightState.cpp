#include "LightState.h"

void LightState::Render(Light* light)
{
    XMFLOAT4 ambientColor = light->GetAmbientColor();
    float ambientColorValue[4] = {ambientColor.x,ambientColor.y,ambientColor.z,ambientColor.w};
    XMFLOAT4 specularColor = light->GetSpecularColor();
    float specularColorValue[4] = { specularColor.x,specularColor.y,specularColor.z,specularColor.w };
    float specularPower = light->GetSpecularPower();
    XMFLOAT3 lightPosition = light->GetPosition();
    float positionValue[3] = { lightPosition.x,lightPosition.y,lightPosition.z };

    ImGui::Begin("Light");
    ImGui::ColorPicker4("AmbinetColor", ambientColorValue);
    ImGui::ColorPicker4("SpecularColor", specularColorValue);
    ImGui::DragFloat("SpecualrPower", &specularPower, 0.1f, 0.0f, 15.0f);
    ImGui::DragFloat3("LightPosition", positionValue, 1.0f, -1000.0f, 1000.0f);
    ImGui::End();

    light->SetAmbientColor(ambientColorValue[0], ambientColorValue[1], ambientColorValue[2], ambientColorValue[3]);
    light->SetSpecularColor(specularColorValue[0], specularColorValue[1], specularColorValue[2], specularColorValue[3]);
    light->SetSpecularPower(specularPower);
    light->SetPosition(positionValue[0], positionValue[1], positionValue[2]);

}
