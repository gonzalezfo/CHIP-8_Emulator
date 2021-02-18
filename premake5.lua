solution("CHIP-8")
  language("c++")
  configurations {"Debug", "Release"}
  platforms "x64"
  includedirs { "include" }
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
    --libdirs {}
    --links {}
