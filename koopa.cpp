#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

struct node{
	node *next;
};

struct arg : node{
	string argument;
	
	char* to_a(){
		return 0;
	}
};

struct job : node{
	char* command;
	arg *root;

	bool hasNext(){
		return next != NULL;
	}
};

void separate_into_jobs(int argc, char *argv[], job *root){
	root->command = argv[0];
	arg *walker = root->root;
	for(int x = 1; x < argc && argv[x] != "&"; x++){
		walker->argument = argv[x];
		walker->next = new arg;
		walker = walker->next;
	}
}

bool run_job(job *job){
	return execv(*job->command, job->root->to_a);
}

void run_jobs(job *root, job *red){
	while(run_job(red) && red->hasNext()){
		red = red->next;
	}
}

int main(int argc, char *argv[]){
	job *root;
	job *red;
	separate_into_jobs(argc, argv, root);
	run_jobs(root, red);	
}


