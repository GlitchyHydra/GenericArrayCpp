workspace "Array"
    architecture "x64"
    startproject "Array"
    configurations {"Debug", "Release"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GoogleTest"] = "vendor/googletest/googletest"

group "Dependecies"
  --  include "vendor/googletest"

group ""

project "Array"
    location "Array"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
	{
		    "%{prj.name}/src/**.h",
		    "%{prj.name}/src/**.cpp",
            "%{prj.name}/src/**.hpp"
    }
    
    defines
	{
       
	}

    libdirs
    {
        --"%{prj.name}/vendor/assimp"
	}

    includedirs
	{
		"%{prj.name}/src",
        "%{IncludeDir.GoogleTest}/include"
    }
    
    links 
	{ 
		--"vendor/googletest/bin/lib/Release/gtest.lib",
        --"vendor/googletest/bin/lib/Release/gtest_main.lib",
        --"vendor/googletest/bin/lib/Release/gmock.lib",
        --"vendor/googletest/bin/lib/Release/gmock_main.lib"
    }
    
    filter "system:windows"
		systemversion "latest"


    filter "configurations:Debug"
        runtime "Debug"
		symbols "on"

        defines
        {
            "_HAS_ITERATOR_DEBUGGING"
        }
       
        links
        {
        "vendor/googletest/bin/lib/Debug/gtestd.lib",
        "vendor/googletest/bin/lib/Debug/gtest_maind.lib"
        }

	filter "configurations:Release"
        runtime "Release"
		optimize "on"

        defines
        {

        }

        links
        {
        "vendor/googletest/bin/lib/Release/gtest.lib",
        "vendor/googletest/bin/lib/Release/gtest_main.lib"
        }

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"