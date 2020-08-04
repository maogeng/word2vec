#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const long long max_size = 2000;         // max length of strings
const long long N = 40;                  // number of closest words that will be shown
const long long max_w = 50;              // max length of vocabulary entries

int main(int argc, char **argv) {
    char evalname[max_size];
    FILE *fp;
    char line[1024];

    FILE *f;
    char st1[max_size];
    char *bestw[N];
    char file_name[max_size], st[100][max_size];
    float dist, len, bestd[N], vec[max_size];
    long long words, size, a, b, c, d, cn, bi[100];
    long long pos=0;
    char ch;
    float *M;
    char *vocab; // 保存所有词的词典
    int flag = 0;
    int positive_num = 0;
    int eval_num = 0;
    int not_in_model = 0;

    if (argc < 2) {
        printf("Usage: ./eval vec.bin eval.data \nwhere FILE contains word projections in the BINARY FORMAT\n");
        return 0;
    }

    strcpy(file_name, argv[1]); // model name
    strcpy(evalname, argv[2]); // eval file name
    f = fopen(file_name, "rb");
    if (f == NULL) {
        printf("Input file not found\n");
        return -1;
    }
    // 读模型
    fscanf(f, "%lld", &words);
    fscanf(f, "%lld", &size);
    vocab = (char *)malloc((long long)words * max_w * sizeof(char));
    for (a = 0; a < N; a++) bestw[a] = (char *)malloc(max_size * sizeof(char));
    M = (float *)malloc((long long)words * (long long)size * sizeof(float));
    if (M == NULL) {
        printf("Cannot allocate memory: %lld MB    %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
        return -1;
    }
    for (b = 0; b < words; b++) {
        if (b % 100000 == 0)
        {
            printf("%lld words has processed\n", b);
        }
        a = 0;
        while (1) {
            vocab[b * max_w + a] = fgetc(f);
            if (feof(f) || (vocab[b * max_w + a] == ' ')) break;
            if ((a < max_w) && (vocab[b * max_w + a] != '\n')) a++;
        }
        vocab[b * max_w + a] = 0;
        for (a = 0; a < size; a++) fread(&M[a + b * size], sizeof(float), 1, f);
        len = 0;
        for (a = 0; a < size; a++) len += M[a + b * size] * M[a + b * size];
        len = sqrt(len);
        for (a = 0; a < size; a++) M[a + b * size] /= len;
    }
    fclose(f);

    printf("\n--------model has been loaded---------\n\n");

    if((fp = fopen(evalname, "r")) ==NULL) {
        printf("error");
        return -1;
    }
    while(!feof(fp)) {
        if (eval_num % 100 == 0)
        {
            printf("positive_num/not_in_model/eval_num/pos: %d/ %d/ %d/ %.4f\n", positive_num, not_in_model, eval_num, pos*1.0/(positive_num+1));
        }
        eval_num ++;
        fgets(line, 1024, fp);
        // printf("%s\n", line);
        cn = 0;
        b = 0;
        c = 0;
        while(1)
        {
            st[cn][b] = line[c];
            b++;
            c++;
            st[cn][b] = 0;
            if (line[c] == 0 || line[c] == '\n') break;
            if (line[c] == ' ')
            {
                cn++;
                b = 0;
                c++;
            }
        }
        // cn ++; // 留最后一个作为预测poi_id
        flag = 1;
        for (a = 0; a < cn; ++a)
        {
            for (b = 0; b < words; ++b)
            {
                if (!strcmp(&vocab[b * max_w], st[a]))
                {
                    break;
                }
            }
            if (b == words)
            {
                b = -1;
            } else {
                flag = 0;
            }
            bi[a] = b;
            // printf("\nWord: %s Position in vocabulary: %lld\n", st[a], bi[a]);
        }
        if (flag ==1)
        {
            not_in_model++;
            // printf("All words out of dictionary word\n");
            continue;
        }
        for (a = 0; a < size; a++) vec[a] = 0;
        // average pooling
        for (b = 0; b < cn; b++) {
            if (bi[b] == -1) continue;
            for (a = 0; a < size; a++) vec[a] += M[a + bi[b] * size];
        }
        // 单位化
        len = 0;
        for (a = 0; a < size; a++) len += vec[a] * vec[a];
        len = sqrt(len);
        for (a = 0; a < size; a++) vec[a] /= len;

        // 求最近距离
        for (a = 0; a < N; a++) bestd[a] = -1;
        for (a = 0; a < N; a++) bestw[a][0] = 0;
        for (c = 0; c < words; c++) {
            a = 0;  
            // // ---------保证不召回在候选集中的poi------------
            // for (b = 0; b < cn; b++) if (bi[b] == c) a = 1;
            // if (a == 1) continue;
            dist = 0;
            for (a = 0; a < size; a++) dist += vec[a] * M[a + c * size];
            for (a = 0; a < N; a++) {
                if (dist > bestd[a]) {
                    for (d = N - 1; d > a; d--) {
                        bestd[d] = bestd[d - 1];
                        strcpy(bestw[d], bestw[d - 1]);
                    }
                    bestd[a] = dist;
                    strcpy(bestw[a], &vocab[c * max_w]);
                    break;
                }
            }
        }
        for (a = 0; a < N; a++)
        {
            if (!strcmp(st[cn],bestw[a]))
            {
                positive_num ++;
                // printf("%s %s in recall, position: %lld\n", st[cn], bestw[a], a);
                pos += a;
                break;
            }
        }
        // for (a = 0; a < N; a++) printf("%30s\t\t%f\n", bestw[a], bestd[a]);
        // break;
    }
    fclose(fp);

    printf("%d:%d\n", positive_num, eval_num);

    return 0;
}
