require "scripts/lua/clear"
require "scripts/lua/info"

workspace "sore-crow"
    architecture "x64"
    configurations {
        "Debug",
        "Release",
        "Deploy"
    }

    startproject "sore-crow"

output_path         = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
binaries_path       = "%{wks.location}/bin/" .. "%{output_path}"
intermediate_path   = "%{wks.location}/intermediate/" .. "%{output_path}"
vendor_path         = "%{wks.location}/vendor/"
qt_path             = os.getenv("QTDIR") or os.getenv("QT_DIR")

include "vendor"
include "sore-crow"