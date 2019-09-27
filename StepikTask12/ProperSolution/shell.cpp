#include <string>
//

#include <iostream>

//
#include <fstream>

//
#include <regex>

//
#include <algorithm>

//


//
// #include <stdio.h>

//
#include <unistd.h>

//
#include <signal.h>

//
#include <string.h>



// void getCmd() {
//     keys.clear();
//     std::smatch result;
//     std::smatch resultKeys;
//     std::regex reg("([\\w]+)[ ]?([- \\w\\d]*)");
//     std::regex regKeys("[-]?([\\w\\d]+)");
//     std::regex_search(buf, result, reg);
    
//     // std::cout << "reslt[0] = " << result[0].str() << std::endl;
//     // std::cout << "prefix = " << result.prefix() << std::endl;
//     // std::cout << "suffix = " << result.suffix() << std::endl;
//     // std::cout << "length = " << result.length() << std::endl;
//     // std::cout << result[0] << ' ' << result[1] << ' ' << result[2] << ' ';
   
//     cmd = result[1].str();
//     std::string keysStr = result[2].str();
//     buf = result.suffix().str();
//     keys.push_back(cmd);
    

//     // cmd.erase(std::remove(cmd.begin(), cmd.end(), ' '), cmd.end());

//     // std::cout << "cmd length = " << cmd.length() << std::endl;
//     // std::cout << "keyStr = " << keysStr << std::endl;
//     // std::cout << "cmd = " << cmd << std::endl;
//     // std::cout << "buf = " << buf << std::endl;
//     // std::vector<std::string> keys;
    
//     while (std::regex_search(keysStr, resultKeys, regKeys)) {
        
//         // std::cout << std::endl << "hello" << ' ';
//         // std::cout << "result[0] = " << resultKeys[0].str() << std::endl;
//         // std::cout << "result[1] = " << resultKeys[1].str() << std::endl;
//         // std::cout << std::endl << "reulst.size() = " << resultKeys.size() << "hello";
//         // std::cout << std::endl << "hello" << ' ';
//         keys.emplace_back(result[1].str().c_str());
//         // std::cout << "keys[0] = " << keys[0] << std::endl;
//         keysStr = resultKeys.suffix().str();
//         // if (keysStr == "")
//         //     break;
//         // std::cout << "keysStr = " << '1' << keysStr << '1' << std::endl;
//     }
//     // std::cout << "Keys are : ";
//     // for (int i = 0; i < keys.size(); ++i)
//     //     std::cout << keys[i] << " ";    
// }

int main() {
    std::string cmd;
    std::vector<std::string> keys;
    std::string buf;

    std::getline(std::cin, buf);
    //std::cout << "buf = " << buf << std::endl;
    int pid;
    while (buf != "") {
        int pfd[2];
        pipe(pfd);keys.clear();
        std::smatch result;
        std::smatch resultKeys;
        std::regex reg("([\\w]+)[ ]?([- \\w\\d]*)");
        std::regex regKeys("[-]?([\\w\\d]+)");
        std::regex_search(buf, result, reg);

        std::cout << "reslt[0] = " << result[0].str() << std::endl;
        std::cout << "prefix = " << result.prefix() << std::endl;
        std::cout << "suffix = " << result.suffix() << std::endl;
        std::cout << "length = " << result.length() << std::endl;
        std::cout << result[0] << ' ' << result[1] << ' ' << result[2] << ' ';
    
        cmd = result[1].str();
        std::string keysStr = result[2].str();
        buf = result.suffix().str();
        keys.push_back(cmd);


        // cmd.erase(std::remove(cmd.begin(), cmd.end(), ' '), cmd.end());

        std::cout << "cmd length = " << cmd.length() << std::endl;
        std::cout << "keyStr = " << keysStr << std::endl;
        std::cout << "cmd = " << cmd << std::endl;
        std::cout << "buf = " << buf << std::endl;
        std::vector<std::string> keys;

        while (std::regex_search(keysStr, resultKeys, regKeys)) {

            // std::cout << std::endl << "hello" << ' ';
            // std::cout << "result[0] = " << resultKeys[0].str() << std::endl;
            std::cout << "result[1] = " << resultKeys[1].str() << std::endl;
            // std::cout << std::endl << "reulst.size() = " << resultKeys.size() << "hello";
            // std::cout << std::endl << "hello" << ' ';
            keys.emplace_back(result[1].str().c_str());
            // std::cout << "keys[0] = " << keys[0] << std::endl;
            keysStr = resultKeys.suffix().str();
            // if (keysStr == "")
            //     break;
            // std::cout << "keysStr = " << '1' << keysStr << '1' << std::endl;
        }
        if (pid = fork()) {
            char** args = new char* [keys.size() + 1];
            for (int i = 0; i < keys.size(); ++i) {
                strcpy(args[i], keys[i].c_str());
                std::cout << args[i] << std::endl;
            }
            args[keys.size()] = NULL;
			close(STDOUT_FILENO);
        	dup2(pfd[1], STDOUT_FILENO);
        	close(pfd[1]);
        	close(pfd[0]);
        	execvp(cmd.c_str(), args);
            kill(pid, SIGUSR1);
            return 0;
		} else {
            signal(SIGUSR1, [](int i){});
            pause();
			close(STDIN_FILENO);
        	dup2(pfd[0], STDIN_FILENO);
        	close(pfd[1]);
        	close(pfd[0]);
            // sleep(1);
		}
    }
    char buffer[1048576];
    memset((void*)buffer, '\0', 1048576);
    fgets(buffer, 1048576, stdin);
    std::cout << "buffer = " << buffer << std::endl;
    // std::ofstream ofs("/home/box/result.out");
    std::ofstream ofs("result.out");
    ofs << "buffer = " << buffer;
    ofs.close();
    return 0;
}