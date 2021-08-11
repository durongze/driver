#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
// #include <dlfcn.h>

using namespace std;

extern int gen_shell_script(const char* sh_file);

int deamon_start(const char *workdir, std::ofstream& fout)
{
    pid_t pid, sid;
    
    // Fork off the parent process
    pid = fork();
    if (pid < 0)
    {
        fout << "pid " << pid << endl;
        exit(-1);
    }

    // If we got a good PID, then we can exit the parent process.
    if (pid > 0)
    {
        fout << "parent pid " << pid << endl;
        exit(-1);
    }
    
    fout << "child pid " << pid << endl;

    // Change the file mode mask
    umask(0);

    // Create a new SID(Session ID) for the child process
    sid = setsid();
    if (sid < 0)
    {
        // Log any failure
        fout << "sid " << sid << endl;
        exit(-1);
    }

    // Change the current working directory
    if ((chdir(workdir)) < 0)
    {
        // Log any failure
        fout << "workdir " << workdir << endl;
        exit(-1);
    }

    // Close out the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    return 0;
}

struct tm* get_cur_time()
{
    time_t timeNow;
    time(&timeNow);
    return localtime(&timeNow);
}

struct tm sub_time(struct tm &run_time, struct tm &cur_time)
{
    struct tm ret;
    ret.tm_sec = cur_time.tm_sec - run_time.tm_sec;
    if (ret.tm_sec < 0) {
        ret.tm_sec += 60;
        cur_time.tm_min -= 1;
    }
    ret.tm_min = cur_time.tm_min - run_time.tm_min;
    if (ret.tm_min < 0) {
        ret.tm_min += 60;
        cur_time.tm_hour -= 1;
    }

    ret.tm_hour = cur_time.tm_hour - run_time.tm_hour;
    ret.tm_hour += ret.tm_hour < 0 ? 24 : 0;
    return ret;
}

int tm_to_sec(struct tm &t)
{
    int sec = 0;
    sec += t.tm_hour * 60 * 60;
    sec += t.tm_min * 60;
    sec += t.tm_sec;
    return sec;
}

const std::string gen_cmd(const std::string &args)
{
    std::string cmd;
    cmd += "rm";
    cmd += " -f ";
    cmd += args;
    std::string ps1;
    do {
        sleep(1);
        ps1 = getenv("PS1");
    }while(ps1.find("\\n\\w\\n") < 0);
    return cmd;
}

std::string to_str(const struct tm &tm_) 
{
    std::ostringstream oss;
    oss << tm_.tm_hour << ":" << tm_.tm_min << ":" << tm_.tm_sec;
    return oss.str();
}

int deamon_main(std::ofstream &fout, const char *my_sh, struct tm &run_tm, struct tm &cur_tm)
{
    // The big loop
    gen_shell_script(my_sh);
    fout << "mysh:" << my_sh << endl;        
    while (true)
    {
        cur_tm = *get_cur_time();
        struct tm diff = sub_time(run_tm, cur_tm);
        fout << to_str(run_tm) << " vs " << to_str(cur_tm) << " " << to_str(diff) << endl;        
        if (tm_to_sec(diff) > 0)
        {
            fout << gen_cmd(my_sh).c_str() << endl;
            system(gen_cmd(my_sh).c_str());
            break;
        } else {
            sleep(4);
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    const char *workdir="./";
    const char *logfile="/tmp/daemon.log";
    
    std::string my_sh = "/tmp/mysh";
    
    struct tm run_tm = (*get_cur_time());
    struct tm cur_tm = (*get_cur_time());

    // Open any logs here
    ofstream fout(logfile, std::ios::trunc);
    if (!fout)
    {
        std::cout << logfile << std::endl;
        exit(-1);
    }
    
    deamon_start(workdir, fout);
    deamon_main(fout, my_sh.c_str(), run_tm, cur_tm);
    return 0;
}

