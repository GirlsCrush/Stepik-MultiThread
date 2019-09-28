#include <string>
#include <iostream>

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

#define KEYS_AMOUNT 20
#define KEYS_SIZE 20

static std::string cmd;
char* keys[20];
static size_t currentPos;
static std::string buf;

void initKeys() {
            
}

void clearKeys() {
    for (int i = 0; i < currentPos; ++i) {
        delete[] keys[i];
    } 
    currentPos = 0;
    for (int i = 0; i < 20; ++i) {
        keys[i] = NULL;
    }
}

// Returns if there is any space left for the keys.
bool addKey(const std::string &s) {
    keys[currentPos] = new char[s.size() + 1];
    strcpy(keys[currentPos++], s.c_str());
    return currentPos == KEYS_AMOUNT;
}

void getCmd() {
    clearKeys();
    std::smatch result;
    std::smatch resultKeys;
    std::regex reg("([\\w]+)[ ]?([- \\w\\d]*)");
    std::regex regKeys("[-]?([\\w\\d]+)");
    std::regex_search(buf, result, reg);
   
    cmd = result[1].str();
    std::string keysStr = result[2].str();
    buf = result.suffix().str();
    addKey(cmd);
    
    while (std::regex_search(keysStr, resultKeys, regKeys)) {
        addKey(resultKeys[1].str());
        keysStr = resultKeys.suffix().str();
    }
}

int main() {

    std::getline(std::cin, buf);
    //std::cout << "buf = " << buf << std::endl;
    int pid;
    int pfd[2];
    while (buf != "") {
        std::cout << buf << std::endl;
        pipe(pfd);
        getCmd();
        if (pid = fork()) {
            
            char* keys[20];
            for (int i = 0; i < 20; ++i) {
                keys[i] = NULL;
            }
            for (int i = 0; i < currentPos; ++i) {
                keys[i] = new char[::keys[i].size() + 1];
                strcpy(keys[i], ::keys[i].c_str());
                std::cout << "keys[i] = " << keys[i] << std::endl;
            }
			close(STDOUT_FILENO);
        	dup2(pfd[1], STDOUT_FILENO);
        	close(pfd[1]);
        	close(pfd[0]);
            // sleep(1);
        	execvp(cmd.c_str(), keys);
            for (int i = 0; i < currentPos; ++i) {
                delete[] keys[i];
            }
		} else {
            sleep(1);
			close(STDIN_FILENO);
        	dup2(pfd[0], STDIN_FILENO);
        	close(pfd[1]);
        	close(pfd[0]);
            // sleep(20);
            // sleep(1);
            //return 0;
		}
    }
    char buffer[1048576];
    memset((void*)buffer, '\0', 1048576);
    fgets(buffer, 1048576, stdin);
    // std::string buffer;
    // std::cin >> buffer;
    std::cout << "RESULT = " << buffer << std::endl;
    // std::ofstream ofs("/home/box/result.out");
    std::ofstream ofs("result.out");
    ofs << buffer;
    ofs.close();
    return 0;
}