#include <stdio.h>
#include <stdlib.h>

typedef struct RandomGenerator
{   
    struct RandomOperations
    {
        int (*next) (struct RandomGenerator *);
        void (*destroy) (struct RandomGenerator *);
    } *ops;
    unsigned a;
    unsigned c;
    int start_value;
    unsigned mod;
    unsigned cur_value;
} RandomGenerator;

static int
generator_next_value(RandomGenerator *gen)
{
    if(gen == NULL){
        fprintf(stderr, "Generator is NULL\n");
        return 0;
    }
    gen->cur_value = (gen->a * gen->cur_value + gen->c) % gen->mod;
    return gen->cur_value;
}

static void
generator_destroy(RandomGenerator *gen)
{   
    if(gen == NULL){
        fprintf(stderr, "Generator is NULL\n");
        return;
    }
    if(gen->ops == NULL){
        fprintf(stderr, "Generator's ops is NULL\n");
        return;
    }
    free(gen->ops);
    free(gen);
    return;
}

static const struct RandomOperations GLOBAL_RAND_OP = {&generator_next_value, &generator_destroy};

RandomGenerator 
*random_create(int seed)
{
    RandomGenerator *my_gen = (RandomGenerator *) calloc(1, sizeof(*my_gen));
    if(my_gen == NULL){
        fprintf(stderr, "Allocated error\n");
        return NULL;
    }

    enum { A = 1103515245, C = 12345, MOD = 0x80000000 };
    my_gen->a = A;
    my_gen->c = C;
    my_gen->mod = MOD;
    my_gen->start_value = seed;
    my_gen->cur_value = seed;
    my_gen->ops = (struct RandomOperations *) calloc(1, sizeof(*my_gen->ops));
    if(my_gen->ops == NULL){
        fprintf(stderr, "Allocated error\n");
        free(my_gen);
        return NULL;
    }

    my_gen->ops->next = GLOBAL_RAND_OP.next;
    my_gen->ops->destroy = GLOBAL_RAND_OP.destroy;
    return my_gen;
}