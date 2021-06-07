#pragma once
#ifndef __MATH_UTIL_H__
#define __MATH_UTIL_H__
#include <glm/glm.hpp>
namespace MathUtil
{
	static inline float GetModuleLength(glm::vec2& v)
	{
		return  sqrtf(pow(v.x, 2.0f) + pow(v.y, 2.0f));
	}

	static inline float GetModuleLength(glm::vec3& v)
	{
		return sqrtf(pow(v.x, 2.0f) + pow(v.y, 2.0f) + pow(v.z, 2.0f));
	}

	static float GetTheta(glm::vec2& v1, glm::vec2 v2)
	{
		float mod1 = GetModuleLength(v1);
		float mod2 = GetModuleLength(v2);
		
		return glm::degrees(acosf(glm::dot(v1, v2) / (mod1 * mod2)));
	}

	static float GetTheta(glm::vec3& v1, glm::vec3 v2)
	{
		float mod1 = GetModuleLength(v1);
		float mod2 = GetModuleLength(v2);

		return glm::degrees(acosf(glm::dot(v1, v2) / (mod1 * mod2)));
	}

}

#endif