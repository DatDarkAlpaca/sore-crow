from premake_utils import *
from argparse import ArgumentParser, Namespace
from commands.command import Command
from constants import *
import subprocess


# Helpers:
def requires_moc(filepath: str) -> bool:
    with open(filepath, mode='r', encoding='utf-8') as file:
        contents = file.read()

        for term in moc_terms:
            if term in contents:
                return True

        return False


class ConfigureQtCommand(Command):
    def __init__(self):
        super().__init__('qt', 'Execute Qt commands')
        self.parser: ArgumentParser | None = None

    def setup(self, subparser):
        super().setup(subparser)
        self.parser.add_argument('-p', '--project', help='Selects the project', default=get_premake_projects(),
                                 choices=get_premake_projects(), nargs='+')
        self.parser.add_argument('-t', '--tools', help='Selects the Qt tools to use', choices=qt_commands, nargs='*')
        self.parser.add_argument('-l', '--list', version=list_premake_projects(), action='version')

    def execute(self, namespace: Namespace):
        super().execute(namespace)

        # Arguments:
        selected_tools = namespace.tools
        is_all_tools = not selected_tools or len(selected_tools) == 0

        # Project:
        project_names = namespace.project
        is_all_projects = not project_names or len(project_names) == 0
        if is_all_projects:
            project_names = get_premake_projects()

        project_configs = get_project_premake_project_configs()
        available_project_names = [x.project_name for x in project_configs]

        # Tools:
        for project_name in project_names:
            print(f"  > Configuring {project_name}...")
            project_config = project_configs[available_project_names.index(project_name)]

            if is_all_tools:
                ConfigureQtCommand.build_uic(project_config)
                ConfigureQtCommand.build_moc(project_config)
                ConfigureQtCommand.build_rcc(project_config)
                continue

            if 'uic' in selected_tools:
                ConfigureQtCommand.build_uic(project_config)

            if 'moc' in selected_tools:
                ConfigureQtCommand.build_moc(project_config)

            if 'rcc' in selected_tools:
                ConfigureQtCommand.build_rcc(project_config)

    @staticmethod
    def build_uic(project_config: PremakeProjectConfig):
        print('    - Running User Interface Compiler [uic]')

        # Creates uic output:
        uic_output_dir = os.path.join(project_config.int_path, qt_output_folder_path, 'uic')
        os.makedirs(uic_output_dir, exist_ok=True)

        for filepath in project_config.files:
            filename, extension = os.path.splitext(os.path.basename(filepath))

            if extension != '.ui':
                continue

            output_filepath = os.path.join(uic_output_dir, f"ui_{filename}.h")
            subprocess.run([uic_command, '-o', output_filepath, filepath])

    @staticmethod
    def build_moc(project_config: PremakeProjectConfig):
        print('    - Running Meta-Object Compiler [moc]')

        # Creates moc output:
        moc_output_dir = os.path.join(project_config.int_path, qt_output_folder_path, 'moc')
        os.makedirs(moc_output_dir, exist_ok=True)

        for filepath in project_config.files:
            filename, extension = os.path.splitext(os.path.basename(filepath))

            if extension != '.h' or not requires_moc(filepath):
                continue

            output_filepath = os.path.join(moc_output_dir, f"moc_{filename}.cpp")

            if project_config.project_name == 'sore-crow':
                subprocess.run([moc_command, '-b', 'pch.h', '-o', output_filepath, filepath])
            else:
                subprocess.run([moc_command, '-o', output_filepath, filepath])

    @staticmethod
    def build_rcc(project_config: PremakeProjectConfig):
        print('    - Running Resource Compiler [rcc]')

        rcc_output_dir = os.path.join(project_config.int_path, qt_output_folder_path, 'rcc')
        os.makedirs(rcc_output_dir, exist_ok=True)

        for filepath in project_config.files:
            filename, extension = os.path.splitext(os.path.basename(filepath))

            if extension != '.qrc':
                continue

            output_filepath = os.path.join(rcc_output_dir, f"qrc_{filename}.cpp")
            subprocess.run([rcc_command, '-o', output_filepath, filepath])

            with open(output_filepath, mode='r+', encoding='utf-8') as file:
                content = file.read()
                file.seek(0, 0)
                file.write('#include "pch.h"' + '\n' + content)
