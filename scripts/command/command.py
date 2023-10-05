from abc import ABC, abstractmethod
from enum import Enum


class CommandType(Enum):
    GENERAL_COMMAND = 0
    PREMAKE_COMMAND = 1
    INTERNAL_COMMAND = 2


class Command(ABC):
    def __init__(self, name: str, description: str, command_type: CommandType = CommandType.GENERAL_COMMAND) -> None:
        super().__init__()
        self.type = command_type
        self.name = name
        self.description = description

    @abstractmethod
    def execute(self, arguments: list[str] = None) -> None:
        print(f"Running {self.name}...")

    @staticmethod
    def has_argument(arguments: list[str], target_arguments: list[str]) -> bool:
        if not arguments:
            return False

        for argument in arguments:
            for target in target_arguments:
                if target == argument:
                    return True
        return False
