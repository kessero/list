#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

//liczba watkow 3
pthread_t tid[3];

struct node
{
    int data;
    struct node *next;
}*head;
struct node *n;
pthread_mutex_t lock;

//funkcja dodajaca element do struktury
void add( int num )
{
    struct node *temp;
    temp=(struct node *)malloc(sizeof(struct node));
    temp->data=num;
    if (head== NULL)
    {
    head=temp;
    head->next=NULL;
    }
    else
    {
    temp->next=head;
    head=temp;
    }
}
void append(int num)
{
    struct node *temp,*right;
    temp= (struct node *)malloc(sizeof(struct node));
    temp->data=num;
    right=(struct node *)head;
    while(right->next != NULL)
    right=right->next;
    right->next =temp;
    right=temp;
    right->next=NULL;
}
//funkcja dodajaca element do listy + mutex
void push_back(int num)
{
  pthread_mutex_lock(&lock);
    int c=0;
    struct node *temp;
    temp=head;
    if(temp==NULL)
    {
    add(num);
    }
    else
    {
    while(temp!=NULL)
    {
        if(temp->data<num)
        c++;
        temp=temp->next;
    }
    if(c==0)
        add(num);
    else if(c<count())
        addafter(num,++c);
    else
        append(num);
    }
    pthread_mutex_unlock(&lock);
}

void addafter(int num, int loc)
{
    int i;
    struct node *temp,*left,*right;
    right=head;
    for(i=1;i<loc;i++)
    {
    left=right;
    right=right->next;
    }
    temp=(struct node *)malloc(sizeof(struct node));
    temp->data=num;
    left->next=temp;
    left=temp;
    left->next=right;
    return;
}
//f-ncja zdejmujaca element z listy
int pop_front(int num)
{

    struct node *temp, *prev;
    temp=head;
    while(temp!=NULL)
    {
    if(temp->data==num)

    {
        if(temp==head)
        {
        head=temp->next;
        free(temp);
        return 1;
        }
        else
        {
        prev->next=temp->next;
        free(temp);


        return 1;
        }

    }
    else
    {
        prev=temp;
        temp= temp->next;
    }
    }
    return 0;
}
//funkcja wyswietlajaca zawartosc listy
void  display(struct node *r)
{
    r=head;
    if(r==NULL)
    {
    return;
    }
    while(r!=NULL)
    {
    printf("%d ",r->data);
    r=r->next;
    }
    printf("\n");
}
int count()
{
    struct node *n;
    int c=0;
    n=head;
    while(n!=NULL)
    {
    n=n->next;
    c++;
    }
    return c;
}
//funkcja obslugujaca watki
void* threadWork(void *arg)
{
  int i;
  pthread_t id = pthread_self();
  //watek 0 dodaje do listy
  if(pthread_equal(id, tid[0])){
    //peta dodajaca zadana ilosc elementow do listy
    for(i = 0; i<1000; i++){

    printf("\n Add value");
      push_back(i);
      display(n);
    }
    //pozostale watki zdejmuja z listy w ramach wyscigu z uzyciem mutex
  }else if(pthread_equal(id, tid[1])){
    pthread_mutex_lock(&lock);
    for(i = 0; i<1000; i++){
      if(pop_front(i)){
          printf("%d pop_front del successfully 1\n",i);

      }else{
          printf("%d not found in the list 1\n",i);
          sleep(1);
        }

    pthread_mutex_unlock(&lock);
        }
      }else if(pthread_equal(id, tid[2])){
        pthread_mutex_lock(&lock);
        for(i = 0; i<1000; i++){
      if(pop_front(i)){
          printf("%d pop_front del successfully 2\n",i);
      }else{
          printf("%d not found in the list 2\n",i);
          sleep(1);
        }
          pthread_mutex_unlock(&lock);
  }
}

    return NULL;
}

int  main()
{
    int i,num;
    head=NULL;
    int err;
    while(i < 3)
    {
        err = pthread_create(&(tid[i]), NULL, &threadWork, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}
