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

### macro 

在C++11和C++14中有更方便的方法：

``` C++
[[carries_dependency]] 让编译期跳过不必要的内存栅栏指令
[[noreturn]] 函数不会返回
[[deprecated]] 函数将弃用的警告

[[noreturn]] void terminate() noexcept;
[[deprecated("use new func instead")]] void func() {}




```

C++17又新增了三个：

``` C++
// [[fallthrough]]，用在switch中提示可以直接落下去，不需要break，让编译期忽略警告

switch (i) {}
    case 1:
        xxx; // warning
    case 2:
        xxx; 
        [[fallthrough]];      // 警告消除
    case 3:
        xxx;
       break;
}


//[[nodiscard]] ：表示修饰的内容不能被忽略，可用于修饰函数，标明返回值一定要被处理

[[nodiscard]] int func();
void F() {
    func(); // warning 没有处理函数返回值
}

//[[maybe_unused]] ：提示编译器修饰的内容可能暂时没有使用，避免产生警告

void func1() {}
[[maybe_unused]] void func2() {} // 警告消除
void func3() {
    int x = 1;
    [[maybe_unused]] int y = 2; // 警告消除
}

```
---


## CMake

CMake 就像一个“项目经理”或“建筑师”，而不是一个实际的“建筑工人”。它本身并不会直接编译你的 C++ 代码（编译的工作由 GCC、MSVC 这些编译器来做），但它会**生成**让这些“建筑工人”知道如何工作的“施工图纸”。

简单来说：

  * **你告诉 CMake 你的项目有哪些文件、依赖了哪些库、怎么构建。**
  * **CMake 根据你的描述，生成特定于你系统的构建文件**（比如 Windows 上的 Visual Studio 项目文件、Linux 上的 Makefile）。
  * **然后，你就可以用这些生成的文件来编译你的代码了。**

### 为什么不直接用编译器？

当你的 C++ 项目只有一个 `.cpp` 文件时，直接用编译器命令行编译很容易。但一旦项目变大，有几十个甚至几百个 `.h` 和 `.cpp` 文件，还依赖了各种第三方库，情况就复杂了：

  * **头文件路径、库文件路径**：一大堆路径要设置。
  * **编译选项**：是编译 Debug 版还是 Release 版？是否开启优化？
  * **多平台兼容**：你的项目可能需要在 Windows、macOS、Linux 上都能编译，而它们各自的编译方式和工具都不一样。

手动管理这些会让你崩溃！

-----

## 为什么要用 CMake？有哪些好处？

CMake 的主要优点就在于它能解决上述的复杂性，并带来以下好处：

1.  **跨平台构建：** 这是 CMake 最核心的优势。你只需编写一套 CMake 脚本（`CMakeLists.txt` 文件），CMake 就能根据你运行的平台（Windows、macOS、Linux），生成对应平台的构建系统文件。比如，在 Windows 上生成 `.sln` (Visual Studio) 文件，在 Linux 上生成 `Makefile`，在 macOS 上生成 `Xcode` 项目。这意味着你的代码库在不同系统下都能方便地编译。
2.  **简化大型项目管理：** 对于包含多个子目录、多个目标（可执行文件、库）的复杂项目，CMake 提供了一套清晰的语法来组织和管理这些组件，定义它们之间的依赖关系。
3.  **自动查找依赖：** CMake 可以帮你查找系统上安装的第三方库（比如 Boost、OpenCV 等），并自动设置好它们的头文件和库文件路径，省去了手动配置的麻烦。
4.  **模块化和可重用性：** 你的 CMake 脚本可以模块化，方便在不同的项目之间重用代码和构建逻辑。
5.  **易于集成 CI/CD：** 自动化构建工具（如 Jenkins、GitHub Actions）能很好地与 CMake 集成，方便实现持续集成和持续部署。

-----

## 在 VS Code 中配置和使用 CMake 开发 C++ 项目

在 VS Code 中开发 C++ 项目并使用 CMake 非常方便，你需要安装一些必要的扩展。

### 1\. 安装必要的 VS Code 扩展

打开 VS Code，进入 **Extensions (扩展)** 视图 (`Ctrl+Shift+X`)，然后搜索并安装以下扩展：

  * **C/C++ Extension Pack** (由 Microsoft 提供)：这是一个扩展包，包含了 C/C++ 核心扩展、CMake Tools、CMake 扩展等，强烈推荐直接安装这个包，因为它包含了所有必需的。
      * 如果你不想安装整个包，至少要安装：
          * **C/C++** (核心语言支持)
          * **CMake Tools** (核心 CMake 集成)
          * **CMake** (CMake 语法高亮和部分支持)

### 2\. 准备你的 C++ 项目

在你的项目根目录下创建一个名为 `CMakeLists.txt` 的文件。这是一个 CMake 的核心配置文件，你在这里定义你的项目是如何构建的。

**一个简单的 `CMakeLists.txt` 示例：**

