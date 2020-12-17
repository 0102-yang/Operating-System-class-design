/*
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-14 14:40:30
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-17 15:29:36
 * @FilePath: /Operating-System-class-design/include/vfs.h
 */
#ifndef __VFS_H_
#define __VFS_H_

#include <map>
#include <string>
#include <vector>

#include "bfs.h"
#include "utils.h"

class Virtual_File_System {
    std::string diskname;  // 当前挂载的磁盘文件名。

    // Super_block super_block; // 超级控制块。

    Basic_File_System bfs;  // 基础文件系统。

    Index_node_bitmap inb;  // 空闲索引块位图。

    Data_blocks_bitmap dnb;  // 空闲索引块位图。

    // Directory directory;

    std::map<std::string, unsigned short>
        exist_file_list;  // 已存在的文件名和它的索引块下标。

    std::map<std::string, Index_node> open_file_list;  // 打开文件表。

   public:
    Virtual_File_System() = default;

    Virtual_File_System(const Virtual_File_System&) = delete;

    Virtual_File_System& operator=(const Virtual_File_System&) = delete;

    ~Virtual_File_System() { save_right_now_status(); }

    /**
     * @brief 将该虚拟文件系统关联一个磁盘文件。
     * @param diskname 磁盘名。
     */
    void associateWithDisk(const std::string& diskname);

    /**
     * @brief 打开一个文件。
     * @param filename 文件名。
     */
    void openFile(const std::string& filename);

    /**
     * @brief 关闭一个文件。
     * @param filename 文件名。
     */
    void closeFile(const std::string& filename);

    /**
     * @brief 通过文件名创建一个文件。
     * @param filename 文件名。
     */
    void createFileByFileName(const std::string& filename);

    /**
     * @brief 通过文件名删除一个文件。
     * @param filename 文件名。
     */
    void removeFileByFileName(const std::string& filename);

    /**
     * @brief 获取并返回所有文件名的列表。
     * @param fileNameList （空）文件名列表。
     */
    std::vector<std::string>& getAllFileName(
        std::vector<std::string>& fileNameList) const {
        for (auto itr : this->exist_file_list)
            fileNameList.push_back(itr.first);
        return fileNameList;
    }

    /**
     * @brief 通过指定内容更新指定文件的内容。
     * @param fileName 文件名。
     * @param fileContent 内容。
     */
    void updateFileContentByFileName(const std::string& fileName,
                                     const std::string& fileContent);

    /**
     * @brief 将当前位图状态写入磁盘。
     */
    void save_right_now_status();

    /**
     * @brief 通过文件名获取文件内容。
     * @param fileName 文件名。
     */
    std::string getFileContentByFileName(const std::string& fileName);

    /**
     * @brief 通过文件名获取文件的属性。
     * @param fileName 文件名。
     */
    Index_node getFileAttributeByFileName(const std::string& fileName) {
        auto file_itr = this->exist_file_list.find(fileName);
        if (file_itr == this->exist_file_list.end())
            throw std::logic_error("没有该文件或未打开文件。");
        Index_node i;
        return *read_single_index_node(&i, file_itr->second);
    }

   private:
    void init_exist_file_directory();
    /* 索引节点I/O控制。 */

    void write_single_index_node(Index_node* __in, unsigned __num) {
        auto address = this->bfs.control()->innode_blocks_start;
        this->bfs.df_seek(address, sizeof(Index_node) * __num);
        this->bfs.df_write(__in, sizeof(Index_node));
    }

    Index_node* read_single_index_node(Index_node* __out, unsigned __num) {
        auto address = this->bfs.control()->innode_blocks_start;
        this->bfs.df_seek(address, sizeof(Index_node) * __num);
        this->bfs.df_read(__out, sizeof(Index_node));
        return __out;
    }

    Super_block* read_single_super_block(Super_block* __out) {
        auto address = this->bfs.control()->disk_start;
        this->bfs.df_seek(address, sizeof(unsigned long));
        this->bfs.df_read(__out, sizeof(Super_block));
        return __out;
    }

    void write_single_super_block(Super_block* __in) {
        auto address = this->bfs.control()->disk_start;
        this->bfs.df_seek(address);
        this->bfs.df_write(__in, sizeof(Super_block));
    }

