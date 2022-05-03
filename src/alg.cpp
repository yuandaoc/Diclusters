#include "..\inc\alg.h"

extern FILE *fresult;

char buf_line[BUF_SIZE];
HeadNode *WGhead = NULL;
FILE *fwgraph = NULL;

void hello(void)
{
    puts("hello, world!");
}


ExtendNode *save_filedata(const char *filepath)
{
    /* filedata_head：存储文件数据的链表头 */
    ExtendNode *filedata_head = (ExtendNode*)malloc(sizeof(ExtendNode));
    ExtendNode *filedata_move = filedata_head;
    filedata_move->next = NULL;
   
   /* 打开文件 */
    FILE* fp = NULL;
    if (! (fp = fopen(filepath,"r")))
    {
        puts("open file failed!");
        exit(-1);
    }
    else
    {
        puts("open input file succeed!");
    }

    /* 处理文件数据 */
    char *tok_p1 = NULL, *tok_p2 = NULL, *saveptr1 = NULL, *saveptr2 = NULL;
    while (!feof(fp))
    {
        fgets(buf_line, BUF_SIZE, fp); // 按行读取文件数据

        /* 处理当前扩展节点 */
        tok_p1 = strtok_s(buf_line, "\t", &saveptr1);
        ExtendNode *filedata_fresh = (ExtendNode*)malloc(sizeof(ExtendNode));
        filedata_fresh->extname = new char[strlen(tok_p1)+1];
        strcpy_s(filedata_fresh->extname, strlen(tok_p1)+1, tok_p1);
        
        /* 处理当前扩展节点下的权值节点，分别使用\t和,进行两次分割 */
        tok_p1 = strtok_s(NULL, "\t", &saveptr1);// 使用strtok_s而不是strtok在这里很重要，因为这样继续分割的时候才不会丢失
        filedata_fresh->wgnode = NULL;
        WgNode *wg_head = (WgNode*)malloc(sizeof(WgNode));
        WgNode *wg_move = wg_head;
        while (NULL != tok_p1)
        {
            WgNode *wg_fresh = (WgNode*)malloc(sizeof(WgNode));
            tok_p2 = strtok_s(tok_p1, ",", &saveptr2);
            int i = 0;
            while (tok_p2 != NULL)
            {
                //printf("%s\t",tok_p2);
                switch (i)
                {
                    case 0: // i=0,存储时序元组功能名
                    {
                        wg_fresh->wgname = new char[strlen(tok_p2)+1];
                        strcpy_s(wg_fresh->wgname, strlen(tok_p2)+1, tok_p2);
                        break;
                    }
                    case 1: // i=1, 存储元组start time
                    {
                        wg_fresh->time_start = atoi(tok_p2);
                        break;
                    }
                    case 2: // i=2, 存储元组end time
                    {
                        wg_fresh->time_end = atoi(tok_p2);
                        break;
                    }
                    default:
                        break;
                }
                i++;
                tok_p2 = strtok_s(NULL, ",", &saveptr2);
            }

            wg_move->next = wg_fresh;
            wg_fresh->next = NULL;
            wg_move = wg_fresh;

            tok_p1 = strtok_s(NULL, "\t", &saveptr1);
        }
        /* 连接当前扩展节点与其下的权值节点 */
        filedata_fresh->wgnode = wg_head->next;
        free(wg_head);
        wg_head = NULL;

        /* 连接扩展节点 */
        filedata_move->next = filedata_fresh;
        filedata_fresh->next = NULL;
        filedata_move = filedata_fresh;
    }

    fclose(fp);
    return filedata_head;
}


