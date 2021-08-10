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

int sh2cc(std::string sh_file)
{
	std::string sh_ctx;
	std::fstream sh_script_in(sh_file, std::ios::in);
	std::fstream sh_script_out(sh_file + ".cc", std::ios::out | std::ios::trunc);
	int idxRow = 0;
	sh_script_out << "#include <string> " << std::endl;
	sh_script_out << "#include <fstream> " << std::endl;
	sh_script_out << "#include <sstream> " << std::endl;
	sh_script_out << "std::string sh_script_ctx=\" \\" << std::endl;
	for (std::string line; std::getline(sh_script_in, sh_ctx); ++idxRow) {
		sh_script_out << proc_line(sh_ctx) << "     \\" << std::endl;
	}
	sh_script_out << "\"; " << std::endl;
	sh_script_out << "int gen_shell_script(const char* sh_file) " << std::endl;
	sh_script_out << "{ " << std::endl;
	sh_script_out << "    std::fstream sh_fs(sh_file, std::ios::out | std::ios::trunc);" << std::endl;
	sh_script_out << "    sh_fs << sh_script_ctx; " << std::endl;
	sh_script_out << "    return 0; " << std::endl;
	sh_script_out << "} " << std::endl;
	sh_script_out << "#ifndef ENV_MAIN " << std::endl;
	sh_script_out << "int main(int argc, char** argv)" << std::endl;
	sh_script_out << "{" << std::endl;
	sh_script_out << "    if (argc == 2)" << std::endl;
	sh_script_out << "    gen_shell_script(argv[1]);" << std::endl;
	sh_script_out << "    return 0;" << std::endl;
	sh_script_out << "}" << std::endl;
	sh_script_out << "#endif " << std::endl;
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
