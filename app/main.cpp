#include "../inc/alg.h"

FILE *fresult = NULL;
BiCluster *current_bicluster = NULL;

int main(void)
{
    /* 输入文件路径 */
    const char *filepath = "..//datasets//sample_data.txt";

    /* 输出文件路径 */
    if (!(fresult = fopen("..//result//alg1-out-test.txt", "w")))
    {
        puts("create file failed!");
        exit(-1);
    }
    else
    {
        puts("create file result.txt succeed!");
    }

    ExtendNode* file_head = save_filedata(filepath);
    ConWG(file_head);
    //extend_mining(current_bicluster);


    free(file_head);
    file_head = NULL;


    fclose(fresult);
    return 0;
}