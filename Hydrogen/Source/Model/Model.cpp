#include "Model.h"

namespace Hydrogen
{
	Model::Model(std::string& pPath)
	{
		FileExtension File = GetFileExtension(pPath);

		switch (File)
		{
		case FILE_GLTF:
			//Load GLTF
			ParseGLTF(pPath);
			break;
		case FILE_GLB:
			//Load GLB
			ParseGLB(pPath);
			break;
		}
	}

	int Model::Free()
	{
		m_Meshes.clear();
		m_Buffers.clear();
		m_VertexArrays.clear();
		return 0;
	}

	void Model::RenderScene()
	{
		for (auto& i: m_Meshes)
		{
			i.Render(this);
		}
	}



	int Model::ParseGLTF(std::string& pPath)
	{
		//Open The File And Parse it
		std::ifstream Source(pPath);
		if (Source.fail())
		{
			return HYD_INVALID_PATH;
		}
		m_GLTF = json::parse(Source);
		Source.close();

		std::string RootPath = pPath.substr(0, pPath.find_last_of("\\")+1);
		
		//Load URIs:
		std::vector<std::string> URIBuffers;

		int Err = LoadURI(m_GLTF["buffers"], RootPath, URIBuffers);
		if(Err != HYD_OK)
		{
			std::cout << "Can not access the uris\n";
			return Err;
		}

		//Construct Buffer Views
		std::vector<BufferView> BufferViews;
		Err = SetupBufferViews(m_GLTF["bufferViews"], BufferViews, URIBuffers);
		if (Err != HYD_OK)
		{
			return Err;
		}
		//Free the Uris cached data:
		URIBuffers.clear();


		//Construct Accessors:
		std::vector<Accessor> Accessors;
		Err = SetupAccessors(m_GLTF["accessors"], Accessors, BufferViews);
		if (Err != HYD_OK)
		{
			return Err;
		}
		//Free Buffer View Cache:
		BufferViews.clear();

		Err = SetupMeshes(m_GLTF["meshes"], Accessors);
		if (Err != HYD_OK)
		{
			return Err;
		}
		Accessors.clear();

		Err = SetupNodes(m_GLTF["nodes"]);
		if (Err != HYD_OK)
		{
			return Err;
		}

		//Clear the Json File
		m_GLTF.clear();
		return HYD_OK;
	}

	int Model::ParseGLB(std::string & pPath)
	{
		return 0;
	}

	FileExtension Model::GetFileExtension(std::string & pPath)
	{
		std::string Prefix = pPath.substr(pPath.find_last_of("."));

		if(Prefix == ".gltf")
			return FILE_GLTF;
		else if (Prefix == ".glb")
			return FILE_GLB;
		
		return FILE_INVALID;
	}

	int Model::LoadURI(json & pBuffer, std::string & pRootPath, std::vector<std::string>& pBuffers)
	{
		if (pBuffer == nullptr)
		{
			return HYD_CORRUPTED_GLTF;
		}

		std::ifstream Uri;
		for (auto &i : pBuffer)
		{
			Uri.open(pRootPath + i.value("uri", ""), std::ios::binary);
			if (Uri.fail())
				return HYD_URI_FAILED;

			std::string Data;
			Data.resize(i.value("byteLength", 0));
			Uri.read(&Data[0], Data.size());
			pBuffers.push_back(std::move(Data));
			Uri.close();
		}
		return HYD_OK;
	}

	int Model::SetupBufferViews(json & pBufferView, std::vector<BufferView>& pBufferViews, std::vector<std::string>& pBuffers)
	{
		if (pBufferView == nullptr)
			return HYD_CORRUPTED_GLTF;

		for (auto &i : pBufferView)
		{
			pBufferViews.push_back({});
			pBufferViews[pBufferViews.size() - 1].Target = i["target"];
			pBufferViews[pBufferViews.size() - 1].Data = pBuffers.at((size_t)i.value("buffer", 0)).substr((size_t)i.value("byteOffset", 0), (size_t)i.value("byteLength", -1));

		}

		return HYD_OK;
	}

	int Model::SetupAccessors(json & pAccessor, std::vector<Accessor>& pAccessors, std::vector<BufferView>& pBufferViews)
	{
		if (pAccessor == nullptr)
			return HYD_CORRUPTED_GLTF;

		for (auto &i : pAccessor)
		{
			pAccessors.push_back({});
			pAccessors.at(pAccessors.size() - 1).Data = pBufferViews.at(i["bufferView"]);
			pAccessors.at(pAccessors.size() - 1).ComponentType = i.value("componentType", GL_INVALID_ENUM);
			pAccessors.at(pAccessors.size() - 1).Count = i.value("count", 0);
			pAccessors.at(pAccessors.size() - 1).DataType = Accessor::RetriveType(std::string(i["type"]));
		}

		return HYD_OK;
	}

	int Model::SetupMeshes(json & pMeshes, std::vector<Accessor>& pAccessors)
	{
		if (pMeshes == nullptr)
			return HYD_CORRUPTED_GLTF;

		for (auto &i : pMeshes)
		{
			std::vector<Primitive> Primitives;
			ProcessPrimitives(i["primitives"], pAccessors, Primitives);

			//Consturct Mesh Object
			//----------------
			m_Meshes.push_back(Mesh(i.value("name", "Unknown"), Primitives));
			std::cout << "Mesh Constructed. Name:" << m_Meshes[m_Meshes.size() - 1].GetMeshName() << '\n';
		}

		return HYD_OK;
	}

