/*
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-12 12:47:31
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-12 22:40:06
 * @FilePath: /Operating-System-class-design/include/disk.h
 */
#ifndef __DISK_H_
#define __DISK_H_

#include <map>
#include <string>

#include "utils.h"

class Disk {
    FILE* fp = nullptr;  // 磁盘文件指针。
    bool bitmap[200];    // 空闲块位图。

   public:
    Disk() = default;

    Disk(const Disk&) = delete;

    Disk& operator=(const Disk&) = delete;

    /**
     * @brief 打开指定磁盘（文件）。
     * @param __filename 磁盘名（文件名）。
     */
    void open_disk(const std::string& __filename);

    /**
     * @brief 挂载磁盘，将控制块数据读入。
     * @param __file_index 文件名和文件索引块物理位置的键值对
     * @param __ib 文件索引块。
     * @return
     */
    void mount(std::map<std::string, unsigned>& __file_index,
               Index_block* __ib);

    void unmount(std::map<std::string, unsigned> __file_index,
                 Index_block* __ib);

    void* read_block_data(unsigned __index, void* __buffer);

    void write_to_a_empty_block_data(const void* __buffer, size_t __n);

    void* read(void* __restrict__ __buffer, long __offset_bytes_from_file_begin,
               size_t __size);

    /**
     * @brief 将指定大小的数据写入磁盘开始的指定位置。
     * @param buffer 写入的数据。
     * @param __offset_bytes_from_file_begin 磁盘开始的偏移位置。
     * @param __size 写入的字节数。
     */
    void write(const void* buffer, long __offset_bytes_from_file_begin,
               size_t __size);

   private:
    /**
     * @brief 找出任一空闲的物理块。返回其在数据块中的编号。
     * @return 返回编号。
     */
    unsigned find_empty_block();

    /**
     * @brief 格式化当前文件指针代表的磁盘。
     */
    void format_disk();

    /**
     * @brief 从索引块设置目录项。
     * @param dentry 设置的目录项对象。
     * @param fg 索引块目录。
     * @return
     */
    std::map<std::string, unsigned>& set_dentry(
        std::map<std::string, unsigned>& dentry, File_group* fg);

    void write_to_disk(bool* bitmap, File_group* fg, Index_block* ib, FILE* fp);

    /**
     * @brief 将指定文件名分解为主文件名和扩展名。
     * @param filename 文件名。
     * @param name 主文件名。
     * @param ext 扩展名。
     * @return
     */
    void get_name_and_ext(const std::string& filename, std::string& name,
                          std::string& ext);
};

#endif  // __DISK_H_