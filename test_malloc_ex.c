#include "includes/libft_malloc.h"
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>

// Color definitions
#define COLOR_BLACK        "\x1b[30m"
#define COLOR_RED          "\x1b[31m"
#define COLOR_GREEN        "\x1b[32m"
#define COLOR_YELLOW       "\x1b[33m"
#define COLOR_YELLOW_BOLD  "\x1b[1;33m"
#define COLOR_BLUE         "\x1b[34m"
#define COLOR_BLUE_BOLD    "\x1b[1;34m"
#define COLOR_MAGENTA      "\x1b[35m"
#define COLOR_CYAN         "\x1b[36m"
#define COLOR_CYAN_BOLD    "\x1b[1;36m"
#define COLOR_WHITE        "\x1b[37m"
#define COLOR_RESET        "\x1b[0m"
#define COLOR_BOLD         "\x1b[1m"

// Test configuration
#define MAX_TEST_ALLOCS 2000
#define THREAD_COUNT 4
#define STRESS_ITERATIONS 10000

// Global test statistics
static int tests_passed = 0;
static int tests_failed = 0;
static int current_test = 0;

void print_test_header(const char *test_name) {
    current_test++;
    printf(COLOR_CYAN_BOLD "\n=== TEST %d: %s ===\n" COLOR_RESET, current_test, test_name);
}

void print_test_result(const char *test_name, int passed) {
    if (passed) {
        printf(COLOR_GREEN "✓ PASSED: %s\n" COLOR_RESET, test_name);
        tests_passed++;
    } else {
        printf(COLOR_RED "✗ FAILED: %s\n" COLOR_RESET, test_name);
        tests_failed++;
    }
}

void print_final_results(void) {
    printf(COLOR_BOLD "\n" "========== TEST SUMMARY ==========\n" COLOR_RESET);
    printf("Total tests: %d\n", tests_passed + tests_failed);
    printf(COLOR_GREEN "Passed: %d\n" COLOR_RESET, tests_passed);
    printf(COLOR_RED "Failed: %d\n" COLOR_RESET, tests_failed);
    
    if (tests_failed == 0) {
        printf(COLOR_GREEN COLOR_BOLD "ALL TESTS PASSED! 🎉\n" COLOR_RESET);
    } else {
        printf(COLOR_RED COLOR_BOLD "SOME TESTS FAILED! ❌\n" COLOR_RESET);
    }
}

// ============================================================================
// BASIC FUNCTIONALITY TESTS
// ============================================================================

void test_malloc_basic(void) {
    print_test_header("Basic malloc functionality");
    
    // Test different sizes
    void *ptr1 = malloc(0);
    void *ptr2 = malloc(1);
    void *ptr3 = malloc(128);
    void *ptr4 = malloc(1024);
    void *ptr5 = malloc(4096);
    void *ptr6 = malloc(100000);
    
    int passed = (ptr1 == NULL) && (ptr2 != NULL) && (ptr3 != NULL) && 
                 (ptr4 != NULL) && (ptr5 != NULL) && (ptr6 != NULL);
    
    // Test writing to allocated memory
    if (passed && ptr2) {
        *(char*)ptr2 = 'A';
        passed = (*(char*)ptr2 == 'A');
    }
    
    if (passed && ptr3) {
        memset(ptr3, 'B', 128);
        passed = (((char*)ptr3)[0] == 'B' && ((char*)ptr3)[127] == 'B');
    }

	    show_alloc_mem();

    free(ptr1); // Should be safe
    free(ptr2);
    free(ptr3);
    free(ptr4);
    free(ptr5);
    free(ptr6);

    
    print_test_result("Basic malloc functionality", passed);
}

void test_free_basic(void) {
    print_test_header("Basic free functionality");
    
    void *ptr1 = malloc(100);
    void *ptr2 = malloc(200);
    void *ptr3 = malloc(300);
    
    int passed = (ptr1 != NULL && ptr2 != NULL && ptr3 != NULL);
    
    // These should not crash
    free(ptr1);
    free(ptr2);
    free(ptr3);
    free(NULL); // Should be safe
    
    print_test_result("Basic free functionality", passed);
}

