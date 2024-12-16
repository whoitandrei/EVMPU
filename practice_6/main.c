#define _CRT_RAND_S
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int thru(int* arr, int size, int M){
	int k = 0;
	for (int i = 0; i < size * M; ++i){
		k = arr[k];
	}
	return k;
}

unsigned long long measure_time(int *arr, int size, int iterations) {
    union ticks {
        // Определяем объединение (union) для хранения временных меток с использованием 64-битного значения.
        unsigned long long t64; // 64-битное значение для хранения времени в тактах.
        struct s32 { long th, tl; } t32; // Структура, разделяющая 64-битное значение на две 32-битные части (th - старшие 32 бита, tl - младшие 32 бита).
    } start, end; // Переменные start и end для хранения времени до и после выполнения обхода массива.

    // Измерение времени до выполнения обхода массива:
    asm("rdtsc\n" : "=a"(start.t32.th), "=d"(start.t32.tl));
    // Вставка ассемблерной инструкции rdtsc, которая считывает значение временной метки процессора.
    // "=a" и "=d" означают, что старшие 32 бита временной метки будут записаны в start.t32.th, 
    // а младшие 32 бита — в start.t32.tl.

    int k = thru(arr, size, iterations);  

    // Измерение времени после выполнения обхода массива:
    asm("rdtsc\n" : "=a"(end.t32.th), "=d"(end.t32.tl));
    // Вставка ассемблерной инструкции rdtsc снова, чтобы получить значение временной метки после выполнения обхода массива.
    // Результат сохраняется в end.t32.th и end.t32.tl.

    printf("technical output: %d \n", k);

    return end.t64 - start.t64;
    // Возвращает разницу между временными метками (в тактах процессора), что дает время, затраченное на выполнение обхода массива.
    // end.t64 - start.t64 — это общее количество тактов процессора, затраченное на выполнение функции thru.
}


// Функция для обхода массива прямым порядком
void traverse_forward(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i+1;  
    }
	arr[size-1] = 0;
}

// Функция для обхода массива обратным порядком
void traverse_reverse(int *arr, int size) {
    for (int i = size - 1; i >= 0; i--) {
        arr[i] = i-1; 
    }
	arr[0] = size-1;
}

// Функция для случайного обхода массива
void traverse_random(int *buffer, int size) {

    unsigned int* indexes = malloc(sizeof(unsigned int) * size);

    for (unsigned int i = 0; i < size; i++) {
        indexes[i] = i;
    }

    for (unsigned int i = size - 1; i > 0; i--) {
        unsigned int j;
		rand_s(&j);
        j = j % (i + 1);
        unsigned int temp = indexes[i];
        indexes[i] = indexes[j];
        indexes[j] = temp;
    }

    for (unsigned int i = 0; i < size - 1; i++) {
        buffer[indexes[i]] = indexes[i + 1];
    }
    buffer[indexes[size - 1]] = indexes[0];

    free(indexes);
}


int heat(int *arr){
	int data1[100] = {1};
	int data2[100] = {2};
	
	clock_t t = clock();
	while (clock()-t < 1000){
		for (int i = 0; i < 100; ++i){
			for (int j = 0; j < 100; ++j){
				data1[i] *= data2[i];
			}
		}
	}
	return arr[0];
}
	
int main() {
    int iterations = 15;  // Количество повторений для точности
    int size = 256;
	int *arr = (int*)malloc(size * sizeof(int));  // Выделяем память для массива
	
	int res = heat(arr);
	printf("%d ", res);
	
	free(arr);
	int c = 0;
	int results[1000] = { 0 };
	
    while (size <= 256 * 1024 * 32) {

        int *arr = (int*)malloc(size * sizeof(int)); 
		int *arr_f = (int*)malloc(size * sizeof(int));
		int *arr_r = (int*)malloc(size * sizeof(int));
		int *arr_rand = (int*)malloc(size * sizeof(int));
		
        if (arr == NULL) {
            printf("Ошибка выделения памяти!\n");
            return 1;
        }

        printf("\n\nSIZE %d\n", size);
		traverse_forward(arr, size);
		res = thru(arr, size, 1);
		printf("\ntechnical output: %d \n", res);
		
        // Замер времени для каждого способа обхода
        traverse_forward(arr_f, size);
		res = thru(arr_f, size, 1);
		results[c] = measure_time(arr_f, size, iterations) / size / iterations;
		
        traverse_reverse(arr_r, size);
		res = thru(arr_r, size, 1);
        results[c+1] = measure_time(arr_r, size, iterations)/ size / iterations;
		
		traverse_random(arr_rand, size);
		res = thru(arr_rand, size, 1);
		results[c+2] =  measure_time(arr_rand, size, iterations)/ size / iterations;
		
		printf("\ntechnical output: %d \n", res);
		printf("\nres: %d %d %d", results[c], results[c+1], results[c+2]);
        size = (int)(size * 1.1);
        free(arr);  // Освобождаем память
		free(arr_f);
		free(arr_r);
		free(arr_rand);
		c += 3;
    }
	
	FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        // Проверка, открылся ли файл
        printf("Ошибка при открытии файла\n");
        return 1;
    }
	for (int j = 0; j < 3; ++j){
		for (int i = 0; i < c/3; i++) {
			fprintf(file, "%d, ", results[3*i+j]); 
		}
		fprintf(file, "\n");
	}

    // Закрываем файл
    fclose(file);
    return 0;
}