# OpenGL Project on macOS

This project demonstrates a basic OpenGL setup using GLFW, GLM, and GLU. Follow these instructions to set up, build, and run the project on macOS using Visual Studio Code.

## Installation

1. **Install Homebrew** (if not already installed):
   ```sh
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. **Install required packages**:
   ```sh
   brew install glfw glm
   ```

   GLU is included with the system, so no separate installation is required.

3. **Set up your project**:
   Create a new directory for your project and navigate to it:
   ```sh
   mkdir my_opengl_project
   cd my_opengl_project
   ```

4. **Create your C++ source file**:
   Create a file named `main.cpp` with the following sample OpenGL code:

   ```cpp
   #include <GLFW/glfw3.h>
   #include <glm/glm.hpp>
   #include <iostream>

   void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
       glViewport(0, 0, width, height);
   }

   int main() {
       // Initialize GLFW
       if (!glfwInit()) {
           std::cerr << "Failed to initialize GLFW" << std::endl;
           return -1;
       }

       // Create a windowed mode window and its OpenGL context
       GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
       if (!window) {
           std::cerr << "Failed to create GLFW window" << std::endl;
           glfwTerminate();
           return -1;
       }

       // Make the window's context current
       glfwMakeContextCurrent(window);
       glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

       // Main loop
       while (!glfwWindowShouldClose(window)) {
           // Render here
           glClear(GL_COLOR_BUFFER_BIT);

           // Swap front and back buffers
           glfwSwapBuffers(window);

           // Poll for and process events
           glfwPollEvents();
       }

       // Cleanup and exit
       glfwDestroyWindow(window);
       glfwTerminate();
       return 0;
   }
   ```

5. **Create a `tasks.json` file**:

   In your project's `.vscode` directory, create or update the `tasks.json` file with the following content:

   ```json
   {
     "version": "2.0.0",
     "tasks": [
       {
         "type": "cppbuild",
         "label": "C/C++: clang++ build active file",
         "command": "/usr/bin/clang++",
         "args": [
           "-std=c++17",
           "-fdiagnostics-color=always",
           "-Wall",
           "-g",
           "-I${workspaceFolder}/dependencies/include",
           "-L${workspaceFolder}/dependencies/library",
           "${workspaceFolder}/dependencies/library/libglfw.3.4.dylib",
           "${workspaceFolder}/dependencies/library/libglm.dylib",
           "${workspaceFolder}/*.cpp",
           "${workspaceFolder}/glad.c",
           "-o",
           "${workspaceFolder}/app",
           "-framework",
           "OpenGL",
           "-framework",
           "Cocoa",
           "-framework",
           "IOKit",
           "-framework",
           "CoreVideo",
           "-framework",
           "CoreFoundation",
           "-Wno-deprecated"
         ],
         "options": {
           "cwd": "${fileDirname}"
         },
         "problemMatcher": [
           "$gcc"
         ],
         "group": {
           "kind": "build",
           "isDefault": true
         },
         "detail": "compiler: /usr/bin/clang++"
       }
     ]
   }
   ```

6. **Create a `Makefile`** (if needed) or use the tasks.json file for building.

## Building and Running

1. **Open your project in Visual Studio Code**:
   ```sh
   code .
   ```

2. **Build the project**:
   Open the Command Palette in VS Code (Cmd+Shift+P), type `Tasks: Run Build Task`, and select `C/C++: clang++ build active file`. This will compile the project according to your `tasks.json` configuration.

3. **Run your application**:
   Open the integrated terminal in VS Code and execute:
   ```sh
   ./app
   ```

## Copyright

© Aashutosh Poudel 2024

Feel free to modify and extend this project as needed. Happy coding!
