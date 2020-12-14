/*
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-12 12:47:31
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-15 00:34:24
 * @FilePath: /Operating-System-class-design/include/bfs.h
 */
#ifndef __DISK_H_
#define __DISK_H_

#include <string>
#include <vector>

#include "utils.h"

class Basic_File_System {
    disk_pointer operator_ptr = nullptr;
    Super_block* control_unit = nullptr;

   public:
    Basic_File_System() = default;

    Basic_File_System(const Basic_File_System&) = delete;

    Basic_File_System& operator=(const Basic_File_System&) = delete;

    ~Basic_File_System() {
        if (this->control_unit != nullptr) {
            delete this->control_unit;
            this->control_unit = nullptr;
        }
    }

    /**
     * @brief 返回当前磁盘的超级控制块。
     * @param
     */
    const Super_block* control() const { return this->control_unit; }

    /**
     * @brief 挂载指定磁盘（文件）。
     * @param __filename 磁盘名（文件名）。
     */
    void mount(const std::string& __filename);

    /**
     * @brief 返回当前磁盘流位置开始的__size个字节。
     * @param __buffer 缓冲区。
     * @param __size 字节数。
     */
    void* df_read(void* __buffer, size_t __size) const {
        fread(__buffer, __size, 1, this->operator_ptr);
        return __buffer;
    }

    /**
     * @brief 将缓冲区内的指定字节数内容读入当前磁盘流位置。
     * @param __buffer 缓冲区。
     * @param __size 字节数。
     */
    void df_write(const void* __buffer, size_t __size) {
        fwrite(__buffer, __size, 1, this->operator_ptr);
    }

    /**
     * @brief 重定位当前磁盘流位置。
     * @param __start_block_index 指定从第几个物理块开始。
     * @param __offset 偏移地址。
     */
    void df_seek(unsigned __start_block_index, long __offset = 0) {
        long offset = __start_block_index * BLOCK_SIZE + __offset;
        fseek(operator_ptr, offset, SEEK_SET);
    }

    /**
     * @brief 格式化磁盘。
     */
    void format_disk();
};

#endif  // __DISK_H_