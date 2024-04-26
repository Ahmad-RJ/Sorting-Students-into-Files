#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
using namespace std;
int main(int argc, char* args[]){
	int stdfile = open(args[1], O_RDONLY);
	int num = argc - 2;
	int secfiles[num];
	ssize_t secfilesbw[num];
	for (int i = 0; i < argc-2; i++){
		secfiles[i] = open(args[i + 2], O_WRONLY);
	}
	ssize_t bytesRead;
	char num1;
	bytesRead = read(stdfile, &num1, 1); 
	int number = stoi(&num1);
	int check = 0;
	pid_t pid = 0;
	int fd[2];
	int fdp[2];
	pipe(fd);
	pipe(fdp);
	while (check < number){
		pid = fork();
		if (pid == 0){
			char lineo[19];
			read(fd[0], &lineo, 19);
			char linetowrite[14];
			char sec = lineo[18];
			int secnum = stoi(&sec);
			for (int i = 0, j = 1; i < 13; i++, j++){
				linetowrite[i] = lineo[j];
			}
			linetowrite[13] = '\n';
			secfilesbw[secnum-1] = write(secfiles[secnum-1], &linetowrite, 14);
			int parent, child;
			read(fdp[0], &parent, sizeof(parent));
			cout << "Parent ID: " << parent << endl;
			cout << "Child " << check + 1 << " ID: " << getpid() << endl << endl;
			exit(0);
		}
		else{
			char line[19];
			bytesRead = read(stdfile, &line, 19);
			check++;
			write(fd[1], &line, 19);
			int parent = getpid();
			write(fdp[1], &parent, sizeof(parent));
			wait(NULL);
			continue;
		}
	}
	close(fd[0]);
	close(fd[1]);
	close(fdp[0]);
	close(fdp[1]);
	close(stdfile);
	for (int i = 0; i < argc-2; i++){
		close(secfiles[i]);
	}
	
	return 0;
}
