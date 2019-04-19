

typedef struct {
    int n;
    void* (*task)(void*);
} Farm;