/* 
    事实上，这里是一个在leetcode上难度为Medium/Hard的算法:
    Find Non-overlapping intervals among a given set of intervals.
    NOTE:形参须使用引用才能改变实参值，若直接放vector名，实参不会改变
*/
void get_nonoverlapping_intervals(vector<vector<int>> &interval)
{
    if ((interval[0].size() <= 1))
    {
        return;
    }

    for (int i=0; i<interval[0].size(); i++)
    {
        for (int j= i+1; j<interval[0].size(); j++)
        {
            /* 下面的if-else分别判断每个时间区间是否是时间点 */
            if ((interval[0][i] != interval[1][i]) && (interval[0][j] != interval[1][j]))
            {
                /* overlaps判断准则：max(L1,L2) < min(R1,R2) */
                if ( max(interval[0][i], interval[0][j]) < min(interval[1][i], interval[1][j]) )
                {
                    /* note:必须先删除下标大的元素，这里j比i大，也就是需要先删除j */
                    interval[0].erase(interval[0].begin()+j);
                    interval[0].erase(interval[0].begin()+i);
                    interval[1].erase(interval[1].begin()+j);
                    interval[1].erase(interval[1].begin()+i);

                    get_nonoverlapping_intervals(interval);
                }
            }
            else if ((interval[0][i] != interval[1][i]) && (interval[0][j]==interval[1][j]))
            {
                if ((interval[0][i]<interval[0][j]) && (interval[1][j]<interval[1][i]))
                {
                    interval[0].erase(interval[0].begin()+j);
                    interval[0].erase(interval[0].begin()+i);
                    interval[1].erase(interval[1].begin()+j);
                    interval[1].erase(interval[1].begin()+i);
                    get_nonoverlapping_intervals(interval);
                }
            }
            else if((interval[0][i] == interval[1][i]) && (interval[0][j]!=interval[1][j]))
            {
                if ((interval[0][j]<interval[0][i]) && (interval[1][i]<interval[1][j]))
                {
                    interval[0].erase(interval[0].begin()+j);
                    interval[0].erase(interval[0].begin()+i);
                    interval[1].erase(interval[1].begin()+j);
                    interval[1].erase(interval[1].begin()+i);
                    get_nonoverlapping_intervals(interval);
                }               
            }
            else if ((interval[0][i]==interval[1][i]) && (interval[0][j]==interval[1][j]))
            {
                if (interval[0][i]==interval[0][j])
                {
                    interval[0].erase(interval[0].begin()+j);
                    interval[0].erase(interval[0].begin()+i);
                    interval[1].erase(interval[1].begin()+j);
                    interval[1].erase(interval[1].begin()+i);
                    get_nonoverlapping_intervals(interval);
                }
            }
        }
    }
    /* 运行到这里，说明重叠的已经全部删除了，直接退出 */
    return;
}


