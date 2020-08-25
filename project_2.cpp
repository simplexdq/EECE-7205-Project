#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <list>
#include <iterator>
#include <chrono>
#include <iomanip>
using namespace std;

void primary_assignment(int tl_k[][3],int C[], int t_re, int n, int k){
    int tl_min[n];
    for(int i=0; i<n; i++){
        tl_min[i] = INT_MAX;
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<k; j++){
            if(tl_min[i]>tl_k[i][j]){
                tl_min[i] = tl_k[i][j];
            }
        }
    }
    for(int i=0; i<n; i++){
        if(tl_min[i] > t_re){
            C[i] = 1;
        }
        else
            C[i] = 0;
    }
}

void task_prioritzing(int tl_k[][3], int priority[], int priority_final[], int succ[][10], int w[], int C[], int t_re, int n, int k){
    for(int i=0; i<n; i++){
        if(C[i] == 1){
            w[i] = t_re;
        }
        else{
            int sum = 0;
            for(int j=0; j<k; j++){
                sum += tl_k[i][j];
            }
            w[i] = sum/k;
        }
    }
    priority[n-1] = w[n-1];//exit task
    for(int i=n-1; i>=0; i--){
        int max_j = 0;
        for(int j=n-1; j>=0; j--){
            if(succ[i][j] == 1 && priority[j] > max_j){
                max_j = priority[j];
                priority[i] = w[i] + max_j;  
            }
        }
    }
    vector<pair<int,int > > vect;
    for (int i=0; i<n; i++){
        vect.push_back(make_pair(priority[i],i));
    }
    sort(vect.begin(), vect.end());
    for(int i=0; i<n; i++){
        priority_final[i] = vect[i].second;
    }
}

void execution_unit_selection(int tl_k[][3],int priority_final[], int succ[][10], int C[], int rt_l[], int rt_c[], int rt_ws[], int ft_ws[], int ft_wr[], int ft_l[], int ft[], int core[], int pos1[], int n, int k, int t_s, int t_r, int t_c, vector<vector<int> > curr_pos){
    int f_i = priority_final[n-1];//schedule first element
    rt_l[f_i] = 0;
    rt_ws[f_i] = 0;
    ft_ws[f_i] = rt_ws[f_i] + t_s;
    rt_c[f_i] = ft_ws[f_i];
    if(C[f_i] == 1){
        ft_wr[f_i] = rt_c[f_i] + t_c + t_r;
        ft_l[f_i] = 0;
        ft[f_i] = ft_wr[f_i];
        core[3] = ft[f_i];
        curr_pos[0].push_back(f_i);
        pos1[f_i] = 3;
    }
    else{
        int tl_min = INT_MAX;
        int index;
        for(int i=0; i<k; i++){
            if(tl_k[f_i][i]<tl_min){
                tl_min = tl_k[f_i][i];
                index = i;
            }
        }
        ft_l[f_i] = rt_l[f_i] + tl_min;
        ft_wr[f_i] = rt_c[f_i] + t_c + t_r;
        if(ft_l[f_i] <= ft_wr[f_i]){
            ft[f_i] = ft_l[f_i];
            ft_wr[f_i] = 0;
            core[3] = ft_ws[f_i];
            curr_pos[index+1].push_back(f_i);
            pos1[f_i] = index;
        }
        else{
            ft[f_i] = ft_wr[f_i];
            ft_l[f_i] = 0;
            core[index] = ft[f_i];
            curr_pos[0].push_back(f_i);
            pos1[f_i] = 3;
        }
    }
    for(int a=n-2; a>=0; a--){
        int i = priority_final[a];
        //cout<<i;
        int max_j_l = 0;
        for(int j=0; j<n; j++){
            if(succ[j][i] == 1 && max_j_l < max(ft_l[j],ft_wr[j])){
                max_j_l = max(ft_l[j],ft_wr[j]);
            }
        }
        rt_l[i] = max_j_l;
        int max_j_ws = 0;
        for(int j=0; j<n; j++){
            if(succ[j][i] == 1 && max_j_ws < max(ft_l[j],ft_ws[j])){
                max_j_ws = max(ft_l[j],ft_ws[j]);
            }
        }
        rt_ws[i] = max_j_ws;
        ft_ws[i] = max(core[3],rt_ws[i]) + t_s;
        int max_j_c = 0;
        for(int j=0; j<n; j++){
            if(succ[j][i] == 1 && max_j_c < ft_wr[j]-t_r){
                max_j_c = ft_wr[j]-t_r;
            }
        }
        rt_c[i] = max(ft_ws[i],max_j_c);
        if(C[i] == 1){
            ft_wr[i] = rt_c[i] + t_c + t_r;
            ft[i] = ft_wr[i];
            ft_l[i] = 0;
            core[3] = ft_ws[i];
            curr_pos[0].push_back(i);
            pos1[i] = 3;
        }
        else{
            int rt, index;
            int f = INT_MAX;
            for(int j=0; j<k; j++){
                rt = max(rt_l[i],core[j]);
                if(f > rt + tl_k[i][j]){
                    f = rt + tl_k[i][j];
                    index = j;
                }
            }
            rt_l[i] = f - tl_k[i][index];
            ft_l[i] = f;
            ft_wr[i] = rt_c[i] + t_c + t_r;
            if(ft_l[i] <= ft_wr[i]){
                ft[i] = ft_l[i];
                ft_wr[i] = 0;
                core[index] = ft[i];
                curr_pos[index+1].push_back(i);
                pos1[i] = index;
            }
            else{
                ft[i] = ft_wr[i];
                ft_l[i] = 0;
                core[3] = ft[i];
                curr_pos[0].push_back(i);
                pos1[i] = 3;
            }
        }
    }
}

