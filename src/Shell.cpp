/*
 * @Author: ChenY
 * @Date: 2020-12-15 17:37:24
 * @LastEditTime: 2020-12-17 15:34:23
 * @FilePath: /Operating-System-class-design/src/Shell.cpp
 */
#include "../include/Shell.h"

Shell::Shell() {
    // 系统用户终端显示界面
    title_ = "[Administrator] <> ";
    // 创建文件系统，命名为“homework.txt”
    std::string disk = "../disk/disk.txt";
    vfs_ = new Virtual_File_System();
    vfs_->associateWithDisk(disk);
}

Shell::~Shell() { delete vfs_; }

void Shell::cmdParser() {
    char cmd[50];
    char ch;
    while (true) {
        allFile_.clear();
        allFile_ = vfs_->getAllFileName(allFile_);
        // note 更简易的写法： cmd[50] = {0};
        for (char &i : cmd) {
            i = '\0';
        }

        std::cout << title_;
        ch = (char) getchar();
        if (ch == '\n') continue;
        std::cin.getline(cmd, 50);
        cmd_.assign(cmd);
        cmd_ = ch + cmd_;
        // std::cout << cmd_ << std::endl;
        if (cmd_.substr(0, 4) == "exit" && cmd_.size() == 4) {
            // std::cout << "exit" << std::endl;
            break;
        } else if (cmd_.substr(0, 2) == "ls") {
            std::cout << "调用ls" << std::endl;
            lsFile(cmd_);
        } else if (cmd_.substr(0, 5) == "touch") {
            // std::cout << "调用touch" << std::endl;
            touchFile(cmd_);
        } else if (cmd_.substr(0, 2) == "vi") {
            std::cout << "调用vi" << std::endl;
            viFile(cmd_);
        } else if (cmd_.substr(0, 2) == "rm") {
            // std::cout << "调用rm" << std::endl;
            rmFile(cmd_);
        } else if (cmd_.substr(0, 2) == "mv") {
            mvFile(cmd_);
        } else if (cmd_.substr(0, 3) == "man") {
            // std::cout << "调用man" << std::endl;
            cmdHelper(cmd_);
        } else if (cmd_.substr(0, 3) == "cat") {
            catFile(cmd_);
        } else {
            std::cout << "命令错误！！！" << std::endl;
            continue;
        }
    }
}

void Shell::cmdHelper(const std::string &tempStr) {
    size_t flag = tempStr.find(' ');
    if (flag >= tempStr.size()) {
        std::cout << "命令格式错误，请通过man man查看" << std::endl;
        return;
    }
    cmdContent_ = tempStr.substr(flag + 1, tempStr.size());
    flag = cmdContent_.find(' ');
    if (flag < tempStr.size()) {
        std::cout << "命令格式错误，请通过man man查看" << std::endl;
        return;
    }
    if (cmdContent_ == "man")
        std::cout << "man cmd\n查看命令操作" << std::endl;
    else if (cmdContent_ == "ls")
        std::cout << "ls [-l] [filaname]\n显示文件属性"
                  << "\nls [-a]\n显示所有文件" << std::endl;
    else if (cmdContent_ == "cat")
        std::cout << "cat filename\n查看文件内容" << std::endl;
    else if (cmdContent_ == "touch")
        std::cout << "touch filename\n创建文件" << std::endl;
    else if (cmdContent_ == "vi")
        std::cout << "vi filename\n创建并且修改文件内容" << std::endl;
    else if (cmdContent_ == "mv")
        std::cout << "mv oldFileName newFileName\n重命名文件" << std::endl;
    else if (cmdContent_ == "rm")
        std::cout << "rm filename\n删除文件" << std::endl;
    else if (cmdContent_ == "exit")
        std::cout << "exit\n退出系统" << std::endl;
    else
        std::cout << "没有这个命令" << std::endl;
}

