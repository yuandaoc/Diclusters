#include "../inc/alg.h"

FILE *fresult = NULL;
BiCluster *current_bicluster = NULL;

int main(void)
{
    // vector<int> obj = {-1,1,1,4,10};
    // sort(obj.begin(),obj.end());
    // auto it = obj.begin();
    // int cnt =0;
    // while(it != obj.end())
    // {
    //     cnt = count(obj.begin(), obj.end(),*it);
    //     cout<<*it<<" occurs "<<cnt<<" times"<<endl;
    //     it += cnt;
    // }

    // vector<WgNode> time;
    // for (int i=0; i<5;i++)
    // {
    //     WgNode buf;
    //     cin>>buf.time_start>>buf.time_end;
    //     time.push_back(buf);
    // }
    // sort(time.begin(), time.end(),compare);
    // for (auto iter : time)
    // {
    //     cout<<"("<<iter.time_start<<","<<iter.time_end<<")"<<endl;
    // }
    // auto it = time.begin();
    // auto iter = it;
    // int cnt =0;
    // while(it != time.end())
    // {
    //     // if ((*it).time_end == 0)
    //     // {
    //     //     puts("find end == 0");
    //     //     break;
    //     // }
    //     cnt = 0;
    //     for (iter = time.begin(); iter != time.end(); iter++)
    //     {
    //         if ((*it)==(*iter))
    //         {
    //             cnt++;
    //         }
    //     }
    //     cout<<"("<<(*it).time_start<<","<<(*it).time_end<<")"<<"\toccurs "<<cnt<<" times"<<endl;
    //     it += cnt;
    // }
    
    // vector<char*> previous;
    // vector<string> candidate;
    
    // previous.push_back((char*)"F2");
    // previous.push_back((char*)"F13");
    // previous.push_back((char*)"F2");
    // previous.clear();
    // if (previous.empty())
    // {
    //     cout<<"clear to empty"<<endl;
    //     return 0;
    // }

    // candidate.push_back("F1");
    // candidate.push_back("F2");

    // int cnt=count(previous.begin(),previous.end(),"F2");
    // cout<<cnt<<endl;


    /* 输入文件路径 */
    const char *filepath = "..//datasets//sample_data.txt";

    /* 输出文件路径 */
    if (!(fresult = fopen("..//result//alg1-out-test1.txt", "w")))
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
    extend_mining(current_bicluster);


    free(file_head);
    file_head = NULL;

    fclose(fresult);
    puts("over");
    return 0;
}