	int Model::SetupNodes(json & pNodes)
	{
		if (pNodes == nullptr)
			return HYD_CORRUPTED_GLTF;
		
		for (auto& i : pNodes)
		{
			std::cout << i.dump(4) << '\n';
		}
		
		return HYD_OK;
	}

	int Model::ProcessPrimitives(json& pPrimitive, std::vector<Accessor>& pAccessors, std::vector<Primitive>& pPrimitives)
	{
		if (pPrimitive == nullptr)
			return HYD_CORRUPTED_GLTF;

		for (auto &i : pPrimitive)
		{
			Primitive primitive;
			
			ProcessAttributes(i["attributes"], pAccessors, primitive);

			//Take care of ElementBuffer
			if (i.value("indices", -1) != -1)
			{
				ProcessElementBuffer(pAccessors[i["indices"]], primitive.EboID);
			}
			pPrimitives.push_back(primitive);
		}
		return HYD_OK;
	}

	int Model::ProcessElementBuffer(Accessor & pAccessors, int32 & pEboID)
	{
		Buffer Ebo;
		Ebo.CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, pAccessors.Data.Data.length(), &pAccessors.Data.Data[0], pAccessors.Count);
		m_Buffers.push_back(std::move(Ebo));
		pEboID = m_Buffers.size() - 1;

		return HYD_OK;
	}


	int Model::ProcessAttributes(json& pAttribute, std::vector<Accessor>& pAccessors, Primitive &pPrimitiveRef)
	{

		int32 BufferSize = 0;

		VertexArray VAO;
		VAO.CreateVertexArray();
		VAO.Bind();

		// A Temp cache for storing Attribs Index
		std::vector<int32> AttributeStorage;

		//Check The Supported Attributes:
		int32 Index = -1;
		if ((Index=pAttribute.value("POSITION", -1)) != -1)
		{
			BufferSize += pAccessors[Index].Data.Data.length();
			pPrimitiveRef.Attributes.POSITION = Index;
			AttributeStorage.push_back(Index);
		}
		if ((Index = pAttribute.value("NORMAL", -1)) != -1)
		{
			BufferSize += pAccessors[Index].Data.Data.length();
			pPrimitiveRef.Attributes.NORMALS = Index;
			AttributeStorage.push_back(Index);
		}
		if ((Index = pAttribute.value("TANGENT", -1)) != -1)
		{
			BufferSize += pAccessors[Index].Data.Data.length();
			pPrimitiveRef.Attributes.TANGENT = Index;
			AttributeStorage.push_back(Index);
		}
		if ((Index = pAttribute.value("TEXCOORD_0", -1)) != -1)
		{
			BufferSize += pAccessors[Index].Data.Data.length();
			pPrimitiveRef.Attributes.TEXCOORDS_0 = Index;
			AttributeStorage.push_back(Index);
		}
		if ((Index = pAttribute.value("TEXCOORD_1", -1)) != -1)
		{
			BufferSize += pAccessors[Index].Data.Data.length();
			pPrimitiveRef.Attributes.TEXCOORDS_1 = Index;
			AttributeStorage.push_back(Index);
		}
		if ((Index = pAttribute.value("COLOR_0", -1)) != -1)
		{
			BufferSize += pAccessors[Index].Data.Data.length();
			pPrimitiveRef.Attributes.COLOR_0 = Index;
		}

		Buffer VBO;
		VBO.CreateBuffer(GL_ARRAY_BUFFER, BufferSize, nullptr);

		int32 Offset = 0;
		for (auto &i : AttributeStorage)
		{
			VBO.CopyDataChunk(Offset, pAccessors[i].Data.Data.length(), &pAccessors[i].Data.Data[0]);
			VAO.AddAttribute(pAccessors[i]);
			Offset += pAccessors[i].Data.Data.length();
		}

		m_Buffers.push_back(std::move(VBO));
		pPrimitiveRef.VboID = m_Buffers.size() - 1;
		m_VertexArrays.push_back(std::move(VAO));
		pPrimitiveRef.VaoID = m_VertexArrays.size() - 1;

		return HYD_OK;
	}

	Type Accessor::RetriveType(std::string & pType)
	{
		if (pType == "SCALAR")
			return SCALER;
		else if (pType == "VEC2")
			return VEC2;
		else if (pType == "VEC3")
			return VEC3;
		else if (pType == "VEC4")
			return VEC4;
		else if (pType == "MAT2")
			return MAT2;
		else if (pType == "MAT3")
			return MAT3;
		else if (pType == "MAT4")
			return MAT4;
		return INVALID;
	}

	void CheckOpenGLErrors(const char* file, uint32 Line)
	{
		GLenum ErrorCode = 0;
		while ((ErrorCode = glGetError()) != GL_NO_ERROR)
		{
			std::cout << "[OpenGL Error] File:" << file << "=>Line:" << Line << "=>ErrorCode:" << ErrorCode << '\n';
		}
	}

};