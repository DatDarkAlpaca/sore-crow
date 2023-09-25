import shutil
import sys
import os


def main():
    # Usage:
    # py script.py $(ProjectDir) $(OutputPath) $(ProjectVendor)

    project_dir = sys.argv[1]
    output_path = sys.argv[2]
    vendor_path = sys.argv[3]

    # Output Paths:
    output_resource_dir = os.path.join(output_path, 'res', 'styles')
    output_mpv_config = os.path.join(output_path, 'res', 'mpv.conf')
    output_libmpv_dll = os.path.join(output_path, 'libmpv-2.dll')

    # Resource Folders:
    os.makedirs(output_resource_dir, exist_ok=True)

    # Styles Folder:
    project_styles_dir = os.path.join(project_dir, 'res', 'styles')
    if os.path.isdir(project_styles_dir) and os.path.isdir(output_resource_dir):
        shutil.copytree(project_styles_dir, output_resource_dir, dirs_exist_ok=True)

    # MPV Config:
    project_mpv_config = os.path.join(project_dir, 'res', 'mpv.conf')
    if os.path.isfile(project_mpv_config):
        shutil.copy2(project_mpv_config, output_mpv_config)

    # libmpv DLLs:
    project_libmpv_dll = os.path.join(vendor_path, 'libmpv', 'libmpv-2.dll')
    if os.path.isfile(project_libmpv_dll):
        shutil.copy2(project_libmpv_dll, output_libmpv_dll)


if __name__ == '__main__':
    main()