void Shell::touchFile(const std::string &tempStr) {
    size_t flag = tempStr.find_first_of(' ');
    if (flag >= tempStr.size()) {
        std::cout << "命令格式错误，请通过man touch查看" << std::endl;
        return;
    }
    cmdContent_ = tempStr.substr(flag + 1, tempStr.size());
    size_t pos = cmdContent_.find(' ');
    // std::cout << cmdContent_ << std::endl;
    if (pos < cmdContent_.size()) {
        std::cout << "命令格式错误，请通过man touch查看" << std::endl;
        return;
    }
    for (auto &i : allFile_) {
        if (cmdContent_ == i) {
            std::cout << "已经存在同名的文件" << std::endl;
            return;
        }
    }
    vfs_->createFileByFileName(cmdContent_);
}

void Shell::rmFile(const std::string &tempStr) {
    size_t flag = tempStr.find(' ');
    if (flag >= tempStr.size()) {
        std::cout << "命令格式错误，请通过man rm查看" << std::endl;
        return;
    }
    cmdContent_ = tempStr.substr(flag + 1, tempStr.size());
    flag = cmdContent_.find(' ');
    if (flag < tempStr.size()) {
        std::cout << "命令格式错误，请通过man rm查看" << std::endl;
        return;
    }
    for (auto &i : allFile_) {
        if (cmdContent_ == i) {
            vfs_->removeFileByFileName(cmdContent_);
            return;
        }
    }
    std::cout << "不存在这个文件" << std::endl;
}

void Shell::mvFile(const std::string &tempStr) {
    size_t flag = tempStr.find(' ');
    if (flag >= tempStr.size()) {
        std::cout << "命令格式错误，请通过man mv查看" << std::endl;
        return;
    }
    cmdContent_ = tempStr.substr(flag + 1, tempStr.size());
    flag = cmdContent_.find(' ');
    if (flag >= cmdContent_.size()) {
        std::cout << "命令格式错误，请通过man mv查看" << std::endl;
        return;
    }
    std::string oldFileName = cmdContent_.substr(0, flag);
    cmdContent_ = cmdContent_.substr(flag + 1, tempStr.size());
    flag = cmdContent_.find(' ');
    if (flag < cmdContent_.size()) {
        std::cout << "命令格式错误，请通过man mv查看" << std::endl;
        return;
    }
    std::string newFileName = cmdContent_;
    // 没有相关的APi，暂停
}

void Shell::catFile(const std::string &tempStr) {
    size_t flag = tempStr.find(' ');
    if (flag >= tempStr.size()) {
        std::cout << "命令格式错误，请通过man cat查看" << std::endl;
        return;
    }
    cmdContent_ = tempStr.substr(flag + 1, tempStr.size());
    flag = cmdContent_.find(' ');
    if (flag < cmdContent_.size()) {
        std::cout << "命令格式错误，请通过man cat查看" << std::endl;
        return;
    }
    for (auto &i : allFile_) {
        if (cmdContent_ == i) {
            vfs_->openFile(cmdContent_);
            std::string fileContent_ =
                    vfs_->getFileContentByFileName(cmdContent_);
            std::cout << fileContent_ << std::endl;
            vfs_->closeFile(cmdContent_);
            return;
        }
    }
    std::cout << "不存在这个文件" << std::endl;
}

