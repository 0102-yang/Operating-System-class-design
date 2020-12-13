/*
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-12 12:47:31
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-13 17:09:49
 * @FilePath: /Operating-System-class-design/include/disk.h
 */
#ifndef __DISK_H_
#define __DISK_H_

#include <string>
#include <vector>

#include "utils.h"

class Disk {
    Super_block* super_block_ptr = nullptr;
    // std::vector<Directory> directories;
    disk_pointer operator_ptr = nullptr;

   public:
    Disk() = default;

    Disk(const Disk&) = delete;

    Disk& operator=(const Disk&) = delete;

    const Super_block* get_super_block() const { return this->super_block_ptr; }

    /**
     * @brief 打开指定磁盘（文件）。
     * @param __filename 磁盘名（文件名）。
     */
    void mount(const std::string& __filename);

    void* df_read(void* __restrict__ __buffer, size_t __size);

    void df_write(const void* __restrict__ __buffer, size_t __size);

    void df_seek(unsigned __start_block_index, long __n = 0L);

    void format_disk();
};

#endif  // __DISK_H_