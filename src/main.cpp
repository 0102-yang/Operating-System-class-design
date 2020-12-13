/*
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-12 17:23:03
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-13 17:02:09
 * @FilePath: /Operating-System-class-design/src/main.cpp
 */
#include <iostream>
#include <map>
#include <string>

#include "../include/disk.h"
#include "../include/utils.h"

int main(int argc, char const* argv[]) {
    Disk d;
    d.mount("../data/disk.txt");
    d.df_seek(d.get_super_block()->inmap_start);
    Index_node_bitmap i;
    d.df_read(&i, sizeof(Index_node_bitmap));

    return 0;
}