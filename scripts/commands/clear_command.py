from commands.command import Command
from argparse import Namespace
import subprocess


class ClearCommand(Command):
    def __init__(self):
        super().__init__('clear', 'Clears project dependencies')

    def setup(self, subparser):
        super().setup(subparser)

    def execute(self, namespace: Namespace):
        super().execute(namespace)
        subprocess.run(['premake5', 'clear'], capture_output=True)
