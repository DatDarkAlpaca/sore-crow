from commands.command import Command
from argparse import Namespace
from premake_utils import *
import subprocess


class GenerateCommand(Command):
    def __init__(self):
        super().__init__('generate', 'Executes premake actions')

        self.requires = [{'conan': []}, {'qt': []}, {'deploy': []}]

        ignore_actions = ['clear', 'info']
        self.premake_actions = list(filter(lambda x: x not in ignore_actions,
                                           [name for name, description in get_premake_actions()]))

    def setup(self, subparser):
        super().setup(subparser)
        self.parser.add_argument('-a', '--action', help='Selects the Premake actions to execute',
                                 choices=self.premake_actions, required=True)

    def execute(self, namespace: Namespace):
        subprocess.run(["premake5", namespace.action])
