/*
 * @Author: ChenY
 * @Date: 2020-12-15 16:33:15
 * @LastEditTime: 2020-12-16 17:27:49
 * @FilePath: /Operating-System-class-design/include/shell.h
 */
#ifndef _SHELL_H_
#define _SHELL_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "bfs.h"
#include "utils.h"
#include "vfs.h"

class shell {
   private:
    std::string title_;
    std::string cmd_;
    std::string cmdContent_;
    struct tm* time_;
    std::vector<std::string> allFile_;
    Virtual_File_System* vfs_;

   public:
    shell();
    shell(const shell&) = delete;
    shell& operator=(const shell&) = delete;
    ~shell();
    void cmdParser();                     //命令解析器
    void cmdHelper(std::string tempStr);  // 命令帮助
    void touchFile(std::string tempStr);  // 创建文件
    void rmFile(std::string tempStr);     // 删除文件
    void mvFile(std::string tempStr);     // 修改文件名称
    void catFile(std::string tempStr);    // 查看文件内容
    void viFile(std::string tempStr);     // 修改文件内容
    void lsFile(std::string tempStr);     // 显示文件或者文件信息
};

#endif