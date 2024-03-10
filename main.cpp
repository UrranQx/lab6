#include <iostream>
#include <string>
#include <random>
#include <iomanip>
#include <functional>

#define EPS 1e-15
#define SET_PRECISION 3
#define ADDITIONAL_SPACING 2
#define TABLE_SPACING 20
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[97m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BOLD    "\x1b[3m\x1b[7m"
#define ANSI_K_ATTR        "\x1b[95m\x1b[3m\x1b[7m"

int amountOfDigits(int n) {
    int c = 0;
    n = abs(n);
    if (n == 0) return 1;
    while (n > 0) {
        c++;
        n /= 10;
    }
    return c;
}


void sub_main();

int findIndexOfMax(const int *_arr, int begin, int end, int &comparisons);

int findIndexOfMin(const int *_arr, int begin, int end, int &comparisons);

void bubbleSort(int *arr, int size, int &swaps, int &comparisons);

void selectionSort(int *arr, int size, int &swaps, int &comparisons);

void selectionSort_minmax(int *arr, int size, int &swaps, int &comparisons);

void insertionSort(int *arr, int size, int &swaps, int &comparisons);

void shellSort(int *arr, int size, int &swaps, int &comparisons);

void quickSort(int *arr, int size, int &swaps, int &comparisons);


int findLengthOfLongest(std::string pString[5]);

bool compareMatrices(int **standard, int **_matrix);

/*
 * Реализовать сортировки для шаблона T: bubble sort, selection sort, insertion sort, Shell sort, quicksort;
 * Пусть функции будут не чистыми и редактируют данный им массив напрямую, т.к. я им все равно буду давать копию.
 * Дана матрица M x N --> где M - количество элементов в столбце, N - количество эл-в в строке.
 * Упорядочить каждый столбец матрицы по убыванию абсолютных величин.
 *  * Глобально у меня есть две идеи, как это реализовать:
 *      * Транспонировать матрицу и отсортировать каждую строку и транспонировать обратно.
 *      * Каким-то образом выделить чисто столбец и сортировать чисто его, с помощью приколов с указателями.
 *      * Т.е. я буду пробегать всю матрицу уже сразу по количеству элементов в строке и итерироваться по столбцу.
 *      * И в этой итерации и использовать методы сортировки.
 *      * Пока что в этой итерации я собираюсь собрать массив элементов из столбца и вставить обратно после сортировки.
 *  * Подсчитать количество перестановок и сравнений в каждом методе сортировки.
 *  * Сравнить эффективность методов сортировки. Составить таблицу
 *
 * Т.е. добавить к функциям сортировки реализацию трекинга количества перестановок, и количества сравнений.
 */


int main() {
    // Улучшенный способ генерации псевдо-рандомных чисел
    sub_main();
    return 0;
}

/**
 * Bubble sort - сортировка, при которой самый надутый элемент всплывает вверх.
 * То есть мы проходимся по массиву size раз, и если i-й и i + 1 -й стоят не в порядке возрастания, то свапаем их.
 *
 */
void bubbleSort(int *arr, int size, int &swaps, int &comparisons) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            // До size - i - 1 т.к. последний элемент уже будет отсортирован после первой итерации, потом предпоследний...
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                swaps++;
                std::swap(arr[j], arr[j + 1]);
                /// Тут std::swap быстрее, т.к. он использует std::move.
                /// Если делать свой swap, то он по логике был бы таков:
                // int __temp = arr[j];
                // arr[j] = arr[j + 1];
                // arr[j + 1] = __temp;
                // Затем этот __temp удаляется из памяти благодаря RAII, т.к. он выходит из области видимости.
            }
        }
    }
}

int findIndexOfMax(const int *_arr, int begin, int end, int &comparisons) {
    int _max = _arr[begin];
    int ans = begin;
    for (int i = begin + 1; i <= end; i++) {
        comparisons++;
        if (_arr[i] > _max) {
            _max = _arr[i];
            ans = i;
        }
    }
    return ans;
}

