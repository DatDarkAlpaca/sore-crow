import subprocess
from premake_utils import get_premake_actions
from constants import premake_utility_commands
from command.command import Command, CommandType
from command.command_holder import CommandHolder


class PremakeCommand(Command):
    def __init__(self, name: str, description: str, command_type: CommandType.PREMAKE_COMMAND):
        super().__init__(name, description)
        self.type = command_type

    def execute(self, _: list[str] = None) -> None:
        super().execute()
        subprocess.run(["premake5", self.name])


class PremakeActionsCommand(Command):
    def __init__(self, name: str, description: str):
        super().__init__(name, description)
        self.type = CommandType.GENERAL_COMMAND

    def execute(self, _: list[str] = None) -> None:
        process = subprocess.run(["premake5", "--help"], capture_output=True)
        output = process.stdout.decode("utf-8")

        actions_index = output.find('ACTIONS')
        actions_output = output[actions_index:]

        output = ''
        for line in actions_output.splitlines():
            sanitized_line = line.strip()

            if sanitized_line.startswith('clean') or sanitized_line.find('https://premake.github.io') != -1:
                continue

            if line.isspace() or len(line) == 0:
                output += '\n'
            elif line.startswith('ACTIONS'):
                output += 'Available Actions:'
            else:
                output += f"* {sanitized_line}\n"

        print(output.rstrip())


def register_premake_actions(command_holder: CommandHolder):
    for name, description in get_premake_actions():
        command_type = CommandType.PREMAKE_COMMAND if name not in premake_utility_commands else CommandType.GENERAL_COMMAND
        command_holder.register(PremakeCommand(name, description, command_type))

    # Actions Command:
    command_holder.register(PremakeActionsCommand('actions', 'Shows a list of the available premake actions'))
