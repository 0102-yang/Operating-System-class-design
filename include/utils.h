/*
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-12 16:43:04
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-13 17:15:17
 * @FilePath: /Operating-System-class-design/include/utils.h
 */
#ifndef __UTILS_H_
#define __UTILS_H_

#include <cstdio>
#include <cstdlib>

typedef FILE* disk_pointer;
typedef FILE* file_pointer;

constexpr size_t BLOCK_SIZE = 0x1000;
constexpr size_t BLOCKS_CAPACITY = 200;
constexpr size_t MAX_INDEX_BLOCKS = 80;
constexpr size_t MAX_DATA_BLOCKS = 192;
constexpr size_t MAX_SIZE_OF_A_INDEX_NODE = 127;

struct Index_node {
    unsigned short blocks_count;
    short data_index[MAX_SIZE_OF_A_INDEX_NODE];
};

struct Index_node_bitmap {
    bool bitmap[MAX_INDEX_BLOCKS];
};

struct Data_node_bitmap {
    bool bitmap[MAX_DATA_BLOCKS];
};

struct Super_block {
    size_t index_nodes_capacity;
    size_t data_blocks_capacity;
    unsigned short disk_start;
    unsigned short inmap_start;
    unsigned short dnmap_start;
    unsigned short innode_blocks_start;
    unsigned short data_blocks_start;
};

struct Directory {
    char filename[10];      // 文件名。
    char file_ext_name[6];  // 扩展文件名。
    // time_t create_time;           // 文件创建时间。
    time_t last_modified_time;        // 最后修改时间。
    time_t last_query_time;           // 最后查看时间。
    unsigned filesize;                // 文件大小（bytes）。
    unsigned short index_node_index;  // 磁盘物理块起始位置。
};

#endif  // __UTILS_H_