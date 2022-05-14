#include "LightManager.h"
LightManager* LightManager::mInstance = nullptr;

LightSturct& LightManager::GetDirectionalLight()
{
	return mLight[0];
}

LightSturct* LightManager::GetLight(int index)
{
	if (index < 0 || index > NUM_OF_LIGHTS - 1)
		return nullptr;

	return &mLight[index];
}
