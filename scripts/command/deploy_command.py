import shutil
from constants import *
from premake_utils import PremakeProjectConfig
from command.command import Command, CommandType


class DeployCommand(Command):
    def __init__(self):
        super().__init__('deploy', 'Copies resource files and DLLs to the binary folders.')
        self.type = CommandType.INTERNAL_COMMAND
        self.project_config: PremakeProjectConfig | None = None

    def set_project_config(self, project_config: PremakeProjectConfig):
        self.project_config = project_config

    def execute(self, arguments: list[str] = None) -> None:
        print('Running Qt6 Configuration...')

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
