# ifndef _ALG_H_
# define _ALG_H_

#include <iostream>
#include <string.h>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

const int BUF_SIZE = 200; // 用来fgets()读行设置的最大个数,确保能读完一行

struct INTERVAL
{
    int start;
    int end;

    bool operator==(const INTERVAL& time)
    {
        return ((this->start == time.start) && (this->end == time.end));
    }
};

/* 权值节点 */
struct WgNode
{
    char *wgname;
    int time_start;
    int time_end;
    WgNode *next;
    bool operator==(const WgNode& interval)
    {
        return ((this->time_start == interval.time_start) && (this->time_end == interval.time_end));
    }
};

/* 十字链--纵向扩展节点-ext */
struct ExtendNode
{
    char *extname;
    WgNode *wgnode;
    ExtendNode *next;
};

/* 十字链--横向扩展头节点-base */
struct HeadNode
{
    char *headname;
    ExtendNode *extnode;
    HeadNode *next;
};

/* 当前双聚类节点定义 */
struct BiCluster
{
    ExtendNode *extlist; // 当前双聚类的扩展节点链
    WgNode *wglist; // 当前双聚类扩展节点下的权值链
    ExtendNode *candilist;  // 当前扩展结点下的候选节点链
    ExtendNode *prelist;    // 当前扩展节点的前驱节点链
    bool flag_output;   // 是否输出的标记，非剪枝标记
};

void hello(void);
void get_nonoverlapping_intervals(vector<vector<int>> &interval);
ExtendNode *save_filedata(const char *filepath);
WgNode *get_weighted_value(const ExtendNode *base, const ExtendNode *extend);
void ConWG(ExtendNode *filedata_head);
bool compare(const WgNode& time1, const WgNode& time2);
void get_pre_and_candi(BiCluster *current_bic);
bool isprun(const ExtendNode *previ, const ExtendNode *candi);
void pruning(BiCluster *current_bic);
void extend_mining(BiCluster *current_bic);
void destroy_bicluster(BiCluster *current_bic);
# endif