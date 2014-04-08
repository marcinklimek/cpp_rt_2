#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <string>

#define test_errno(info) do {if (errno) {perror(info); exit(EXIT_FAILURE);}} while(0)
 
pthread_key_t   key;
 
void wyswietl() 
{
    std::string* text = (std::string*)pthread_getspecific(key);

    if (text != NULL)
        std::cout << *text << std::endl;
}

void destruktor(void* arg)
{
    std::string* text = (std::string*)arg;
    std::cout << "destruktor " << text << std::endl;

    delete text;
}
//---------------------------------------------------------------------------
 
void* watek(void* arg) 
{
    std::string* text = new std::string( *(std::string*)arg );

    int status = pthread_setspecific(key, text);

    if (status)
        fprintf(stderr, "pthread_setspecific: %s\n", strerror(status));
    else
        std::cout << "address : " << std::hex << text << " " << *text << std::endl;
 
    sleep(1);

    wyswietl();

    std::cout << "thread end\n";
    return NULL;
}
 

#define N 3
 
int main() 
{
    pthread_t id[N];
    int i;
    std::string text[3] = {"abcefgh", "12356789", "!@#$%^&*"}; // prefiksy dla komunikatów z wątków
 
    errno = pthread_key_create(&key, destruktor);
    test_errno("pthread_key_create");
 
    
    for (i=0; i < N; i++) 
    {
        errno = pthread_create(&id[i], NULL, watek, (void*)(&text[i % 3]));
        test_errno("pthread_create");
    }
 
    
    for (i=0; i < N; i++)
        pthread_join(id[i], NULL);
 
    errno = pthread_key_delete(key);
    test_errno("pthread_key_delete");
 
    return EXIT_SUCCESS;
}
