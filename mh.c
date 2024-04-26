#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <semaphore.h> 
#include <time.h> 
#include <unistd.h> 
int hc = 0; 
int pc = 0; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
sem_t h_turn; 
sem_t p_turn;
sem_t passcomplete;
void *heathens(void *arg) 
{ 
    pthread_mutex_lock(&mutex); 
    hc++; 
    printf("A Heathen arrives\n"); 
    pthread_mutex_unlock(&mutex); 
    pthread_mutex_lock(&mutex);
    if (pc > hc) 
    {  
        printf("Prudes have the precedence.\n"); 
        sem_post(&p_turn); 
    } 
    else 
    { 
        printf("Heathens have the precedence.\n"); 
        sem_post(&h_turn); 
    }
    pthread_mutex_unlock(&mutex);
    sem_wait(&h_turn); 
    pthread_mutex_lock(&mutex); 
    hc--; 
    printf("A heathen crosses\n"); 
    if (!hc && pc) 
    { 
        printf("No heathens left\n"); 
        sem_post(&p_turn); 
    }
    pthread_mutex_unlock(&mutex); 
    return NULL; 
}
void *prudes(void *arg) 
{ 
    pthread_mutex_lock(&mutex); 
    pc++; 
    printf("A prude arrives\n");
    pthread_mutex_unlock(&mutex); 
    pthread_mutex_lock(&mutex);
    if (hc > pc) 
    { 
        printf("Heathens have the precedence.\n"); 
        sem_post(&h_turn); 
    } 
    else 
    { 
        printf("Prudes have the precedence.\n"); 
        sem_post(&p_turn); 
    }
    pthread_mutex_unlock(&mutex);
    sem_wait(&p_turn); 
    pthread_mutex_lock(&mutex); 
    printf("A Prude crosses\n"); 
    pc--; 
    if (!pc && hc) 
    { 
        printf("No prudes left\n"); 
        sem_post(&h_turn); 
    } 
    pthread_mutex_unlock(&mutex); 
    return NULL; 
} 
int main(int argc, char *argv[]) 
{ 
    int num_h, num_p;
    printf("enter the number of heathens: "); 
    scanf("%d", &num_h);
    printf("enter the number of prudes: "); 
    scanf("%d", &num_p); 
    pthread_t *ht = (pthread_t *)malloc(num_h * sizeof(pthread_t));
    pthread_t *pt = (pthread_t *)malloc(num_p * sizeof(pthread_t));
    sem_init(&h_turn, 0, 0); 
    sem_init(&p_turn, 0, 0); 
    printf("\n --- Start --- \n");
    int count = (num_h < num_p) ? num_h : num_p;
    for (int i = 0; i < count; i++) {
        pthread_create(&ht[i], NULL, heathens, NULL);
        pthread_create(&pt[i], NULL, prudes, NULL);
    }
    for (int i = count; i < num_h; i++)
        pthread_create(&ht[i], NULL, heathens, NULL);
    for (int i = count; i < num_p; i++)
        pthread_create(&pt[i], NULL, prudes, NULL);
    for (int i = 0; i < num_h; i++) 
        pthread_join(ht[i], NULL); 
    for (int i = 0; i < num_p; i++) 
        pthread_join(pt[i], NULL); 
    printf("\n --- End --- \n");
    free(ht);
    free(pt);
    sem_destroy(&h_turn);
    sem_destroy(&p_turn);
    return 0; 
}
