from premake_utils import *
from command.command import Command, CommandType
from command.deploy_command import DeployCommand
from command.qt_command import QtConfigureCommand


class CommandHolder:
    def __init__(self):
        self.commands: dict[str, Command] = {}

    def get(self, command_name: str) -> Command | None:
        return self.commands.get(command_name)

    def register(self, command: Command):
        self.commands[command.name] = command

    def execute(self, command_id: str, arguments: list[str] = None):
        command = self.commands.get(command_id)
        if not command:
            return print(f"Failed to execute command: {command_id}. "
                         f"Use the 'help' command to see a list of available commands")

        if command.type != CommandType.PREMAKE_COMMAND:
            return command.execute(arguments)

        # Premake Pre-Project Generation:
        CommandHolder.__configure_premake_command(command, arguments)

    @staticmethod
    def __configure_premake_command(command: Command, arguments: list[str] = None):
        qt_command = QtConfigureCommand()
        deploy_command = DeployCommand()

        project_configs = get_project_premake_project_configs()
        for project_config in project_configs:
            prepare_binary_directories(project_config)

            # Build steps:
            qt_command.set_project_config(project_config)
            qt_command.execute()

            command.execute(arguments)

            deploy_command.set_project_config(project_config)
            deploy_command.execute()
