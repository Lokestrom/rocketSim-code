# Rocket simulation
Code that simulates and planets.<br>
Renders using OpenGL with Dear ImGui.

Units: SI<br>

### Building and running
The project uses CMake and requires C++ 20 or later.
>Only tested building on windows 10 so results on other platforms may vary.
1. Clone the repository and submodules
    ```bash
    git clone --recursive https://github.com/Lokestrom/rocketSim-code
    ```
    >Forgot to use --recursive just run: `git submodule update --init --recursive`

2. Build using CMake (version 4.1 or later)<br>
    ```bash
    cd rocketSim-code
    cmake -S . -B build # create CMake files
    cmake --build build --config <configuration> # debug or release
    ```

3. Run the program<br>
    ```bash
    build/<configuration>/app # debug or release
    ```

### Using
1. Click **"new simulation"** then **"create"** in the popup (ignore the text input not in use currently).
2. Now 3 windows will open (they may be small just expand them as needed). 
3. In the Test Adder window, you can:
    * Add an example system (one light body orbiting a heavier one), or
    * Create your own planet(s).
>The simulation uses Newton’s Law of Universal Gravitation, integrated using Runge-Kutta 4 (RK4).

#### For Vulkan suported version:
To use the Vulkan version, check out the following commit:
``` bash 
git checkout 6084fc72a0f7616009d146f76b29d1b6b456b0c9
```
[View on GitHub](https://github.com/Lokestrom/rocketSim-code/tree/6084fc72a0f7616009d146f76b29d1b6b456b0c9)<br>