int findIndexOfMin(const int *_arr, int begin, int end, int &comparisons) {
    int _min = _arr[begin];
    int ans = begin;
    for (int i = begin + 1; i <= end; i++) {
        comparisons++;
        if (_arr[i] < _min) {
            _min = _arr[i];
            ans = i;
        }
    }
    return ans;
}

void selectionSort_minmax(int *arr, int size, int &swaps, int &comparisons) {
    int index;
    for (int i = 0; i < size - 1; i++) {
        index = findIndexOfMin(arr, i + 1, size - 1, comparisons);
        if (arr[index] < arr[i]) {
            std::swap(arr[index], arr[i]);
            swaps++;
        }
        comparisons++;

    }
}

/**
 * Selection sort - сортировка, идея его такова:
 * Смотрим на неотсортированный массив, ищем минимальный его элемент и меняем местами с тем, что в начале такого массива
 *
 */
void selectionSort(int *arr, int size, int &swaps, int &comparisons) {
    for (int i = 0; i < size - 1; i++) {
        // Предположим, что минимальный элемент - текущий элемент с индексом i
        int indexOfMin = i;
        for (int j = i + 1; j < size; j++) {
            // В этом цикле надо найти индекс самого малого элемента. Потом мы им воспользуемся, чтобы свапнуться с i-м
            comparisons++;
            if (arr[indexOfMin] > arr[j]) {
                indexOfMin = j;
            }
        }
        if (indexOfMin != i) {
            std::swap(arr[indexOfMin], arr[i]);
            swaps++;
        }

    }
}

void insertionSort(int *arr, int size, int &swaps, int &comparisons) {
    for (int key = 1; key < size; key++) {
        for (int i = key; i > 0 && arr[i - 1] > arr[i]; i--) {
            std::swap(arr[i - 1], arr[i]);
            swaps++;
            comparisons++;
        }
    }
}

void shellSort(int *arr, int size, int &swaps, int &comparisons) {
    for (int gap = size / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < size; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
                swaps++;
                comparisons++;
            }
            arr[j] = temp;
        }
    }
}

void quickSort(int *arr, int size, int &swaps, int &comparisons) {
    if (size <= 1) return;
    int pivot = arr[size / 2]; // pivot - указатель на половину (ну с учетом округления при целочисленном делении)
    int i = 0, j = size - 1;
    while (i <= j) {
        while (arr[i] < pivot) {
            i++;
            comparisons++;
        }
        while (arr[j] > pivot) {
            j--;
            comparisons++;
        }
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            // Перебрасываем элементы большие pivot вправо, а меньшие влево.
            // Чтобы потом можно было запустить quickSort на двух подмассивах.
            swaps++;
            i++;
            j--;
        }
    }
    quickSort(arr, j + 1, swaps, comparisons); // Левая часть от pivot
    quickSort(arr + i, size - i, swaps, comparisons); // Правая часть от pivot
    // Сам pivot стоит на индексе
}

/*
 * This function copying matrix;
 * To properly do that you should provide @params matrix, M, N.
 * M - number of rows;
 * N - number of columns;
 */
int **copyMatrix(int **matrix, int M, int N) {
    int **ans = (int **) malloc(sizeof(int *) * M);
    for (int i = 0; i < M; i++) {
        ans[i] = (int *) malloc(sizeof(int) * N);
        for (int j = 0; j < N; j++) {
            ans[i][j] = matrix[i][j];
        }
    }
    return ans;

}

/*
 * This function deletes matrix;
 * To properly do that you should provide @params matrix, M.
 * M - number of rows;
 */
