#include <iostream>
#include <string>

class StringPointer {
	std::string* sP;
	bool selfConstructed = false;
public:
	std::string* operator->() {
		if (sP == NULL) {
			sP = new std::string();
			selfConstructed = true;
		}
		return sP;
	}
	operator std::string* () {
		if (sP == NULL) {
			sP = new std::string();
			selfConstructed = true;
		}
		return sP;
	}
	StringPointer(std::string* Pointer) : sP(Pointer) {}
	~StringPointer() {
		if (selfConstructed)
			delete sP;
	}
};

int main(int argc, char** argv) {
	std::string s1 = "Hello, world!";

	StringPointer sp1(&s1);
	StringPointer sp2(NULL);

	std::cout << sp1->length() << std::endl;
	std::cout << *sp1 << std::endl;
	std::cout << sp2->length() << std::endl;
	std::cout << *sp2 << std::endl;
}