/*
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-12 12:52:22
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-13 17:04:56
 * @FilePath: /Operating-System-class-design/src/disk.cpp
 */
#include "../include/disk.h"

#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>

#include "../include/utils.h"

void Disk::mount(const std::string& __filename) {
    if (this->super_block_ptr != nullptr)
        throw std::logic_error("当前文件系统已经挂载了磁盘，请检查。");

    this->operator_ptr = fopen(__filename.c_str(), "rb+");
    if (this->operator_ptr == nullptr)
        throw std::runtime_error("无法打开指定磁盘（文件）。");

    unsigned long magic_number;
    df_read((void*)&magic_number, sizeof(unsigned long));
    if (magic_number != 0x55ff55ffUL) {
        printf("当前磁盘格式有误，是否进行格式化？(Y/n)");
        char c;
        scanf("%c", &c);
        if (c == 'Y' || c == 'y') format_disk();
    }

    df_seek(1, sizeof(unsigned long));
    this->super_block_ptr = new Super_block();
    df_read(this->super_block_ptr, sizeof(Super_block));
}

void* Disk::df_read(void* __restrict__ __buffer, size_t __size) {
    fread(__buffer, __size, 1, this->operator_ptr);
    return __buffer;
}

void Disk::df_write(const void* __restrict__ __buffer, size_t __size) {
    fwrite(__buffer, __size, 1, this->operator_ptr);
}

void Disk::format_disk() {
    // 初始化磁盘空间，分配200个物理块，每个512bytes。
    // 初始化第一个物理块。
    Super_block super_block{5, 192, 1, 2, 3, 4, 9};
    Index_node_bitmap inb{0};
    Data_node_bitmap dnb{0};
    Index_node in;
    for (size_t i = 0; i < MAX_SIZE_OF_A_INDEX_NODE; i++) in.data_index[i] = -1;

    df_seek(1);
    df_write("\0", BLOCK_SIZE * BLOCKS_CAPACITY);

    df_seek(1);
    unsigned long magic_number = 0x55ff55ff;
    df_write(&magic_number, sizeof(unsigned long));

    df_seek(1, sizeof(unsigned long));
    df_write(&super_block, sizeof(Super_block));

    df_seek(2);
    df_write(&inb, sizeof(Index_node_bitmap));

    df_seek(3);
    df_write(&dnb, sizeof(Data_node_bitmap));

    for (int i = 4; i < 9; i++) {
        for (int j = 0; j < 16; j++) {
            df_seek(i, j * 256);
            df_write(&in, sizeof(Index_node));
        }
    }
}

void Disk::df_seek(unsigned __start_block_index, long __n) {
    long offset = (__start_block_index - 1) * BLOCK_SIZE + __n;
    fseek(operator_ptr, offset, SEEK_SET);
}