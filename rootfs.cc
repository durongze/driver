#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#define PRINTK_FILE "/proc/sys/kernel/printk"

int show_file(const std::string &file)
{
    std::string line;
    std::fstream fs(file.c_str(), std::ios::in);
    
    std::cout << "\033[31m file:" << file << "\033[0m" << std::endl;
    for (int idx = 0; std::getline(fs, line); idx++) {
        std::cout << "\033[32m line[" << idx << "]:\033[0m" << line << std::endl;
    }

    return 0;
}

int main(int argc, char **argv)
{
    std::vector<std::string>::const_iterator iter;
    std::vector<std::string> files;
    files.push_back(PRINTK_FILE);
	while(1) {
        for (iter = files.cbegin(); iter != files.cend(); ++iter) {
            show_file(*iter);
        }
		sleep(1);
	}
	return 0;
}
