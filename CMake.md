
# CMake

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


