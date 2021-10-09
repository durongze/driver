#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#define PRINTK_PATH "/proc/sys/kernel/"
#define PRINTK_FILE PRINTK_PATH "printk"

#ifdef WIN32
#include <io.h>
#include <direct.h> 
#else
#include <unistd.h>
#include <sys/stat.h>
#endif
#include <stdint.h>
#include <string>
#define MAX_PATH_LEN 256

#ifdef WIN32
#define ACCESS(fileName,accessMode) _access(fileName,accessMode)
#define MKDIR(path) _mkdir(path)
#else
#define ACCESS(fileName,accessMode) access(fileName,accessMode)
#define MKDIR(path) mkdir(path,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#endif

int32_t create_dir(const std::string &dir_path)
{
    uint32_t dirPathLen = dir_path.length();
    if (dirPathLen > MAX_PATH_LEN)
    {
        return -1;
    }
    char tmpDirPath[MAX_PATH_LEN] = { 0 };
    for (uint32_t i = 0; i < dirPathLen; ++i)
    {
        tmpDirPath[i] = dir_path[i];
        if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/')
        {
            if (ACCESS(tmpDirPath, 0) != 0)
            {
                int32_t ret = MKDIR(tmpDirPath);
                if (ret != 0)
                {
                    return ret;
                }
            }
        }
    }
    return 0;
}

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

int write_printk_file(const std::string &file)
{
    std::string line;
    std::fstream fs(file.c_str(), std::ios::out);

    create_dir(PRINTK_PATH);

    fs << "7	4	1	7" << std::endl;

    return 0;
}

int main(int argc, char **argv)
{
    std::vector<std::string>::const_iterator iter;
    std::vector<std::string> files;

    files.push_back(PRINTK_FILE);
    
    write_printk_file(PRINTK_FILE);

	while(1) {
        for (iter = files.cbegin(); iter != files.cend(); ++iter) {
            show_file(*iter);
        }
		sleep(1);
	}
	return 0;
}
