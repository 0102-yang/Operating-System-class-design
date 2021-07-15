/*
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-12 16:43:04
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-15 14:37:44
 * @FilePath: /Operating-System-class-design/include/utils.h
 */
#ifndef __UTILS_H_
#define __UTILS_H_

typedef FILE *disk_pointer;  // 磁盘内控制I/O流指针。
typedef FILE *file_pointer;  // 文件内控制I/O流指针。

constexpr size_t BLOCK_SIZE = 0x1000;    // 物理块的大小。
constexpr size_t BLOCKS_CAPACITY = 200;  // 磁盘物理块容量。
constexpr size_t MAX_INDEX_NODES = 64;   // 索引节点数量最大值。
constexpr size_t MAX_DATA_BLOCKS = 192;  // 数据块数量最大值。
constexpr size_t MAX_INDEX_COUNT_IN_A_INDEX_NODE =
        118;  // 索引节点内索引的最大个数。
constexpr size_t MAX_FILE_NAME_SIZE = 46;

/* 索引节点数据结构。 */
struct Index_node {
    // time_t create_time;           // 文件创建时间。
    time_t last_modified_time = 0;  // 最后修改时间。
    time_t last_query_time = 0;     // 最后查看时间。
    unsigned short fileSize = 0;    // 文件大小（bytes）。
    unsigned short blocks_count = 0;  // 当前索引节点索引的数据块数量。
    short data_index[MAX_INDEX_COUNT_IN_A_INDEX_NODE];  // 索引的数据块下标。。
};

/* 文件目录/文件控制块。 */
struct Directory {
    char fullname[MAX_FILE_NAME_SIZE];  // 文件全名。
    unsigned short index_node_index;  // 当前文件的磁盘索引块位置。
};

/* 空闲索引节点位图。 */
struct Index_node_bitmap {
    size_t free_index_nodes_count;  // 空闲的索引节点数量。
    bool bitmap[MAX_INDEX_NODES];   // 位图。
};

/* 空闲数据块位图。 */
struct Data_blocks_bitmap {
    size_t free_data_blocks_count;  // 空闲的数据块数量。
    bool bitmap[MAX_DATA_BLOCKS];   // 位图。
};

/* 超级块数据结构。 */
struct Super_block {
    size_t max_index_nodes_capacity;  // 最大索引节点容量。
    size_t max_data_blocks_capacity;  // 最大数据块容量。
    unsigned short disk_start;        // 磁盘开始的块编号。
    unsigned short directory_start;   // 目录开始的块编号。
    unsigned short inmap_start;       // 索引节点位图开始的块编号。
    unsigned short dnmap_start;       // 数据块位图开始的块编号。
    unsigned short innode_blocks_start;  // 索引节点块开始的块编号。
    unsigned short data_blocks_start;    // 数据块开始的块编号。
};

#endif  // __UTILS_H_