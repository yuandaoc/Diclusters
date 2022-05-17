#include "..\inc\alg.h"

extern FILE *fresult;

char buf_line[BUF_SIZE];
HeadNode *WGhead = NULL;
FILE *fwgraph = NULL;

void hello(void)
{
    puts("hello, world!");
}

bool compare(const WgNode& time1, const WgNode& time2)
{
    return time1.time_start < time2.time_end;
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
                wg_fresh->time_start = -1; // 若全部冲突而被消除，则令权值节点interval区间为[-1,0]
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
            wg_fresh->time_start = -1; // 若全部冲突而被消除，则令权值节点interval区间为[-1,0]
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
            wg_fresh->time_start = -1; // 若全部冲突而被消除，则令权值节点interval区间为[-1,0]
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
            wg_fresh->time_start = -1; // 若全部冲突而被消除，则令权值节点interval区间为[-1,0]
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

    fprintf(stdout, "succeed to create weighted graph!\n");
    return;
}


void get_pre_and_candi(BiCluster *current_bic)
{
    HeadNode *wgbase = WGhead->next;
    ExtendNode *bic_extnode = current_bic->extlist;
    ExtendNode *base_extnode = NULL;
    WgNode *func1,*func2,*func3;

    /* 依次和权值图对应链比较 */
    while (NULL != wgbase)
    {
        /* 找当前双聚类的前驱和候选扩展节点 */
        if (0 == strcmp(wgbase->headname, bic_extnode->extname))
        {
            bic_extnode = bic_extnode->next;
            base_extnode = wgbase->extnode;
            /* 找前驱 */
            ExtendNode *pre_head = (ExtendNode*)malloc(sizeof(ExtendNode));
            ExtendNode *pre_move = pre_head;
            pre_move->next = NULL;
            while((NULL != bic_extnode) && (NULL != base_extnode))
            {
                if (0 == strcmp(base_extnode->extname, bic_extnode->extname))
                {
                    bic_extnode = bic_extnode->next;
                    base_extnode = base_extnode->next;
                }
                else
                {
                    /* 在到达null之前有不同，说明此时base->extnode这个节点为当前双聚类的前驱 */
                    ExtendNode *pre_fresh = (ExtendNode*)malloc(sizeof(ExtendNode));
                    pre_fresh->extname = base_extnode->extname;
                    pre_fresh->wgnode = NULL;
                    
                    pre_move->next = pre_fresh;
                    pre_fresh->next = NULL;
                    pre_move = pre_fresh;
                    
                    base_extnode = base_extnode->next;
                }
            }
            /* 将前驱链接到当前双聚类的prelist */
            current_bic->prelist = pre_head->next;
            free(pre_head);
            pre_head = NULL;

            /* base->extnode不为空，说明此后的节点为当前双聚类的候选 */
            ExtendNode *candi_head = (ExtendNode*)malloc(sizeof(ExtendNode));
            ExtendNode *candi_move = candi_head;
            candi_move->next = NULL;
            while (NULL != base_extnode)
            {
                ExtendNode *candi_fresh = (ExtendNode*)malloc(sizeof(ExtendNode));
                candi_fresh->extname = base_extnode->extname;
                candi_fresh->wgnode = NULL;
                
                candi_move->next = candi_fresh;
                candi_fresh->next = NULL;
                candi_move = candi_fresh;

                base_extnode = base_extnode->next;
            }
            /* 将候选链接到当前双聚类的candilist */
            current_bic->candilist = candi_head->next;
            free(candi_head);
            candi_head = NULL;

            /* 进入权值图对应链，找到所有前驱和候选之后，退出 */
            break;
        }
        wgbase = wgbase->next;
    }

    
    /* 计算前驱和候选节点的权值 */
    WgNode* jointpairs[100] = {NULL};//100:预设的所有扩展节点数量,这里预设最大为100个任务
    wgbase = WGhead->next;
    
    bic_extnode = current_bic->extlist;
    int cnt_jointpairs = 1; // 向下的扩展需要求交集的数量，例如T1T2T3需要T1T2、T1T3、T2T3三对.特别注意从1开始是因为这个1表示相交的pair中的当前双聚类已记录
    while ( NULL != bic_extnode)
    {
        cnt_jointpairs++;
        bic_extnode = bic_extnode->next;
    }
    bic_extnode = current_bic->extlist;

    ExtendNode *candidate = current_bic->candilist;
    ExtendNode *previous = current_bic->prelist;
    int i = 0;
    

    while (NULL != candidate)
    {
        i = 0;
        jointpairs[0] = current_bic->wglist;
        while (NULL != wgbase)
        {
            if ( 0 == strcmpi(wgbase->headname, candidate->extname))
            {
                bic_extnode = current_bic->extlist;
                base_extnode = wgbase->extnode;
                while (NULL != bic_extnode)
                {
                    if ( 0 == strcmpi(base_extnode->extname, bic_extnode->extname))
                    {
                        i++;
                        jointpairs[i] = base_extnode->wgnode;

                        base_extnode = base_extnode->next;
                        bic_extnode = bic_extnode->next;
                    }
                    else
                    {
                        base_extnode = base_extnode->next;
                    }
                }


                func1 = (WgNode*)malloc(sizeof(WgNode));
                func2 = func1;
                vector<WgNode> time;
                char *func_category = jointpairs[0]->wgname;
                bool endflag = false;
                while (NULL != func_category)
                {
                    for (i=0; i<cnt_jointpairs; i++)
                    {
                        while (0 == strcmpi(jointpairs[i]->wgname, func_category)) 
                        {
                            WgNode buf;
                            buf.time_start = jointpairs[i]->time_start;
                            buf.time_end = jointpairs[i]->time_end;
                            time.push_back(buf);
                            if (NULL != jointpairs[i]->next)
                            {
                                jointpairs[i] = jointpairs[i]->next;
                            }
                            else
                            {
                                endflag =true;
                                break;
                            }
                        }
                    }

                    /* 处理每个func_category的intervals */
                    sort(time.begin(), time.end(),compare);
                    auto it = time.begin();
                    auto iter = it;
                    int cnt =0;
                    while(it != time.end())
                    {
                        cnt = 0;
                        if ((*it).time_end == 0)
                        {
                            //puts("find end == 0");
                            func3 = (WgNode*)malloc(sizeof(WgNode));
                            func3->wgname = func_category;
                            func3->time_start = -1;
                            func3->time_end = 0;

                            func2->next = func3;
                            func3->next = NULL;
                            func2 = func3;
                            break;
                        }
                        
                        for (iter = time.begin(); iter != time.end(); iter++)
                        {
                            if ((*it)==(*iter))
                            {
                                cnt++;
                            }
                        }
                        if (cnt == (cnt_jointpairs-1))
                        {
                            func3 = (WgNode*)malloc(sizeof(WgNode));
                            func3->wgname = func_category;
                            func3->time_start = (*it).time_start;
                            func3->time_end = (*it).time_end;

                            func2->next = func3;
                            func3->next = NULL;
                            func2 = func3;
                        }
                        it += cnt;
                    }

                    time.clear();
                    if (!endflag)
                    {
                        func_category = jointpairs[0]->wgname;
                    }
                    else
                    {
                        func_category = NULL;
                    }
                }

                
                candidate->wgnode = func1->next;
                func1 = NULL;
                
                wgbase = wgbase->next;
                break;
            }
            else
            {
                wgbase = wgbase->next;
            }
        }
        candidate = candidate->next;
    }
    

    while (NULL != previous)
    {
        i=0;
        jointpairs[0] = current_bic->wglist;
        while (NULL !=  wgbase)
        {
            if ( 0 == strcmpi(wgbase->headname, previous->extname))
            {
                bic_extnode = current_bic->extlist;
                base_extnode = wgbase->extnode;
                while (NULL != bic_extnode)
                {
                    if ( 0 == strcmpi(base_extnode->extname, bic_extnode->extname))
                    {
                        i++;
                        jointpairs[i] = base_extnode->wgnode;

                        base_extnode = base_extnode->next;
                        bic_extnode = bic_extnode->next;
                    }
                    else
                    {
                        base_extnode = base_extnode->next;
                    }
                }

                func1 = (WgNode*)malloc(sizeof(WgNode));
                func2 = func1;
                vector<WgNode> time;
                char *func_category = jointpairs[0]->wgname;
                bool endflag = false;
                while (NULL != func_category)
                {
                    for (i=0; i<cnt_jointpairs; i++)
                    {
                        while (0 == strcmpi(jointpairs[i]->wgname, func_category)) 
                        {
                            WgNode buf;
                            buf.time_start = jointpairs[i]->time_start;
                            buf.time_end = jointpairs[i]->time_end;
                            time.push_back(buf);
                            if (NULL != jointpairs[i]->next)
                            {
                                jointpairs[i] = jointpairs[i]->next;
                            }
                            else
                            {
                                endflag =true;
                                break;
                            }
                        }
                    }
                    /* 处理每个func_category的intervals */

                    sort(time.begin(), time.end(),compare);
                    auto it = time.begin();
                    auto iter = it;
                    int cnt =0;
                    while(it != time.end())
                    {
                        cnt = 0;
                        if ((*it).time_end == 0)
                        {
                            //puts("find end == 0");
                            func3 = (WgNode*)malloc(sizeof(WgNode));
                            func3->wgname = func_category;
                            func3->time_start = -1;
                            func3->time_end = 0;

                            func2->next = func3;
                            func3->next = NULL;
                            func2 = func3;
                            break;
                        }
                        
                        for (iter = time.begin(); iter != time.end(); iter++)
                        {
                            if ((*it)==(*iter))
                            {
                                cnt++;
                            }
                        }
                        if (cnt == (cnt_jointpairs-1))
                        {
                            func3 = (WgNode*)malloc(sizeof(WgNode));
                            func3->wgname = func_category;
                            func3->time_start = (*it).time_start;
                            func3->time_end = (*it).time_end;

                            func2->next = func3;
                            func3->next = NULL;
                            func2 = func3;
                        }
                        it += cnt;
                    }

                    time.clear();
                    if (!endflag)
                    {
                        func_category = jointpairs[0]->wgname;
                    }
                    else
                    {
                        func_category = NULL;
                    }
                }

                previous->wgnode = func1->next;
                func1 = NULL;

                wgbase = wgbase->next;
                break;
            }
            else
            {
                wgbase = wgbase->next;
            }
        }
        previous = previous->next;
    }

    /* 判断候选/前驱节点的权值是否每一类都不存在,若都不存在,则删除该节点
        bic_extnode;func2用在删除节点时记录地址
        save_ext = bic_extnode
        save_wg = func2
    */

    candidate = current_bic->candilist;
    bool delete_flag;
    while ( NULL != candidate )
    {
        delete_flag = true;
        func1 = candidate->wgnode;
        while (NULL != func1)
        {
            if (func1->time_end != 0)
            {
                delete_flag = false;
                break;
            }
            else
            {
                func1 = func1->next;
            }
        }
        if (delete_flag)
        {
           /* 若此candidate需要删除，那么其下的wgnode全删 */
            func1 = candidate->wgnode;
            while ( NULL!= func1)
            {
                func2 = func1->next;
                free(func1);
                func1 = func2;
            }
            /* 主要是看第一个candidate删没删 */
            if (candidate == current_bic->candilist)
            {
                /* 第一个candidate删了 */
                current_bic->candilist = candidate->next;// 因为candidate不是全删，只删除此candi，故在更新candidate之前需要先更新current_bic->candilist指向
                bic_extnode = candidate;// 记录需要删除的candidate节点地址
                candidate = candidate->next;// 更新candidate指向下一个
                free(bic_extnode);
                bic_extnode = current_bic->candilist;
                continue;
            }
            else
            {
                /* 第一个candidate没删 */
                bic_extnode->next = candidate->next;
                base_extnode = candidate;
                candidate = candidate->next;
                free(base_extnode);
                continue;
            }

        }
        else
        {
            bic_extnode = candidate;
            candidate = candidate->next;
        }
    }


    previous = current_bic->prelist;
    while ( NULL != previous )
    {
        delete_flag = true;
        func1 = previous->wgnode;
        while (NULL != func1)
        {
            if (func1->time_end != 0)
            {
                delete_flag = false;
                break;
            }
            else
            {
                func1 = func1->next;
            }
        }
        if (delete_flag)
        {
           /* 若此candidate需要删除，那么其下的wgnode全删 */
            func1 = previous->wgnode;
            while ( NULL!= func1)
            {
                func2 = func1->next;
                free(func1);
                func1 = func2;
            }
            /* 主要是看第一个candidate删没删 */
            if (previous == current_bic->prelist)
            {
                /* 第一个candidate删了 */
                current_bic->prelist = previous->next;// 因为candidate不是全删，只删除此candi，故在更新candidate之前需要先更新current_bic->candilist指向
                bic_extnode = previous;// 记录需要删除的candidate节点地址
                previous = previous->next;// 更新candidate指向下一个
                free(bic_extnode);
                bic_extnode = current_bic->prelist;
                continue;
            }
            else
            {
                /* 第一个candidate没删 */
                bic_extnode->next = previous->next;
                base_extnode = previous;
                previous = previous->next;
                free(base_extnode);
                continue;
            }

        }
        else
        {
            bic_extnode = previous;
            previous = previous->next;
        }
    }
}

