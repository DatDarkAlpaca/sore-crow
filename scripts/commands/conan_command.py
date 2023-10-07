from commands.command import Command
from argparse import Namespace
from constants import *
import subprocess


class ConanCommand(Command):
    def __init__(self):
        super().__init__('conan', 'Installs conan dependencies')

    def setup(self, subparser):
        super().setup(subparser)

        self.parser.add_argument('-b', '--build-type', help='Selects the configuration to run the conan command for',
                                 choices=workspace_build_types, default=['Debug'])

    def execute(self, namespace: Namespace):
        super().execute(namespace)

        subprocess.run(['conan', 'install', '.', '--build', 'missing',
                        '--output-folder=./dependencies',
                        f"--settings=build_type={namespace.build_type}"])
