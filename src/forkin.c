// Name:    Zachary Dehaan
// File:    forkin.c
// 
// CS 470 - Lab 2
// I pledge that all work is my own.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>

const char* in_file = "input.txt";      // filename for the input file
const char* out_file = "output.txt";    // filename for the output file

/** 
 * PRINT_PRIMES
 * 
 * Print the prime numbers from 2 to N that are written in the file "input.txt".
 * 
 * Parameters:
 *      1. file_ptr - pointer to the file
 * */
void print_primes(FILE *file_ptr) {
    char *buffer = NULL;    // buffer to hold each number
    size_t buffer_size = 0; // size of the buffer
    ssize_t line_size;      // size of the line in the file

    line_size = getline(&buffer,&buffer_size,file_ptr); // get the first prime
    printf("Primes:\n");

    // iterate until reaching the end of the file
    while(line_size > 0) {
        int curr = atoi(buffer);    // convert to an int
        printf("%d, ",curr);        // print the prime to STDOUT
        line_size = getline(&buffer,&buffer_size,file_ptr); // move to next line
    }
    printf("\n");   // separator
}

/**
 * SIEVE
 * 
 * Runs the Sieve of Eratosthenes algorithm using the fork() system call. Each child process
 * handles removing the multiples of a prime between 2 and N. The child processes are forked
 * during a while(1) loop where each child process runs one iteration before waiting for the
 * next and breaking out of the loop. The loop ends when the all primes between 2 and N have
 * been found, then the parent process finishes by printing the primes.
 * 
 * Parameters: NONE
 * */
void sieve() {
    // Open the files
    FILE *in_ptr = fopen(in_file,"r+");     // pointer for input file
    FILE *out_ptr = fopen(out_file,"a+");   // pointer for output file

    // Error handling
    if(!in_ptr) {
        perror("ERROR: Could not open file");
    }
    if(!out_ptr) {
        perror("ERROR: Could not open file");
    }

    // Get first line
    char *buffer = NULL;
    size_t buffer_size = 0;
    ssize_t line_size;
    line_size = getline(&buffer,&buffer_size,in_ptr);

    // Continue only if the file has content
    if(line_size > 0) {
        int count = 1;      // counter for the number of primes processed
        pid_t pid = fork(); // create a child process! yay

        // Error handling
        if(pid < 0) {
            printf("ERROR: Could not create child process.\n");
            exit(-1);
        }

        // Child process created successfully:
        else {
            // Child process
            if(pid == 0) {
                bool status = true; // will be false when all primes between 2 and N have been processed
                while(1) {
                    int i = 1;      // start at 1 since the first element has been processed

                    // All elements accessed in this loop are primes that have already been processed.
                    // The loop will add them to the output file until we reach a new number that hasn't been processed.
                    while(i != count && line_size > 0) {    // loop until i equals the total number of primes processed
                        int current = atoi(buffer);         // convert to an int
                        fprintf(out_ptr,"%d\n",current);    // print the prime t
                        line_size = getline(&buffer,&buffer_size,in_ptr);
                        if(line_size == -1) {   // if we reach the end of the file, we are done (end all child processes)
                            status = false;
                            break;
                        }
                        i++;
                    }
                    if(!status) {
                        break;  // ends all child processes
                    }

                    // Remove multiples
                    int current_prime = atoi(buffer);
                    fprintf(out_ptr,"%d\n",current_prime);  // write the current prime to the output file
                    line_size = getline(&buffer,&buffer_size,in_ptr);   // get next number

                    // iterate until reaching end of file
                    while(line_size > 0) {
                        int curr_num = atoi(buffer);            // the current number
                        if(curr_num % current_prime != 0) {
                            fprintf(out_ptr,"%d\n",curr_num);   // write to output if not divisible by the current prime
                        }
                        line_size = getline(&buffer,&buffer_size,in_ptr);
                    }

                    // Close files temporarily
                    fclose(in_ptr);
                    fclose(out_ptr);

                    // Overwriting the input.txt file:
                    // Remove the file named "input.txt"
                    remove(in_file);

                    // Rename "output.txt" as "input.txt"
                    rename(out_file,in_file);

                    // Open them back up
                    in_ptr = fopen(in_file,"r+");
                    out_ptr = fopen(out_file,"a+");

                    // Error handling
                    if(!in_ptr) {
                        perror("ERROR: Could not open file");
                    }
                    if(!out_ptr) {
                        perror("ERROR: Could not open file");
                    }

                    // Create another child process
                    pid_t pidc = fork();
                    
                    // Error handling
                    if(pidc < 0) {
                        perror("ERROR: Could not create child process.");
                    }

                    // Child process
                    else if(pidc == 0) {
                        line_size = getline(&buffer,&buffer_size,in_ptr);
                        count++;
                    }
                    
                    // Parent process
                    else {
                        wait(NULL); // wait for child processes to finish
                        break;
                    }
                }
                exit(0);    // exit from child processes
            }
            // Parent process
            else {
                wait(NULL); // wait for child processes to finish

                // Close the file
                fclose(in_ptr);
                fclose(out_ptr);

                // Re-open input.txt to send to print_primes
                in_ptr = fopen(in_file,"r+");

                // Error handling
                if(!in_ptr) {
                    printf("ERROR: could not open file.\n");
                    exit(-1);
                }

                // Call print_primes to print the result
                print_primes(in_ptr);
            }
        }
    }
    
    // Reached if there is no content in the input file
    else {
        printf("ERROR: No content in input file.\n");
        exit(-1);
    }
}

int main() {
    sieve();    // call to run the Sieve of Eratosthenes
    return 0;   // success!
}
