# Operating System in 1,000 Lines

这是我跟随 [Operating System in 1,000 Lines](https://1000os.seiya.me/) 教程完成的操作系统学习项目。项目使用 C 和少量 RISC-V 汇编，在 QEMU 的 32 位 RISC-V `virt` 平台上运行。

代码基于原项目的教程和参考实现，仓库保留了学习过程中的提交记录和中文注释。

原代码仓库：[nuta/operating-system-in-1000-lines](https://github.com/nuta/operating-system-in-1000-lines)

## 已实现的功能

- 内核启动、异常处理和基于 SBI 的字符输入输出。
- 物理页分配与 Sv32 分页。
- 进程创建、上下文切换和协作式调度。
- 用户态程序与系统调用。
- 简单的交互式 Shell。
- VirtIO 块设备读写。
- 基于 tar 格式的简单文件系统，支持读取和写入已有文件。

## 构建与运行

当前 `run.sh` 按 macOS 上 Homebrew 的 `/opt/homebrew` 安装路径配置。需要 Clang、LLD、llvm-objcopy、QEMU，以及支持 ustar 格式的 tar。

使用 Homebrew 安装依赖：

```bash
brew install llvm lld qemu
export PATH="$(brew --prefix llvm)/bin:$(brew --prefix lld)/bin:$PATH"
```

依赖说明见 Homebrew 的 [LLVM](https://formulae.brew.sh/formula/llvm)、[LLD](https://formulae.brew.sh/formula/lld) 和 [QEMU](https://formulae.brew.sh/formula/qemu) 页面。LLD 目前作为独立软件包提供。

在项目根目录运行：

```bash
./run.sh
```

脚本会编译 Shell 和内核，将 `disk/*.txt` 打包为 `disk.tar`，然后启动 QEMU。启动后可以在终端中的 `> ` 提示符下输入命令。

如果使用 Linux、Intel Mac 或不同的安装路径，需要先修改 `run.sh` 中的 `CC` 和 `OBJCOPY`，指向本机的 Clang 和 llvm-objcopy，并确保 `ld.lld` 与 `qemu-system-riscv32` 可在 `PATH` 中找到。

## Shell 命令

| 命令 | 功能 |
| --- | --- |
| `hello` | 输出 `Hello world from shell!` |
| `readfile` | 读取并显示 `hello.txt` 的内容 |
| `writefile` | 向 `hello.txt` 写入 `Hello from shell!` 和换行符 |
| `exit` | 退出当前 Shell 进程 |

可以依次输入 `readfile`、`writefile`、`readfile` 观察文件内容变化。这些命令目前不接受文件名参数。

退出 QEMU：先按 `Ctrl+A`，松开后再按 `X`，参见 [QEMU 快捷键说明](https://www.qemu.org/docs/master/system/mux-chardev.html)。Shell 的 `exit` 命令只退出进程，不负责关闭 QEMU；当前内核在 Shell 退出、切回空闲进程后会触发 panic。

## 项目结构

| 文件 | 用途 |
| --- | --- |
| `kernel.c` / `kernel.h` | 内核、进程管理、分页、异常处理、VirtIO 驱动和文件系统 |
| `kernel.ld` | 内核链接脚本 |
| `user.c` / `user.h` | 用户程序入口和系统调用封装 |
| `user.ld` | 用户程序链接脚本 |
| `shell.c` | 用户态 Shell |
| `common.c` / `common.h` | 内核与用户程序共用的类型和基础函数 |
| `disk/hello.txt` / `disk/meow.txt` | 初始磁盘文件 |
| `run.sh` | 构建并启动 QEMU |

## 当前限制

这是用于理解操作系统基本机制的教学项目。文件系统最多容纳 2 个文件，每个文件的数据缓冲区为 1024 字节；没有实现通用的文件创建、目录管理或完整的输入校验。

运行中的文件写入会更新 `disk.tar`，不会修改宿主机上的 `disk/*.txt`。每次执行 `run.sh` 都会重新打包磁盘镜像，因此上一次运行中写入的内容会被初始文件覆盖。

编译产物、磁盘镜像和 QEMU 日志由脚本生成，不纳入版本控制。

## 来源与许可

感谢原作者提供教程和参考实现。根据[上游许可声明](https://github.com/nuta/operating-system-in-1000-lines/blob/main/LICENCE.md)，操作系统代码采用 MIT 许可证；上游 `website/` 目录下的 Markdown 文档采用 CC BY 4.0。此仓库中的教程衍生代码遵循上游 MIT 许可，分发时应保留相关版权与许可声明。
