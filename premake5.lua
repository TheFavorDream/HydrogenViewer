workspace "HydrogenViewer"
    configurations { "Debug", "Release" }
    platforms {"x64"}

	OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    filter { "platforms:x64" }
        system "Windows"
        architecture "x86_64"

    filter {"configurations:Debug"}
        defines {"DEBUG"}
        optimize "off"

    filter {"configurations:Release"}
        defines {"RELEASE"}
        optimize "on"

        startproject "HydrogenViewer"

    project "HydrogenViewer"
        location "./HydrogenViewer"
        kind "ConsoleApp"
        language "C++"

        targetdir ("./bin/"..OutputDir.."/PlayGround")
        objdir ("./bin-obj/"..OutputDir.."/PlayGround")


        filter ("system:windows")
			defines {"WINDOWS"}


        defines {"GLEW_STATIC"}

        files {
            "./HydrogenViewer/**.cpp",
            "./HydrogenViewer/**.h"
        }

        includedirs{
            "./3rdParty/glfw/include/",
            "./3rdParty/glew/",
            "./HydrogenViewer/"
        }

        libdirs{
			"./bin/"..OutputDir.."/glfw",
			"./3rdParty/glew/",

        }

        links {"opengl32", "glew32s", "glfw","kernel32", "user32" , "gdi32", "winspool", "comdlg32", "advapi32", "shell32","ole32", 
		"oleaut32", "uuid", "odbc32", "odbccp32"}


    project "Hydrogen"
        location "./Hydrogen"
        kind "StaticLib"
        language "C++"

        files{
            "./Hydrogen/**.cpp",
            "./Hydrogen/**.h"
        }
        
        filter {"configurations:Debug"}
            defines {"HYD_DEBUG"}
            optimize "off"

        filter {"configurations:Release"}
            defines {"HYD_RELEASE"}
            optimize "on"
        

        includedirs{
            "./Hydrogen"
        }

    project "glfw"
        location "./3rdParty/glfw"
        kind "SharedLib"
		language "C"

        targetdir ("./bin/"..OutputDir.."/glfw")
        objdir ("./bin-obj/"..OutputDir.."/glfw")


        files {
            "./3rdParty/glfw/src/**.c",
            "./3rdParty/glfw/include/**.h"
        }

        filter ("system:windows")
			defines {"_WIN32", "_GLFW_WIN32"}
			systemversion "10.0.17763.0"
			staticruntime "On"

		filter("configurations:Debug")
			symbols "On"
			defines {"_GLFW_BUILD_DLL"}
		filter("configurations:Release")
			optimize "On"
			defines {"_GLFW_BUILD_DLL"}

		filter {"system:windows", "configurations:Debug"}
			buildoptions {"/MDd"}
		filter {"system:windows", "configurations:Release"}
			buildoptions {"/MT"}