#include <iostream>
#include <fstream>
#include <sstream>

std::string proc_line(std::string &sh_ctx)
{

    for (auto iter = sh_ctx.begin(); iter != sh_ctx.end(); ++iter) {
        if (*iter == '"' || *iter == '\\') {
            iter = sh_ctx.insert(iter, '\\');
            ++iter;
        }
    }
    return sh_ctx + "\\n";
}

std::string gen_cc_header()
{
    std::ostringstream oss;
    oss << "#include <string> " << std::endl;
    oss << "#include <fstream> " << std::endl;
    oss << "#include <sstream> " << std::endl;
    return oss.str();
}

std::string gen_cc_var_by_script(const std::string &sh_file)
{
    std::ostringstream oss;
    std::string sh_ctx;
    std::fstream sh_script_in(sh_file, std::ios::in);
    oss << "std::string sh_script_ctx=\"\\" << std::endl;
    for (std::string line; std::getline(sh_script_in, sh_ctx);) {
        oss << proc_line(sh_ctx) << "\\" << std::endl;
    }
    oss << "\";" << std::endl;
    return oss.str();
}

std::string gen_cc_func_lib()
{
    std::ostringstream oss;
    oss << "int gen_shell_script(const char* sh_file) " << std::endl;
    oss << "{ " << std::endl;
    oss << "    {" << std::endl;
    oss << "        std::fstream sh_fs(sh_file, std::ios::out | std::ios::trunc);" << std::endl;
    oss << "        sh_fs << sh_script_ctx; sh_fs << std::endl;" << std::endl;
    oss << "    }" << std::endl;
    oss << "    std::string cmd = \"chmod +x \";" << std::endl;
    oss << "    cmd += sh_file;" << std::endl;
    oss << "    system(cmd.c_str());" << std::endl;
    oss << "    return 0;" << std::endl;
    oss << "} " << std::endl;
    return oss.str();
}

std::string gen_cc_func_main()
{
    std::ostringstream oss;
    oss << "#ifndef ENV_MAIN " << std::endl;
    oss << "int main(int argc, char** argv)" << std::endl;
    oss << "{" << std::endl;
    oss << "    if (argc == 2)" << std::endl;
    oss << "    gen_shell_script(argv[1]);" << std::endl;
    oss << "    return 0;" << std::endl;
    oss << "}" << std::endl;
    oss << "#endif " << std::endl;
    return oss.str();
}

int sh2cc(std::string sh_file)
{
    std::fstream sh_script_out(sh_file + ".cc", std::ios::out | std::ios::trunc);
    sh_script_out << gen_cc_header();
    sh_script_out << gen_cc_var_by_script(sh_file);
    sh_script_out << gen_cc_func_lib();
    sh_script_out << gen_cc_func_main();
    return 0;
}

int main(int argc, char** argv)
{
    if (argc == 2) {
        sh2cc(argv[1]);
    } else {
        std::cout << argv[0] << " <shell script>" << std::endl;
    }
    return 0;
}
