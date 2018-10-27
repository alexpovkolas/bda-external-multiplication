#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>


//#define __PROFILE__

#ifdef __PROFILE__

#include <algorithm>
#include <chrono>

#endif

using namespace std;

int main() {

#ifdef __PROFILE__
    //    gen_test(100000, 100);
    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#endif

    ifstream in("input.bin", ios::binary);
    ofstream out("output.bin", ios::binary | ios::out);

    int n = 0;

    in.read((char *)&n, 4);
    out.write((char *)&n, 4);
    out.write((char *)&n, 4);

    int memory_limit = 500000 - 1000;


    out.close();

#ifdef __PROFILE__
    chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    cout << " Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
    cout << "Result correct - " << compare_files("output.bin", "answer.txt");
#endif


    return 0;
}