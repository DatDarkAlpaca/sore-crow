import shutil
from constants import *
from premake_utils import PremakeProjectConfig, get_project_premake_project_configs
from command.command import Command, CommandType


class DeployCommand(Command):
    def __init__(self):
        super().__init__('deploy', 'Copies resource files and DLLs to the binary folders.')
        self.type = CommandType.INTERNAL_COMMAND
        self.project_config: PremakeProjectConfig | None = None

    def set_project_config(self, project_config: PremakeProjectConfig):
        self.project_config = project_config

    def execute(self, arguments: list[str] = None) -> None:
        print('Deploing Resources...')

        if self.has_argument(arguments, ['--project']) and  len(arguments) >= 2:
            project_name = arguments[1]
            for project in get_project_premake_project_configs():
                if project.project_name == project_name:
                    self.set_project_config(project)

        self.__copy_resources_path()
        self.__copy_libmpv_dll()

    def __copy_resources_path(self):
        for configuration in self.project_config.configurations:
            output_name = self.get_project_output(configuration)
            output_folder = os.path.join(self.project_config.bin_path, output_name, self.project_config.project_name, styles_folder)

            shutil.copytree(global_resources_dir, output_folder, dirs_exist_ok=True)

    def __copy_libmpv_dll(self):
        for configuration in self.project_config.configurations:
            output_name = self.get_project_output(configuration)
            output_folder = os.path.join(self.project_config.bin_path, output_name, self.project_config.project_name)

            shutil.copy2(libmpv_dll, output_folder)

    def get_project_output(self, configuration):
        return f"{configuration}-{self.project_config.system}-{self.project_config.architecture}"
