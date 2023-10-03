import pathlib
import shutil
import sys
import os

# Usage:
# py script.py $(OutputPath) $(ProjectName)

project_output_path = sys.argv[1]
project_name        = sys.argv[2]

output_path         = os.path.join(project_output_path, '..')
solution_dir        = os.path.join(project_output_path, '..', '..', '..')
project_dir         = os.path.join(solution_dir, project_name)
vendor_dir          = os.path.join(solution_dir, 'vendor')

output_resource_dir = os.path.join(project_output_path, 'res', 'styles')
output_plugin_dir   = os.path.join(project_output_path, 'plugins')


def create_resource_folders():
    os.makedirs(output_resource_dir, exist_ok=True)
    os.makedirs(output_plugin_dir, exist_ok=True)


def copy_styles():
    project_styles_dir = os.path.join(project_dir, 'res', 'styles')    
    if os.path.isdir(project_styles_dir) and os.path.isdir(output_resource_dir):
        shutil.copytree(project_styles_dir, output_resource_dir, dirs_exist_ok=True)


def copy_mpv_config():
    project_mpv_config = os.path.join(project_dir, 'res', 'mpv.conf')
    output_mpv_config = os.path.join(project_output_path, 'res', 'mpv.conf')
    if os.path.isfile(project_mpv_config):
        shutil.copy2(project_mpv_config, output_mpv_config)


def copy_mpv_dll():
    project_libmpv_dll = os.path.join(vendor_dir, 'libmpv', 'libmpv-2.dll')
    output_libmpv_dll = os.path.join(project_output_path, 'libmpv-2.dll')
    if os.path.isfile(project_libmpv_dll):
        shutil.copy2(project_libmpv_dll, output_libmpv_dll)


def generate_project_plugins():
    for root, directories, files in os.walk(output_path):
        for plugin_folder in directories:
            if plugin_folder == project_name:
                continue
            
            # Create folder in sore-crow output:
            plugin_output_folder = os.path.join(output_plugin_dir, plugin_folder)
            os.makedirs(plugin_output_folder, exist_ok=True)

            # Copy plugin's manifest:
            plugin_folder_fullpath = os.path.join(output_path, plugin_folder)

            manifest_file = os.path.join(plugin_folder_fullpath, 'manifest.json')
            shutil.copy2(manifest_file, plugin_output_folder)

            # Copy plugin's DLLs:
            for plugin_file in os.listdir(plugin_folder_fullpath):
                extension = os.path.splitext(plugin_file)[-1].lower()
                if extension == '.dll':
                    plugin_filepath = os.path.join(plugin_folder_fullpath, plugin_file)
                    shutil.copy2(plugin_filepath, plugin_output_folder)

        break

def main():
    
    # Resources:
    create_resource_folders()

    copy_styles()

    copy_mpv_config()

    copy_mpv_dll()

    # Plugins:
    generate_project_plugins()


if __name__ == '__main__':
    main()