void deleteMatrix(int **matrix, int M) {
    for (int i = 0; i < M; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void sub_main() {
    double bottom = 0.0;
    double top = 100.0;

    int SIZE_M, SIZE_N;
    std::cout << "Enter matrix SIZE_M: ";
    std::cin >> SIZE_M;
    std::cout << std::endl;

    std::cout << "Enter matrix SIZE_N: ";
    std::cin >> SIZE_N;
    std::cout << std::endl;

    if (SIZE_M <= 0 || SIZE_N <= 0) throw std::invalid_argument("ERROR: Matrix cannot exist");

    int spaces = amountOfDigits((int) top) +
                 ADDITIONAL_SPACING; // -> дополнительные пробелы для того, чтобы цифры не слипались
    std::cout << "\n";
    int precision = SET_PRECISION;
    std::cout.precision(precision);


    /// Принято считать индексацию в матрицах начиная с 1,
    /// т.е. K_PARAM = 3 соответствует третья строчка -> i = 2
    /// Чтобы все время не исправлять эту индексацию в коде, сразу сделаем так


    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(bottom, top);

    int **matrix = (int **) malloc(sizeof(int *) * SIZE_M);
    for (int i = 0; i < SIZE_M; i++) {
        matrix[i] = (int *) malloc(sizeof(int) * SIZE_N);
        // Сразу заполним матрицу элементами
        for (int j = 0; j < SIZE_N; j++) {
            matrix[i][j] = (int) dist(mt); // Случайное число от 0 до 10000 не включительно
        }
    }

    int element;
    std::string color;

    for (int i = 0; i < SIZE_M; i++) {
        for (int j = 0; j < SIZE_N; j++) {
            element = matrix[i][j];
            color = "";

            std::cout
                    << std::setw(spaces)
                    << element;

        }
        std::cout << std::endl;
    }
    // Для транспонирования матрицы воспользуемся тем, что у нас есть два индекса. Т.е. мы можем просто их поменять местами.
    // Т.е. если у нас есть матрица 3x3, то мы можем просто поменять местами элементы с индексами (0, 1) и (1, 0)

    // Транспонирование матрицы
    /*
    for (int i = 0; i < SIZE_M; i++) {
        for (int j = 0; j < SIZE_N; j++) {
            if (i != j) {
                std::swap(matrix[i][j], matrix[j][i]);
            }
        }
    }
    */

    std::function<void(int *, int, int &, int &)> sortingAlgorithms[] = {
            bubbleSort, selectionSort, insertionSort, shellSort, quickSort
    };
    int SORTING_ALGORITHMS_SIZE = sizeof(sortingAlgorithms) / sizeof(sortingAlgorithms[0]);
    std::string sortingAlgorithmsNames[] = {"Bubble", "Selection", "Insertion", "Shell", "Quick"};
    /// temp_mx - массив из пяти одинаковый матриц, каждая из который изначально равна исходной matrix
    /// Она служит для того, чтобы на ней провести опыты с сортировками и потом сравнить их работу и сам результат
    /// Конечно можно было бы просто не делать строчку с присваиванием отсортированных значений в старую матрицу,
    /// а создавать новую, но это просто был не мой выбор, т.к. мне не так просто копировать колонки, нежели чем строчки

    int ***temp_mx = (int ***) malloc(sizeof(int ***) * SORTING_ALGORITHMS_SIZE);
    for (int k = 0; k < SORTING_ALGORITHMS_SIZE; k++) {
        temp_mx[k] = (int **) malloc(sizeof(int *) * SIZE_M);
        for (int i = 0; i < SIZE_M; i++) {
            temp_mx[k][i] = (int *) malloc(sizeof(int) * SIZE_N);
            for (int j = 0; j < SIZE_N; j++) {
                temp_mx[k][i][j] = matrix[i][j];
            }
        }
    }
    int *total_swaps = (int *) malloc(sizeof(int) * SORTING_ALGORITHMS_SIZE);
    int *total_comparisons = (int *) malloc(sizeof(int) * SORTING_ALGORITHMS_SIZE);
    for (int i = 0; i < SORTING_ALGORITHMS_SIZE; i++) {
        total_swaps[i] = 0;
        total_comparisons[i] = 0;
    }

    for (int algorithm_index = 0; algorithm_index < SORTING_ALGORITHMS_SIZE; algorithm_index++) {
        for (int i = 0; i < SIZE_N; i++) {
            int *column = (int *) malloc(sizeof(int) * SIZE_M);
            for (int j = 0; j < SIZE_M; j++) {
                column[j] = temp_mx[algorithm_index][j][i];
            }
            int size = SIZE_M;
            auto sortingAlgo = sortingAlgorithms[algorithm_index];
            int swaps = 0, comparisons = 0;
            sortingAlgo(column, size, swaps, comparisons);

            total_swaps[algorithm_index] += swaps;
            total_comparisons[algorithm_index] += comparisons;

            std::cout
                    << sortingAlgorithmsNames[algorithm_index]
                    << " sort: "
                    << std::setw(7 + 9 -
                                 (int) sortingAlgorithmsNames[algorithm_index].length())


                    << "swaps: " << swaps
                    << " comparisons: " << comparisons
                    << std::endl;

            for (int j = 0; j < SIZE_M; j++) temp_mx[algorithm_index][j][i] = column[j];
            free(column);
        }

        std::cout << std::endl;

    }

    /*
     * Сейчас, когда мы получили результаты сортировок всех созданных методов, то мы можем попробовать их сравнить.
     * Моя аксиома такая, что bubble sort работает безупречно, поэтому он послужит эталоном.
     * Standard = temp_mx[0];
     */

    std::cout << "Comparing results of sorting algorithms to Bubble Sort";

    int **standard = temp_mx[0];
    for (int i = 1; i < SORTING_ALGORITHMS_SIZE; i++) {
        bool fl = false;
        int **t_mx = temp_mx[i];
        for (int j = 0; j < SIZE_M; ++j) {
            for (int k = 0; k < SIZE_N; ++k) {
                if (standard[j][k] != t_mx[j][k]) {
                    fl = true;
                    std::cout << "\nMethod" << sortingAlgorithmsNames[i] << " is a total failure" << std::endl;
                    break;
                }

            }
            if (fl) break;
        }
        if (!fl) std::cout << "\nEverything is okay with: " << sortingAlgorithmsNames[i] << " sort" << std::endl;
    }

    std::cout << "\n\nCHANGED MATRIX \n\n";

    for (int i = 0; i < SIZE_M; i++) {
        for (int j = 0; j < SIZE_N; j++) {
            element = standard[i][j];
            std::cout
                    << std::setw(spaces)
                    << element;
            //printf(" %4d ", matrix[i * SIZE + j]); // Deprecated

        }
        std::cout << std::endl;
    }

    /// Удаление матриц

    for (int i = 0; i < SORTING_ALGORITHMS_SIZE; i++) {
        for (int j = 0; j < SIZE_M; j++) {
            free(temp_mx[i][j]);
        }
        free(temp_mx[i]);
    }
    free(temp_mx);
    deleteMatrix(matrix, SIZE_M);

    /// Подведение итогов
    std::cout << "\nFINAL COMPARISON" << std::endl;
    std::cout << "=================" << std::endl;
    std::cout << "METHOD" << std::setw(TABLE_SPACING) << "SWAPS" << std::setw(TABLE_SPACING) << "COMPARISONS"
              << std::endl;
    for (int i = 0; i < SORTING_ALGORITHMS_SIZE; i++) {
        std::cout
                << sortingAlgorithmsNames[i] << std::setw(TABLE_SPACING - sortingAlgorithmsNames[i].length() + 4)
                << total_swaps[i]
                << std::setw(TABLE_SPACING)
                << total_comparisons[i]
                << std::endl;
    }

}




/**
 * ADDITIONAL INFO ON HOW TO CREATE A MATRIX
 * int** matrix = new int*[M];
 * for(int i = 0; i < M; ++i) matrix[i] = new int[N];
 * for(int i = 0; i < M; ++i) delete [] matrix[i];
 * delete [] matrix;
 *
*/