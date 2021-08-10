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

const std::string gen_cmd()
{
    std::string cmd;
    cmd += "dmake";
    cmd += " -h ";
    return cmd;
}

std::string to_str(struct tm* tm_) 
{
	std::ostringstream oss;
	oss << tm_->tm_hour << ":" << tm_->tm_min << ":" << tm_->tm_sec;
	return oss.str();
}

int deamon_main(std::ofstream &fout, const char *my_sh, std::string &run_tm, std::string &cur_tm)
{
    // The big loop
	gen_shell_script(my_sh);
    while (true)
    {
        cur_tm = to_str(get_cur_time());
		fout << "wait:" << run_tm << " vs " << cur_tm << endl;        
        if (run_tm < cur_tm) {
            fout << gen_cmd().c_str() << endl;
            system(gen_cmd().c_str());
            std::remove(my_sh);
            break;
        }
        sleep(4);
    }
	fout << "over:" << run_tm << " vs " << cur_tm << endl;        
    return 0;
}

int main(int argc, char **argv)
{
    const char *workdir="./";
    const char *logfile="/tmp/daemon.log";
	
	std::string my_sh = "/tmp/mysh";
    
	std::string run_tm = to_str(get_cur_time());
    std::string cur_tm = to_str(get_cur_time());

    // Open any logs here
    ofstream fout(logfile, std::ios::trunc);
    if (!fout)
    {
        std::cout << logfile << std::endl;
        exit(-1);
    }
    
    deamon_start(workdir, fout);
	deamon_main(fout, workdir, run_tm, cur_tm);
	return 0;
}

