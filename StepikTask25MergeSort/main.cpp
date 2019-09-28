#include <stdio.h>
#include <omp.h>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>

#define THREADS_AMOUNT 4

int main() {
    std::vector<int> v;
    std::ifstream fin("input.txt", std::fstream::in);
    while(!fin.eof()) {
        int tmp;
        fin >> tmp;
        v.push_back(tmp);
    }

    std::vector<std::vector<int>::iterator> ends;

    size_t blockSize = v.size() / THREADS_AMOUNT;
    for (int i = 0; i < THREADS_AMOUNT; ++i) {
        ends.push_back(v.begin() + i * blockSize);
    }
    ends.push_back(v.end());

    #pragma omp parallel for
    for (int i = 0; i < ends.size() - 1 ; ++i) {
            std::sort(ends[i], ends[i + 1]);
    }
    while (ends.size() > 2) {
        #pragma omp parallel for
        for (int i = 0; i < ends.size() - 2 ; i += 2) {
            std::vector<int> res;
            std::vector<int>::iterator it1 = ends[i];
            std::vector<int>::iterator it2 = ends[i + 1];
            for (int j = 0; it1 != ends[i + 1] && it2 != ends[i + 2]; j++) {
                if (*it1 < *it2) {
                    res.push_back(*it1);
                    ++it1;
                } else {
                    res.push_back(*it2);
                    ++it2;
                }
            }
            if (it1 == ends[i + 1])
                for(; it2 != ends[i + 2]; ++it2)
                    res.push_back(*it2);
            if (it2 == ends[i + 2])
                for(; it1 != ends[i + 1]; ++it1)
                    res.push_back(*it1);

            for (int j = 0; j < res.size(); ++j) {
                *(ends[i] + j) = res[j];
            }
            ends.erase(ends.begin() + i + 1);
        }    
    }
    std::ofstream fout("output.txt", std::fstream::out);
    for (int i = 0; i < v.size() - 1; ++i) {
        fout << v[i] << ' ';
    }
    fout << v.back();
    return 0;
}
