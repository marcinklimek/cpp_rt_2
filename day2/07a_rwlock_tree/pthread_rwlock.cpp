#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include "launch_thread.h"

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)

//----------------------------------
void ms_sleep(const unsigned ms)
{
    struct timespec req;
    req.tv_sec  = (ms / 1000);
    req.tv_nsec = (ms % 1000 * 1000000);
    nanosleep(&req, NULL);
}

pthread_rwlock_t	rw_lock;

void* writer(void* arg)
{
    int* number = (int*)arg;

    while (1)
    {
        printf(" writer #%d - wait\n", *number);
        errno = pthread_rwlock_wrlock(&rw_lock);
        test_errno("pthread_rwlock_wrlock");
        printf(" writer #%d set a new value\n", *number);

        ms_sleep(113);

        printf(" writer #%d unlock\n", *number);
        errno = pthread_rwlock_unlock(&rw_lock);
        test_errno("pthread_rwlock_unlock");

        ms_sleep(317);
    }

    return NULL;
}
//------------------------------------------------------------------------


void* reader(void* arg)
{
    int* number = (int*)arg;

    int errno;
    while (1)
    {
        printf("  reader #%d waiting for access\n", *number);
        errno = pthread_rwlock_rdlock(&rw_lock);
        test_errno("pthread_rwlock_rdlock");
        printf("  reader #%d read a value\n", *number);

        ms_sleep(13);

        printf("  reader #%d unlock\n", *number);
        errno = pthread_rwlock_unlock(&rw_lock);
        test_errno("pthread_rwlock_unlock");

        ms_sleep(13);
    }

    return NULL;
}
//------------------------------------------------------------------------

#define N 5
#define K 2

int main_example_01()
{
    pthread_t id;
    int i;

    pthread_rwlock_init(&rw_lock, NULL);


    for (i=0; i < K; i++)
    {
        errno = pthread_create(&id, NULL, writer, (void*)&i);
        test_errno("pthread_create");
    }

    for (i=0; i < N; i++)
    {
        errno = pthread_create(&id, NULL, reader, (void*)&i);
        test_errno("pthread_create");
    }

    ms_sleep(1500);
}
//------------------------------------------------------------------------


struct node
{
    int key_value;
    node *left;
    node *right;
};


class btree
{
    node *root;
    pthread_rwlock_t rw_lock;

public:
    btree()
    {
        root = NULL;
        pthread_rwlock_init(&rw_lock, NULL);
    }

    ~btree()
    {
        destroy_tree();
        pthread_rwlock_destroy(&rw_lock);
    }

    void insert(int key)
    {
        pthread_rwlock_wrlock(&rw_lock);

        if(root!=NULL)
            insert(key, root);
        else
        {
            root=new node;
            root->key_value=key;
            root->left=NULL;
            root->right=NULL;
        }

        pthread_rwlock_unlock(&rw_lock);
    }

    bool search(int key)
    {
        pthread_rwlock_rdlock(&rw_lock);

        node* result = search(key, root);

        pthread_rwlock_unlock(&rw_lock);

        return result!=nullptr;
    }

    void destroy_tree()
    {
        pthread_rwlock_wrlock(&rw_lock);
        destroy_tree( root );
        pthread_rwlock_unlock(&rw_lock);
    }

private:
    void destroy_tree(node* leaf)
    {
        if(leaf!=NULL)
        {
            destroy_tree(leaf->left);
            destroy_tree(leaf->right);
            delete leaf;
            leaf = NULL;
        }
    }

    void insert(int key, node* leaf)
    {
        if(key < leaf->key_value)
        {
            if(leaf->left!=NULL)
                insert(key, leaf->left);
            else
            {
                leaf->left=new node;
                leaf->left->key_value=key;
                leaf->left->left=NULL;
                leaf->left->right=NULL;
            }
        }
        else if(key >= leaf->key_value)
        {
            if(leaf->right!=NULL)
                insert(key, leaf->right);
            else
            {
                leaf->right=new node;
                leaf->right->key_value=key;
                leaf->right->left=NULL;
                leaf->right->right=NULL;
            }
        }
    }

    node* search(int key, node* leaf)
    {
        if(leaf!=NULL)
        {
            if (key == leaf->key_value)
                return leaf;

            if (key < leaf->key_value)
                return search(key, leaf->left);
            else
                return search(key, leaf->right);
        }
        else
            return NULL;
    }

};


void tree_writer(btree& bt)
{
    for(u_int idx = 0; idx<20; idx++)
    {
        bt.insert(idx);
        std::cout << "writer add " << idx <<std::endl;
        ms_sleep(33);
    }

    std::cout << "writer stop "<< std::endl;
}

void tree_reader(btree& bt)
{
    for(u_int i = 0; i<10; i++)
    {
        for(u_int idx = 0; idx<20; idx++)
        {
            if (bt.search(idx))
                std::cout << "found " << idx <<std::endl;

            ms_sleep(11);
        }
        std::cout << "reader next round " <<std::endl;
    }
}

int main_example_02()
{
    btree bt;

    pthread_t tid_writer;
    pthread_t tid_reader[2];

    launch_thread(&tid_writer, std::bind(tree_writer, std::ref(bt)) );
    launch_thread(&tid_reader[0], std::bind(tree_reader, std::ref(bt)) );
    launch_thread(&tid_reader[1], std::bind(tree_reader, std::ref(bt)) );

    pthread_join(tid_writer, NULL);
    pthread_join(tid_reader[0], NULL);
    pthread_join(tid_reader[1], NULL);
}


int main()
{
    main_example_02();
}