WgNode *get_weighted_value(const ExtendNode *base, const ExtendNode *extend)
{
    /* 
        不要直接用形参的base，extend访问权值节点,因为此函数后面会移动权值节点
        这样会破坏扩展头节点与其下权值节点的连接关系，用以下两个指针来代替访问
    */
    WgNode *wg_base = base->wgnode;
    WgNode *wg_ext = extend->wgnode;
    

    WgNode *wg_head = (WgNode*)malloc(sizeof(WgNode));
    WgNode *wg_move = wg_head;
    wg_move->next = NULL;

    vector<vector<int>> intervals(2);
    char *copyname = wg_base->wgname;
    while ((NULL != wg_base) && (NULL != wg_ext))
    {
        if (0==strcmpi(copyname, wg_base->wgname) && (0 == strcmpi(copyname, wg_ext->wgname)))
        {
            intervals[0].push_back(wg_base->time_start);
            intervals[1].push_back(wg_base->time_end);
            intervals[0].push_back(wg_ext->time_start);
            intervals[1].push_back(wg_ext->time_end);

            wg_base = wg_base->next;
            wg_ext = wg_ext->next;
            continue;
        }
        else if(0==strcmpi(copyname, wg_base->wgname) && (0 != strcmpi(copyname, wg_ext->wgname)))
        {
            intervals[0].push_back(wg_base->time_start);
            intervals[1].push_back(wg_base->time_end);
            wg_base = wg_base->next;
            continue;        
        }
        else if (0 !=strcmpi(copyname, wg_base->wgname) && (0 == strcmpi(copyname, wg_ext->wgname)))
        {
            intervals[0].push_back(wg_ext->time_start);
            intervals[1].push_back(wg_ext->time_end);
            wg_ext = wg_ext->next;
            continue;        
        }
        else if (0 !=strcmpi(copyname, wg_base->wgname) && (0 != strcmpi(copyname, wg_ext->wgname)))
        {
            get_nonoverlapping_intervals(intervals);
            if (intervals[0].empty())
            {
                WgNode *wg_fresh = (WgNode*)malloc(sizeof(WgNode));
                wg_fresh->wgname = copyname;
                wg_fresh->time_start = -1; // 若全部冲突而被消除，则令权值节点interval区间为[0,0]
                wg_fresh->time_end = 0;

                wg_move->next = wg_fresh;
                wg_fresh->next = NULL;
                wg_move = wg_fresh;
            }
            else
            {
                for (int i=0; i<intervals[0].size(); i++)
                {
                    WgNode *wg_fresh = (WgNode*)malloc(sizeof(WgNode));
                    wg_fresh->wgname = copyname;
                    wg_fresh->time_start = intervals[0][i];
                    wg_fresh->time_end = intervals[1][i];

                    wg_move->next = wg_fresh;
                    wg_fresh->next = NULL;
                    wg_move = wg_fresh;                   
                }
            }
            /* clear二维vector要各个分别clear，不要直接用二维vector.clear */
            intervals[0].clear();
            intervals[1].clear();
            copyname = wg_base->wgname;
        }
    }
    
    if ((NULL == wg_base) && (NULL == wg_ext))
    {
        get_nonoverlapping_intervals(intervals);
        if (intervals[0].empty())
        {
            WgNode *wg_fresh = (WgNode*)malloc(sizeof(WgNode));
            wg_fresh->wgname = copyname;
            wg_fresh->time_start = -1; // 若全部冲突而被消除，则令权值节点interval区间为[0,0]
            wg_fresh->time_end = 0;

            wg_move->next = wg_fresh;
            wg_fresh->next = NULL;
            wg_move = wg_fresh;
        }
        else
        {
            for (int i=0; i<intervals[0].size(); i++)
            {
                WgNode *wg_fresh = (WgNode*)malloc(sizeof(WgNode));
                wg_fresh->wgname = copyname;
                wg_fresh->time_start = intervals[0][i];
                wg_fresh->time_end = intervals[1][i];

                wg_move->next = wg_fresh;
                wg_fresh->next = NULL;
                wg_move = wg_fresh;
            }           
        }
        intervals[0].clear();
        intervals[1].clear();
    }
    else if ((NULL != wg_base) && (NULL == wg_ext))
    {
        while (NULL != wg_base)
        {
            intervals[0].push_back(wg_base->time_start);
            intervals[1].push_back(wg_base->time_end);
            wg_base = wg_base->next;
        }
        get_nonoverlapping_intervals(intervals);
        if (intervals[0].empty())
        {
            WgNode *wg_fresh = (WgNode*)malloc(sizeof(WgNode));
            wg_fresh->wgname = copyname;
            wg_fresh->time_start = -1; // 若全部冲突而被消除，则令权值节点interval区间为[0,0]
            wg_fresh->time_end = 0;

            wg_move->next = wg_fresh;
            wg_fresh->next = NULL;
            wg_move = wg_fresh;
        }
        else
        {
            for (int i=0; i<intervals[0].size(); i++)
            {
                WgNode *wg_fresh = (WgNode*)malloc(sizeof(WgNode));
                wg_fresh->wgname = copyname;
                wg_fresh->time_start = intervals[0][i];
                wg_fresh->time_end = intervals[1][i];

                wg_move->next = wg_fresh;
                wg_fresh->next = NULL;
                wg_move = wg_fresh;                   
            }           
        }
        intervals[0].clear();
        intervals[1].clear();
    }
    else if ((NULL == wg_base) && (NULL != wg_ext))
    {
        while (NULL != wg_ext)
        {
            intervals[0].push_back(wg_ext->time_start);
            intervals[1].push_back(wg_ext->time_end);
            wg_ext = wg_ext->next;
        }
        get_nonoverlapping_intervals(intervals);
        if (intervals[0].empty())
        {
            WgNode *wg_fresh = (WgNode*)malloc(sizeof(WgNode));
            wg_fresh->wgname = copyname;
            wg_fresh->time_start = -1; // 若全部冲突而被消除，则令权值节点interval区间为[0,0]
            wg_fresh->time_end = 0;

            wg_move->next = wg_fresh;
            wg_fresh->next = NULL;
            wg_move = wg_fresh;
        }
        else
        {
            for (int i=0; i<intervals[0].size(); i++)
            {
                WgNode *wg_fresh = (WgNode*)malloc(sizeof(WgNode));
                wg_fresh->wgname = copyname;
                wg_fresh->time_start = intervals[0][i];
                wg_fresh->time_end = intervals[1][i];

                wg_move->next = wg_fresh;
                wg_fresh->next = NULL;
                wg_move = wg_fresh;                   
            }          
        }
        intervals[0].clear();
        intervals[1].clear();
    }
    return wg_head->next;
}


