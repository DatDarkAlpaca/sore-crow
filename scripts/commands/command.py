from argparse import Namespace, ArgumentParser
from abc import ABC, abstractmethod
from enum import Enum


class CommandType(Enum):
    GENERAL = 0
    BUILD = 1


class Command(ABC):
    def __init__(self, name: str, description: str, command_type: CommandType = CommandType.GENERAL):
        self.name = name
        self.type = command_type
        self.requires: list[dict] = []
        self.description = description
        self.parser: ArgumentParser | None = None

    @abstractmethod
    def setup(self, subparser):
        self.parser: ArgumentParser = subparser.add_parser(self.name, help=self.description)

    @abstractmethod
    def execute(self, namespace: Namespace):
        print(f"> Running {self.name}...")
