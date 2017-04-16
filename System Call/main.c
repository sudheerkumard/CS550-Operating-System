#include<unistd.h>
#include<time.h>
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
	
/*
	Main Function Starts here	

	*/
int my_xtime(struct timespec *ktime){
	
	//printf("\n Call to system to display time:");

	int ret_value= syscall(326,ktime);
	if(ret_value != 0){
		printf("\nSyscall not executed.\n");
	}

	printf("\nGot time from kernel :%s", ctime(&ktime->tv_sec)      );
	printf("\n%lld : %lld\n",ktime->tv_nsec, ktime->tv_sec);
	return 0;
}	
int main(void){
	/*
	timespec *ptr to pass to kernel.
	give a syscall
	print the results returned by kernel in the timespec pointer.
	*/

	struct timespec *ktime = malloc(sizeof(struct timespec));
	int retval= my_xtime(ktime);
	if(retval != 0){
		printf("\napplication level: program showed time.\n");
		free(ktime);
		exit(1);
	}	
	free(ktime);
	ktime=NULL;
	
	int choice=0;
	do{
	printf("\nSelect a method :\n0. Divide by zero\n1.Pointer Dereference\n2.Infinite recursion\n3.Infinite loop.\n4.Wrong Return Value\n5.kmalloc\n6.Crash\n7.Exit\n.Enter Choice:\t");
	scanf("%d",&choice);
	syscall(327,choice);
	}while(choice != 7);
	

	/*
	give a call to kernel.
	it wont retun.	
	
	*/
	
	//syscall(327,0);		// divide by zero. killed.
	//syscall(327,1);		// pointer dereference. killed.
	//syscall(327,2);		// stack overflow by infinite recursion. soft lockup. crashed.
	//syscall(327,3);		// system infinite loop.
	//syscall(327,4);		// wrong return value. didnt not panic because of wrong return value.
	//syscall(327,5);		// kmalloc. crash.
	//syscall(327,6);		//crash.
	return 0;
}