    void write_index_node_bitmap(Index_node_bitmap* __in) {
        auto address = this->bfs.control()->inmap_start;
        this->bfs.df_seek(address);
        this->bfs.df_write(__in, sizeof(Index_node_bitmap));
    }

    Index_node_bitmap* read_index_node_bitmap(Index_node_bitmap* __out) {
        auto address = this->bfs.control()->inmap_start;
        this->bfs.df_seek(address);
        this->bfs.df_read(__out, sizeof(Index_node_bitmap));
        return __out;
    }

    void write_data_node_bitmap(Data_blocks_bitmap* __in) {
        auto address = this->bfs.control()->dnmap_start;
        this->bfs.df_seek(address);
        this->bfs.df_write(__in, sizeof(Data_blocks_bitmap));
    }

    Data_blocks_bitmap* read_data_node_bitmap(Data_blocks_bitmap* __out) {
        auto address = this->bfs.control()->dnmap_start;
        this->bfs.df_seek(address);
        this->bfs.df_read(__out, sizeof(Data_blocks_bitmap));
        return __out;
    }

    void write_single_directory_node(Directory* __in, unsigned __num) {
        auto address = this->bfs.control()->directory_start;
        this->bfs.df_seek(address, sizeof(Directory) * __num);
        this->bfs.df_write(__in, sizeof(Directory));
    }

    Directory* read_single_directory_node(Directory* __out, unsigned __num) {
        auto address = this->bfs.control()->directory_start;
        this->bfs.df_seek(address, sizeof(Directory) * __num);
        this->bfs.df_read(__out, sizeof(Directory));
        return __out;
    }

    void* read_single_data_block(void* __buffer, unsigned __block_num) {
        auto address = this->bfs.control()->data_blocks_start + __block_num;
        this->bfs.df_seek(address);
        this->bfs.df_read(__buffer, BLOCK_SIZE);
        return __buffer;
    }

    void write_single_data_block(void* __buffer, unsigned __block_num) {
        auto address = this->bfs.control()->data_blocks_start + __block_num;
        this->bfs.df_seek(address);
        this->bfs.df_write(__buffer, BLOCK_SIZE);
    }

    unsigned short find_free_index_node_index() {
        if (this->inb.free_index_nodes_count == 0)
            throw std::logic_error("没有更多的空索引节点。");
        for (size_t i = 0; i < MAX_INDEX_NODES; i++) {
            if (this->inb.bitmap[i] == false) return i;
        }
        return 0;
    }

    void allocate_specified_index_node(unsigned short __num) {
        if (this->inb.bitmap[__num] == true ||
            this->inb.free_index_nodes_count == 0)
            throw std::logic_error("该索引节点已被分配，分配新索引节点失败。");

        this->inb.bitmap[__num] = true;
        this->inb.free_index_nodes_count--;
    }

    void deallocate_specified_index_node(unsigned short __num) {
        if (this->inb.bitmap[__num] == false)
            throw std::logic_error("该索引节点未被分配，归还该索引节点失败。");

        this->inb.bitmap[__num] = false;
        this->inb.free_index_nodes_count++;
    }

    unsigned short find_free_data_block() {
        if (this->dnb.free_data_blocks_count == 0)
            throw std::logic_error("没有更多的空数据块。");
        for (size_t i = 0; i < MAX_DATA_BLOCKS; i++) {
            if (this->dnb.bitmap[i] == false) return i;
        }
        return 0;
    }

    void allocate_specified_data_block(unsigned short __num) {
        if (this->dnb.free_data_blocks_count == 0 ||
            this->dnb.bitmap[__num] == true)
            throw std::logic_error(
                "该数据块已被分配或已经没有空闲数据块，分配数据块失败。");

        this->dnb.bitmap[__num] = true;
        this->dnb.free_data_blocks_count--;
    }

    void deallocate_specified_data_block(unsigned short __num) {
        if (this->dnb.bitmap[__num] == false)
            throw std::logic_error("该数据块未被分配，归还该数据块失败。");

        this->dnb.bitmap[__num] = false;
        this->dnb.free_data_blocks_count++;
    }
};

#endif  // __VFS_H_