```cmake
# CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 定义项目名称
project(MyCppProject VERSION 1.0)

# 添加一个可执行文件
# MyCppProject 是生成的可执行文件名
# main.cpp 是源文件
add_executable(MyCppProject main.cpp)

# 如果你的项目有多个源文件，可以这样添加：
# add_executable(MyCppProject
#     src/main.cpp
#     src/utils.cpp
#     src/another_file.cpp
# )

# 如果你想添加一个库：
# add_library(MyStaticLib STATIC src/lib_file.cpp) # 静态库
# add_library(MySharedLib SHARED src/lib_file.cpp) # 动态库

# 如果你的可执行文件依赖某个库：
# target_link_libraries(MyCppProject MyStaticLib)
```

假设你的项目结构如下：

```
MyCppProject/
├── CMakeLists.txt
└── main.cpp
```

`main.cpp` 内容示例：

```cpp
#include <iostream>

int main() {
    std::cout << "Hello from CMake and VS Code!" << std::endl;
    return 0;
}
```

### 3\. 在 VS Code 中使用 CMake Tools

安装了 CMake Tools 扩展后，VS Code 会自动识别你的 `CMakeLists.txt` 文件，并在底部状态栏显示 CMake 相关的选项。

#### a. 配置 CMake (Configure)

这是第一步，CMake 会根据你的 `CMakeLists.txt` 生成构建文件。

  * 在 VS Code 底部状态栏找到 **"Build"** (构建) 按钮旁边的 **"No Kit Selected"** 或 **"\[ ]"** 按钮。
  * 点击它，VS Code 会提示你选择一个 **"Kit"** (工具链)。Kit 是 CMake 用来编译你的代码的编译器集合（例如 MinGW-w64、MSVC、Clang 等）。
  * 选择一个你系统上安装的编译器（比如如果你安装了 MinGW-w64，它会自动检测到并显示出来）。如果列表中没有，你可能需要手动添加 Kit（通常在 VS Code 设置中搜索 "CMake: Scan For Kits" 或 "CMake: Configure" 来触发扫描）。
  * 选择 Kit 后，CMake Tools 会自动开始配置你的项目。你可以在 **"Output" (输出)** 面板中选择 **"CMake"** 选项卡查看配置过程。

#### b. 构建项目 (Build)

配置成功后，你就可以编译你的项目了。

  * 在 VS Code 底部状态栏找到 **"Build" (构建)** 按钮。
  * 点击它，CMake Tools 会执行编译命令，生成你的可执行文件。
  * 编译过程和结果也会显示在 **"Output" (输出)** 面板的 **"CMake"** 选项卡中。

#### c. 运行和调试项目 (Run & Debug)

  * 构建成功后，在 VS Code 底部状态栏的构建目标旁边，会有一个 **"Run" (运行)** 按钮（通常是一个播放图标）。点击它可以直接运行你的程序。
  * 要进行调试，你可以在 `main.cpp` 中设置断点。然后点击 VS Code 左侧的 **Run and Debug (运行和调试)** 图标 (`Ctrl+Shift+D`)。
  * 如果 CMake Tools 已经成功配置，它会自动生成一个调试配置。你只需点击顶部的绿色播放按钮旁的下拉菜单，选择你的程序（例如 `MyCppProject`），然后点击绿色播放按钮开始调试。

### 总结

CMake 是一个强大的构建系统生成器，能大大简化 C++ 项目在多平台下的开发和管理。结合 VS Code 的 CMake Tools 扩展，你可以获得一个非常流畅和高效的 C++ 开发体验。一开始可能会觉得 `CMakeLists.txt` 有点陌生，但随着你项目的增长，你会发现它的价值所在。

---

## vscode sample project.

/*
* windows 创建工程过的程:
* 1. ctrl+shift+p 打开快捷菜单-> input"cmake" , select"cmake->quick start" , follow the step for create project.
* 2. project create finish , create folders under project root folder, such lib , inc , src
* 
* 
* 
* 
* 头文件包括, 配置测试
* 路径,文件名宏引用
  ♻︎launch.json文件是VSCode启动程序的配置文件，着重关注以下几个参数：
  ✈program：代表要运行的二进制文件（也就是你的C代码编译出来的程序）所在路径
  ✈miDebuggerPath：代表调试器（GDB）所在路径
  ✈preLaunchTask：在运行program前要做的前置任务，比如编译，task.json就是用于定义前置任务

♻︎task.json也是配置文件，有几个重要参数：
✈label：指定前置任务（比如：“C/C++: gcc 生成活动文件”）名称
✈command：任务执行命令，一般来说执行编译命令：gcc
✈args：用于command后面的参数，比如：-g（debug选项），-f等
*/

如果编译出错, 清空build目录,重编.
a. 工程01,头文件包括,编译成功和调试.


**ai study path
**https://www.cloudskillsboost.google/paths/118

https://www.deeplearning.ai/courses/generative-ai-for-everyone/

https://microsoft.github.io/AI-For-Beginners/

https://www.deeplearning.ai/courses/ai-for-everyone/



https://ir.optinose.com/news-releases/news-release-details/xhance-approved-fda-first-and-only-medication-indicated
