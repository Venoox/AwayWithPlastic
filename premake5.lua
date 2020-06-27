workspace "AwayWithPlastic"
   configurations { "Debug", "Release" }

   project "AwayWithPlastic"
      kind "ConsoleApp"
      language "C++"
	   targetdir("bin/")
	   objdir("bin/int/")
      cppdialect "C++17"
      files { "AwayWithPlastic/src/**.h", "AwayWithPlastic/src/**.cpp" }
      
      filter "system:windows"
          defines { "WINDOWS", "GLEW_STATIC" }
          includedirs { "AwayWithPlastic/src/vendor/", "Dependencies/glew-2.1.0/include/", "Dependencies/SDL2-2.0.9/include/", "Dependencies/freetype-2.10.0/include/" }
          libdirs { "Dependencies/glew-2.1.0/lib/Release/Win32/", "Dependencies/SDL2-2.0.9/lib/x86/", "Dependencies/freetype-2.10.0/win32" }
          links { "opengl32", "glu32", "glew32s", "sdl2", "sdl2main", "freetype" }

      filter "system:linux"
          defines { "LINUX" }
      	 includedirs { "AwayWithPlastic/src/vendor/", "/usr/include/SDL2", "Dependencies/freetype-2.10.0/include/" }
      	 links { "SDL2", "SDL2main", "GLEW", "GL", "GLU", "X11", "Xxf86vm", "Xrandr", "pthread", "Xi", "dl", "Xinerama", "freetype" }

      filter "configurations:Debug"
         defines { "DEBUG" }
         symbols "On"

      filter "configurations:Release"
         defines { "NDEBUG" }
         optimize "On"