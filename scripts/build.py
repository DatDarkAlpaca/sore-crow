import os
import sys
import subprocess
from build_qt import *
from deploy import deploy
from project_config import *

utility_actions = ['clear', 'info']


# Premake Comands:
def get_premake_actions() -> list[str]:
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

        actions.append(filtered[0])

    actions.append('deploy')
    return actions


def help_premake_actions():
    process = subprocess.run(["premake5", "--help"], capture_output=True)
    output = process.stdout.decode("utf-8")

    # Only show actions:
    actions_index = output.find('ACTIONS')
    actions_output = output[actions_index:]

    # Extra Cleanup:
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
    

def execute_premake(action: str) -> None:
    subprocess.run(["premake5", action])


# Premake Actions:
def is_action_utility(action: str) -> bool:
    return action in utility_actions


def is_action_build(action: str) -> bool:
    return not is_action_utility(action)


# Main:
def main():
    if len(sys.argv) < 2:
        help_premake_actions()
        return

    action = sys.argv[1]

    project_configs = get_project_premake_project_configs()

    if not action in get_premake_actions():
        print(f"The action '{action}' is not valid. Run the command without arguments to see a list of available actions.")
        return

    if is_action_utility(action):
        execute_premake(action)
        return

    # Project Information:    
    for project_config in project_configs:
        if action == 'deploy':
            if project_config.project_name == 'sore-crow':
                deploy(project_config)
            continue

        prepare_binary_directories(project_config)

        # Build steps:
        build_qt_step(project_config)
        execute_premake(action)
        deploy(project_config)
    
    
if __name__ == '__main__':
    main()
