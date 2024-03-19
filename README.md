# vscode_cpp_conf
vscode sample project.

/*
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