bool prefix_prun_candi(WgNode *pre, WgNode *candi)
{
    WgNode *wg_base = pre;
    WgNode *wg_ext = candi;

    vector<vector<int>> intervals(2);
    char *copyname = wg_base->wgname;
    while ((NULL != wg_base) && (NULL != wg_ext))
    {
        if (0==strcmpi(copyname, wg_base->wgname) && (0 == strcmpi(copyname, wg_ext->wgname)))
        {
            intervals[0].push_back(wg_base->time_start);
            intervals[1].push_back(wg_base->time_end);
            intervals[0].push_back(wg_ext->time_start);
            intervals[1].push_back(wg_ext->time_end);

            wg_base = wg_base->next;
            wg_ext = wg_ext->next;
            continue;
        }
        else if(0==strcmpi(copyname, wg_base->wgname) && (0 != strcmpi(copyname, wg_ext->wgname)))
        {
            intervals[0].push_back(wg_base->time_start);
            intervals[1].push_back(wg_base->time_end);
            wg_base = wg_base->next;
            continue;        
        }
        else if (0 !=strcmpi(copyname, wg_base->wgname) && (0 == strcmpi(copyname, wg_ext->wgname)))
        {
            intervals[0].push_back(wg_ext->time_start);
            intervals[1].push_back(wg_ext->time_end);
            wg_ext = wg_ext->next;
            continue;        
        }
        else if (0 !=strcmpi(copyname, wg_base->wgname) && (0 != strcmpi(copyname, wg_ext->wgname)))
        {
            get_nonoverlapping_intervals(intervals);
            if (intervals[0].empty())
            {

            }
            else
            {
                for (int i=0; i<intervals[0].size(); i++)
                {
                 
                }
            }
            /* clear二维vector要各个分别clear，不要直接用二维vector.clear */
            intervals[0].clear();
            intervals[1].clear();
            copyname = wg_base->wgname;
        }
    }
    
    if ((NULL == wg_base) && (NULL == wg_ext))
    {
        get_nonoverlapping_intervals(intervals);
        if (intervals[0].empty())
        {

        }
        else
        {
            for (int i=0; i<intervals[0].size(); i++)
            {

            }           
        }
        intervals[0].clear();
        intervals[1].clear();
    }
    else if ((NULL != wg_base) && (NULL == wg_ext))
    {
        while (NULL != wg_base)
        {
            intervals[0].push_back(wg_base->time_start);
            intervals[1].push_back(wg_base->time_end);
            wg_base = wg_base->next;
        }
        get_nonoverlapping_intervals(intervals);
        if (intervals[0].empty())
        {

        }
        else
        {
            for (int i=0; i<intervals[0].size(); i++)
            {
                 
            }           
        }
        intervals[0].clear();
        intervals[1].clear();
    }
    else if ((NULL == wg_base) && (NULL != wg_ext))
    {
        while (NULL != wg_ext)
        {
            intervals[0].push_back(wg_ext->time_start);
            intervals[1].push_back(wg_ext->time_end);
            wg_ext = wg_ext->next;
        }
        get_nonoverlapping_intervals(intervals);
        if (intervals[0].empty())
        {

        }
        else
        {
       
        }
        intervals[0].clear();
        intervals[1].clear();
    }

}

