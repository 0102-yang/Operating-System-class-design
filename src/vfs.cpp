/*
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-14 14:43:21
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-16 23:32:17
 * @FilePath: /Operating-System-class-design/src/vfs.cpp
 */
#include "../include/vfs.h"

#include <cstring>
#include <string>
#include <vector>

void Virtual_File_System::associateWithDisk(const std::string& diskname) {
    this->diskname = diskname;
    this->bfs.mount(diskname.c_str());

    /* 从磁盘中读取文件控制块和位图信息。 */
    read_index_node_bitmap(&this->inb);
    read_data_node_bitmap(&this->dnb);

    init_exist_file_directory();
}

void Virtual_File_System::init_exist_file_directory() {
    Directory tmp_directory;

    for (unsigned i = 0; i < this->bfs.control()->max_index_nodes_capacity;
         i++) {
        if (this->inb.bitmap[i] == false) continue;

        read_single_directory_node(&tmp_directory, i);

        this->exist_file_list.emplace(tmp_directory.fullname,
                                      tmp_directory.index_node_index);
    }
}

void Virtual_File_System::openFile(const std::string& filename) {
    auto file_itr = this->exist_file_list.find(filename);
    if (file_itr == this->exist_file_list.end())
        throw std::logic_error("无法打开一个不存在的文件。");

    Index_node tmp_index_node;
    auto file_name = file_itr->first;
    auto offset = file_itr->second;
    read_single_index_node(&tmp_index_node, offset);

    this->open_file_list.insert(
        {std::move(file_name), std::move(tmp_index_node)});
}

void Virtual_File_System::closeFile(const std::string& filename) {
    auto file_itr = this->open_file_list.find(filename);
    if (file_itr == this->open_file_list.end())
        throw std::logic_error("无法关闭一个未打开的文件。");

    write_single_index_node(&file_itr->second,
                            this->exist_file_list.at(filename));

    this->open_file_list.erase(filename);
}

void Virtual_File_System::createFileByFileName(const std::string& filename) {
    if (this->exist_file_list.find(filename) != this->exist_file_list.end())
        throw std::logic_error("文件名已经存在，无法创建文件。");

    auto free_index_node_index = find_free_index_node_index();
    allocate_specified_index_node(free_index_node_index);

    this->exist_file_list.insert({filename, free_index_node_index});
}

void Virtual_File_System::removeFileByFileName(const std::string& filename) {
    auto file_itr = this->exist_file_list.find(filename);
    if (file_itr == this->exist_file_list.end())
        throw std::logic_error("无法删除一个不存在的文件。");

    Index_node tmp_index_node;
    memset(&tmp_index_node, 0, sizeof(Index_node));

    auto index_node_index = file_itr->second;

    write_single_index_node(&tmp_index_node, index_node_index);
    deallocate_specified_index_node(index_node_index);

    this->exist_file_list.erase(filename);
}

void Virtual_File_System::save_right_now_status() {
    for (auto itr : exist_file_list) {
        Directory d;
        strncpy(d.fullname, itr.first.c_str(), MAX_FILE_NAME_SIZE);
        d.index_node_index = itr.second;
        write_single_directory_node(&d, d.index_node_index);
    }

    write_index_node_bitmap(&this->inb);
    write_data_node_bitmap(&this->dnb);
}

std::string Virtual_File_System::getFileContentByFileName(
    const std::string& fileName) {
    auto file_ptr = this->open_file_list.find(fileName);
    if (file_ptr == this->open_file_list.end())
        throw std::logic_error("无法获取文件的内容，或许没有打开文件。");

    std::string content;

    auto& tmp_index_node = file_ptr->second;

    for (unsigned short i = 0; i < tmp_index_node.blocks_count; i++) {
        void* buffer = malloc(BLOCK_SIZE);
        buffer = read_single_data_block(buffer, tmp_index_node.data_index[i]);
        content += std::string((char*)buffer);
    }

    tmp_index_node.last_query_time = time(NULL);
    return content.substr(0, tmp_index_node.filesize);
}

void Virtual_File_System::updateFileContentByFileName(
    const std::string& fileName, const std::string& fileContent) {
    auto file_itr = this->open_file_list.find(fileName);
    if (file_itr == this->open_file_list.end())
        throw std::logic_error("无法更新指定文件内容。");

    auto& tmp_index_node = file_itr->second;

    auto tmp_block_count = fileContent.size() / BLOCK_SIZE + 1;

    auto write_file = [this, &fileContent](Index_node& tmp_index_node,
                                           unsigned long tmp_block_count) {
        for (size_t i = 0; i < tmp_block_count; i++)
            write_single_data_block(
                const_cast<char*>(
                    fileContent.substr(i * BLOCK_SIZE, (i + 1) * BLOCK_SIZE)
                        .c_str()),
                tmp_index_node.data_index[i]);
    };

    if (tmp_block_count > tmp_index_node.blocks_count) {
        /* 先申请新的数据块。 */
        for (auto i = tmp_index_node.blocks_count; i < tmp_block_count; i++) {
            auto tmp_free_data_block_index = find_free_data_block();
            allocate_specified_data_block(tmp_free_data_block_index);

            tmp_index_node.blocks_count++;
            tmp_index_node.data_index[i] = tmp_free_data_block_index;
        }

        /* 将当前内容覆盖写入文件。 */
        write_file(tmp_index_node, tmp_block_count);
    } else if (tmp_block_count < tmp_index_node.blocks_count) {
        /* 先归还超出部分的数据块。 */
        for (auto i = tmp_block_count; i < tmp_index_node.blocks_count; i++) {
            deallocate_specified_data_block(tmp_index_node.data_index[i]);
            tmp_index_node.blocks_count--;
        }
        /* 将当前内容覆盖读入文件。 */
        write_file(tmp_index_node, tmp_block_count);
    } else {
        write_file(tmp_index_node, tmp_block_count);
    }

    tmp_index_node.filesize = fileContent.size();
    tmp_index_node.last_modified_time = time(NULL);
}