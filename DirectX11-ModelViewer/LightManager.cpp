#include "LightManager.h"

Light& LightManager::GetDirectionalLight()
{
	return mLight[0];
}

Light& LightManager::GetLight(int index)
{
	if (index < 1)
		index = 1;
	if (index > 7)
		index = 7;
	return mLight[index];
}
