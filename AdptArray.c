#include <stdlib.h>
#include <stdio.h>
#include "AdptArray.h"

// Definition of the struct AdptArray
struct AdptArray_ {
    PElement* arr;   // Pointer to an array of elements
    int size;        // Size of the array
    COPY_FUNC copy_func;    // Pointer to a function that copies an element
    DEL_FUNC del_func;      // Pointer to a function that deletes an element
    PRINT_FUNC print_func;  // Pointer to a function that prints an element
};

// Function to create a new AdptArray object
PAdptArray CreateAdptArray(COPY_FUNC copy_func, DEL_FUNC del_func, PRINT_FUNC print_func) {
    // Allocate memory for a new AdptArray object
    PAdptArray arr = (PAdptArray) malloc(sizeof(struct AdptArray_));
    if (arr) {
        // Initialize fields of the AdptArray object
        arr->size = 0;
        arr->copy_func = copy_func;
        arr->del_func = del_func;
        arr->print_func = print_func;
        arr->arr = NULL;
        return arr;
    }else{ return NULL; }
}

// Function to delete an AdptArray object and its contents
void DeleteAdptArray(PAdptArray arr) {
    if (arr == NULL) {
        return;
    }
    if (arr->arr != NULL) {
        // Delete each element in the array using the del_func function
        for (int i = 0; i < arr->size; i++) {
            if (arr->arr[i] != NULL) {
                arr->del_func(arr->arr[i]);
                arr->arr[i] = NULL;
            }
        }
        // Free the memory used by the array
        free(arr->arr);
        arr->arr = NULL;
    }
    // Free the memory used by the AdptArray object
    free(arr);
}

// Function to set an element in the array at a specific index
Result SetAdptArrayAt(PAdptArray arr, int index, PElement element) {
    if (arr == NULL || index < 0) {
        return FAIL;
    }
    if (index >= arr->size) {
        // If the index is outside of the current array size, allocate more memory
        int newSize = index + 1;
        PElement* new_arr = (PElement*) realloc(arr->arr, sizeof(PElement) * newSize);
        if (new_arr) {
            // Initialize any new elements in the array to NULL
            for (int i = arr->size; i < newSize; i++) {
                new_arr[i] = NULL;
            }
            // Update the AdptArray object with the new array and size
            arr->arr = new_arr;
            arr->size = newSize;
        } else { return FAIL; }
        
    }
    // If the element at the index is not NULL, delete it using the del_func function
    if (arr->arr[index] != NULL) {
        arr->del_func(arr->arr[index]);
    }
    // Copy the new element using the copy_func function and store it in the array
    arr->arr[index] = arr->copy_func(element);
    return SUCCESS;
}

// Function to get an element from the array at a specific index
PElement GetAdptArrayAt(PAdptArray arr, int index) {
    // Check for null pointer or invalid index
    if (arr == NULL || index < 0 || index >= arr->size) {
        return NULL;
    }
    // Check if the element at index is NULL
    if (arr->arr[index] == NULL) {
        return NULL;
    }
    // Copy the element at index and return it
    return arr->copy_func(arr->arr[index]);;
}

int GetAdptArraySize(PAdptArray arr) {
    // Check for null pointer
    if (arr == NULL) {
        return -1;
    }
    // Return the size of the array
    return arr->size;
}

void PrintDB(PAdptArray arr) {
    // Check for null pointer
    if (arr == NULL) {
        return;
    }
    // Iterate through the array and print each element
    for (int i = 0; i < arr->size; i++) {
        if (arr->arr[i] != NULL) {
            arr->print_func(arr->arr[i]);
        }
    }
}

