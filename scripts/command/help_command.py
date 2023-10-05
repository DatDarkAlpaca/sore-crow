from command.command import Command, CommandType
from command.command_holder import CommandHolder


class HelpCommand(Command):
    def __init__(self, command_holder: CommandHolder):
        super().__init__('help', 'Shows a list of the available commands')
        self.command_holder = command_holder

    def execute(self, arguments: list[str] = None) -> None:
        show_internal = Command.has_argument(arguments, ['--i', '--internal'])

        print('> SoreCrow Building Tool')
        print('Usage: build [command]')
        print('\nAvailable commands:')

        largest_command_length = max([len(x) for x in self.command_holder.commands.keys()])
        command_space_offset = 2

        for command_name, command in self.command_holder.commands.items():
            if command.type == CommandType.INTERNAL_COMMAND and not show_internal:
                continue

            print(f"* {command_name:<{largest_command_length + command_space_offset}} {command.description}", end='')
            print(' [internal]\n' if command.type == CommandType.INTERNAL_COMMAND else '')
