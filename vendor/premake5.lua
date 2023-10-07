project "QASS"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    files {
        "qass/src/acss_globals.h",
        "qass/src/QtAdvancedStylesheet.cpp",
        "qass/src/QtAdvancedStylesheet.h",
        '../intermediate/QtTemp/moc/moc_QtAdvancedStylesheet.cpp',
    }

    includedirs {
        "%{prj.location}",
        "%{prj.location}/qass/src",

        qt_path .. '/include',
        qt_path .. '/include/QtOpenGLWidgets',
        qt_path .. '/include/QtWidgets',
        qt_path .. '/include/QtOpenGL',
        qt_path .. '/include/QtSvg',
        qt_path .. '/include/QtGui',
        qt_path .. '/include/QtCore',
    }

    libdirs {
        qt_path .. 'lib',
    }

    defines {
        "ACSS_STATIC"
    }

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "on"

    filter { "configurations:Release" }
        runtime "Release"
        optimize "on"

    filter { "configurations:Deploy" }
        runtime "Release"
        optimize "on"
        inlining "auto"

    filter "action:vs*"
        buildoptions "/Zc:__cplusplus"
        buildoptions "/permissive-"
        flags { "MultiProcessorCompile" }
        
    filter { }