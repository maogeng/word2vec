//  Copyright 2013 Google Inc. All Rights Reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

const long long max_size = 2000;         // max length of strings
const long long N = 40;                  // number of closest words that will be shown
const long long max_w = 50;              // max length of vocabulary entries

int main(int argc, char **argv) {
  FILE *f, *fo;
  char file_name[max_size], out_name[max_size];
  float len;
  long long words, size, a, b;
  float *M;
  char vocab[max_w];
  if (argc < 2) {
    printf("Usage: ./distance <FILE>\nwhere FILE contains word projections in the BINARY FORMAT\n");
    return 0;
  }
  strcpy(file_name, argv[1]);
  strcpy(out_name, argv[2]);
  f = fopen(file_name, "rb");
  if (f == NULL) {
    printf("Input file not found\n");
    return -1;
  }
  fo = fopen(out_name, "w");
  fscanf(f, "%lld", &words);
  fscanf(f, "%lld", &size);
  //vocab = (char *)malloc((long long)words * max_w * sizeof(char));
  M = (float *)malloc((long long)words * (long long)size * sizeof(float));
  if (M == NULL) {
    printf("Cannot allocate memory: %lld MB    %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
    return -1;
  }
  for (b = 0; b < words; b++) {
    a = 0;
    while (1) {
      vocab[a] = fgetc(f);
      if (feof(f) || (vocab[a] == ' ')) break;
      if ((a < max_w) && (vocab[a] != '\n')) a++;
    }
    vocab[a] = 0;
    if(b>0) fprintf(fo, "%s,", vocab);
    for (a = 0; a < size; a++) fread(&M[a + b * size], sizeof(float), 1, f);
    len = 0;
    for (a = 0; a < size; a++) len += M[a + b * size] * M[a + b * size];
    len = sqrt(len);
    if(b == 0) continue;
    for (a = 0; a < size; a++) {
      if (a==size-1)
      {
        fprintf(fo,"%f\n",M[a + b * size] /= len);
      } else{
        fprintf(fo,"%f:",M[a + b * size] /= len);
      }
    } 
  }
  fclose(f);
  fclose(fo);
  
  return 0;
}