void test_realloc_basic(void) {
    print_test_header("Basic realloc functionality");
    int passed = 1;
    
    // Test realloc with NULL (should act like malloc)
    void *ptr1 = realloc(NULL, 100);
    passed = passed && (ptr1 != NULL);
    
    // Test expanding
    strcpy((char*)ptr1, "Hello");
    void *ptr2 = realloc(ptr1, 200);
    passed = passed && (ptr2 != NULL) && (strcmp((char*)ptr2, "Hello") == 0);
    
    // Test shrinking
    void *ptr3 = realloc(ptr2, 50);
    passed = passed && (ptr3 != NULL) && (strncmp((char*)ptr3, "Hello", 5) == 0);
    
    // Test realloc to 0 (should act like free)
    void *ptr4 = realloc(ptr3, 0);
    passed = passed && (ptr4 == NULL);
    
    print_test_result("Basic realloc functionality", passed);
}

// ============================================================================
// EDGE CASES TESTS
// ============================================================================

void test_edge_cases(void) {
    print_test_header("Edge cases");
    int passed = 1;
    
    // Multiple frees (should not crash but behavior is undefined)
    void *ptr = malloc(100);
    free(ptr);
    // free(ptr); // Don't actually do this in tests
    
    // Very large allocation
    void *large_ptr = malloc(INTMAX_MAX);
    passed = passed && (large_ptr == NULL); // Should fail gracefully

    // Zero size malloc
    void *zero_ptr = malloc(0);
    passed = passed && (zero_ptr == NULL);
    
    // Alignment test
    for (int i = 1; i <= 64; i++) {
        void *aligned_ptr = malloc(i);
        if (aligned_ptr) {
            // Check alignment (assuming 8-byte alignment)
            passed = passed && (((uintptr_t)aligned_ptr % 8) == 0);
            free(aligned_ptr);
        }
    }
    
    print_test_result("Edge cases", passed);
}

// ============================================================================
// ZONE TESTS (TINY/SMALL/LARGE)
// ============================================================================

void test_zone_allocation(void) {
    print_test_header("Zone allocation (TINY/SMALL/LARGE)");
    int passed = 1;
    
    // TINY allocations (assuming <= 128 bytes)
    void *tiny_ptrs[50];
    for (int i = 0; i < 50; i++) {
        tiny_ptrs[i] = malloc(64);
        passed = passed && (tiny_ptrs[i] != NULL);
        memset(tiny_ptrs[i], 'T', 64);
    }
    
    // SMALL allocations (assuming 129-1024 bytes)
    void *small_ptrs[20];
    for (int i = 0; i < 20; i++) {
        small_ptrs[i] = malloc(512);
        passed = passed && (small_ptrs[i] != NULL);
        memset(small_ptrs[i], 'S', 512);
    }
    
    // LARGE allocations (assuming > 1024 bytes)
    void *large_ptrs[5];
    for (int i = 0; i < 5; i++) {
        large_ptrs[i] = malloc(4096);
        passed = passed && (large_ptrs[i] != NULL);
        memset(large_ptrs[i], 'L', 4096);
    }
    
    // Verify data integrity
    for (int i = 0; i < 50; i++) {
        passed = passed && (((char*)tiny_ptrs[i])[0] == 'T');
        passed = passed && (((char*)tiny_ptrs[i])[63] == 'T');
    }
    
    for (int i = 0; i < 20; i++) {
        passed = passed && (((char*)small_ptrs[i])[0] == 'S');
        passed = passed && (((char*)small_ptrs[i])[511] == 'S');
    }
    
    for (int i = 0; i < 5; i++) {
        passed = passed && (((char*)large_ptrs[i])[0] == 'L');
        passed = passed && (((char*)large_ptrs[i])[4095] == 'L');
    }
    
    // Clean up
    for (int i = 0; i < 50; i++) free(tiny_ptrs[i]);
    for (int i = 0; i < 20; i++) free(small_ptrs[i]);
    for (int i = 0; i < 5; i++) free(large_ptrs[i]);
    
    print_test_result("Zone allocation", passed);
}

// ============================================================================
// COALESCING TESTS
// ============================================================================

