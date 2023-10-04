import os
import pathlib
from project_config import *

__all__ = ['build_qt_step']

qt_dir = os.getenv('QTDIR') or os.getenv('QT_DIR')


# Helpers:
def requires_moc(filepath: str) -> bool:
    with open(filepath, mode='r', encoding='utf-8') as file:
        contents = file.read()

        moc_terms = ['Q_OBJECT', 'Q_GADGET']

        for term in moc_terms:
            if term in contents:
                return True
        
        return False


# Build Methods:
def __qt_uic_step(project_config: ProjectConfig):
    uic_command = os.path.join(qt_dir, 'bin', 'uic.exe')

    # Creates uic output:
    uic_output_dir = os.path.join(project_config.int_path, qt_output_folder_path, 'uic')
    os.makedirs(uic_output_dir, exist_ok=True)

    for filepath in project_config.files:
        filename, extension = os.path.splitext(os.path.basename(filepath))
        
        if extension != '.ui':
            continue

        output_filepath = os.path.join(uic_output_dir, f"ui_{filename}.h")
        subprocess.run([uic_command, '-o', output_filepath, filepath])

    
def __qt_moc_step(project_config: ProjectConfig):
    moc_command = os.path.join(qt_dir, 'bin', 'moc.exe')
    
    # Creates moc output:
    moc_output_dir = os.path.join(project_config.int_path, qt_output_folder_path, 'moc')
    os.makedirs(moc_output_dir, exist_ok=True)

    for filepath in project_config.files:
        filename, extension = os.path.splitext(os.path.basename(filepath))
        
        if extension != '.h' or not requires_moc(filepath):
            continue

        output_filepath = os.path.join(moc_output_dir, f"moc_{filename}.cpp")

        if project_config.project_name == 'sore-crow':
            subprocess.run([moc_command, '-b', 'pch.h' ,'-o', output_filepath, filepath])
        else:
            subprocess.run([moc_command, '-o', output_filepath, filepath])


def __qt_rcc_step(project_config: ProjectConfig):
    rcc_command = os.path.join(qt_dir, 'bin', 'rcc.exe')

    # Creates rcc output:
    rcc_output_dir = os.path.join(project_config.int_path, qt_output_folder_path, 'rcc')
    os.makedirs(rcc_output_dir, exist_ok=True)

    for filepath in project_config.files:
        filename, extension = os.path.splitext(os.path.basename(filepath))
        
        if extension != '.qrc':
            continue

        output_filepath = os.path.join(rcc_output_dir, f"qrc_{filename}.cpp")
        subprocess.run([rcc_command, '-o', output_filepath, filepath])

        with open(output_filepath, mode='r+', encoding='utf-8') as file:
            content = file.read()
            file.seek(0, 0)
            file.write('#include "pch.h"' + '\n' + content)


def build_qt_step(project_config: ProjectConfig):
    print('Running Qt6 build step..')

    # Create temporary qt files:
    __qt_uic_step(project_config)
    __qt_moc_step(project_config)
    __qt_rcc_step(project_config)
