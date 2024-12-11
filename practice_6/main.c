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
        arr[i] = i+1;  // Просто доступ к элементу для измерения времени
    }
	arr[size-1] = 0;
}

// Функция для обхода массива обратным порядком
void traverse_reverse(int *arr, int size) {
    for (int i = size - 1; i >= 0; i--) {
        arr[i] = i-1;  // Просто доступ к элементу для измерения времени
    }
	arr[0] = size-1;
}

// Функция для случайного обхода массива
void traverse_random(int *arr, int size) {
    int *indices = (int*)malloc(size * sizeof(int));  // Выделяем память для индексов
    if (indices == NULL) {
        printf("Ошибка выделения памяти для индексов!\n");
        exit(1);
    }

    // Инициализация индексов от 0 до size-1
    for (int i = 0; i < size; i++) {
        indices[i] = i;
    }

    // Перемешивание индексов с использованием алгоритма Фишера-Йейтса
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    // Доступ к элементам в случайном порядке
    for (int i = 0; i < size; i++) {
        arr[i] = indices[i];  // Просто доступ к элементу для измерения времени
    }

    free(indices);  // Освобождаем память для индексов
}



int heat(int *arr){
	int data1[100] = {1};
	int data2[100] = {2};
	
	clock_t t = clock();
	while (clock()-t < 1000){
		for (int i = 0; i < 100; ++i){
			data1[i] += data2[i];
		}
	}
	return arr[0];
}
	
int main() {
    int iterations = 5;  // Количество повторений для точности
    int size = 256;
	int *arr = (int*)malloc(size * sizeof(int));  // Выделяем память для массива
	
	int res = heat(arr);
	printf("%d ", res);
	
	traverse_forward(arr, size);
	res = thru(arr, size, 1);
	printf("%d ", res);
	
	free(arr);
	int c = 0;
	int results[58 * 3];
	
    while (size <= 8388608) {

        int *arr = (int*)malloc(size * sizeof(int)); 
		
        if (arr == NULL) {
            printf("Ошибка выделения памяти!\n");
            return 1;
        }

        printf("\n\nSIZE %d\n", size);
		traverse_forward(arr, size);
		res = thru(arr, size, 1);
		printf("\ntechnical output: %d \n", res);

        // Замер времени для каждого способа обхода
        traverse_forward(arr, size);
		results[c] = measure_time(arr, size, iterations) / size / iterations;
		
        traverse_reverse(arr, size);
        results[58+c] = measure_time(arr, size, iterations)/ size / iterations;
		
		traverse_random(arr, size);
		results[58 * 2 + c] =  measure_time(arr, size, iterations)/ size / iterations;
		printf("\nres: %d %d %d", results[c], results[58 +c], results[58*2 +c]);
        size = (int)(size * 1.2);
        free(arr);  // Освобождаем память
		c++;
    }
	
	FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        // Проверка, открылся ли файл
        printf("Ошибка при открытии файла\n");
        return 1;
    }
	for (int j = 0; j < 3; ++j){
		for (int i = 0; i < 58; i++) {
			fprintf(file, "%d, ", results[58*j +i]); 
		}
		fprintf(file, "\n");
	}

    // Закрываем файл
    fclose(file);
    return 0;
}