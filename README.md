# sore-crow

![SoreCrow Logo](sore-crow.png)

> それCrow (SoreCrow) is an open-source video player based on mpv and crafted for language learners.

## Getting Started

If you don't want to go through the trouble of manually installing the dependencies and building this project yourself, you can download the Windows binaries [here](https://drive.google.com/drive/folders/1a-K_n_KEaqrdZv_rGpANBtGOebJ6Nwsf?usp=sharing). Keep in mind that the project is currently in alpha, and some people reported several nasty bugs.


## Prerequisites

The project is currently only available on Windows, but as soon as I figure out how to build mpv, I'll provide a full-fledged cross-platform building process.

Before you begin, make sure you've met the following prerequisites:
* [Qt6](https://www.qt.io/product/qt6)
* [Python 3](https://www.python.org/downloads/)
* [Premake 5](https://github.com/premake/premake-core/releases)

> Ensure that the `QTDIR` environment variable is set to your Qt version path, for example: `C:\Qt\6.x.y\msvc2019_64`.

Premake deserves special attention since the latest version - `Premake 5.0-beta2` - didn't work for some reason, so I had to roll back to [Premake 5.0-beta1](https://github.com/premake/premake-core/releases/tag/v5.0.0-beta1), which I strongly recommend.

Besides that, the other prerequisite versions aren't as strict. For sanity's sake, that's the environment this project was built with:
* Visual Studio 2022 v17.6.5 (x64)
* Premake 5.0-beta1
* Python 3.11.5 
* Qt 6.5.2 (msvc2019_64)

## Installation
Sore-crow uses submodules, so follow these steps - otherwise, things might break:

1. **Clone the repository**

    ```bash
    git clone --recurse-submodules https://github.com/DatDarkAlpaca/sore-crow
    ```

    If you've rushed in, cloned the repository without using the `--recurse-submodules` option, and somehow broke the fragile Python command tool, you can initialize the submodules manually:

    ```bash
    cd sore-crow
    git submodule update --init
    ```

2. **Generate the project**

    If you're building on a Windows machine, you can use the `build.cmd` file to execute the Python script. If you don't use arguments, it'll show a list of available premake actions and commands.

    ```bash
    build vs2022
    ```

    If you want to work on plugin development, and all you need is to copy the generated resources to the binaries, you can use the deploy command:
    
    ```bash
    build deploy
    ```

    Lastly, you can use the clear command to remove the generated binaries and project files:

    ```bash
    build clear
    ```

3. **Compile the project**

    Use the generated project files to build the project. After that, you should have a working copy under `sore-crow/bin/{configuration}/sore-crow`. Use the `Deploy` configuration to execute the Qt deployment tool.


# License

This project is licensed under the [GNU GPL v3.0](https://opensource.org/license/gpl-3-0/) - see the [COPYING](COPYING) file for details.