int kernel( vector<vector<int> > curr_pos, int tl_k[][3], int succ[][10],int pos1[], int tmax, int t_total, float e_total, int n, int k, int st[], int et[], int E_c, int E_l[][3]){
    int out = 0;
    int count = 0;
    while(out == 0){
        float max_ratio = 0;
        int new_n = 0, new_k = 0, new_index1 = 0, new_index2 = 0, new_t = t_total;
        float new_e = e_total;
        int less_t1 =0, less_t2 = 0;
        int temp_core[10], new_st[10], new_ft[10];;
        for(int i=0; i<n; i++){
            for(int j=0; j<k+1; j++){
                int pos2[10], core3[4], rt[10], rt1[10], ft[10], ft1[10], pushed[10];
                vector<vector<int> > curr_pos1(4);
                int index1, index2 = 0;
                for(int i=0; i<10; i++){
                    rt[i] = 0;
                    ft[i] = 0;
                    pos2[i] = pos1[i];
                    ft1[i] = et[i];
                    rt1[i] = st[i];
                    pushed[i] = 0;
                }
                for(int a=0; a<curr_pos.size(); a++){
                    core3[a] = 0;
                    for(int b=0; b<curr_pos[a].size(); b++){
                        curr_pos1[a].push_back(curr_pos[a][b]);
                    }
                }
                int current_core = pos1[i];
                for(int a=0; a<curr_pos1[current_core].size(); a++){
                    if(curr_pos1[current_core][a] == i){
                        index1 = a;
                    }
                }
                curr_pos1[current_core].erase(curr_pos1[current_core].begin()+index1);
                if(j == 3){
                    int max_j_ws = 0;
                    for(int a=0; a<n; a++){
                        if(succ[a][i] == 1 && max_j_ws < ft1[a]){
                            max_j_ws = ft1[a];
                        }
                    }
                    rt[i] = max_j_ws;
                }
                else{
                    int max_j_l = 0;
                    for(int a=0; a<n; a++){
                        if(succ[a][i] == 1 && max_j_l < ft1[a]){
                            max_j_l = ft1[a];
                        }
                    }
                    rt[i] = max_j_l;
                }
                pos2[i] = j;
                if(curr_pos1[j].size() == 0){
                    index2 = 0;
                }
                else if(curr_pos1[j].size() == 1){
                    if(rt1[curr_pos1[j][0]] > rt[i]){
                        index2 = 0;
                    }
                    else{
                        index2 = 1;
                    }
                }
                else{
                    if(rt1[curr_pos1[j][0]] > rt[i]){
                        index2 = 0;
                    }
                    else if(rt1[curr_pos1[j][curr_pos1[j].size()-1]] <= rt[i]){
                        index2 = curr_pos1[j].size();
                    }
                    else{
                        for(int b=0; b<curr_pos1[j].size()-1; b++){
                            if(rt[i]>=rt1[curr_pos1[j][b]] && rt[i]<=rt1[curr_pos1[j][b+1]]){
                                index2 = b+1;
                            }
                        }
                    }
                }
                curr_pos1[j].insert(curr_pos1[j].begin()+index2,i);
                int ready1[10], ready2[10];
                for(int a=0; a<10; a++){
                    ready1[a] = 0;
                }
                for(int a=0; a<10; a++){
                    for(int b=0; b<10; b++){
                        if(succ[a][b] == 1){
                            ready1[b] += 1;
                        }
                    }
                    ready2[a] = 1;
                }
                for(int a=0; a<4; a++){
                    if(curr_pos1[a].size()>0){
                        ready2[curr_pos1[a][0]] = 0;
                    }
                }
              stack<int> s;
                for(int a=0; a<10; a++){
                    if(ready1[a] == 0 && ready2[a] == 0 && pushed[a] == 0){
                        s.push(a);
                        pushed[a] = 1;
                    }
                }
                int current1 = s.top();
                s.pop();
                rt[current1] = 0;
                if(pos2[current1] == 3){
                    rt[current1] = max(core3[pos2[current1]],rt[current1]);
                    ft[current1] = rt[current1] + 5;
                    core3[pos2[current1]] = rt[current1] + 3;
                }
                else{
                    rt[current1] = max(core3[pos2[current1]],rt[current1]);
                    ft[current1] = rt[current1] + tl_k[current1][pos2[current1]];
                    core3[pos2[current1]] = ft[current1];
                }
                for(int a=0; a<10; a++){
                    if(succ[current1][a] == 1){
                        ready1[a] -= 1;
                    }
                }
                ready2[current1] = 1;
                if(curr_pos1[pos2[current1]].size()>1){
                    for(int a=1; a<curr_pos1[pos2[current1]].size(); a++){
                        if(curr_pos1[pos2[current1]][a-1] == current1){
                            ready2[curr_pos1[pos2[current1]][a]] = 0;
                        }
                    }
                }
                for(int a=0; a<10; a++){
                    if(ready1[a] == 0 && ready2[a] == 0 && pushed[a] == 0){
                        s.push(a);
                        pushed[a] = 1;
                    }
                }
                while(s.size() != 0){
                    int current = s.top();
                    s.pop();
                    if(pos2[current] == 3){
                        int max_j_ws1 = 0;
                        for(int a=0; a<n; a++){
                            if(succ[a][current] == 1 && max_j_ws1 < ft[a]){
                                max_j_ws1 = ft[a];
                            }
                        }
                        rt[current] = max_j_ws1;
                    }
                    else{
                        int max_j_l1 = 0;
                        for(int a=0; a<n; a++){
                            if(succ[a][current] == 1 && max_j_l1 < ft[a]){
                                max_j_l1 = ft[a];
                            }
                        }
                        rt[current] = max_j_l1;
                    }
                    if(pos2[current] == 3){
                        rt[current] = max(core3[pos2[current]],rt[current]);
                        ft[current] = rt[current] + 5;
                        core3[pos2[current]] = rt[current] + 3;
                    }
                    else{
                        rt[current] = max(core3[pos2[current]],rt[current]);
                        ft[current] = rt[current] + tl_k[current][pos2[current]];
                        core3[pos2[current]] = ft[current];
                    }
                    for(int a=0; a<10; a++){
                        if(succ[current][a] == 1){
                            ready1[a] -= 1;
                        }
                    }
                    ready2[current] = 1;
                    if(curr_pos1[pos2[current]].size()>1){
                        for(int a=1; a<curr_pos1[pos2[current]].size(); a++){
                            if(curr_pos1[pos2[current]][a-1] == current){
                                ready2[curr_pos1[pos2[current]][a]] = 0;
                            }
                        }
                    }
                    for(int a=0; a<10; a++){
                        if(ready1[a] == 0 && ready2[a] == 0 && pushed[a] == 0){
                            s.push(a);
                            pushed[a] = 1;
                        }
                    }
                }
                int current_t = ft[n-1];
                int current_e = 0;
                for(int a=0; a<10; a++){
                    if(pos2[a] == 3){
                        current_e += E_c;
                    }
                    else{
                        current_e += E_l[a][pos2[a]];
                    }
                }
                if(current_t <= t_total && current_e < new_e){
                    less_t1 = 1;
                    new_n = i;
                    new_k = j;
                    new_index1 = index1;
                    new_index2 = index2;
                    new_t = current_t;
                    new_e = current_e;
                    for(int a=0; a<10; a++){
                        temp_core[a] = pos2[a];
                        new_st[a] = rt[a];
                        new_ft[a] = ft[a];
                     }
                }
                if(current_t > t_total && current_t <= tmax && less_t1 == 0 && current_e < e_total && max_ratio < double((e_total - current_e) / (current_t - t_total))){
                    max_ratio = double((e_total - current_e) / (current_t - t_total));
                    less_t2 = 1;
                    new_n = i;
                    new_k = j;
                    new_index1 = index1;
                    new_index2 = index2;
                    new_t = current_t;
                    new_e = current_e;
                    for(int a=0; a<10; a++){
                        temp_core[a] = pos2[a];
                        new_st[a] = rt[a];
                        new_ft[a] = ft[a];
                    }
                }

            }
        }
        if(less_t1 != 1 && less_t2 != 1){
            out = 1;
        }
        else{
            curr_pos[pos1[new_n]].erase(curr_pos[pos1[new_n]].begin()+new_index1);
            curr_pos[new_k].insert(curr_pos[new_k].begin()+new_index2,new_n);
            t_total = new_t;
            e_total = new_e;
            for(int a=0; a<10; a++){
                pos1[a] = temp_core[a];
                st[a] = new_st[a];
                et[a] = new_ft[a];
            }
            if(less_t1 != 1 && less_t2 != 1){
                out = 1;
            }
            count += 1;
        }
    }

    cout<<endl;
    cout<<"Task scheduling result by the MCC task scheduling algorithm:"<<endl;

    for(int i=0; i<curr_pos.size(); i++){
        if(i == 3){
            cout<<"wireless sending : ";
            for(int j=0; j<curr_pos[i].size(); j++){
                cout<<st[curr_pos[i][j]]<<"-"<<et[curr_pos[i][j]] - 2 - 2<<" for Task"<<curr_pos[i][j]+1<<"; ";
            }//2=t_c+t_r
            cout<<endl;
            cout<<"Cloud: ";
            for(int j=0; j<curr_pos[i].size(); j++){
                cout<<st[curr_pos[i][j]] + 1<<"-"<<et[curr_pos[i][j]] - 3<<" for Task"<<curr_pos[i][j]+1<<"; ";
            }
            cout<<endl;
            cout<<"wireless receiving : ";
            for(int j=0; j<curr_pos[i].size(); j++){
                cout<<st[curr_pos[i][j]] + 2<<"-"<<et[curr_pos[i][j]] - 2<<" for Task"<<curr_pos[i][j]+1<<"; ";
            }//2=t_c+t_r
            cout<<endl;
        }
    
        else{
            cout<<"Core "<<i+1<<": ";
            for(int j=0; j<curr_pos[i].size(); j++){
                cout<<st[curr_pos[i][j]]<<"-"<<et[curr_pos[i][j]]<<" for Task"<<curr_pos[i][j]+1<<"; ";

        }
        }
        cout<<endl;
    }

        cout<<"Energy Consumption of scheduling: "<<e_total<<"   Total Completion Time: "<<t_total<<endl;

}


