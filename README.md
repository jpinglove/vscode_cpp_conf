# vscode_cpp_conf

## MinGW W64 download
1. [MinGW download](https://www.mingw-w64.org/downloads/)
1. [MinGW-W64-builds-github](https://github.com/niXman/mingw-builds-binaries/releases)

---
## MinGW x64
这些 MinGW-w64 二进制文件包的名字确实包含了大量信息，每个部分都代表了不同的配置选项。理解它们能帮你选择最适合你项目和系统的版本。

---

### MinGW 文件名解析

让我们分解这些文件名，以 `x86_64-15.1.0-release-posix-seh-ucrt-rt_v12-rev0.7z` 为例进行说明。

1.  **`i686` 或 `x86_64`：**
    * **`i686`**: 这表示 MinGW-w64 是为 **32 位 Windows 系统**编译的。如果你需要编译 32 位的程序，或者你的 Windows 系统是 32 位的，就选择这类。
    * **`x86_64`**: 这表示 MinGW-w64 是为 **64 位 Windows 系统**编译的。这是目前主流的选择，能让你编译 64 位的程序。

2.  **`15.1.0`：**
    * 这指的是 **GCC (GNU Compiler Collection)** 的版本号，也就是 MinGW-w64 内部使用的 C/C++/Fortran 等编译器的版本。`15.1.0` 是一个很新的版本。

3.  **`release`：**
    * 表明这是一个 **发布版本**，通常意味着它经过了测试，稳定性较好。

4.  **`mcf` / `posix` / `win32`：**
    * 这部分代表了 **线程模型 (Threading Model)**，它决定了编译器如何处理多线程。
        * **`posix`**: 使用 **POSIX 线程模型**。这是最常用且推荐的模型，它支持 C++11 及更高版本中的 `<thread>`、`<mutex>` 等标准库功能。如果你使用 `std::thread` 等，通常会选择这个。
        * **`win32`**: 使用 **Win32 线程模型**。这是 Windows 原生的线程 API。它的优点是生成的可执行文件可能略小，对旧版 Windows 兼容性可能更好，但它不支持 `std::thread` 等标准 C++11 并发特性。
        * **`mcf`**: 这是 **`posix` 线程模型的一种变体**，通常用于 GCC 13 及更高版本，是 `posix` 的改进版本，提供了更好的性能和兼容性，特别是对于一些高级特性。如果存在 `mcf` 选项，通常意味着它比 `posix` 更现代和优化。

5.  **`dwarf` / `seh`：**
    * 这指的是 **异常处理模型 (Exception Handling Model)**。它决定了程序在遇到错误（如除以零、访问无效内存）时如何处理。
        * **`dwarf`**: 使用 **DWARF (Debugging With Attributed Record Formats)** 异常处理。它通常用于 32 位（`i686`）版本，因为它在 32 位环境下兼容性较好。在 64 位（`x86_64`）版本中不推荐使用 DWARF，因为它的性能和稳定性不如 SEH。
        * **`seh`**: 使用 **结构化异常处理 (Structured Exception Handling)**。这是 Windows 原生的异常处理机制。对于 **64 位（`x86_64`）MinGW-w64，强烈推荐使用 `seh`**，因为它提供了更好的兼容性、稳定性和性能。

6.  **`ucrt` / `msvcrt`：**
    * 这代表了 **C 运行时库 (C Runtime Library)**。这是程序运行时所需的底层库，提供了 `printf`、`malloc` 等基本功能。
        * **`ucrt` (Universal C Runtime)**: 这是 **Windows 10 及更高版本推荐的运行时库**。它更加现代化，与 Visual Studio 2015 及更高版本使用的 UCRT 兼容性更好。如果你在 Windows 10/11 上开发，并希望与 Visual Studio 编译的库进行互操作，这是最佳选择。
        * **`msvcrt`**: 这是 **旧版 Windows 默认的运行时库** (Windows XP 到 Windows 8.1)。如果你需要让你的程序在旧版 Windows 系统上运行，或者与使用旧版 Visual C++ 编译的库进行互操作，可以选择它。

7.  **`rt_v12-rev0`：**
    * 这通常是 MinGW-w64 项目内部的版本标记，可能表示 Runtime 的版本或其他一些内部修订。对于大多数用户来说，这部分信息不那么关键，通常选择最新的 `rev` 即可。

8.  **`.7z`：**
    * 表示文件是使用 **7-Zip 压缩格式**打包的。你需要一个支持 7-Zip 的解压工具（如 7-Zip 自身）来解压它。

---

### 如何选择安装哪个 MinGW 文件？

选择正确的 MinGW 文件取决于你的操作系统位数、目标程序的位数、以及对性能和兼容性的要求。

**以下是选择建议：**

1.  **确定你的 Windows 系统是 32 位还是 64 位：**
    * 大多数现代电脑都是 64 位的。如果你的系统是 64 位，**强烈推荐选择 `x86_64` 版本**，因为它能更好地利用现代硬件的性能，并允许你编译 64 位程序。
    * 如果你的系统是 32 位，或者你只打算编译 32 位程序，则选择 `i686` 版本。

2.  **推荐的配置（通用且现代）：**
    * **如果你使用 64 位 Windows 10/11，并需要编译现代 C++ 项目（包含多线程）：**
        * 选择：**`x86_64-15.1.0-release-posix-seh-ucrt-rt_v12-rev0.7z`**
        * 理由：`x86_64` (64位), `posix` (支持 C++11+ 多线程), `seh` (64位推荐的异常处理), `ucrt` (现代 Windows 推荐的运行时库)。如果存在 `mcf` 选项，例如 `x86_64-15.1.0-release-mcf-seh-ucrt-rt_v12-rev0.7z`，它通常是更好的选择。

    * **如果你使用 32 位 Windows，或只编译 32 位程序，并需要支持现代 C++ 特性：**
        * 选择：**`i686-15.1.0-release-posix-dwarf-ucrt-rt_v12-rev0.7z`**
        * 理由：`i686` (32位), `posix` (支持 C++11+ 多线程), `dwarf` (32位推荐的异常处理), `ucrt` (现代 Windows 推荐的运行时库)。

3.  **特殊情况：**
    * **需要兼容旧版 Windows (Windows 7/8) 或旧版 Visual C++ 编译的库：**
        * 考虑使用 `msvcrt` 版本。例如：`x86_64-15.1.0-release-posix-seh-msvcrt-rt_v12-rev0.7z`。
    * **如果你的项目不使用 C++11/14/17/20 的多线程特性，且追求最小的可执行文件大小（不常见）：**
        * 可以考虑 `win32` 线程模型。例如：`x86_64-15.1.0-release-win32-seh-ucrt-rt_v12-rev0.7z`。

**总结一下：**

对于大多数现代 Windows 10/11 用户来说，选择 **`x86_64`** (64位系统) 或 **`i686`** (32位系统)，然后通常是 **`posix`** (或 `mcf` 如果有)，**`seh`** (64位) 或 **`dwarf`** (32位)，以及 **`ucrt`** 的组合，将是你最佳的选择。

希望这个详细解释能帮助你理解并选择合适的 MinGW-w64 版本！你通常会安装到哪个文件夹呢？

## Compile C++

### Configuring VS Code for C++ compilation

Here's how to configure Visual Studio Code to compile and run C++ code, covering both Windows (using MinGW-w64 or MSVC) and Linux/macOS (using GCC/Clang):

1. Prerequisites
Install Visual Studio Code: Download and install VS Code from the Visual Studio Code website.
Install the C/C++ Extension: Open VS Code, go to the Extensions view `(Ctrl+Shift+X)`, search for "C/C++", and install the extension provided by Microsoft. 
2. Choose and install a compiler
You need a C++ compiler installed on your system to compile your C++ code. The choice of compiler depends on your operating system:
Windows:
MinGW-w64 (recommended): Download and install MSYS2, then open an MSYS2 terminal and run pacman -S --needed base-devel mingw-w64-x86_64-toolchain to install the GCC compiler and related tools.
Microsoft Visual C++ (MSVC): If you have Visual Studio installed, open the Visual Studio Installer and ensure the "Desktop development with C++" workload is checked. If not, install the Build Tools for Visual Studio from the Visual Studio Downloads page.
Linux: Open a terminal and run sudo apt-get install build-essential to install the GNU compiler tools (including GCC).
macOS:
Clang (default): Install Xcode Command Line Tools (you might have them already).
GCC (optional): Install Homebrew and then run brew install gcc to install GCC. 
3. Set environment variables (Windows, MinGW-w64 only)
If you installed MinGW-w64 on Windows, you must add its bin folder to your system's PATH environment variable. This process involves accessing System Settings, navigating to environment variables, selecting the "Path" variable for your user, editing it to add the MinGW-w64 bin path `(e.g., C:\msys64\ucrt64\bin)`, and confirming the changes. Remember to reopen any console windows for the changes to take effect. 
4. Configure VS Code
Create a workspace: Open your project folder in VS Code to establish it as your workspace.
C/C++ Configurations (optional): To customize compiler settings and C++ standards, use the `"C/C++: Edit Configurations (UI)"` command in the Command Palette `(Ctrl+Shift+P)` to generate a c_cpp_properties.json file.
Tasks (optional but recommended): Automate building and running with a tasks.json file. Access  `"Tasks: Configure Tasks..."`  from the Command Palette `(Ctrl+Shift+P)` and choose "Others" from the templates. Configure the command and arguments for your compiler (like g++ or cl.exe) and specify necessary flags (such as -g for debugging). You can define separate tasks for debugging and compiling/running.
Launch Configurations (optional): For advanced debugging, including passing command-line arguments, create a launch.json file by selecting "Add Debug Configuration" from the play button drop-down. Choose the configuration for your compiler (e.g., "C/C++: cl.exe build and debug active file") and modify the program and args fields as needed. 
5. Compile and run C++ code
Open your C++ file in VS Code.
To run without debugging: Click the play button in the top right. The first time, you might need to select your compiler configuration. The output will appear in the integrated Terminal.
To debug: Set a breakpoint by clicking in the margin or pressing F9. Select `"Debug C/C++ File"` from the Command Palette `(Ctrl+Shift+P)`. from the play button drop-down. Choose your compiler if prompted. Use the debugger controls to step through your code and examine variables. 
This setup allows you to efficiently develop, compile, and debug C++ projects within VS Code. 


### compile c++ 2

To configure VS Code for compiling C++ code, you'll need to install a C++ compiler (like MinGW or MSVC) and the C/C++ extension from Microsoft. After installing these, you can configure VS Code to use the compiler by creating a tasks.json file and optionally a c_cpp_properties.json file for more advanced settings. 
Steps:
1. Install a C++ Compiler:
MinGW (Windows): Download and install MinGW-w64 from the MSYS2 website. Add the MinGW bin directory to your system's PATH environment variable, according to Microsoft. 
MSVC (Windows): Install the Microsoft Visual C++ (MSVC) compiler toolset, which can be found within the Visual Studio Build Tools, according to Visual Studio Code. 
Clang (macOS/Linux): Install Clang using your system's package manager or Xcode command line tools on macOS. 
2. Install the C/C++ Extension:
Open VS Code and go to the Extensions view `(Ctrl+Shift+X)`.
Search for `"C/C++"` and install the extension by Microsoft. 
3. Configure tasks.json:
Open your C++ file in VS Code.
Open the Command Palette `(Ctrl+Shift+P)` and type `"Tasks: Configure Task"`.
Select `"Create tasks.json file from template"`.
Choose `"Others"` to create a custom task.
Modify the command in the tasks.json file to use your compiler's command (e.g., g++ or cl) and specify the source file and output executable name. 
Example for MinGW:

Code

```json
    {
        "version": "2.0.0",
        "tasks": [
            {
                "label": "build",
                "type": "shell",
                "command": "g++",
                "args": [
                    "-g",
                    "${file}",
                    "-o",
                    "${fileDirname}/${fileBasenameNoExtension}"
                ],
                "group": {
                    "kind": "build",
                    "isDefault": true
                },
                "problemMatcher": "$gcc"
            }
        ]
    }
 ```

1. (Optional) Configure c_cpp_properties.json:
Open the Command Palette `(Ctrl+Shift+P)` and type `"C/C++: Edit Configurations (UI)"`.
This will open the C/C++ Configurations page, where you can modify settings like include paths, C++ standard, and compiler path. 
VS Code will save these settings in a `c_cpp_properties.json` file within the .vscode folder. 
2. Compile and Run:
Open your C++ file in VS Code.
Press `Ctrl+Shift+B` (or `Cmd+Shift+B` on macOS) to run the build task defined in tasks.json.
You can also run the compiled executable from the terminal within VS Code or by using the Code Runner extension, according to Stack Overflow. 
