# Rocket simulation
Code that simulates a rocket and planets.<br>
Renders using OpenGL with Dear ImGui or Vulkan with self-made GUI.

Units: SI<br>

### Building and running
The project uses CMake and requires C++ 20 or later. For now the project requires the vulkan sdk for both OpenGL and Vulkan rendering.
>Download the Vulkan SDK here: https://vulkan.lunarg.com/sdk/home.<br>
>Only tested building on windows 10 so results on other platforms may vary.
1. Clone the repository and submodules
    ```bash
    git clone --recursive https://github.com/Lokestrom/rocketSim-code
    ```
    >Forgot to use --recursive just run: `git submodule update --init --recursive`

2. Build using CMake (version 4.1 or greater)<br>
use -DUSE_VULKAN when creating cmake files for vulkan rendering.
    ```bash
    cd rocketSim-code
    mkdir build
    cd build
    cmake .. 
    cmake --build .
    ```

3. Run the program<br>
Moves the `app.exe` to the simulation folder, creates a testing folder then runs it.
    ```bash
    cd ../
    move build\<configuration>\app.exe simulation
    cd simulation
    mkdir testing
    app
    ```

### Using
Inside of the `Simulation/FileSystem/default/config` is the files for the simulation settings.
How to use the program is somewhat stated in the `Simulation/Rocket simulation.txt` file.

The default currently runs a program with a rocket orbiting a **small** planet,
then after 10 seconds the rocket runs its engine for 1 second.