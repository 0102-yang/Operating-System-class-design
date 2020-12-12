/*
 * @Author: Yang
 * @Copyright: Yang
 * @Date: 2020-12-12 17:23:03
 * @LastEditors: Yang
 * @LastEditTime: 2020-12-12 22:14:53
 * @FilePath: /Operating-System-class-design/src/main.cpp
 */
#include <map>
#include <string>

#include "../include/disk.h"
#include "../include/utils.h"

int main(int argc, char const* argv[]) {
    Disk d;
    std::map<std::string, unsigned> test;
    test.insert({"Hello.txt", 9});
    test.insert({"Hell.txt", 9});

    Index_block ib[10];
    d.open_disk("../data/disk.txt");
    d.mount(test, ib);

    d.unmount(test, ib);
    d.mount(test, ib);

    for (auto i : test) {
        printf("%s %u", i.first.c_str(), i.second);
    }

    return 0;
}