void test_block_coalescing(void) {
    print_test_header("Block coalescing");
    int passed = 1;
    
    // Allocate several blocks
    void *ptrs[10];
    for (int i = 0; i < 10; i++) {
        ptrs[i] = malloc(100);
        passed = passed && (ptrs[i] != NULL);
    }
    
    // Free every other block to create fragmentation
    for (int i = 1; i < 10; i += 2) {
        free(ptrs[i]);
    }
    
    // Free remaining blocks (should trigger coalescing)
    for (int i = 0; i < 10; i += 2) {
        free(ptrs[i]);
    }
    
    // Allocate a larger block (should reuse coalesced space)
    void *large_block = malloc(800);
    passed = passed && (large_block != NULL);
    free(large_block);
    
    print_test_result("Block coalescing", passed);
}

// ============================================================================
// STRESS TESTS
// ============================================================================

void test_stress_random_alloc_free(void) {
    print_test_header("Stress test - Random allocations and frees");
    int passed = 1;
    
    srand((unsigned int)time(NULL));
    void *ptrs[1000];
    int allocated[1000] = {0};
    
    for (int iter = 0; iter < 5000 && passed; iter++) {
        int index = rand() % 1000;
        
        if (allocated[index]) {
            // Free existing allocation
            free(ptrs[index]);
            allocated[index] = 0;
        } else {
            // Make new allocation
            size_t size = (rand() % 2048) + 1;
            ptrs[index] = malloc(size);
            if (ptrs[index]) {
                allocated[index] = 1;
                // Write pattern to verify integrity
                memset(ptrs[index], (char)(index & 0xFF), size);
            } else {
                passed = 0;
            }
        }
    }
    
    // Clean up remaining allocations
    for (int i = 0; i < 1000; i++) {
        if (allocated[i]) {
            free(ptrs[i]);
        }
    }
    
    print_test_result("Stress test - Random allocations", passed);
}

void test_realloc_stress(void) {
    print_test_header("Realloc stress test");
    int passed = 1;
    
    void *ptr = malloc(10);
    passed = passed && (ptr != NULL);
    
    // Fill with pattern
	char char_to_fill = 'a';
    memset(ptr, char_to_fill, 10);
    printf("Gradually increase size\n");
    // Gradually increase size
    for (size_t size = 20; size <= 10000 && passed; size *= 2)
	{
		printf("size = %d\n", size);
        void *new_ptr = realloc(ptr, size);
        passed = passed && (new_ptr != NULL);
        printf("realloc size = %d passed = %d\n", size, passed);

        if (new_ptr)
		{
            // Verify old data is preserved
            passed = passed && (((char*)new_ptr)[0] == char_to_fill);
			printf("realloc size = %d [9]=A is [%c] passed = %d\n", size, ((char*)new_ptr)[0], passed);

            passed = passed && (((char*)new_ptr)[9] == char_to_fill);
	        printf("realloc size = %d [9]=A is [%c] passed = %d\n", size, ((char*)new_ptr)[9], passed);

            char_to_fill += 1;
            // Fill new space
            memset(new_ptr, char_to_fill, size);
            ptr = new_ptr;
        }
    }
	printf("Gradually increase size res = %d\n", passed);

	printf("Gradually decrease size\n");

    // Gradually decrease size
    for (size_t size = 5000; size >= 10 && passed; size /= 2) {
        void *new_ptr = realloc(ptr, size);
        passed = passed && (new_ptr != NULL);
        
        if (new_ptr) {
            // Verify data integrity
            passed = passed && (((char*)new_ptr)[0] == char_to_fill);
            ptr = new_ptr;
        }
    }
	printf("Gradually decrease size res = %d\n", passed);


    free(ptr);
    print_test_result("Realloc stress test", passed);
}

// ============================================================================
// THREADING TESTS
// ============================================================================

typedef struct {
    int thread_id;
    int iterations;
    int *success;
} thread_data_t;