int main(int argc, char *argv[])
{
      int tl_k[][3]={{9,7,5},
                     {8,6,5},
                     {6,5,4},
                     {7,5,3},
                     {5,4,2},
                     {7,6,4},
                     {8,5,3},
                     {6,4,2},
                     {5,3,2},
                     {7,4,2}};
    // int tl_k[][3]={{5,7,9},
    //                  {5,6,8},
    //                  {4,5,6},
    //                  {3,5,7},
    //                  {2,4,5},
    //                  {4,6,7},
    //                  {2,5,8},
    //                  {2,4,6},
    //                  {2,3,5},
    //                  {2,4,7}};
        int succ[][10]={{0,1,1,1,1,1,0,0,0,0},
                    {0,0,0,0,0,0,0,1,1,0},
                    {0,0,0,0,0,0,1,0,0,0},
                    {0,0,0,0,0,0,0,1,1,0},
                    {0,0,0,0,0,0,0,0,1,0},
                    {0,0,0,0,0,0,0,1,0,0},
                    {0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,0}}; 
     //int succ[][10]={{0,1,1,1,1,1,0,0,0,0},
    //                 {0,0,0,0,0,0,1,0,0,0},
    //                 {0,0,0,0,0,0,1,0,0,0},
    //                 {0,0,0,0,0,0,0,1,0,0},
    //                 {0,0,0,0,0,0,0,0,1,0},
    //                 {0,0,0,0,0,0,0,0,1,0},
    //                 {0,0,0,0,0,0,0,0,0,1},
    //                 {0,0,0,0,0,0,0,0,0,1},
    //                 {0,0,0,0,0,0,0,0,0,1},
    //                 {0,0,0,0,0,0,0,0,0,0}};
    int C[10]; 
    int priority[10], priority_final[10], w[10], core[4], pos1[10];
    int rt_l[10], rt_c[10], rt_ws[10], ft_ws[10], ft_wr[10], ft_l[10], ft[10];
    vector<vector<int> > curr_pos(4);
    for(int i=0; i<10; i++){
        C[i] = 0;
        priority[i] = 0;
        priority_final[i] = 0;
        w[i] = 0;
        rt_l[i] = 0;
        rt_ws[i] = 0;
        rt_c[i] = 0;
        ft_ws[i] = 0;
        ft_wr[i] = 0;
        ft_l[i] = 0;
        ft[i] = 0;
        pos1[i] = 0;
    }
    for(int i=0; i<4; i++){
        core[i] = 0;
    }
    int k = 3, n = 10;
    int t_s = 3, t_c = 1, t_r = 1;
    int t_re = t_s + t_c + t_r;
    int E_l[10][3];
    int pk[] = {1,2,4};
    float ps = 0.5;
    float E_c = ps * t_s;
    for(int i=0; i<10; i++){
      for(int j=0; j<3; j++){
          E_l[i][j] = pk[j] * tl_k[i][j];
      }
    }
    auto start = chrono::high_resolution_clock::now();
    ios_base::sync_with_stdio(false);
    primary_assignment(tl_k,C,t_re,n,k);
    task_prioritzing(tl_k,priority,priority_final,succ,w,C,t_re,n,k);
    execution_unit_selection(tl_k,priority_final,succ,C,rt_l,rt_c,rt_ws,ft_ws,ft_wr,ft_l,ft,core,pos1,n,k,t_s,t_r,t_c,curr_pos);
    for(int i=0; i<4; i++){
        for(int j=0; j<10; j++){
            if(pos1[j] == i){
                curr_pos[i].push_back(j);
            }
        }
    }
    float e_total = 0;
    for(int i=0; i<10; i++){
        if(pos1[i] == 3){
            e_total += E_c;
        }
        else{
            e_total += E_l[i][pos1[i]];
        }
    }
    int st[10];
    for(int i=0; i<10; i++){
        st[i] = max(rt_l[i],rt_ws[i]);
    }
    int tmin = ft[n-1];
    int tmax = 27;
    cout<<"Task scheduling result by the initial task scheduling algorithm:"<<endl;
    for(int i=0; i<curr_pos.size(); i++){
        if(i == 3){
            cout<<"wireless sending : ";
            for(int j=0; j<curr_pos[i].size(); j++){
                cout<<st[curr_pos[i][j]]<<"-"<<ft[curr_pos[i][j]]  - 2<<" for Task"<<curr_pos[i][j]+1<<"; ";
            }//2=t_c+t_r
            cout<<endl;
            cout<<"Cloud: ";
            for(int j=0; j<curr_pos[i].size(); j++){
                cout<<st[curr_pos[i][j]] +3<<"-"<<ft[curr_pos[i][j]] -1 <<" for Task"<<curr_pos[i][j]+1<<"; ";
            }//2=t_c+t_r
            cout<<endl;
            cout<<"wireless receiving : ";
            for(int j=0; j<curr_pos[i].size(); j++){
                cout<<st[curr_pos[i][j]] + 4<<"-"<<ft[curr_pos[i][j]] <<" for Task"<<curr_pos[i][j]+1<<"; ";
            }//2=t_c+t_r
            cout<<endl;
        }
        else{
            cout<<"Core"<<i+1<<": ";
            for(int j=0; j<curr_pos[i].size(); j++){
            cout<<st[curr_pos[i][j]]<<"-"<<ft[curr_pos[i][j]]<<" for Task"<<curr_pos[i][j]+1<<"; ";
            }
        }
        cout<<endl;
    }
    cout<<"Energy Consumption of initial scheduling: "<<e_total<<"   Total Completion Time: "<<tmin<<endl;
    cout<<endl;
    kernel(curr_pos,tl_k,succ,pos1,tmax,tmin,e_total,10,3,st,ft,E_c,E_l);
    return 0;
}
