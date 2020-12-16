<!--
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-11 21:43:41
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-15 15:43:42
 * @FilePath: /Operating-System-class-design/README.md
-->

# 操作系统课程设计

---

## 目录

* [课程设计主题思想](#课程设计主题思想)
* [课程设计基本信息](#课程设计基本信息)
* [快速开始](#快速开始)
  * [运行程序](#运行程序)
    * [Cmake](#Cmake)
    * [g++](#g++)
  * [数据结构](#数据结构)
    * [磁盘内数据结构](#磁盘内数据结构)
    * [文件数据结构](#文件数据结构)
  * [程序结构]

---

## 课程设计主题思想

## 课程设计基本信息

课程设计创建日期：2020/12/12 星期六
组长：杨柳青
组员：陈勇、雷银吉、张鹏振、// todo
指导老师：刘丽萍

---

## 快速开始

快速在本地编译项目以及了解程序结构。

### 运行程序

快速是用Cmake或g++将代码编译成能在本地运行的可执行文件。

#### Cmake

使用Cmake进行跨平台编译，所需Cmake最低版本3.16.0
'''Cmake
mkdir build
cd build
cmake ..
make -j4
'''

#### g++

使用GNU g++对项目进行编译。
'''
g++ ./src/bfs.cpp ./src/main.cpp ./src/vfs.cpp -std=c++11 -Wall -g -o [${executable_file_name}]
'''