void *thread_malloc_test(void *arg) {
    thread_data_t *data = (thread_data_t*)arg;
    void *ptrs[100];
    
    for (int i = 0; i < data->iterations; i++) {
        // Allocate
        for (int j = 0; j < 100; j++) {
            size_t size = (i * j) % 1000 + 1;
            ptrs[j] = malloc(size);
            if (!ptrs[j]) {
                *data->success = 0;
                return NULL;
            }
            memset(ptrs[j], data->thread_id, size);
        }
        
        // Verify and free
        for (int j = 0; j < 100; j++) {
            if (((char*)ptrs[j])[0] != data->thread_id) {
                *data->success = 0;
            }
            free(ptrs[j]);
        }
    }
    
    return NULL;
}

void test_threading(void) {
    print_test_header("Multi-threading test");
    
    pthread_t threads[THREAD_COUNT];
    thread_data_t thread_data[THREAD_COUNT];
    int success = 1;
    
    // Create threads
    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_data[i].thread_id = i + 1;
        thread_data[i].iterations = 100;
        thread_data[i].success = &success;
        
        if (pthread_create(&threads[i], NULL, thread_malloc_test, &thread_data[i]) != 0) {
            success = 0;
            break;
        }
    }
    
    // Wait for threads
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    
    print_test_result("Multi-threading test", success);
}

// ============================================================================
// MEMORY PATTERN TESTS
// ============================================================================

void test_memory_patterns(void) {
    print_test_header("Memory pattern integrity");
    int passed = 1;
    
    // Test different allocation patterns
    struct {
        size_t size;
        char pattern;
    } test_cases[] = {
        {1, 0x01}, {7, 0x07}, {15, 0x0F}, {31, 0x1F},
        {63, 0x3F}, {127, 0x7F}, {255, 0xFF}, {511, 0xAA},
        {1023, 0x55}, {2047, 0xCC}, {4095, 0x33}
    };
    
    void *ptrs[sizeof(test_cases) / sizeof(test_cases[0])];
    
    // Allocate and fill with patterns
    for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        ptrs[i] = malloc(test_cases[i].size);
        passed = passed && (ptrs[i] != NULL);
        
        if (ptrs[i]) {
            memset(ptrs[i], test_cases[i].pattern, test_cases[i].size);
        }
    }
    
    // Verify patterns
    for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        if (ptrs[i]) {
            char *data = (char*)ptrs[i];
            for (size_t j = 0; j < test_cases[i].size; j++) {
                if (data[j] != test_cases[i].pattern) {
                    passed = 0;
                    break;
                }
            }
        }
    }
    
    // Clean up
    for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        free(ptrs[i]);
    }
    
    print_test_result("Memory pattern integrity", passed);
}

// ============================================================================
// PERFORMANCE TESTS
// ============================================================================

void test_performance(void) {
    print_test_header("Basic performance test");
    
    clock_t start = clock();
    
    // Allocation performance
    void *ptrs[10000];
    for (int i = 0; i < 10000; i++) {
        ptrs[i] = malloc((i % 1000) + 1);
    }
    
    // Free performance
    for (int i = 0; i < 10000; i++) {
        free(ptrs[i]);
    }
    
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Performance: 20000 operations in %f seconds\n", cpu_time);
    print_test_result("Performance test", cpu_time < 1.0); // Should complete in under 1 second
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================

int main(void) {
    printf(COLOR_BOLD COLOR_CYAN "🧪 COMPREHENSIVE MALLOC TEST SUITE 🧪\n" COLOR_RESET);
    printf("Testing custom malloc implementation...\n\n");
    
    // Basic functionality tests
    show_alloc_mem();

    test_malloc_basic();
    test_free_basic();
    test_realloc_basic();

    // Edge cases
    test_edge_cases();

    // Zone allocation
    test_zone_allocation();

    // Coalescing
    test_block_coalescing();

    // Memory patterns
    test_memory_patterns();

    // Stress tests
    test_stress_random_alloc_free();
    test_realloc_stress();

    // Threading (comment out if threading isn't implemented)
    test_threading();

    // Performance
    test_performance();

    // Show final memory state
    printf(COLOR_YELLOW "\nFinal memory state:\n" COLOR_RESET);
    show_alloc_mem();
    
    print_final_results();
    
    return (tests_failed == 0) ? 0 : 1;
}
