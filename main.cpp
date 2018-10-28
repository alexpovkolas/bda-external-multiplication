#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>


#define __PROFILE__

#ifdef __PROFILE__

#include <algorithm>
#include <chrono>

#endif

using namespace std;

void mul(const unsigned char *a, size_t na, size_t ma, const unsigned char *b, size_t nb, size_t mb, unsigned char *result) {
    for (int i = 0; i < na; ++i) {
        for (int j = 0; j < mb; ++j) {
            for (int k = 0; k < ma; ++k) {
                //result[i][j] += a[i][k] + b[k][j];
                result[i * na + j] += a[i * na + k] + b[k * nb + j];
            }
        }
    }
}


void external_multiplication(ifstream &in, ofstream &out, int n, int m, int memory_size) {
    int read_block_size = memory_size / 3;

    int lines_block_count = 0;    // lines count we read for one block
    int cols_block_count = sqrt(
            read_block_size); // columns count we read for one block. One block is matrix lines_count * cols_count
    if (cols_block_count < n && cols_block_count < m) {
        lines_block_count = cols_block_count;
    } else if (cols_block_count >= n && cols_block_count >= m) {
        lines_block_count = n;
        cols_block_count = m;
    } else if (cols_block_count >= n) {
        lines_block_count = n;
        cols_block_count = read_block_size / lines_block_count;
    } else if (cols_block_count >= m) {
        cols_block_count = m;
        lines_block_count = read_block_size / cols_block_count;
    }

#ifdef __PROFILE__
    int file_reads_count = 0;
    int file_writes_count = 0;
    cout << "lines_block_count = " << lines_block_count << " cols_block_count = " << cols_block_count << endl;
#endif

    char *a = new char[lines_block_count * cols_block_count];
    char *b = new char[lines_block_count * cols_block_count];
    char *c = new char[lines_block_count * max(cols_block_count, lines_block_count)];
    int params_offset = 8;

    int blocks = ceil(n / (double) lines_block_count);

#ifdef __PROFILE__
    cout << "blocks = " << blocks << endl;
#endif

    for (int i = 0; i < blocks; ++i) {
        for (int j = 0; j < blocks; ++j) {

            fill(c, c + lines_block_count * max(cols_block_count, lines_block_count), 0);

            for (int k = 0; k < blocks; ++k) {

                // read block
                int lines_to_read =
                        n - lines_block_count * i > lines_block_count ? lines_block_count : n - lines_block_count * i;
                int cols_to_read =
                        m - cols_block_count * k > cols_block_count ? cols_block_count : m - cols_block_count * k;

#ifdef __PROFILE__
                cout << "lines_to_read = " << lines_to_read << " cols_to_read = " << cols_to_read << endl;
#endif
                // Read a block from A matrix
                for (int l = 0; l < lines_to_read; ++l) {
                    long a_offset = params_offset +         // n and m 8 bytes
                                         (l + i * lines_block_count) * m +    // lines offset
                                         k * cols_block_count;   // offset inside current line
                    in.seekg(a_offset);
                    in.read(a + l * cols_to_read, cols_to_read);
                }


                // Read a block from B matrix
                for (int l = 0; l < cols_to_read; ++l) {
                    // Mirror source offset
                    long b_offset = 2 * params_offset + n * n +
                                       (l + k * cols_block_count) * n +
                                       i * lines_block_count;
                    in.seekg(b_offset);
                    in.read(b + l * lines_to_read, lines_to_read);
                }

                mul((unsigned char*)a, lines_to_read, cols_to_read, (unsigned char*)b, cols_to_read, lines_to_read, (unsigned char*)c);
            }


            // write block
            int lines_to_read =
                    n - lines_block_count * i > lines_block_count ? lines_block_count : n - lines_block_count * i;
            int cols_to_read =
                    m - cols_block_count * j > cols_block_count ? cols_block_count : m - cols_block_count * j;

            for (int k = 0; k < lines_to_read; ++k) {
                // Mirror source offset
                long dest_offset = params_offset +
                                   (k + j * cols_block_count) * n +
                                   i * lines_block_count;
                out.seekp(dest_offset);
                out.write(c + k * cols_to_read, cols_to_read);

            }

        }
    }

#ifdef __PROFILE__
    cout << "file_writes_count = " << file_writes_count << " file_reads_count = " << file_reads_count << endl;
#endif

}

#ifdef __PROFILE__

void gen_test(int n) {

    ofstream file("input.bin", ios::binary);

    file.write((char *)&n, 4);
    file.write((char *)&n, 4);

    for (int i = 1; i <= n * n; i++){
        file.write((char *)&i, 1);
    }

    file.write((char *)&n, 4);
    file.write((char *)&n, 4);

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) {
            char r = i == j ? 1 : 0;
            file.write((char *) &r, 1);
        }
    }

    file.close();
}

bool compare_files(const string& filename1, const string& filename2)
{
    ifstream file1(filename1, ifstream::ate | ifstream::binary);
    std::ifstream file2(filename2, std::ifstream::ate | std::ifstream::binary);
    const ifstream::pos_type fileSize = file1.tellg();

    if (fileSize != file2.tellg()) {
        return false; //different file size
    }

    file1.seekg(0);
    file2.seekg(0);

    istreambuf_iterator<char> begin1(file1);
    istreambuf_iterator<char> begin2(file2);

    return equal(begin1, istreambuf_iterator<char>(), begin2); //Second argument is end-of-range iterator
}

#endif

int main() {

#ifdef __PROFILE__
    gen_test(3);
    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#endif

    ifstream in("input.bin", ios::binary);
    ofstream out("output.bin", ios::binary | ios::out);

    int n = 0;

    in.read((char *) &n, 4);
    out.write((char *) &n, 4);
    out.write((char *) &n, 4);

    int memory_limit = 500000 - 1000;

    external_multiplication(in, out, n, n, memory_limit);

    out.close();

#ifdef __PROFILE__
    chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    cout << " Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
    //cout << "Result correct - " << compare_files("output.bin", "answer.txt");
#endif


    return 0;
}