void ConWG(ExtendNode *filedata_head)
{
    /* 
        权值图是指对每个需要扩展的节点，进行初始扩展节点添加，最终是每个基础节点后，扩展增加一个节点
        为了描述方便，使用wg_base表示基础节点；wg_ext表示扩展节点；
        与之前矩阵存储文件值不同的是此算法使用链表存储了文件数据，这里需要两个指针访问文件数据，一个用于wg_base，一个用于wg_ext
    */

    ExtendNode *filedata_base = filedata_head->next;
    ExtendNode *filedata_ext = filedata_head->next;

    WGhead = (HeadNode*)malloc(sizeof(HeadNode));
    HeadNode *wg_base_move = WGhead;
    wg_base_move->next = NULL;
    
    while (NULL != filedata_base)
    {
        HeadNode *wg_base_fresh = (HeadNode*)malloc(sizeof(HeadNode));
        wg_base_fresh->headname = filedata_base->extname;
        wg_base_fresh->extnode = NULL;
        
        ExtendNode *wg_ext_head = (ExtendNode*)malloc(sizeof(ExtendNode));
        ExtendNode *wg_ext_move = wg_ext_head;
        wg_ext_move->next = NULL;

        while (NULL != filedata_ext)
        {
            if ( atoi(filedata_base->extname+1) < atoi(filedata_ext->extname+1) )
            {
                ExtendNode *wg_ext_fresh = (ExtendNode*)malloc(sizeof(ExtendNode));
                wg_ext_fresh->extname = filedata_ext->extname;
                wg_ext_fresh->wgnode = get_weighted_value(filedata_base, filedata_ext);

                wg_ext_move->next = wg_ext_fresh;
                wg_ext_fresh->next = NULL;
                wg_ext_move = wg_ext_fresh;
            }
            else if ( atoi(filedata_base->extname+1) > atoi(filedata_ext->extname+1) )
            {
                ExtendNode *wg_ext_fresh = (ExtendNode*)malloc(sizeof(ExtendNode));
                wg_ext_fresh->extname = filedata_ext->extname;
                wg_ext_fresh->wgnode = get_weighted_value(filedata_ext, filedata_base); // T2T1的权值与T1T2相同

                wg_ext_move->next = wg_ext_fresh;
                wg_ext_fresh->next = NULL;
                wg_ext_move = wg_ext_fresh;
            }
            filedata_ext = filedata_ext->next;
        }
        wg_base_fresh->extnode = wg_ext_head->next;
        free(wg_ext_head);
        wg_ext_head = NULL;

        wg_base_move->next = wg_base_fresh;
        wg_base_fresh->next = NULL;
        wg_base_move = wg_base_fresh;

        filedata_ext = filedata_head->next;
        filedata_base = filedata_base->next;
    }

    // if (!(fwgraph = fopen("..//result//alg1-WeightedGraph-test.txt", "w")))
    // {
    //     puts("create file alg1-WeightedGraph failed!");
    //     exit(-1);
    // }
    // else
    // {
    //     puts("\ncreate file alg1-WeightedGraph succeed!");
    // }
    // wg_base_move = WGhead->next;
    // while (wg_base_move != NULL)
    // {
    //     while(wg_base_move->extnode !=NULL)
    //     {
    //         fprintf(fwgraph, "%s-%s:", wg_base_move->headname,wg_base_move->extnode->extname);
    //         while(wg_base_move->extnode->wgnode != NULL)
    //         {
    //             fprintf(fwgraph, "\t(%s,%d,%d)",wg_base_move->extnode->wgnode->wgname, wg_base_move->extnode->wgnode->time_start, wg_base_move->extnode->wgnode->time_end);
    //             wg_base_move->extnode->wgnode = wg_base_move->extnode->wgnode->next;
    //         }
    //         fprintf(fwgraph, "\n");
    //         wg_base_move->extnode = wg_base_move->extnode->next;
    //     }
    //     wg_base_move = wg_base_move->next;
    // }
    // fclose(fwgraph);

    fprintf(stdout, "succeed to create weighted graph!");
    return;
}

void get_pre_and_candi(BiCluster *current_bic)
{
    ;
}


void pruning(BiCluster *current_bic)
{
    ;
}


void destroy_bicluster(BiCluster *current_bic)
{
    ExtendNode *save_ext = NULL;
    WgNode *save_wg = NULL;
    while (NULL != current_bic->extlist)
    {
        save_ext = current_bic->extlist->next;
        free(current_bic->extlist);
        current_bic->extlist = save_ext;
    }
    while (NULL != current_bic->wglist)
    {
        save_wg = current_bic->wglist->next;
        free(current_bic->wglist);
        current_bic->wglist = save_wg;
    }
    while (NULL != current_bic->candilist)
    {
        while (NULL != current_bic->candilist->wgnode)
        {
            save_wg = current_bic->candilist->wgnode->next;
            free(current_bic->candilist->wgnode);
            current_bic->candilist->wgnode = save_wg;
        }
        save_ext = current_bic->candilist->next;
        free(current_bic->candilist);
        current_bic->candilist = save_ext;
    }
    while (NULL != current_bic->prelist)
    {
        while (NULL != current_bic->prelist->wgnode)
        {
            save_wg = current_bic->prelist->wgnode->next;
            free(current_bic->prelist->wgnode);
            current_bic->prelist->wgnode = save_wg;
        }
        save_ext = current_bic->prelist->next;
        free(current_bic->prelist);
        current_bic->prelist = save_ext;
    }
    free(current_bic);
    current_bic = NULL;
    return;
}