bool isprun(const ExtendNode *previ, const ExtendNode *candi)
{
    vector<char*> pre;
    vector<char*> can;
    WgNode *previous = previ->wgnode;
    WgNode *candidate = candi->wgnode;
    while (NULL != previous)
    {
        if (pre.empty())
        {
            if (previous->time_end != 0)
            {
                pre.push_back(previous->wgname);
            }
            previous = previous->next;
        }
        else
        {
            if (0 == strcmpi(pre.back(),previous->wgname))
            {
                previous = previous->next;
            }
            else
            {
                if (previous->time_end != 0)
                { 
                    pre.push_back(previous->wgname);
                }
                previous = previous->next;
            }
        }
    }


    while (NULL != candidate)
    {
        if (can.empty())
        {
            if (candidate->time_end != 0)
            {
                can.push_back(candidate->wgname);
            }
            candidate = candidate->next;
        }
        else
        {
            if (0 == strcmpi(can.back(),candidate->wgname))
            {
                candidate = candidate->next;
            }
            else
            {
                if (candidate->time_end !=0)
                {    
                    can.push_back(candidate->wgname);
                }
                candidate = candidate->next;
            }
        }
    }

    auto iter_can = can.begin();
    auto iter_pre = pre.begin();
    while (iter_can != can.end())
    {
        for (iter_pre = pre.begin(); iter_pre != pre.end(); iter_pre++)
        {
            if (0 == strcmpi((*iter_can),(*iter_pre)))
            {
                break;
            }
        }
        if (iter_pre == pre.end())
        {
            return false;// 如果存在候选有某类wgnode，但前驱没有，则不能剪枝
        }
        else
        {
            iter_can++;
        }
    }

    /* 前驱和候选的wgnode，同一类型的权值节点，除去相同的部分，剩下的全是follows或者meets关系，才能剪枝 */
    for (iter_can = can.begin(); iter_can != can.end(); iter_can++)
    {
        candidate = candi->wgnode;
        while (NULL != candidate)
        {
            if (atoi(candidate->wgname+1) < atoi((*iter_can)+1))
            {
                candidate = candidate->next;
                continue;
            }
            else if (atoi(candidate->wgname+1) == atoi((*iter_can)+1))
            {
                previous = previ->wgnode;
                while (NULL != previous)
                {
                    if (atoi(previous->wgname+1) < atoi(candidate->wgname+1))
                    {
                        previous = previous->next;
                        continue;
                    }
                    else if (atoi(previous->wgname+1) == atoi(candidate->wgname+1))
                    {
                        if (previous==candidate)
                        {
                            previous = previous->next;
                            continue;
                        }

                        if ((previous->time_start != previous->time_end) && (candidate->time_start != candidate->time_end))
                        {
                            if (max(previous->time_start,candidate->time_start) < min(previous->time_end, candidate->time_end))
                            {
                                return false;
                            }
                        }
                        else if ((previous->time_start != previous->time_end) && (candidate->time_start == candidate->time_end))
                        {
                            if ((previous->time_start < candidate->time_start) && (candidate->time_end < previous->time_end))
                            {
                                return false;
                            }
                        }
                        else if ((previous->time_start == previous->time_end) && (candidate->time_start != candidate->time_end))
                        {
                            if  ((candidate->time_start<previous->time_start) && (previous->time_end<candidate->time_end))
                            {
                                return false;
                            }
                        }
                        else if ((previous->time_start == previous->time_end) && (candidate->time_end == candidate->time_end))
                        {
                            if (previous->time_start==candidate->time_start)
                            {
                                return false;
                            }
                        }

                        previous = previous->next;
                        continue;
                    }
                    else if (atoi(previous->wgname+1) > atoi(candidate->wgname+1))
                    {
                        break;
                    }
                }
                candidate = candidate->next;
                continue;
            }
            else if (atoi(candidate->wgname+1) > atoi((*iter_can)+1))
            {
                break;
            }
        }
    }

    /* 运行到这里，说明满足剪枝条件，需要剪枝 */
    return true;

}


