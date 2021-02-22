solution("CHIP-8")
  language("c++")
  configurations {"Debug", "Release"}
  platforms "x64"
  includedirs { "include", "deps/include/SDL2-2.0.14/include" }
  location "build"
  objdir "build/obj"

  configuration "Debug"
   defines {"DEBUG"}
   symbols "On"
   targetdir "bin"
   targetsuffix "_d"

  configuration "Release"
   defines {"RELEASE"}
   optimize "On"
   targetdir "bin"

  project("demo")
    kind "ConsoleApp"
    files { "src/*.cpp", "include/*.h" }
    libdirs { "deps/include/SDL2-2.0.14/lib/x64" }
    links {"SDL2", "SDL2main", "SDL2test"}

    postbuildcommands
    {
      ("{COPY} ../deps/include/SDL2-2.0.14/lib/x64/SDL2.dll" .. " ../bin")
    }
