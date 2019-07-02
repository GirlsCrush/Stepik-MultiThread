#include <iostream>
#include <map>

class SmallAllocator {
private:
	static const size_t MEMORY_SIZE = 1000; //1048576;
	char Memory[MEMORY_SIZE];
	std::map<char*, size_t> used_memory;
public:
	void* Alloc(unsigned int Size) {
		char* offset = Memory;
		for (auto it = used_memory.begin(); it != used_memory.end(); ++it) {
			if (offset + Size <= it->first)
				break;
			offset = it->first + it->second;
		}
		if (offset - Memory > MEMORY_SIZE - Size)
			throw(std::bad_alloc());
		used_memory.emplace(offset, Size);
		return (void*)offset;
	};
	void* ReAlloc(void* Pointer, unsigned int Size) {
		char* Pointer_char = (char*)Pointer;
		if (used_memory.at(Pointer_char) >= Size)
			used_memory[Pointer_char] = Size;
		else {
			std::string prev(Pointer_char, used_memory[Pointer_char]);
			Free(Pointer);
			void* PointerNew = Alloc(Size);
			char* charPointerNew = (char*)PointerNew;
			for (int i = 0; i < prev.size(); ++i)
				charPointerNew[i] = prev[i];
			Pointer = PointerNew;
		}
		return Pointer;
	};
	void Free(void* Pointer) {
		used_memory.erase((char*)Pointer);
	};
};

int main(int argc, char** argv) {
	SmallAllocator A1;
	int* A1_P1 = (int*)A1.Alloc(sizeof(int));
	A1_P1 = (int*)A1.ReAlloc(A1_P1, 2 * sizeof(int));
	A1.Free(A1_P1);
	SmallAllocator A2;
	int* A2_P1 = (int*)A2.Alloc(10 * sizeof(int));
	for (unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
	for (unsigned int i = 0; i < 10; i++) if (A2_P1[i] != i) std::cout << "ERROR 1" << std::endl;
	int* A2_P2 = (int*)A2.Alloc(10 * sizeof(int));
	for (unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
	for (unsigned int i = 0; i < 10; i++) if (A2_P1[i] != i) std::cout << "ERROR 2" << std::endl;
	for (unsigned int i = 0; i < 10; i++) if (A2_P2[i] != -1) std::cout << "ERROR 3" << std::endl;
	A2_P1 = (int*)A2.ReAlloc(A2_P1, 20 * sizeof(int));
	for (unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
	for (unsigned int i = 0; i < 20; i++) if (A2_P1[i] != i) std::cout << "ERROR 4" << std::endl;
	for (unsigned int i = 0; i < 10; i++) if (A2_P2[i] != -1) std::cout << "ERROR 5" << std::endl;
	A2_P1 = (int*)A2.ReAlloc(A2_P1, 5 * sizeof(int));
	for (unsigned int i = 0; i < 5; i++) if (A2_P1[i] != i) std::cout << "ERROR 6" << std::endl;
	for (unsigned int i = 0; i < 10; i++) if (A2_P2[i] != -1) std::cout << "ERROR 7" << std::endl;
	A2.Free(A2_P1);
	A2.Free(A2_P2);
}