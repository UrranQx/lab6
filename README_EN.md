# C++ Sorting Algorithms Project
To see the code, click [here](main.cpp)
This project is a comprehensive implementation of various sorting algorithms in C++. The code is written in a way that allows for easy comparison of the performance of different sorting algorithms. The sorting algorithms implemented include Bubble Sort, Selection Sort, Insertion Sort, Shell Sort, and Quick Sort.

## Code Structure

The code is structured around a main function and a sub_main function. The main function is responsible for taking user input for the size of the matrix and the comparator function to be used for sorting. The sub_main function is where the actual sorting takes place.

The code also includes a number of helper functions for sorting and manipulating the matrix. These include functions for copying and deleting matrices, finding the index of the maximum and minimum elements in an array, and various comparator functions for different sorting orders.

## Sorting Algorithms

Each sorting algorithm is implemented as a separate function that takes an array, its size, a comparator function, and references to variables for tracking the number of swaps and comparisons made during sorting.

The comparator functions are used to determine the order in which elements should be sorted. They are implemented as function pointers, allowing for different sorting orders to be easily swapped in and out.

## Performance Comparison

The code includes functionality for comparing the performance of the different sorting algorithms. This is done by tracking the number of swaps and comparisons made during sorting. The results are then outputted in a table for easy comparison.

## Usage

To use the code, simply run the main function and follow the prompts to enter the size of the matrix and the comparator function to be used for sorting. The code will then sort the matrix using each of the implemented sorting algorithms and output the results.