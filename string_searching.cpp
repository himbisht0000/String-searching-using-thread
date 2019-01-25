#include<stdio.h>
#include<iostream>
#include<time.h>
#include<pthread.h>
using namespace std;

#define MAX_THREAD 4
#define thread1 0
#define thread2 1

pthread_mutex_t lock;
struct data{string original,pattern; int count,start,length_of_big_string,length_of_pattern; };

void *thread_of_sum(void *args)
{
	struct data *work;
	work=(struct data *)args;

	int current,j;
	current=(*work).start;

	pthread_mutex_unlock(&lock);
	int k=0,flag=1;
	
	for(;(current+(*work).length_of_pattern - 1)<(*work).length_of_big_string;)
	{	k=0;
		flag=1;
		//cout<<"in thread "<<current<<endl;
		for(j=current;j<(current+(*work).length_of_pattern);j++,k++)
		{	
			if((*work).original[j]!=(*work).pattern[k])
			{
				flag=0;
				break;				
			}
		}

		if(flag==1)
		{
			(*work).count++;
		}

		current+=MAX_THREAD;
	}
	
}

int main()
{
	int i,n;
	pthread_t thread_id[MAX_THREAD];
	
	struct data container;
	cout<<"enter the string\n";
	cin>>container.original;
	cout<<"enter the pattern\n";
	cin>>container.pattern;

	if(pthread_mutex_init(&lock,NULL)!=0)
	{
		cout<<"error in mutex\n";
		return -1;
	}

	container.count=0;
	container.length_of_big_string=container.original.length();
	container.length_of_pattern=container.pattern.length();	

	//cout<<container.length_of_pattern<<endl;
	//cout<<container.length_of_big_string<<endl;
	time_t t1,t2;
	(void) time(&t1);
	
	for(i=0;i<MAX_THREAD;i++)
	{
		pthread_mutex_lock(&lock);
		container.start=i;
		pthread_create(&thread_id[i],NULL,thread_of_sum,(void *)&container);	
	}

	for(i=0;i<MAX_THREAD;i++)
		pthread_join(thread_id[i],NULL);
	
	
	
	
	(void) time(&t2);	
	cout<<"time taken :"<<(float)(t2-t1)<<endl;
	cout<<"number of thread used :"<<MAX_THREAD<<endl;
	cout<<container.count<<endl;
}