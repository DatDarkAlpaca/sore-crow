import os

# Premake:
premake_utility_commands = ['clear', 'info']

# Qt:
qt_output_folder_path = 'QtTemp'
moc_terms = ['Q_OBJECT', 'Q_GADGET']
qt_dir = os.getenv('QTDIR') or os.getenv('QT_DIR')
uic_command = os.path.join(qt_dir, 'bin', 'uic.exe')
moc_command = os.path.join(qt_dir, 'bin', 'moc.exe')
rcc_command = os.path.join(qt_dir, 'bin', 'rcc.exe')

# Deploy:
global_resources_dir = 'res'
styles_folder = 'res/styles'
libmpv_dll = 'vendor/libmpv/libmpv-2.dll'
