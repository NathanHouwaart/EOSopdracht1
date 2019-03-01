 //
// Created by Nathan Houwaart on 2019-02-19.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

//Voorbeeld voor aanroepen:
//./opdracht2 4

using namespace std;

int main(int argc, char *argv[]){

    if(argc != 2){
        cout << "Geef een argument mee!" << endl;
        exit(1);
    }

    int fd1, fd2;
    int offset = atoi(argv[1]);
    char buf[2];


    /* read */
    fd1 = open("myfile.txt", O_RDONLY);
    fd2 = open("outputfile.txt", O_CREAT | O_WRONLY, 0600);

    if(fd1 == -1 ||fd2 == -1){
        printf("Failed to open and read the file.\n");
        exit(1);
    }

    off_t endindex = lseek(fd1, 0, SEEK_END);
    cout << endindex << endl;

    int index = 0;

    while(true){
        lseek(fd1, index, SEEK_SET);
        read(fd1, buf, 1);
        buf[1] = '\0';
        index += offset;
        write(fd2, buf, 1);
        if (index >= endindex){
            index -= static_cast<int>(endindex);

        }
        if (index == 0){
            break;
        }

    }


    close(fd1);
    close(fd2);


    return 0;
}



//    /* write */

//
//    if(fd == -1){
//        printf("Failed to create and open the file.\n");
//        exit(1);
//    }
//    write(fd, "Hello, World!\n", 13);
//    close(fd);