void extend_mining(BiCluster *current_bic)
{
    HeadNode *wgbase = WGhead->next;
    ExtendNode *base_ext = NULL, *base_ext1 = NULL;
    WgNode *Wvalue0 = NULL, *Wvalue1 = NULL, *Wvalue_move = NULL,*Wvalue_fresh = NULL;
    BiCluster *next_bic = NULL;
    bool flag_conbic = false;

    /* 
        if负责造基础双聚类，也就是只有两个扩展节点的
        else负责在此基础上扩展挖掘
    */
    if (NULL == current_bic)
    {
        /* 遍历权值图，依次尝试构造基础双聚类 */
        while (NULL != wgbase)
        {
            base_ext= wgbase->extnode;
            while (NULL != base_ext)
            {
                /* 无需构造F2F1这样的基础双聚类 */
                if ( atoi(wgbase->headname+1) > atoi(base_ext->extname+1) )
                {
                    base_ext = base_ext->next;
                    continue;
                }
                
                /* 看需要构造的基础双聚类是否能利用其前驱剪枝掉，如果可以，也不构造 */
                base_ext1 = wgbase->extnode;
                flag_conbic = false;
                /* base_ext1 != base_ext说明是ext1是ext的前驱 */
                while (base_ext1 != base_ext)
                {
                    /* 应用剪枝策略判断 */
                    Wvalue0 = base_ext->wgnode;
                    Wvalue1 = base_ext1->wgnode; 
                    bool flag_prun = true;



                    // char *copy_wgname = Wvalue1->wgname;
                    // Wvalue_move = Wvalue0;
                    
                    // while (NULL != Wvalue1)
                    // {
                    //     while (0 == strcmp(copy_wgname, Wvalue0->wgname))
                    //     {
                    //         if ((Wvalue0->time_start == Wvalue1->time_start) && (Wvalue0->time_end==Wvalue1->time_end))
                    //         {
                    //             Wvalue0 = Wvalue0->next;
                    //             Wvalue1 = Wvalue1->next;
                    //         }
                    //         else if ((Wvalue0->time_start != Wvalue0->time_end) && (Wvalue1->time_start != Wvalue1->time_end))
                    //         {
                    //             if (max(Wvalue0->time_start, Wvalue1->time_start) < min(Wvalue0->time_end, Wvalue1->time_end))
                    //             {
                    //                 flag_prun = false;
                    //                 break;
                    //             }
                    //             else
                    //             {
                    //                 Wvalue0 = Wvalue0->next;
                    //                 continue;                                    
                    //             }
                    //         }
                    //         else if ((Wvalue0->time_start == Wvalue0->time_end) && (Wvalue1->time_start != Wvalue1->time_end))
                    //         {
                    //             if ((Wvalue1->time_start < Wvalue0->time_start) && (Wvalue0->time_end < Wvalue1->time_end))
                    //             {
                    //                 flag_prun = false;
                    //                 break;
                    //             }
                    //             else
                    //             {
                    //                 Wvalue0 = Wvalue0->next;
                    //                 Wvalue1 = Wvalue1->next;
                    //             }
                    //         }
                    //         else if ((Wvalue0->time_start != Wvalue0->time_end) && (Wvalue1->time_start == Wvalue1->time_end))
                    //         {
                    //             if ((Wvalue0->time_start < Wvalue1->time_start) && (Wvalue1->time_end < Wvalue0->time_end))
                    //             {
                    //                 flag_prun = false;
                    //                 break;
                    //             }
                    //             else
                    //             {
                    //                 Wvalue0 = Wvalue0->next;
                    //                 Wvalue1 = Wvalue1->next;
                    //             }
                    //         }

                    //     }
                    // }

                    // while (NULL != Wvalue1)
                    // {
                    //     while (NULL != Wvalue0)
                    //     {
                    //         if (atoi(Wvalue0->wgname+1) == atoi(Wvalue1->wgname+1))
                    //         {
                    //             if ((Wvalue0->time_start == Wvalue1->time_start) && (Wvalue0->time_end==Wvalue1->time_end))
                    //             {
                    //                 Wvalue0 = Wvalue0->next;
                    //                 Wvalue1 = Wvalue1->next;
                    //             }
                    //             else if ((Wvalue0->time_start != Wvalue0->time_end) && (Wvalue1->time_start != Wvalue1->time_end))
                    //             {
                    //                 if (max(Wvalue0->time_start, Wvalue1->time_start) < min(Wvalue0->time_end, Wvalue1->time_end))
                    //                 {
                    //                     flag_prun = false;
                    //                     break;
                    //                 }
                    //                 else
                    //                 {
                    //                     Wvalue0 = Wvalue0->next;
                    //                     continue;                                    
                    //                 }
                    //             }
                    //             else if ((Wvalue0->time_start == Wvalue0->time_end) && (Wvalue1->time_start != Wvalue1->time_end))
                    //             {
                    //                 if ((Wvalue1->time_start < Wvalue0->time_start) && (Wvalue0->time_end < Wvalue1->time_end))
                    //                 {
                    //                     flag_prun = false;
                    //                     break;
                    //                 }
                    //                 else
                    //                 {
                    //                     Wvalue0 = Wvalue0->next;
                    //                     Wvalue1 = Wvalue1->next;
                    //                 }
                    //             }
                    //             else if ((Wvalue0->time_start != Wvalue0->time_end) && (Wvalue1->time_start == Wvalue1->time_end))
                    //             {
                    //                 if ((Wvalue0->time_start < Wvalue1->time_start) && (Wvalue1->time_end < Wvalue0->time_end))
                    //                 {
                    //                     flag_prun = false;
                    //                     break;
                    //                 }
                    //                 else
                    //                 {
                    //                     Wvalue0 = Wvalue0->next;
                    //                     Wvalue1 = Wvalue1->next;
                    //                 }
                    //             }
                    //         }
                    //         else if (atoi(Wvalue0->wgname+1) > atoi(Wvalue1->wgname+1))
                    //         {
                                
                    //             Wvalue1 = Wvalue1->next;
                    //             if (0 != strcmpi(Wvalue1->wgname, Wvalue0->wgname))
                    //             {
                    //                 Wvalue0 = base_ext->wgnode;
                    //             }
                                
                    //         }
                    //         else if (atoi(Wvalue1->wgname+1) > atoi(Wvalue0->wgname+1))
                    //         {
                    //             Wvalue0 = Wvalue0->next;
                    //             if (0 != strcmpi(Wvalue1->wgname, Wvalue0->wgname))
                    //             {
                    //                 Wvalue1 = Wvalue1->next;
                    //             }
                    //         }

                    //     } 
                    // }
                    if (!flag_prun)
                    {
                        flag_conbic = true;
                        break;
                    }

                    base_ext1 = base_ext1->next;
                }

                /* 若被剪枝则不构造，否则构造 */
                if (!flag_conbic)
                {
                    base_ext = base_ext->next;
                    continue;
                }
                else
                {
                    /* 依据权值图构造基础双聚类 */
                    current_bic = (BiCluster*)malloc(sizeof(BiCluster));
                    current_bic->candilist = NULL;
                    current_bic->prelist = NULL;
                    current_bic->extlist = NULL;
                    current_bic->wglist = NULL;
                    current_bic->flag_output = true;

                    current_bic->extlist = (ExtendNode*)malloc(sizeof(ExtendNode));
                    current_bic->extlist->extname = wgbase->headname;
                    current_bic->extlist->wgnode = NULL;
                    current_bic->extlist->next = (ExtendNode*)malloc(sizeof(ExtendNode));
                    current_bic->extlist->next->extname = base_ext->extname;
                    current_bic->extlist->next->wgnode = NULL;
                    current_bic->extlist->next->next = NULL;

                    Wvalue0 = base_ext->wgnode;
                    Wvalue1 = (WgNode*)malloc(sizeof(WgNode));
                    Wvalue_move = Wvalue1;
                    Wvalue_move->next = NULL;
                    while (NULL != Wvalue0)
                    {
                        Wvalue_fresh = (WgNode*)malloc(sizeof(WgNode));
                        Wvalue_fresh->wgname = Wvalue0->wgname;
                        Wvalue_fresh->time_start = Wvalue0->time_start;
                        Wvalue_fresh->time_end = Wvalue0->time_end;
                        
                        Wvalue_move->next = Wvalue_fresh;
                        Wvalue_fresh->next = NULL;
                        Wvalue_move = Wvalue_fresh;

                        Wvalue0 = Wvalue0->next;
                    }
                    current_bic->wglist = Wvalue1->next;
                    free(Wvalue1);
                    Wvalue1 = NULL;
                    /* ----------------------------- */

                    /* 创建完基础双聚类后，开始扩展挖掘 */
                    extend_mining(current_bic);

                    /* 基础双聚类深度扩展挖掘完成后，递归返回到这里，释放当前的基础双聚类空间 */
                    destroy_bicluster(current_bic);
                    // current_bic = NULL; // 在destroy后已经置空了 
                }
                base_ext = base_ext->next;
            }
            wgbase = wgbase->next;
        }
    }
    else
    {
        get_pre_and_candi(current_bic);
        pruning(current_bic);

        /* 经判断后输出，待补充 */
        if (current_bic->flag_output)
        {
            ;
        }

        /* ------------------------------------------------- */
        /* 当前双聚类处理结束后，依据其候选节点链继续扩展挖掘 */
        ExtendNode *candi = current_bic->candilist;
        while (NULL != candi)
        {
            /* 创建下一双聚类 */
            next_bic = (BiCluster*)malloc(sizeof(BiCluster));
            next_bic->extlist = NULL;
            next_bic->wglist = NULL;
            next_bic->prelist = NULL;
            next_bic->candilist = NULL;
            next_bic->flag_output =true;

            /* 复制当前双聚类的扩展节点信息到下一双聚类 */
            ExtendNode *move = current_bic->extlist;
            while (NULL != move)
            {
                /* 这里借用之前的base_ext、base_ext1变量作移动和更新用，勿以变量名称理解 */
                if (NULL == next_bic->extlist)
                {
                    base_ext = (ExtendNode*)malloc(sizeof(ExtendNode));
                    base_ext->extname = move->extname;
                    base_ext->wgnode = NULL;
                    base_ext->next = NULL;
                    current_bic->extlist = base_ext;
                }
                else
                {
                    base_ext1 = (ExtendNode*)malloc(sizeof(ExtendNode));
                    base_ext1->extname = move->extname;
                    base_ext1->wgnode = NULL;
                    base_ext1->next = NULL;
                    base_ext->next = base_ext1;
                    base_ext = base_ext1;
                }
                move = move->next;
            }

            /* 增加curbic的candidate节点到nextbic的扩展节点链extlist */
            base_ext1 = (ExtendNode*)malloc(sizeof(ExtendNode));
            base_ext1->extname = candi->extname;
            base_ext1->wgnode = NULL;
            base_ext1->next = NULL;
            base_ext->next = base_ext1;
            base_ext = base_ext1;

            /* 复制curbic->candidate->wgnode到nextbic的权值节点链wglist */
            Wvalue0 = candi->wgnode;
            while (NULL != Wvalue0)
            {
                if (NULL != next_bic->wglist)
                {
                    Wvalue_move = (WgNode*)malloc(sizeof(WgNode));
                    Wvalue_move->wgname = Wvalue0->wgname;
                    Wvalue_move->time_start = Wvalue0->time_start;
                    Wvalue_move->time_end = Wvalue0->time_end;
                    Wvalue_move->next = NULL;
                    next_bic->wglist = Wvalue_move;
                }
                else
                {
                    Wvalue_fresh = (WgNode*)malloc(sizeof(WgNode));
                    Wvalue_fresh->wgname = Wvalue0->wgname;
                    Wvalue_fresh->time_start = Wvalue0->time_start;
                    Wvalue_fresh->time_end = Wvalue0->time_end;
                    Wvalue_fresh->next = NULL;
                    Wvalue_move->next = Wvalue_fresh;
                    Wvalue_move = Wvalue_fresh;
                }
                Wvalue0 = Wvalue0->next;
            }

            /* 构造好扩展双聚类next_bic后，继续递归挖掘 */
            extend_mining(next_bic);

            /* 扩展挖掘完成后，递归返回到这里，释放掉当前扩展双聚类空间 */
            destroy_bicluster(next_bic);

            /* 继续依据下一个候选节点扩展挖掘 */
            candi = candi->next;
        }
    }
}