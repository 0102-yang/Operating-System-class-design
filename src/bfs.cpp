/*
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-12 12:52:22
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-15 00:45:50
 * @FilePath: /Operating-System-class-design/src/bfs.cpp
 */
#include "../include/bfs.h"

#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>

#include "../include/utils.h"

void Basic_File_System::mount(const std::string& __filename) {
    /* 判断是否挂载了磁盘文件。 */
    if (this->operator_ptr != nullptr)
        throw std::logic_error("当前文件系统已经挂载了磁盘，请检查。");

    /* 挂载磁盘文件。 */
    this->operator_ptr = fopen(__filename.c_str(), "rb+");
    if (this->operator_ptr == nullptr)
        throw std::runtime_error("无法打开指定磁盘（文件）。");

    /* 为超级控制块赋值。 */
    // note 暂时写死磁盘的数据。
    this->control_unit = new Super_block{5, 192, 0, 1, 2, 3, 4, 8};

    /* 判断磁盘文件是否被格式化。 */
    unsigned long magic_number;  // 磁盘魔数。
    df_read((void*)&magic_number, sizeof(unsigned long));

    if (magic_number != 0x55ff55ffUL) {
        printf("当前磁盘格式有误，是否进行格式化？(Y/n)");

        char c;
        scanf("%c", &c);
        if (c == 'y' || c == 'Y') format_disk();
    }
}

void Basic_File_System::format_disk() {
    /* 初始化数据结构。 */

    Index_node_bitmap inb{MAX_INDEX_NODES, 0};

    Data_blocks_bitmap dnb{MAX_DATA_BLOCKS, 0};

    Index_node in;

    Directory directory;
    memset(&directory, 0, sizeof(Directory));

    /* 格式化磁盘的每一个字节。 */
    df_seek(this->control_unit->disk_start);
    df_write("\0", BLOCK_SIZE * BLOCKS_CAPACITY);

    /* 写入磁盘魔数。 */
    df_seek(this->control_unit->disk_start);
    unsigned long magic_number = 0x55ff55ff;
    df_write(&magic_number, sizeof(unsigned long));

    /* 写入超级控制单元。 */
    df_seek(this->control_unit->disk_start, sizeof(unsigned long));
    df_write(control_unit, sizeof(Super_block));

    /* 写入索引块空闲位图。 */
    df_seek(this->control_unit->inmap_start);
    df_write(&inb, sizeof(Index_node_bitmap));

    /* 写入数据块空闲位图。 */
    df_seek(this->control_unit->dnmap_start);
    df_write(&dnb, sizeof(Data_blocks_bitmap));

    /* 写入目录和索引块。 */
    for (unsigned i = 0; i < MAX_INDEX_NODES; i++) {
        // 写入目录。
        df_seek(this->control_unit->directory_start, sizeof(Directory) * i);
        df_write(&directory, sizeof(Directory));

        // 写入索引块。
        df_seek(this->control_unit->innode_blocks_start,
                sizeof(Index_node) * i);
        df_write(&in, sizeof(Index_node));
    }
}