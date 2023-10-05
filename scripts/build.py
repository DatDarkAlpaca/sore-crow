import sys
from command.command import CommandType
from command.command_holder import CommandHolder
from command.premake_command import register_premake_actions
from command.help_command import HelpCommand
from command.qt_command import QtConfigureCommand


def main():
    # Command Holder:
    command_holder = CommandHolder()

    # Registration:
    register_premake_actions(command_holder)
    command_holder.register(HelpCommand(command_holder))
    command_holder.register(QtConfigureCommand())

    # Input:
    if len(sys.argv) < 2:
        command_holder.execute('help')
        return

    command, args = sys.argv[1], sys.argv[2:]
    command_holder.execute(command, args)


if __name__ == '__main__':
    main()
