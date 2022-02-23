#include <iostream>
#include <fstream>
#include <pthread.h>

using namespace std;

int array[64];

void merge(int first, int mid, int last)
{
    // Calculating size of the left half and right half of the array, Then creating two array with the calculated sizes
    int sizeLeft = mid - first + 1;
    int sizeRight = last - mid;
    int Left[sizeLeft], Right[sizeRight];

    // Filling the temporary arrays with the data from the Main array
    for (int i = 0; i < sizeLeft; i++)
        Left[i] = array[first + i];

    for (int j = 0; j < sizeRight; j++)
        Right[j] = array[mid + 1 + j];

    // i --> index for the left half of the array (the temp array)
    // j --> index for the right half of the array (the temp array)
    // k --> index for the Main array
    int i = 0, j = 0, k = first;

    // Comparing elements from the Temporary arrays and Sorting (merging) them into the main array
    while (i < sizeLeft && j < sizeRight)
    {
        if (Left[i] <= Right[j])
        {
            array[k] = Left[i];
            i++;
        }
        else
        {
            array[k] = Right[j];
            j++;
        }
        k++;
    }

    // Move All elements left -if any- in the Right half Temp Array into the Main array
    while (j < sizeRight)
    {
        array[k] = Right[j];
        j++;
        k++;
    }

    // Move All elements left -if any- in the Left half Temp Array into the Main array
    while (i < sizeLeft)
    {
        array[k] = Left[i];
        i++;
        k++;
    }
}

void *mergeSort(void *arg)
{
    int *info1 = (int *)arg;
    int first = info1[0];
    int last = info1[1];

    // Returning condition if the index of the first is equal to the index of the last element
    if (first >= last)
        return NULL;

    int mid = (first + last) / 2;

    // Creating two threads
    // Arrays Left[] & Right[] contains the Indices of the left and right half of the main array to be sorted
    pthread_t th;
    int left[2] = {first, mid};

    pthread_t th1;
    int right[2] = {mid + 1, last};

    // Recursively calling mergeSort function for the left half of the array
    pthread_create(&th, NULL, mergeSort, &left);

    // Recursively calling mergeSort function for the right half of the array
    pthread_create(&th1, NULL, mergeSort, &right);

    // Wait for the threads to finish execution
    pthread_join(th, NULL);
    pthread_join(th1, NULL);

    // Merge the two sorted arrays
    merge(first, mid, last);

    return NULL;
}

int main()
{
    // Getting input file name from the user
    string fileName;
    int size;
    cout << "Enter Input File Name: ";
    cin >> fileName;
    fileName = fileName + ".txt";

    // Open the required file to read the data
    ifstream inputFile;
    inputFile.open(fileName);

    // Reading the Array size from the file
    if (inputFile.is_open())
    {
        inputFile >> size;
        getchar();
    }

    // Reading data of the array from the file to the variable array[]
    for (int i = 0; i < size; i++)
        inputFile >> array[i];

    inputFile.close();

    // Opening the output file to write the output data
    ofstream outputFile;
    outputFile.open("msort-output.txt");

    // if the input array size is a negative value
    if (size < 0)
    {
        outputFile << "Input size can't be less than 1" << endl;
        cout << "Array size error!" << endl;
        return 0;
    }

    // Array info contains the indices of the beginning and the end of the array
    int info[2] = {0, size - 1};

    // Creating thread to call mergeSort function with argument info[]
    pthread_t th;
    pthread_create(&th, NULL, mergeSort, &info);
    pthread_join(th, NULL);

    // Write the Sorted array into the output file
    outputFile << "Sorted Array: ";
    for (int i = 0; i < size; i++)
    {
        outputFile << array[i] << " ";
    }
    cout << "Process Done Successfully.." << endl;
    outputFile.close();

    return 0;
}
