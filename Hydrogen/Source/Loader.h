#pragma once

#include "../Hydrogen.h"
#include <string>

namespace Hydrogen
{
	class Loader
	{
	public:

		static Model* Load(std::string pModelPath, int pFlag = 0);
		static Model* Load(const std::string& pModelPath, int pFlag=0);

		static int Free(Model** pModel);

	private:
	};
};