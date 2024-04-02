#include <iostream>
#include <string>
#include <random>
#include <iomanip>
#include <thread>
#include <algorithm>
#include <functional>
//#include <functional>

#define EPS 1e-15
#define SET_PRECISION 3
#define ADDITIONAL_SPACING 2
#define NO_DEBUG 0
#define DEBUG_MIN 1
#define DEBUG_MAX 2
#define TABLE_SPACING 20
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[97m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BOLD    "\x1b[3;7;97m"
#define ANSI_K_ATTR        "\x1b[95m\x1b[3m\x1b[7m"

/**
 * @brief Создадим тип Comparator, который будет принимать два аргумента типа int и возвращать bool.
 */
typedef bool (*Comparator)(int, int);

/// @brief Проверяет, если элементы соответственно стоят в порядке возрастания
bool ascendingOrder(int a, int b) {
    return a <= b;
}

/// @brief Проверяет, если сумма цифр числа стоит в порядке возрастания
/// @brief При совпадении суммы, сравнивает сами числа
bool compareBySumOfDigits(int a, int b) {
    int absA = abs(a);
    int absB = abs(b);
    if (absA == absB) { // Сразу проверяем, если числа равны по модулю, то большее число расположим левее меньшего
        return a > b;   // Сначала 11, потом -11
    }
    int sumA = 0, sumB = 0;
    while (absA > 0) {
        sumA += absA % 10;
        absA /= 10;
    }
    while (absB > 0) {
        sumB += absB % 10;
        absB /= 10;
    }
    if (sumA == sumB) return a > b; // Если суммы цифр чисел равны, то большее число расположим левее меньшего
    // Сначала 58, потом 49; Сначала -49, потом -58
    return sumA < sumB;             // 0 -> 1 -> 44 -> 8 -> 96 -> 99 -> ...
}

/// @brief Проверяет, если элементы соответственно стоят в порядке убывания
bool descendingOrder(int a, int b) {
    return a > b;
}

/// @brief Проверяет, если элементы по их абсолютным значениям расположены в порядке возрастания.
/// В случае совпадения абсолютных значений, меньшее число располагается левее большего.
bool absAscendingOrder(int a, int b) {
    int absA = abs(a);
    int absB = abs(b);
    if (absA == absB) return a < b;
    return absA < absB;
}

/// @brief Проверяет, если элементы по их абсолютным значениям расположены в порядке возрастания.
/// В случае совпадения абсолютных значений, большее число располагается левее меньшего.
bool absDescendingOrder(int a, int b) {
    if (a == b) return true;
    int absA = abs(a);
    int absB = abs(b);
    if (absA == absB) return a > b;
    return absA > absB;
}

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

int amountOfDigits(unsigned long long n) {
    int c = 0;
    if (n == 0) return 1;
    while (n > 0) {
        c++;
        n /= 10;
    }
    return c;
}

unsigned long long maxFrom(const unsigned long long *arr, int size) {
    unsigned long long max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

void sub_main(int SIZE_M = 3, int SIZE_N = 4, Comparator myComparator = ascendingOrder);

int findIndexOfMax(const int *_arr, int begin, int end, int &comparisons);

int findIndexOfMin(const int *_arr, int begin, int end, int &comparisons);

void bubbleSort(int *arr, int size, Comparator inOrder, int &swaps, int &comparisons);

void selectionSort(int *arr, int size, Comparator inOrder, int &swaps, int &comparisons);

void selectionSort_minmax(int *arr, int size, Comparator inOrder, int &swaps, int &comparisons);

void insertionSort(int *arr, int size, Comparator inOrder, int &swaps, int &comparisons);

void shellSort(int *arr, int size, Comparator inOrder, int &swaps, int &comparisons);

void quickSort(int *arr, int size, Comparator inOrder, int &swaps, int &comparisons);

void selectionSort_orderFinder(int *arr, int size, Comparator inOrder, int &swaps, int &comparisons);


/*
 * Реализовать сортировки bubble sort, selection sort, insertion sort, Shell sort, quicksort;
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
    Comparator myComparator = absDescendingOrder;
    int SIZE_M, SIZE_N;
    std::cout << "Enter matrix SIZE_M: ";
    std::cin >> SIZE_M;
    std::cout << std::endl;

    std::cout << "Enter matrix SIZE_N: ";
    std::cin >> SIZE_N;
    std::cout << std::endl;

    if (SIZE_M <= 0 || SIZE_N <= 0) throw std::invalid_argument("ERROR: Matrix cannot exist");

    sub_main(SIZE_M, SIZE_N, myComparator);


    ///TESTING DIFFERENT COMPARATORS
    //sub_main(5,5,compareBySumOfDigits);
    //sub_main(5,5,compareBySumOfDigits);


    // std::cout << "Sub main in default(prints a random 3x4 array, orders column's elements by ascension) ";
    // sub_main();

    return 0;
}

/// ВСЕ ИЗНАЧАЛЬНО НАПИСАННЫЕ КОММЕНТАРИИ К СОРТИРОВКАМ ОРИЕНТИРОВАЛИСЬ НА СОРТИРОВКУ ПО ВОЗРАСТАНИЮ
/**
 * Bubble sort - сортировка, при которой самый надутый элемент всплывает вверх.
 * То есть мы проходимся по массиву size раз, и если i-й и i + 1 -й стоят не в порядке возрастания, то свапаем их.
 *
 */
void bubbleSort(int *arr, int size, Comparator inOrder, int &swaps, int &comparisons) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            // До size - i - 1 т.к. последний элемент уже будет отсортирован после первой итерации, потом предпоследний...
            comparisons++;
            if (!inOrder(arr[j], arr[j + 1])) {
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

int findIndexOfBestInOrder(const int *_arr, int begin, int end, Comparator order, int &comparisons) {
    int _bestInOrder = _arr[begin]; // Best in order - лучший в порядке, удовлетворяющем myOrder.
    int ans = begin;
    for (int i = begin + 1; i <= end; i++) {
        comparisons++;
        if (order(_arr[i], _bestInOrder)) {
            _bestInOrder = _arr[i];
            ans = i;
        }
    }
    return ans;
}

void selectionSort_orderFinder(int *arr, int size, Comparator inOrder, int &swaps, int &comparisons) {
    int index;
    for (int i = 0; i < size - 1; i++) {
        index = findIndexOfBestInOrder(arr, i + 1, size - 1, inOrder, comparisons);
        if (inOrder(arr[index], arr[i])) {
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
void selectionSort(int *arr, int size, Comparator inOrder, int &swaps, int &comparisons) {
    for (int i = 0; i < size - 1; i++) {
        // Предположим, что минимальный элемент - текущий элемент с индексом i
        int indexOfElement = i;
        for (int j = i + 1; j < size; j++) {
            // В этом цикле надо найти индекс самого малого элемента. Потом мы им воспользуемся, чтобы свапнуться с i-м
            comparisons++;
            if (!inOrder(arr[indexOfElement], arr[j])) {
                indexOfElement = j;
            }
        }
        if (indexOfElement != i) {
            std::swap(arr[indexOfElement], arr[i]);
            swaps++;
        }

    }
}

void insertionSort(int *arr, int size, Comparator inOrder, int &swaps, int &comparisons) {
    for (int key = 1; key < size; key++) {
        for (int i = key; i > 0; i--) {
            comparisons++;
            if (!inOrder(arr[i - 1], arr[i])) {
                std::swap(arr[i - 1], arr[i]);
                swaps++;
            } else break;
        }
    }
}

/*
 * Shell sort - улучшенная версия insertion sort.
 * Он работает так: он сначала сортирует элементы, находящиеся на расстоянии gap друг от друга.
 * Потом уменьшает gap вдвое и снова сортирует элементы на расстоянии gap друг от друга.
 * И так до тех пор, пока gap не станет равным 1, т.е. пока не отсортирует все элементы.
 */
void shellSort(int *arr, int size, Comparator inOrder, int &swaps, int &comparisons) {
    for (int gap = size / 2; gap > 0; gap /= 2) {
        // Сначала сортируем элементы на расстоянии gap друг от друга
        for (int i = gap; i < size; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap; j -= gap) {
                comparisons++;
                if (!inOrder(arr[j - gap], temp)) {
                    arr[j] = arr[j - gap];
                    swaps++;
                } else break;
            }
            arr[j] = temp;
        }
    }
}

/*
 * Задача - написать quick sort, который будет работать стабильно (т.е. если значения равны, то их порядок не меняется)
 * Для этого будем использовать способ разбивки на подмассивы, который не меняет порядок элементов, если они равны.
 * Т.е. если элементы равны, то мы просто пропускаем их.
 */

void quickSort3Way(int *arr, int low, int high, Comparator inOrder, int &swaps, int &comparisons) {
    if (high <= low) return;
    if (high - low <2) {
        comparisons++;
        if(inOrder(arr[high], arr[low]) && arr[high] != arr[low]) {
            swaps++;
            std::swap(arr[high], arr[low]);
        }
        return;
    }


    int lt = low;
    int gt = high;
    int pivot = arr[low];
    int i = low;

    while (i <= gt) {
        comparisons++;
        if (inOrder(arr[i], pivot) && arr[i] != pivot) {
            swaps++;
            std::swap(arr[lt++], arr[i++]);
        } else if (inOrder(pivot, arr[i]) && arr[i] != pivot) {
            swaps++;
            std::swap(arr[i], arr[gt--]);
        } else {
            i++;
        }
    }

    quickSort3Way(arr, low, lt - 1, inOrder, swaps, comparisons);
    quickSort3Way(arr, gt + 1, high, inOrder, swaps, comparisons);
}

void quickSort(int *arr, int size, Comparator inOrder, int &swaps, int &comparisons) {
    quickSort3Way(arr, 0, size - 1, inOrder, swaps, comparisons);
}

void reverseArray(int *arr, int size) {
    for (int i = 0; i < size / 2; i++) {
        std::swap(arr[i], arr[size - i - 1]);
    }
}


/**
 * This function copying matrix;
 * To properly do that you should provide
 * @param matrix - initial matrix to copy
 * @param M - number of rows @param N - number of columns;
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

void sub_main(int SIZE_M, int SIZE_N, Comparator myComparator) {
    // По умолчанию сортируем по возрастанию, но можно передать и другой компаратор.
    int range_bottom = (-SIZE_M * SIZE_N);
    int range_top = (SIZE_M * SIZE_N + 1);
    int rangeBorder = SIZE_M * SIZE_N + 1;
    int spaces = amountOfDigits(abs(range_bottom) > abs(range_top) ? range_bottom : range_top) +
                 ADDITIONAL_SPACING; // -> дополнительные пробелы для того, чтобы цифры не слипались
    int precision = SET_PRECISION;
    std::cout.precision(precision);


    std::random_device rd;
    std::mt19937 mt(rd()); // Mersenne Twister Algorithm (https://en.wikipedia.org/wiki/Mersenne_Twister)
    std::uniform_real_distribution<double> dist(-rangeBorder, rangeBorder);
    /// Debug level
    short debug = DEBUG_MAX;

    int **matrix = (int **) malloc(sizeof(int *) * SIZE_M);
    for (int i = 0; i < SIZE_M; i++) {
        matrix[i] = (int *) malloc(sizeof(int) * SIZE_N);
        // Сразу заполним матрицу элементами
        for (int j = 0; j < SIZE_N; j++) {
            matrix[i][j] = i;//(int) dist(mt); // Случайное число от 0 до 10000 не включительно
        }
    }

    int element;
    if (debug > NO_DEBUG) {
        for (int i = 0; i < SIZE_M; i++) {
            for (int j = 0; j < SIZE_N; j++) {
                element = matrix[i][j];
                std::cout
                        << std::setw(spaces)
                        << element;

            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // Для транспонирования матрицы воспользуемся тем, что у нас есть два индекса. Т.е. мы можем просто их поменять местами.
    // Т.е. если у нас есть матрица 3x3, то мы можем просто поменять местами элементы с индексами (0, 1) и (1, 0)

    /// Транспонирование матрицы если мы не хотим отдельно выделять столбец.
    /*
    for (int i = 0; i < SIZE_M; i++) {
        for (int j = 0; j < SIZE_N; j++) {
            if (i != j) {
                std::swap(matrix[i][j], matrix[j][i]);
            }
        }
    }
    */
    typedef void (*SortFunction)(int *, int, Comparator, int &, int &);

    SortFunction sortingAlgorithms[] = {
            bubbleSort, selectionSort, /*selectionSort_orderFinder, */insertionSort, shellSort, quickSort
    };
    int SORTING_ALGORITHMS_SIZE = sizeof(sortingAlgorithms) / sizeof(sortingAlgorithms[0]);
    std::string sortingAlgorithmsNames[] = {"Bubble", "Selection", /*"SpecialSelection",*/ "Insertion", "Shell",
                                            "Quick"};
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
    auto *total_swaps = (unsigned long long *) malloc(sizeof(unsigned long long) * SORTING_ALGORITHMS_SIZE);
    auto *total_comparisons = (unsigned long long *) malloc(
            sizeof(unsigned long long) * SORTING_ALGORITHMS_SIZE);

    for (int i = 0; i < SORTING_ALGORITHMS_SIZE; i++) {
        total_swaps[i] = 0;
        total_comparisons[i] = 0;
    }


    // Основной проход по всем алгоритмам сортировки;

    for (int algorithm_index = 0; algorithm_index < SORTING_ALGORITHMS_SIZE; algorithm_index++) {
        for (int i = 0; i < SIZE_N; i++) {
            int *column = (int *) malloc(sizeof(int) * SIZE_M);
            for (int j = 0; j < SIZE_M; j++) {
                column[j] = temp_mx[algorithm_index][j][i];
            }
            int size = SIZE_M;
            auto sortingAlgo = sortingAlgorithms[algorithm_index];
            int swaps = 0, comparisons = 0;

            sortingAlgo(column, size, myComparator, swaps, comparisons);

            total_swaps[algorithm_index] += swaps;
            total_comparisons[algorithm_index] += comparisons;

            if (debug >= DEBUG_MAX)
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

        if (debug >= DEBUG_MAX)std::cout << std::endl;

    }

    /*
     * Сейчас, когда мы получили результаты сортировок всех созданных методов, то мы можем попробовать их сравнить.
     * Моя аксиома такая, что bubble sort работает безупречно, поэтому он послужит эталоном.
     * Standard = temp_mx[0];
     */
    if (debug > NO_DEBUG) {
        std::cout << "Comparing results of sorting algorithms to Bubble Sort\n";
        std::cout << "======================================================\n";
        int **standard = temp_mx[0];
        for (int i = 1; i < SORTING_ALGORITHMS_SIZE; i++) {
            bool fl = false;
            int **t_mx = temp_mx[i];
            for (int j = 0; j < SIZE_M; ++j) {
                for (int k = 0; k < SIZE_N; ++k) {
                    if (standard[j][k] != t_mx[j][k]) {
                        fl = true;
                        std::cout << "\nMethod" << sortingAlgorithmsNames[i] << " is a total failure"
                                  << std::endl;
                        break;
                    }

                }
                if (fl) break;
            }
            if (!fl)
                std::cout << "Everything is okay with: " << sortingAlgorithmsNames[i] << " sort" << std::endl;
        }

        std::cout << ANSI_COLOR_MAGENTA "\nCHANGED MATRIX \n" << ANSI_COLOR_RESET;

        for (int k = 0; k < SORTING_ALGORITHMS_SIZE; k++) {
            if (debug < DEBUG_MAX && k > 0)continue;
            std::cout << "Method: " << sortingAlgorithmsNames[k] << std::endl;
            for (int i = 0; i < SIZE_M; i++) {
                for (int j = 0; j < SIZE_N; j++) {
                    element = temp_mx[k][i][j];
                    std::cout
                            << std::setw(spaces)
                            << element;
                    //printf(" %4d ", matrix[i * SIZE + j]); // Deprecated

                }
                std::cout << std::endl;
            }
        }
    }
    /// Подведение итогов
    /// - некрасивый код, ради красивого вывода
    std::cout << "\n"
              << std::string(20, ' ')
              << ANSI_COLOR_MAGENTA << "FINAL COMPARISON" << ANSI_COLOR_RESET
              << std::string(20, ' ') << std::endl;

    std::cout << ANSI_COLOR_BOLD << ANSI_COLOR_YELLOW << std::string(56, '=') << ANSI_COLOR_RESET << std::endl;
    int maxSwapsDigits = amountOfDigits(maxFrom(total_swaps, SORTING_ALGORITHMS_SIZE));
    int maxComparisonsDigits = amountOfDigits(maxFrom(total_comparisons, SORTING_ALGORITHMS_SIZE));
    maxSwapsDigits += 2;
    maxComparisonsDigits += 2;
    std::cout << std::left << std::setw(20) << "Sorting Algorithm"
              << std::right
              << std::setw(10 > maxSwapsDigits ? 10 : maxSwapsDigits) << "Swaps"
              << std::setw(15 > maxComparisonsDigits ? 15 : maxComparisonsDigits) << "Comparisons" << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::string(56, '-') << ANSI_COLOR_RESET << std::endl;

    for (int i = 0; i < SORTING_ALGORITHMS_SIZE; i++) {
        std::cout << std::left << std::setw(20) << sortingAlgorithmsNames[i]
                  << std::right
                  << std::setw(10 > maxSwapsDigits ? 10 : maxSwapsDigits) << total_swaps[i]
                  << std::setw(15 > maxComparisonsDigits ? 15 : maxComparisonsDigits) << total_comparisons[i]
                  << std::endl;
    }
    std::cout << ANSI_COLOR_GREEN << std::string(56, '-') << ANSI_COLOR_RESET << std::endl;

    /// Удаление матриц
    free(total_swaps);
    free(total_comparisons);
    for (int i = 0; i < SORTING_ALGORITHMS_SIZE; i++) {
        for (int j = 0; j < SIZE_M; j++) {
            free(temp_mx[i][j]);
        }
        free(temp_mx[i]);
    }
    free(temp_mx);
    deleteMatrix(matrix, SIZE_M);

}



/**
 * ADDITIONAL INFO ON HOW TO CREATE A MATRIX
 * int** matrix = new int*[M];
 * for(int i = 0; i < M; ++i) matrix[i] = new int[N];
 * for(int i = 0; i < M; ++i) delete [] matrix[i];
 * delete [] matrix;
 *
*/