/*
 * @Author: ChenY
 * @Date: 2020-12-15 16:33:15
 * @LastEditTime: 2020-12-16 17:27:49
 * @FilePath: /Operating-System-class-design/include/Shell.h
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

class Shell {
private:
    std::string title_;
    std::string cmd_;
    std::string cmdContent_;
    struct tm *time_{};
    std::vector<std::string> allFile_;
    Virtual_File_System *vfs_;

public:
    Shell();

    Shell(const Shell &) = delete;

    Shell &operator=(const Shell &) = delete;

    ~Shell();

    void cmdParser();                     //命令解析器
    void cmdHelper(const std::string &tempStr);  // 命令帮助
    void touchFile(const std::string &tempStr);  // 创建文件
    void rmFile(const std::string &tempStr);     // 删除文件
    void mvFile(const std::string &tempStr);     // 修改文件名称
    void catFile(const std::string &tempStr);    // 查看文件内容
    void viFile(const std::string &tempStr);     // 修改文件内容
    void lsFile(const std::string &tempStr);     // 显示文件或者文件信息
};

#endif