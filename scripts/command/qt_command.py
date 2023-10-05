import subprocess
from constants import *
from premake_utils import PremakeProjectConfig
from command.command import Command, CommandType


# Helpers:
def requires_moc(filepath: str) -> bool:
    with open(filepath, mode='r', encoding='utf-8') as file:
        contents = file.read()

        for term in moc_terms:
            if term in contents:
                return True

        return False


class QtConfigureCommand(Command):
    def __init__(self):
        super().__init__('configure-qt', 'Configures the workspace to handle qt related files.')
        self.type = CommandType.INTERNAL_COMMAND
        self.project_config: PremakeProjectConfig | None = None

    def set_project_config(self, project_config: PremakeProjectConfig):
        self.project_config = project_config

    def execute(self, arguments: list[str] = None) -> None:
        print('Running Qt6 Configuration...')

        self.__build_uic()
        self.__build_moc()
        self.__build_rcc()

    def __build_uic(self):
        # Creates uic output:
        uic_output_dir = os.path.join(self.project_config.int_path, qt_output_folder_path, 'uic')
        os.makedirs(uic_output_dir, exist_ok=True)

        for filepath in self.project_config.files:
            filename, extension = os.path.splitext(os.path.basename(filepath))

            if extension != '.ui':
                continue

            output_filepath = os.path.join(uic_output_dir, f"ui_{filename}.h")
            subprocess.run([uic_command, '-o', output_filepath, filepath])

    def __build_moc(self):
        # Creates moc output:
        moc_output_dir = os.path.join(self.project_config.int_path, qt_output_folder_path, 'moc')
        os.makedirs(moc_output_dir, exist_ok=True)

        for filepath in self.project_config.files:
            filename, extension = os.path.splitext(os.path.basename(filepath))

            if extension != '.h' or not requires_moc(filepath):
                continue

            output_filepath = os.path.join(moc_output_dir, f"moc_{filename}.cpp")

            if self.project_config.project_name == 'sore-crow':
                subprocess.run([moc_command, '-b', 'pch.h', '-o', output_filepath, filepath])
            else:
                subprocess.run([moc_command, '-o', output_filepath, filepath])

    def __build_rcc(self):
        # Creates rcc output:
        rcc_output_dir = os.path.join(self.project_config.int_path, qt_output_folder_path, 'rcc')
        os.makedirs(rcc_output_dir, exist_ok=True)

        for filepath in self.project_config.files:
            filename, extension = os.path.splitext(os.path.basename(filepath))

            if extension != '.qrc':
                continue

            output_filepath = os.path.join(rcc_output_dir, f"qrc_{filename}.cpp")
            subprocess.run([rcc_command, '-o', output_filepath, filepath])

            with open(output_filepath, mode='r+', encoding='utf-8') as file:
                content = file.read()
                file.seek(0, 0)
                file.write('#include "pch.h"' + '\n' + content)
