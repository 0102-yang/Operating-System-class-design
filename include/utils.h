/*
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-12 16:43:04
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-12 17:25:12
 * @FilePath: /Operating-System-class-design/include/utils.h
 */
#ifndef __UTILS_H_
#define __UTILS_H_

constexpr size_t BLOCK_SIZE = 512;
constexpr size_t BLOCKS_COUNT = 200;
constexpr size_t MAX_OPEN_FILES = 10;

struct File_group {
    char name[10];
    char ext[6];
    unsigned index;
};

struct Super_block {
    std::map<const char*, unsigned> file_index;
    bool bitmap[BLOCKS_COUNT];
};

struct Index_block {
    bool use_flag;  // true表示索引块被使用，false表示没被使用。
    // char filename[10];  // 文件名。
    // char file_ext_name[6];      // 扩展文件名。
    time_t create_time;         // 文件创建时间。
    time_t last_modified_time;  // 最后修改时间。
    time_t last_query_time;     // 最后查看时间。
    unsigned filesize;          // 文件大小（bytes）。
    unsigned start_index;       // 磁盘物理块起始位置。
};

#endif  // __UTILS_H_