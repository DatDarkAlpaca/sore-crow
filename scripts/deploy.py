import os
import shutil
from project_config import ProjectConfig

global_resources_dir = 'res'
styles_folder = 'res/styles'
libmpv_dll = 'vendor/libmpv/libmpv-2.dll'


def copy_resources_path(project_config: ProjectConfig):
    for configuration in project_config.configurations:
        output_name = f"{configuration}-{project_config.system}-{project_config.architecture}"
        output_folder = os.path.join(project_config.bin_path, output_name, project_config.project_name, styles_folder)
        
        shutil.copytree(global_resources_dir, output_folder, dirs_exist_ok=True)


def copy_libmpv_dll(project_config: ProjectConfig):
    for configuration in project_config.configurations:
        output_name = f"{configuration}-{project_config.system}-{project_config.architecture}"       
        output_folder = os.path.join(project_config.bin_path, output_name, project_config.project_name)
        
        shutil.copy2(libmpv_dll, output_folder)


def deploy(project_config: ProjectConfig):
    print('Copying resources...')
    copy_resources_path(project_config)
    copy_libmpv_dll(project_config)

if __name__ == '__main__':
    deploy()
