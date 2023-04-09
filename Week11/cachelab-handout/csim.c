// 2019011449

#include "cachelab.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef int bool_t;
#define TRUE 1
#define FALSE 0

#define LINE_LENGTH 43

typedef unsigned long long address_t;
typedef unsigned long long tag_t;

typedef struct cache_line_t {
    int valid;
    tag_t tag;
    int lru;
} cache_line_t;

typedef struct cache_t {
    int s;
    int E;
    int b;
    int lru_counter;
    cache_line_t** memory;
} cache_t;

typedef struct cache_result_t {
    int hit_count;
    int miss_count;
    int eviction_count;
} cache_result_t;

void help(void);
void init_cache(cache_t* cache, int s, int E, int b);
void op_cache(cache_t* cache, bool_t v, address_t memory_address, cache_result_t* cache_result);
cache_result_t csim(char* tf, bool_t v, int s, int E, int b);
void destroy_cache(cache_t* cache);

int main(int argc, char** argv) {

    char opt;
    bool_t v_flag;
    int s_flag, E_flag, b_flag;
    char* t_flag;     // file descriptor
    cache_result_t cache_result;

    // Initialize the flags
    v_flag = FALSE;
    s_flag = 0;
    E_flag = 0;
    b_flag = 0;
    t_flag = NULL;
    // Get flags.
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (opt) {
            case 'h':
                help();
                break;
            case 'v':
                v_flag = TRUE;
                break;
            case 's':
                s_flag = atoi(optarg);
                break;
            case 'E':
                E_flag = atoi(optarg);
                break;
            case 'b':
                b_flag = atoi(optarg);
                break;
            case 't':
                t_flag = optarg;
                break;
            default:
                help();
                break;
        }
    }

    // Check flags.
    if (s_flag == 0 || E_flag == 0 || b_flag == 0 || t_flag == NULL) {
        printf("N/A flags.\n");
        return 0;
    }

    cache_result = csim(t_flag, v_flag, s_flag, E_flag, b_flag);
    printSummary(cache_result.hit_count, cache_result.miss_count, cache_result.eviction_count);

    return 0;
}

void help(void) {
    printf("-h\tPrint this help message.\n"
           "-v\tVerbose.\n"
           "-s <n>\tnumber of bits for set.\n"
           "-E <n>\tnumber of line in each set.\n"
           "-b <n>\tnumber of bits in each block.\n"
           "-t <f>\tname of trace file\n");
}

void init_cache(cache_t* cache, int s, int E, int b) {

    int i, j;

    // set
    cache->s = s;
    cache->E = E;
    cache->b = b;
    cache->lru_counter = 0;
    cache->memory = (cache_line_t**) malloc((1 << s) * sizeof(cache_line_t*));
    // line per set
    for (i = 0; i < (1 << s); i++) {
        (cache->memory)[i] = (cache_line_t*) malloc(E * sizeof(cache_line_t));
        for (j = 0; j < E; j++) {
            (cache->memory)[i][j].valid = FALSE;
            (cache->memory)[i][j].tag = 0;
            (cache->memory)[i][j].lru = 0;
        }
    }

}

void op_cache(cache_t* cache, bool_t v, address_t memory_address, cache_result_t* cache_result) {

    int set_mask;
    int line_index;
    int set_index;
    tag_t tag;

    cache_line_t* cache_line;

    int min_lru;
    int eviction_index;
    
    set_mask = (1 << cache->s) - 1;
    set_index = (memory_address >> cache->b) & set_mask;
    tag = (memory_address >> (cache->b + cache->s));

    // I. hit
    for (line_index = 0; line_index < cache->E; line_index++) {
        cache_line = &((cache->memory)[set_index][line_index]);
        // TODO the case: tag is same but valid is FALSE ?
        if (cache_line->tag == tag)
            if (cache_line->valid == TRUE) {
                cache_result->hit_count++;
                // verbose
                if (v == TRUE)
                    printf("hit ");
                cache_line->lru = (cache->lru_counter)++;
                return;
            }
    }

    // II. miss
    cache_result->miss_count++;
    //verbose
    if (v == TRUE)
        printf("miss ");
    min_lru = ~(1 << 31);
    for (line_index = 0; line_index < cache->E; line_index++) {
        cache_line = &((cache->memory)[set_index][line_index]);
        // II-1. find empty line.
        if (cache_line->valid == FALSE) {
            cache_line->valid = TRUE;
            cache_line->tag = tag;
            cache_line->lru = (cache->lru_counter)++;
            return;
        }
        // find min_lru for eviction.
        if (cache_line->lru < min_lru) {
            min_lru = cache_line->lru;
            eviction_index = line_index;
        }
    }

    // II-2. eviction.
    cache_result->eviction_count++;
    //verbose
    if (v == TRUE)
        printf("eviction ");
    cache_line = &((cache->memory)[set_index][eviction_index]);
    cache_line->valid = TRUE;
    cache_line->tag = tag;
    cache_line->lru = (cache->lru_counter)++;
    return ;
}

cache_result_t csim(char* tf, bool_t v, int s, int E, int b) {

    FILE *fp;
    char instr;
    address_t memory_address;
    int byte_length;

    cache_t cache;
    cache_result_t cache_result;

    cache_result.hit_count = 0;
    cache_result.miss_count = 0;
    cache_result.eviction_count = 0;

    fp = fopen(tf, "r");
    if (fp == NULL) {
        printf("There is no file \"%s\".\n", tf);
        return cache_result;
    }

    init_cache(&cache, s, E, b);

    while (fscanf(fp, " %c %llx, %d", &instr, &memory_address, &byte_length) == 3) {
        switch (instr) {
            case 'L':
            case 'S':
                // verbose
                if (v == TRUE)
                    printf("%c %llx,%d ", instr, memory_address, byte_length);

                op_cache(&cache, v, memory_address, &cache_result);
                // verbose
                if (v == TRUE)
                    printf("\n");
                break;
            case 'M':
                if (v == TRUE)
                    printf("%c %llx,%d", instr, memory_address, byte_length);

                op_cache(&cache, v, memory_address, &cache_result);
                op_cache(&cache, v, memory_address, &cache_result);

                if (v == TRUE)
                    printf("\n");
                break;
            default:
                break;
        }
    }

    destroy_cache(&cache);
    fclose(fp);

    return cache_result;
}

void destroy_cache(cache_t* cache) {

    int i;

    for (i = 0; i < (1 << (cache->s)); i++)
        free((cache->memory)[i]);

    free(cache->memory);
}

