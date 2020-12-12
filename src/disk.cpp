/*
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-12 12:52:22
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-12 22:40:32
 * @FilePath: /Operating-System-class-design/src/disk.cpp
 */
#include "../include/disk.h"

#include <cstdio>
#include <cstring>
#include <map>
#include <string>

void Disk::open_disk(const std::string& __filename) {
    if (fp != nullptr) throw std::runtime_error("可能已经挂载磁盘，请检查。");

    fp = fopen(__filename.c_str(), "rb+");
    if (fp == nullptr) throw std::runtime_error("无法打开指定磁盘（文件）。");
}

void Disk::mount(std::map<std::string, unsigned>& __file_index,
                 Index_block* __ib) {
    /* 判断是不是格式化后的磁盘，如果不是则格式化它。 */
    if (fp == nullptr) throw std::runtime_error("还未打开磁盘。");
    unsigned magic_number;
    read(&magic_number, 0, sizeof(unsigned));
    if (magic_number != 0x55ffU) format_disk();

    /* 读出超级块和索引块的内容。 */
    read(this->bitmap, ftell(fp), sizeof(bool) * BLOCKS_COUNT);

    File_group fg[MAX_OPEN_FILES];
    read(fg, ftell(fp), sizeof(File_group) * MAX_OPEN_FILES);
    __file_index = set_dentry(__file_index, fg);

    read(__ib, 512, sizeof(Index_block) * MAX_OPEN_FILES);
    rewind(fp);
}

void* Disk::read(void* __restrict__ __buffer,
                 long __offset_bytes_from_file_begin, size_t __size) {
    rewind(fp);
    fseek(fp, __offset_bytes_from_file_begin, SEEK_SET);
    fread(__buffer, __size, 1, fp);
    return __buffer;
}

void Disk::write(const void* __restrict__ __buffer,
                 long __offset_bytes_from_file_begin, size_t __size) {
    rewind(fp);
    fseek(fp, __offset_bytes_from_file_begin, SEEK_SET);
    fwrite(__buffer, __size, 1, fp);
}

void Disk::unmount(std::map<std::string, unsigned> __file_index,
                   Index_block* __restrict__ __ib) {
    File_group* fg = (File_group*)malloc(sizeof(File_group) * MAX_OPEN_FILES);
    int i = 0;
    for (auto itr : __file_index) {
        std::string name, ext;
        get_name_and_ext(itr.first, name, ext);
        strcpy(fg[i].name, name.c_str());
        strcpy(fg[i].ext, ext.c_str());
        fg[i].index = itr.second;
    }

    write_to_disk(this->bitmap, fg, __ib, fp);

    free(fg);
}

void* Disk::read_block_data(unsigned index, void* buffer) {
    if (index > BLOCK_SIZE) throw std::out_of_range("读取的内容超过块大小");

    read(buffer, BLOCK_SIZE * (index - 1), BLOCK_SIZE);
    return buffer;
}

void Disk::write_to_a_empty_block_data(const void* __buffer, size_t __n) {
    auto index = find_empty_block();
    bitmap[index] = true;
    write(__buffer, BLOCK_SIZE * index, __n);
}

unsigned Disk::find_empty_block() {
    for (int i = 2; i < BLOCKS_COUNT; i++)
        if (bitmap[i] == false) return i;
    throw std::logic_error("没有空闲的数据块了。");
}

void Disk::get_name_and_ext(const std::string& filename, std::string& name,
                            std::string& ext) {
    auto index = filename.find('.');

    name = filename.substr(0, index);
    ext = filename.substr(index + 1);
}

void Disk::format_disk() {
    // 初始化磁盘空间，分配200个物理块，每个512bytes。
    unsigned size = BLOCK_SIZE * BLOCKS_COUNT;
    for (int i = 0; i < size; i++) fputc('\0', fp);

    // 初始化第一个物理块。
    unsigned magic_number = 0x55ff;
    write(&magic_number, 0, sizeof(unsigned));
    for (size_t i = 2; i < BLOCKS_COUNT; i++) this->bitmap[i] = false;

    File_group fg[MAX_OPEN_FILES];
    for (size_t i = 0; i < MAX_OPEN_FILES; i++)
        memset(&fg[i], 0, sizeof(File_group));

    /* 初始化第二个物理块。 */
    Index_block ib[MAX_OPEN_FILES];
    for (size_t i = 0; i < MAX_OPEN_FILES; i++) {
        memset(&ib[i], 0, sizeof(Index_block));
        (ib + i)->use_flag = false;
    }
    /* 将数据写入磁盘文件。 */
    write_to_disk(this->bitmap, fg, ib, fp);
}

void Disk::write_to_disk(bool* bitmap, File_group* fg, Index_block* ib,
                         FILE* fp) {
    /* 写入第一块。 */
    write(bitmap, sizeof(unsigned), sizeof(bool) * BLOCKS_COUNT);
    write(fg, ftell(fp), sizeof(File_group) * MAX_OPEN_FILES);

    /* 写入第二块。 */
    write(ib, BLOCK_SIZE, sizeof(Index_block) * MAX_OPEN_FILES);
}

std::map<std::string, unsigned>& Disk::set_dentry(
    std::map<std::string, unsigned>& dentry, File_group* fg) {
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (strcmp(fg[i].name, "\0") == 0) continue;

        char tmp[16];
        strcpy(tmp, fg[i].name);
        strcat(tmp, ".");
        strcat(tmp, fg[i].ext);
        dentry.insert({std::string(tmp), fg[i].index});
    }
    return dentry;
}