/*
	Hydrogen is a GLTF asset loader designed by Pooya aka TheVoltage.
	Hydrogen is fast and reliable 3D gltf asset parser that can work with any Graphical API.
*/
#pragma once

#define HYD_INVALID_FILE 0x00
#define HYD_INVALID_PATH 0x01
#define HYD_INVALID_GLTF 0x02
#define HYD_INVALID_BIN  0x03
#define HYD_INVALID_GLB  0x04

#define HYD_OK		     0x05		

namespace Hydrogen
{
	class Loader;
	class Model;
	class Scene;
	class Object;
	class Mesh;
	class Material;
	class Matrix;
};