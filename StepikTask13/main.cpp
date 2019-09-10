#include <stdio.h>
#include <unistd.h>
#include <regex>
#include <string>
#include <vector>
#include <iostream>


int main(int argc, char** argv) {
	char c_command[1024];
	fgets(c_command, 1024, stdin);
	std::stringstream ss(c_command);
	std::vector<const char *> cmd;
	std::string tmp;
    int pfd[2];
    pipe(pfd);
	
	do {
		ss >> tmp;
		if (tmp != "|" && !ss.eof()) {
			cmd.push_back(tmp.c_str());
		} else {
			pipe(pfd);
			cmd.push_back(NULL);
			if (fork()) {
				close(STDOUT_FILENO);
        		dup2(pfd[1], STDOUT_FILENO);
        		close(pfd[1]);
        		close(pfd[0]);
        		execvp(cmd[0], const_cast<char * const*>(&cmd[0]));
			} else {
				close(STDIN_FILENO);
        		dup2(pfd[0], STDIN_FILENO);
        		close(pfd[1]);
        		close(pfd[0]);
			}
		}
	} while (!ss.eof());

    	// FILE* stream = popen(result[0].str().c_str(), "r");
		// printf(fgets(cres, ))
	char res_str[1024];
	fgets(res_str, 1024, stdin);
	printf(res_str);

	return 0;
}
