premake.modules.clear = {}
local m = premake.modules.clear
local p = premake

newaction {
    trigger = "clear",
    description = "Clears generated project files and binaries",

    execute = function()
        os.rmdir("./bin")
        os.rmdir("./intermediate")
        os.rmdir("./.vs")
        os.rmdir("./scripts/__pycache__")
        os.rmdir("./scripts/command/__pycache__")
        os.remove('**.sln')
        os.remove('**.vcxproj')
        os.remove('**.vcxproj.filters')
        os.remove("**.vcxproj.user")
        os.remove('**.props')
        os.remove('**.targets')
        os.remove('**.Debug')
        os.remove('**.Release')
    end
}

return m