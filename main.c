/* NOTES:
need to add "-lz" in complier code, e.g: gcc -o main main.c -lz
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "zlib.h"

char *input_file = "100MB.txt";
char *output_file = "100MB_output.txt";

int thread_counter = 1;
int read_counter = 0;
int compress_counter = 0;
int write_counter = 0;

int write_finished = 1;
int last_print = 0;

int max_itr=0;
int str_reminder=0;

char* read_arr = NULL;
char* comp_arr = NULL;
char* temp_read = NULL;

pthread_t thread;
clock_t start;

void *reader(void *arg)
{
    FILE* infile = fopen(input_file, "r");//Do we need to open this every time?`
    fseek(infile,0,4000*read_counter);//moving the pointer to the correct multiple of 4kb
    if(read_counter==max_itr-1 && str_reminder!=0)
    {
        char* buffer = (char*) malloc(sizeof(char)*(str_reminder));//preparing buffer
        read_arr[read_counter] = fread(buffer, sizeof(char), str_reminder, infile);//last read is not 4000 long, only remainder
        free(buffer);
    }
    else
    {
        char* buffer = (char*) malloc(sizeof(char)*(4000));//preparing buffer
        read_arr[read_counter]= fread(buffer, sizeof(char), 4000, infile);//any read that isn't the last is 4000
        free(buffer);
    }
    fclose(infile);//Do we need to close this every time? or just at the end of main function or w/e
    thread_counter--;
}

void *compression(void *arg)
{
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    defstream.avail_in = (uInt)strlen(&read_arr[compress_counter])+1; // size of input, string + terminator
    defstream.next_in = (Bytef *)&read_arr[compress_counter]; // input char array
    defstream.avail_out = (uInt)sizeof(&comp_arr[compress_counter]); // size of output
    defstream.next_out = (Bytef *)&comp_arr[compress_counter]; // output char array

    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    //comp_arr[compress_counter]=compress(read_arr[compress_counter]);
    //basically we want comparr[compress_counter] to = compressed readarr[compress counter]
    //not sure exactly how tf this works if we even use the compress function, or deflate function, or if we have to change variable types then compress then switch back.
    thread_counter--;
}


void *writer(void *arg)
{
    write_finished = 0;

    FILE* outfile= fopen(output_file, "a");
    if(write_counter==max_itr-1)
    {
        if (str_reminder==0)
        {
            fwrite(&comp_arr[write_counter], sizeof(char), 4000, outfile);
        }
        else
        {
            fwrite(&comp_arr[write_counter], sizeof(char), str_reminder, outfile);
        }
    }
    else
    {
        fwrite(&comp_arr[write_counter], sizeof(char), 4000, outfile);
    }

    fclose(outfile);
    write_finished = 1;
    thread_counter--;
    if ((write_counter == max_itr) && last_print == 0)
    {
        last_print = 1;
        float endtimer = clock() - start;
        int msec= endtimer *1000 / CLOCKS_PER_SEC;
        printf("Read -> Compress -> Write took %d seconds %d milliseconds: \r\n", msec/1000, msec%1000);
        
    }
}

int main(int argc, char *argv[])
{
    int max_thread;
    int string_size;

    //get user input
    printf("Enter # of threads: \r\n");
    scanf("%d", &max_thread);

    //clears document
    fclose(fopen(output_file, "w"));

    //get max file size and number in indexes
    FILE* infile = fopen(input_file, "r");
    fseek(infile,0, SEEK_END);
    string_size=ftell(infile);
    
    max_itr= string_size/4000;//finding how many 4000 strings
    //finding remainder if not a multiple of 4000
    if (string_size%4000!=0)
    {
        max_itr+=1;
        str_reminder=string_size%4000;
    }

    read_arr = malloc(max_itr);
    comp_arr = malloc(max_itr);

    printf("Starting timer for compression with %d threads...\r\n", max_thread);//now that stuff is set up, we start timing
    clock_t start = clock();

        while(1)
        {
            //check thread count
            while (thread_counter >= max_thread){}
            //make read thread
            if (read_counter < max_itr)
            {
                thread_counter++;
                //printf("Read counter: %d Thread counter:%d\r\n", read_counter, thread_counter);
                pthread_create(&thread, NULL, &reader, NULL);
                read_counter++;
            }

            //check thread count
            while (thread_counter >= max_thread){}
            //make compression thread
            if (compress_counter < max_itr)
            {
                thread_counter++;
                //printf("Compress counter: %d Thread counter:%d\r\n", compress_counter, thread_counter);
                pthread_create(&thread, NULL, &compression, NULL);
                compress_counter++;
            }

            //check thread count
            while (thread_counter >= max_thread){}
            //make write thread
            if ((write_finished == 1) && (write_counter < max_itr))
            {
                thread_counter++;
                //printf("Write counter: %d Thread counter:%d\r\n", write_counter, thread_counter);
                pthread_create(&thread, NULL, &writer, NULL);
                write_counter++;
            }
            if (write_counter == max_itr)
                break;
        }

    // Last thing that main() should do *also needs to be in each function, see commented out function as example

    pthread_exit(NULL);
    free(read_arr);
    free(comp_arr);
}

