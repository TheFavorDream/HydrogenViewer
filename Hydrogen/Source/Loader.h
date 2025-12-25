#pragma once

#include "Common.h"
#include <string>
#include "Model/Model.h"

#include "Glew/glew.h"
#include "Parser/Json.h"

namespace Hydrogen
{

	enum SupportedAPIs {NONE, OPENGL};

	class Loader
	{
	public:

		static int SetUpHydrogen(uint32 pGraphicalAPI);
		static Model* Load(std::string pModelPath, int pFlag = 0);

		static int Free(Model** pModel);

	private:
		static SupportedAPIs m_CurrentAPI;
		friend class Model;
	};
};