void pruning(BiCluster *current_bic)
{
    ExtendNode *candidate, *previous, *move;
    WgNode * wg1, *wg2;

    /* 剪枝判断 */
    candidate = current_bic->candilist;
    bool prun_flag = false;
    while (NULL != candidate) // 每一个候选节点
    {
        prun_flag = false;
        previous = current_bic->prelist;
        while (NULL != previous) // 用前驱节点来判断
        {
            prun_flag = isprun(previous, candidate);
            if (prun_flag) // 存在一个前驱节点能将此候选节点剪枝，则删除此候选节点
            {
                if (candidate == current_bic->candilist) // 第一个候选节点
                {
                    current_bic->candilist = candidate->next;
                    wg1 = candidate->wgnode;
                    while(NULL != wg1)
                    {
                        wg2 = wg1;
                        wg1 = wg1->next;
                        free(wg2);
                        wg2 = NULL;
                    }
                    free(candidate);
                    candidate = current_bic->candilist;
                }
                else
                {
                    move->next = candidate->next;
                    wg1 = candidate->wgnode;
                    while (NULL != wg1)
                    {
                        wg2 = wg1;
                        wg1 = wg1->next;
                        free(wg2);
                        wg2 = NULL;
                    }
                    free(candidate);
                    candidate = move->next;
                }
                break;// 跳出，判断下一个候选节点
            }
            previous = previous->next;
        }
        if (!prun_flag)
        {
            move = candidate;
            candidate = candidate->next;
        }
    }

    /* 如果存在前驱节点的权值点类别是当前双聚类权值类别的超集，则当前双聚类不输出 */
    previous = current_bic->prelist;
    vector<char*> pre;
    vector<char*> cur;
    while(NULL != previous)
    {
        wg1 = previous->wgnode;
        while (NULL != wg1)
        {
            if (pre.empty())
            {
                if (wg1->time_end != 0)
                {
                    pre.push_back(wg1->wgname);
                }
                wg1 = wg1->next;
            }
            else
            {
                if (0 == strcmpi(pre.back(),wg1->wgname))
                {
                    wg1 = wg1->next;
                }
                else
                {
                    if (wg1->time_end != 0)
                    { 
                        pre.push_back(wg1->wgname);
                    }
                    wg1 = wg1->next;
                }
            }
        }

        wg2 = current_bic->wglist;
        while (NULL != wg2)
        {
            if (cur.empty())
            {
                if (wg2->time_end != 0)
                {
                    cur.push_back(wg2->wgname);
                }
                wg2 = wg2->next;
            }
            else
            {
                if (0 == strcmpi(cur.back(),wg2->wgname))
                {
                    wg2 =wg2->next;
                }
                else
                {
                    if (wg2->time_end != 0)
                    { 
                        cur.push_back(wg2->wgname);
                    }
                    wg2 = wg2->next;
                }
            }
        }

        auto iter_cur = cur.begin();
        auto iter_pre = pre.begin();
        while (iter_cur != cur.end())
        {
            int cnt = count(pre.begin(),pre.end(),(*iter_cur));
            if (cnt > 0)
            {
                iter_cur++;
                continue;
            }
            else if(cnt == 0)
            {
                break;
            }
        }
        if (iter_cur == cur.end())
        {
            current_bic->flag_output = false;
            break;// 找到前驱节点使当前双聚类不输出后，跳出
        }
        previous = previous->next;
    }    
    

    /* 
        如果存在候选节点的权值点类别是当前双聚类权值类别的超集，则当前双聚类不输出
        这里用previous指向candidate 
    */
    candidate = current_bic->candilist;
    previous = candidate;
    pre.clear();
    cur.clear();
    while(NULL != previous)
    {
        wg1 = previous->wgnode;
        while (NULL != wg1)
        {
            if (pre.empty())
            {
                if (wg1->time_end != 0)
                {
                    pre.push_back(wg1->wgname);
                }
                wg1 = wg1->next;
            }
            else
            {
                if (0 == strcmpi(pre.back(),wg1->wgname))
                {
                    wg1 = wg1->next;
                }
                else
                {
                    if (wg1->time_end != 0)
                    { 
                        pre.push_back(wg1->wgname);
                    }
                    wg1 = wg1->next;
                }
            }
        }

        wg2 = current_bic->wglist;
        while (NULL != wg2)
        {
            if (cur.empty())
            {
                if (wg2->time_end != 0)
                {
                    cur.push_back(wg2->wgname);
                }
                wg2 = wg2->next;
            }
            else
            {
                if (0 == strcmpi(cur.back(),wg2->wgname))
                {
                    wg2 =wg2->next;
                }
                else
                {
                    if (wg2->time_end != 0)
                    { 
                        cur.push_back(wg2->wgname);
                    }
                    wg2 = wg2->next;
                }
            }
        }

        auto iter_cur = cur.begin();
        auto iter_pre = pre.begin();
        while (iter_cur != cur.end())
        {
            int cnt = count(pre.begin(),pre.end(),(*iter_cur));
            if (cnt > 0)
            {
                iter_cur++;
                continue;
            }
            else if(cnt == 0)
            {
                break;
            }
        }
        if (iter_cur == cur.end())
        {
            current_bic->flag_output = false;
            break;
        }
        previous = previous->next;
    }
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
    bool flag_prun = false;

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
                flag_prun = false;
                /* base_ext1 != base_ext说明是ext1是ext的前驱 */
                while (base_ext1 != base_ext)
                {
                    /* 应用剪枝策略判断 */
 
                    if (isprun(base_ext1, base_ext))
                    {
                        flag_prun = true;
                        break;
                    }

                    base_ext1 = base_ext1->next;
                }

                /* 若被剪枝则不构造，否则构造 */
                if (flag_prun)
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
                    //current_bic = NULL; // 在destroy后已经置空了
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

        /* 经判断后输出 */
        if (current_bic->flag_output)
        {
            base_ext = current_bic->extlist;
            while (NULL != base_ext)
            {
                fprintf(fresult, "%s", base_ext->extname);
                base_ext = base_ext->next;
            }
            fprintf(fresult,":\n");
            Wvalue_move = current_bic->wglist;
            while(NULL != Wvalue_move)
            {
                if (Wvalue_move->time_end != 0)
                {
                    fprintf(fresult, "\t(%s,%d,%d)", Wvalue_move->wgname, Wvalue_move->time_start, Wvalue_move->time_end);
                }
                Wvalue_move = Wvalue_move->next;
            }
            fprintf(fresult,"\n");
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
                    next_bic->extlist = base_ext;
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
                if (NULL == next_bic->wglist)
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
            //next_bic = NULL;
            /* 继续依据下一个候选节点扩展挖掘 */
            candi = candi->next;
        }
    }
}