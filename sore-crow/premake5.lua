project "sore-crow"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    links { "QASS" }

    files {
        '../intermediate/QtTemp/**.cpp',
        '../intermediate/QtTemp/**.h',
        "src/**.cpp",
        "src/**.h",
        "**.rc",
        "**.ui",
        "**.qrc"
    }

    removefiles { '../intermediate/QtTemp/moc/moc_QtAdvancedStylesheet.cpp' }
    
    includedirs {
        "%{prj.location}",
        "%{prj.location}/src",
        vendor_path .. "libmpv/include",
        vendor_path .. "qass/src",

        qt_path .. '/include',
        qt_path .. '/include/QtOpenGLWidgets',
        qt_path .. '/include/QtWidgets',
        qt_path .. '/include/QtOpenGL',
        qt_path .. '/include/QtSvg',
        qt_path .. '/include/QtGui',
        qt_path .. '/include/QtCore',
        '../intermediate/QtTemp/uic'
    }

    libdirs {
        qt_path .. '/lib',
        vendor_path .. "/libmpv",
    }

    defines {
        "ACSS_STATIC"
    }

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "on"

        links {
            "Qt6OpenGLWidgetsd.lib",
            "Qt6Widgetsd.lib",
            "Qt6OpenGLd.lib",
            "Qt6Svgd.lib",
            "Qt6Guid.lib",
            "Qt6Cored.lib",
            "Qt6EntryPointd.lib"
        }

    filter { "configurations:Release" }
        runtime "Release"
        optimize "on"

        links {
            "Qt6OpenGLWidgets.lib",
            "Qt6Widgets.lib",
            "Qt6OpenGL.lib",
            "Qt6Svg.lib",
            "Qt6Gui.lib",
            "Qt6Core.lib",
            "Qt6EntryPoint.lib"
        }

    filter { "configurations:Deploy" }
        runtime "Release"
        optimize "on"
        inlining "auto"

        links {
            "Qt6OpenGLWidgets.lib",
            "Qt6Widgets.lib",
            "Qt6OpenGL.lib",
            "Qt6Svg.lib",
            "Qt6Gui.lib",
            "Qt6Core.lib",
            "Qt6EntryPoint.lib"
        }

    filter "action:vs*"
        linkoptions "libmpv.dll.a"
        buildoptions "/Zc:__cplusplus"
        buildoptions "/permissive-"
        pchheader "pch.h"
        pchsource "./src/pch.cpp"
        flags { "MultiProcessorCompile" }

    filter "action:not vs*" 
        pchheader "pch.h"
    
    filter "files:qrc_sore_crow.cpp"
        flags { "NoPCH" }

    filter "files:moc_**.cpp"
        flags { "NoPCH" }

    filter { }
