import os
import subprocess
from constants import qt_output_folder_path
from dataclasses import dataclass, field


def get_premake_actions() -> list[tuple[str, str]]:
    process = subprocess.run(["premake5", "--help"], capture_output=True)
    output = process.stdout.decode("utf-8")
    actions_index = output.find('ACTIONS') + len('ACTIONS')
    actions_output = output[actions_index:]
    actions = []
    for action_string in actions_output.splitlines():
        if action_string.find('https://premake.github.io') != -1:
            continue

        action_args = action_string.split(' ')
        filtered = list(filter(lambda x: len(x) > 0, action_args))
        if len(filtered) <= 0:
            continue

        # Ignores clean:
        action = filtered[0]
        if action == 'clean':
            continue

        action_description = ' '.join(filtered[1:])
        actions.append((filtered[0], action_description))

    return actions


@dataclass
class PremakeProjectConfig:
    project_name: str
    architecture: str
    system: str
    bin_path: str
    int_path: str
    prj_path: str
    configurations: field(default_factory=list)
    files: field(default_factory=list)


def prepare_binary_directories(project_config: PremakeProjectConfig):
    os.makedirs(project_config.bin_path, exist_ok=True)
    os.makedirs(project_config.int_path, exist_ok=True)

    # Qt Output folder:
    os.makedirs(os.path.join(project_config.int_path, qt_output_folder_path), exist_ok=True)


def get_project_premake_project_configs() -> list[PremakeProjectConfig]:
    process = subprocess.run(["premake5", "info"], capture_output=True)
    output = process.stdout.decode("utf-8")

    projects = []
    current_project = None
    for line in output.splitlines():
        if line.startswith('<>'):
            projects.append(PremakeProjectConfig('', '', '', '', '', '', [], []))
            current_project = projects[len(projects) - 1]

        if not line.startswith('>'):
            continue

        key, value = line.split(":", 1)
        key = key.replace('>', '').strip()

        if key == 'architecture':
            current_project.architecture = value
        if key == 'system':
            current_project.system = value
        if key == 'project_name':
            current_project.project_name = value
        if key == 'prj_path':
            current_project.prj_path = value
        if key == 'bin_path':
            current_project.bin_path = value
        if key == 'int_path':
            current_project.int_path = value
        if key == 'configurations':
            current_project.configurations = value.split(',')
            current_project.configurations = list(map(lambda x: x.strip(), current_project.configurations))
            current_project.configurations = set(list(current_project.configurations))
            current_project.configurations = list(filter(lambda x: len(x) > 0, current_project.configurations))
        if key == 'files':
            current_project.files = value.split(',')
            current_project.files = list(map(lambda x: x.strip(), current_project.files))
            current_project.files = set(list(current_project.files))
            current_project.files = list(filter(lambda x: len(x) > 0, current_project.files))

    return projects
