from commands import CommandHolder, ConanCommand, ConfigureQtCommand, GenerateCommand, ClearCommand, DeployCommand


def register_commands(command_holder: CommandHolder):
    commands = [
        ConanCommand(),
        ConfigureQtCommand(),
        GenerateCommand(),
        ClearCommand(),
        DeployCommand()
    ]

    for command in commands:
        command_holder.register_command(command)


def main():
    command_holder = CommandHolder()

    register_commands(command_holder)

    command_holder.execute()


if __name__ == '__main__':
    main()