void Shell::viFile(const std::string &tempStr) {
    int site_;
    char insertContent_[50];
    size_t flag = tempStr.find(' ');
    if (flag >= tempStr.size()) {
        std::cout << "命令格式错误，请通过man cat查看" << std::endl;
        return;
    }
    cmdContent_ = tempStr.substr(flag + 1, tempStr.size());
    flag = cmdContent_.find(' ');
    if (flag < cmdContent_.size()) {
        std::cout << "命令格式错误，请通过man cat查看" << std::endl;
        return;
    }
    for (auto &i : allFile_)
        if (cmdContent_ == i) {
            vfs_->openFile(cmdContent_);
            std::string fileContent_ =
                    vfs_->getFileContentByFileName(cmdContent_);
            while (true) {
                std::cout << "文件内容如下：\n" << fileContent_ << std::endl;
                std::cout << "i(插入);w(写入文件);q(退出);d(删除):"
                          << std::endl;
                char ch = (char) getchar();
                std::cin.get();
                for (char &content : insertContent_) content = '\0';
                switch (ch) {
                    case 'i':
                        std::cout << "请输入插入位置：";
                        (std::cin >> site_).get();
                        if (site_ < 0 || site_ > fileContent_.size()) {
                            std::cout << "插入位置错误" << std::endl;
                            break;
                        }
                        std::cout << "请输入插入内容：";
                        std::cin.getline(insertContent_, 50);
                        fileContent_ =
                                fileContent_.substr(0, site_) + insertContent_ +
                                fileContent_.substr(site_, fileContent_.size());
                        break;
                    case 'w':
                        vfs_->updateFileContentByFileName(cmdContent_,
                                                          fileContent_);
                        break;
                    case 'q':
                        vfs_->closeFile(cmdContent_);
                        return;
                    case 'd':
                        std::cout << "请输入删除位置：";
                        std::cin >> site_;
                        if (site_ < 0 || site_ > fileContent_.size()) {
                            std::cout << "删除位置错误" << std::endl;
                            break;
                        }
                        std::cout << "请输入删除内容：";
                        size_t quantity;
                        std::cin >> quantity;
                        if (quantity == 0)
                            break;
                        else if (quantity > site_) {
                            fileContent_ =
                                    fileContent_.substr(site_, fileContent_.size());
                            break;
                        } else {
                            fileContent_ =
                                    fileContent_.substr(0, site_ - quantity) +
                                    fileContent_.substr(site_, fileContent_.size());
                            break;
                        }
                    default:
                        std::cout << "输入错误" << std::endl;
                }
            }
        }
    std::cout << "不存在这个文件" << std::endl;
}

void Shell::lsFile(const std::string &tempStr) {
    if (allFile_.empty()) {
        std::cout << "无文件" << std::endl;
        return;
    }
    if (tempStr == "ls -a" || tempStr == "ls") {
        std::cout << "调用ls -a" << std::endl;
        // fixme 一行打印5个数据有误
        /* for (int i = 0; i < allFile_.size(); i++) {
            std::cout << allFile_[i] << "   ";
            if (i % 5 == 0) std::cout << '\n';
        }
        if (allFile_.size() % 5 == 0) std::cout << '\n'; */

        int count = 0;
        auto size = allFile_.size();
        for (size_t i = 0; i < size; i++) {
            if (++count / 5 == 0) {
                std::cout << allFile_[i] << "\t";
            } else {
                std::cout << allFile_[i] << std::endl;
                count = 0;
            }
        }
        if (count != 0) {
            std::cout << '\n';
        }
        return;
    } else if (tempStr == "ls -l") {
        std::cout << "调用ls -l" << std::endl;
        for (auto &i : allFile_) {
            struct Index_node file =
                    vfs_->getFileAttributeByFileName(i);
            time_ = localtime(&file.last_modified_time);
            std::cout << "文件名： " << i << std::endl;
            if (0 != file.last_modified_time) {
                std::cout << "最后修改时间： " << asctime(time_);
            } else {
                std::cout << "该文件从未被修改过。" << std::endl;
            }
            std::cout << "文件大小： " << file.filesize << '\n' << std::endl;
            // printf("%-10d %-46s", file.filesize, allFile_[i].c_str());
        }
        return;
    } else {
        size_t flag = tempStr.find_first_of(' ');
        if (flag >= tempStr.size()) {
            std::cout << "命令格式错误，请通过man ls查看" << std::endl;
            return;
        }
        cmdContent_ = tempStr.substr(flag + 1, tempStr.size());
        if (cmdContent_.find(' ') < cmdContent_.size()) {
            std::cout << "命令格式错误，请通过man ls查看" << std::endl;
            return;
        }
        for (auto &i : allFile_)
            if (cmdContent_ == i) {
                std::cout << cmdContent_ << std::endl;
                return;
            }
    }
    std::cout << "不存在这个文件" << std::endl;
}