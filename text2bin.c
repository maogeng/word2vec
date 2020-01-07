#include <stdio.h>
#include <string.h>


const long long max_size = 2000;
int main(int argc, char **argv) {
    FILE *f, *fo;
    long long words, size, i, j;
    char file_name[max_size], out_name[max_size];
    float *M, elem;
    char id[max_size];
    if (argc < 2) {
        printf("Usage: ./text2bin <FILE> <OUT FILE> ");
    }
    strcpy(file_name, argv[1]);
    strcpy(out_name, argv[2]);
    f = fopen(file_name, "r");
    if(f == NULL) {
        printf("Input file not found \n");
        return -1;
    }
    fscanf(f, "%lld", &words);
    fscanf(f, "%lld", &size);
    fo = fopen(out_name, "w");
    fprintf(fo, "%lld %lld\n", words, size);
    M = (float*)malloc((long long)size * sizeof(float));
    
    for(i = 0; i < words; ++i) {
        fscanf(f, "%s", id);
        fprintf(fo, "%s ", id); 
        for(j = 0; j < size; ++j) {
            fscanf(f, "%f,", &elem);
            M[j] = elem;
            fwrite(&M[j], sizeof(float), 1, fo);
        }
        fprintf(fo, "\n");
    }
    fclose(fo);
    fclose(f);
    return 0;
}
