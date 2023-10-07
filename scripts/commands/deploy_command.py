from premake_utils import *
from argparse import ArgumentParser, Namespace
from commands.command import Command
from constants import *
import shutil


class DeployCommand(Command):
    def __init__(self):
        super().__init__('deploy', 'Copies resources over to the binary directories')
        self.parser: ArgumentParser | None = None

    def setup(self, subparser):
        super().setup(subparser)
        self.parser.add_argument('-p', '--project', help='Selects the project', default=get_premake_projects(),
                                 choices=get_premake_projects(), nargs='+')
        self.parser.add_argument('-l', '--list', version=list_premake_projects(), action='version')

    def execute(self, namespace: Namespace):
        super().execute(namespace)

        # Project:
        project_names = namespace.project
        is_all_projects = not project_names or len(project_names) == 0
        if is_all_projects:
            project_names = get_premake_projects()

        project_configs = get_project_premake_project_configs()
        available_project_names = [x.project_name for x in project_configs]

        # Tools:
        for project_name in project_names:
            print(f"  - Deploying {project_name} resources...")
            project_config = project_configs[available_project_names.index(project_name)]

            self.copy_resources_path(project_config)
            self.copy_libmpv_dll(project_config)

    def copy_resources_path(self, project_config: PremakeProjectConfig):
        for configuration in project_config.configurations:
            output_name = self.get_project_output(project_config, configuration)
            output_folder = os.path.join(project_config.bin_path, output_name, project_config.project_name,
                                         styles_folder)

            shutil.copytree(global_resources_dir, output_folder, dirs_exist_ok=True)

    def copy_libmpv_dll(self, project_config: PremakeProjectConfig):
        for configuration in project_config.configurations:
            output_name = self.get_project_output(project_config, configuration)
            output_folder = os.path.join(project_config.bin_path, output_name, project_config.project_name)

            shutil.copy2(libmpv_dll, output_folder)

    @staticmethod
    def get_project_output(project_config: PremakeProjectConfig, configuration):
        return f"{configuration}-{project_config.system}-{project_config.architecture}"
