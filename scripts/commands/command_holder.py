from commands.command import Command
from argparse import ArgumentParser
import sys


class CommandHolder:
    def __init__(self):
        self.commands: dict[str, Command] = {}
        self.parser = ArgumentParser(prog='crowtools')
        self.subparsers = self.parser.add_subparsers(help='Provides building tools for SoreCrow', dest='command')

    def register_command(self, command: Command):
        self.commands[command.name] = command
        self.commands[command.name].setup(self.subparsers)

    def execute(self):
        namespace = self.parser.parse_args()
        if not namespace.command:
            return self.parser.print_help()

        command = self.commands.get(namespace.command)
        for requirement in command.requires:
            for requirement_name, args in requirement.items():
                required_command = self.commands.get(requirement_name)
                if required_command:
                    required_command.execute(required_command.parser.parse_args(args))

        command.execute(namespace)
