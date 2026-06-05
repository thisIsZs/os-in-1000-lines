#pragma once

//页表
#define SATP_SV32 (1u << 31)
#define PAGE_V    (1 << 0)   // "Valid" 位（表项已启用）
#define PAGE_R    (1 << 1)   // 可读
#define PAGE_W    (1 << 2)   // 可写
#define PAGE_X    (1 << 3)   // 可执行
#define PAGE_U    (1 << 4)   // 用户（用户模式可访问）

// 应用程序镜像的基础虚拟地址。这需要与 `user.ld` 中定义的起始地址匹配。
#define USER_BASE 0x1000000

//系统调用
#define SCAUSE_ECALL 8

//用户模式
#define SSTATUS_SPIE (1 << 5)

#define PANIC(fmt, ...)                                                        \
    do {                                                                       \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
        while (1) {}                                                           \
    } while (0)

struct sbiret {
    long error;
    long value;
};