Problem 1.2. Произведение матриц
Input file name: input.bin
Output file name: output.bin
Time limit: 45 s
Memory limit: 1 MB

Требуется вычислить значение произведения матриц в бинарном формате. Общий размер матриц не помещается в ограничение по памяти.

Каждый элемент результирующей матрицы нужно взять по модулю 256.

Формат входного файла

Первые 4 байта входного файла задают число N1 (1 ≤ N1) — количество строк в матрице. Далее 4 байта задают число M1 (1 ≤ M1) — количество столбцов. Следующие N1 × M1 байт задают элементы матрицы (по одному байту на каждый элемент матрицы), строки записаны в порядке от 1 до N1.
Далее 4 байта входного файла задают число N2 (1 ≤ N2) — количество строк в матрице. Далее 4 байта задают число M2 (1 ≤ M2) — количество столбцов. Следующие N2 × M2 байт задают элементы матрицы (по одному байту на каждый элемент матрицы), строки записаны в порядке от 1 до N2.

Все матрицы квадратные (10 ≤ N1, M1, N2, M2 ≤ 2 500).

Формат выходного файла

Выведите результирующую матрицу в таком же формате, как представлены матрицы во входном файле. Каждый элемент результирующей матрицы должен занимать один байт в выходном файле.

Пример

input.bin	output.bin
3 3 2 0 0 0 1 0 0 0 1 3 3 1 2 3 4 5 6 7 8 9
3 3 2 4 6 4 5 6 7 8 9
Замечание

Пример в задаче приведён в текстовом формате для пояснения. Входной файл в бинарном формате.
Если ваше решение использует более 700 МБ, то места для создания дополнительных файлов может не хватить.

