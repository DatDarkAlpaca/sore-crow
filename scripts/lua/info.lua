premake.modules.info = {}
local m = premake.modules.info
local p = premake

newaction {
    trigger = "info",
    description = "Displays a set of information required for the build commands",

    onProject = function(prj)
        -- Project indicator:
        print("<>")

        -- Settings:
        print("> architecture:" .. prj.architecture)
        print("> system:" .. prj.system)
        print("> project_name:" .. prj.name)

        io.write("> configurations: ")
        for _, key in pairs(prj.configurations) do
            io.write(key .. ',')
        end
        io.write('\n')

        -- Binaries:
        bin_path = prj.workspace.location .. "/bin/"
        int_path = prj.workspace.location .. "/intermediate/"

        print("> bin_path:" .. bin_path)
        print("> int_path:" .. int_path)
        print("> prj_path:" .. prj.location)      
        
        io.write("> files: ")
        for _, value in pairs(prj.files) do
            io.write(value .. ',')
        end
        io.write('\n')
    end
}

return m