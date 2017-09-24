#include<algorithm>
#include<iostream>
#include<vector>
#include<queue>
#include<map>
using namespace std;
vector<int> adjacency_list_A[1024], adjacency_list_B[1024];
int n, m;
struct cell
{
    bool positive;
    int distance;
    int nodes_numer;
    int edges_numer;
    long long code;
    void calcCode()
    {
        code = 10000;
        code += distance;
        code *= 10000;
        code += nodes_numer;
        code *= 10000;
        code+=edges_numer;
        if(!positive) code *= (-1);
    }

};
cell  cell_info(int from, int to, vector<int>* adjacency_list)
{
    int path[1024];

    for(int i = 0;i <= n;i++)
        path[i] = -1;

    path[from] = 0;
    queue<int>q;
    q.push(from);
    while(!q.empty())
    {
        int curr = q.front();
        q.pop();
        for(int i = 0;i < adjacency_list[curr].size();i++)
        {
            if(curr == from && adjacency_list[curr][i] == to )
                continue;
            if(path[adjacency_list[curr][i]] == -1)
            {
                path[adjacency_list[curr][i]] = path[curr]+1;
                q.push(adjacency_list[curr][i]);
            }
        }
    }

    bool fromPath[1024];

    for(int i = 0;i <= n;i++)
        fromPath[i]=false;

    q.push(to);
    fromPath[to]=true;
    while(!q.empty())
    {
        int curr=q.front();
        q.pop();
        for(int i = 0;i < adjacency_list[curr].size();i++)
        {
            if(path[adjacency_list[curr][i]] == path[curr]-1)
            {
                q.push(adjacency_list[curr][i]);
                fromPath[adjacency_list[curr][i]]=true;
            }
        }
    }

    int edges_number = 0,nodes_number = 0;
    for(int i = 0;i <= n;i++)
    {
        if(fromPath[i])
        {
            nodes_number++;
            for(int j = 0;j < adjacency_list[i].size();j++)
            {
                if(fromPath[adjacency_list[i][j]])
                    edges_number++;
            }
        }
    }
    edges_number/=2;
    cell res;
    res.positive=false;
    for(int i = 0;i<adjacency_list[from].size();i++)
    {
        if(adjacency_list[from][i] == to)
        {
            res.positive = true;
            break;
        }
    }
    res.distance=path[to];
    res.nodes_numer = nodes_number;
    res.nodes_numer = edges_number;

    return res;
}
bool compare( const vector<int>& first, const vector<int>& second )
{
    for(int i=0;i<first.size();i++)
    {
        if(first[i]==second[i]) continue;
        if(first[i]<second[i]) return true;
        else return false;
    }
    return false;
}

cell Sa[1024][1024], Sb[1024][1024];

vector<int>frequencyA[1024], frequencyB[1024];

void calc_S( cell S[][1024], vector<int>* adjList )
{
    for(int y=1;y<=n;y++)
    {
        for(int x=1;x<=n;x++)
        {
            S[x][y]=cell_info(x, y, adjList);
            S[x][y].calcCode();
        }
    }
}
map<long long, int> make_signs_map(cell S[][1024])
{
    map<long long, int>map;
    for(int y=1;y <= n;y++)
    {
        for(int x = 1;x <= n;x++)
        {
            if(map[S[x][y].code] == 0)
            {
                map[S[x][y].code] = 1;
            }
        }
    }
    return map;
}
int signs_number(map<long long, int> &m)
{
    int currN = 1, res = 0;
    for(auto i = m.begin(); i != m.end();i++){
        res++;
        i->second=currN;
        currN++;
    }
    return res;
}

void reorderS(cell S[][1024], vector<int>* freq, int numberOfSigns)
{
    auto buffer = new cell[1024][1024];
    for(int y=1;y <= n;y++)
    {
        for(int x = 1;x <= n;x++)
        {
            buffer[x][y] = S[freq[x][numberOfSigns+1]][freq[y][numberOfSigns+1]];
        }
    }
    for(int y=1;y <= n;y++)
    {
        for(int x=1;x <= n;x++)
        {
           S[x][y] = buffer[x][y];
        }
    }

}
void fillFrequencyMatrix(vector<int>* freq, int numberOfSigns,map<long long, int> map, cell S[][1024])
{
    for(int i = 1;i <= n;i++) freq[i].resize(numberOfSigns+20,0);
    for(int y = 1;y <= n;y++)
    {
        for(int x = 1;x <= n;x++)
        {
            freq[y][map[S[x][y].code]]++;
        }
        freq[y][numberOfSigns+1]=y;
    }

}

void swapColumns(int first, int second)
{
     cell* buffer = new cell[n+2];
     for(int y=1;y <= n;y++)
     {
        buffer[y] = Sb[first][y];
     }
     for(int y=1;y <= n;y++)
     {
        Sb[first][y] = Sb[second][y];
     }
     for(int y = 1;y <= n;y++)
     {
        Sb[second][y] = buffer[y];
     }
     delete[] buffer;
}
void swapRolls(int first, int second)
{
    cell* buffer = new cell[n+2];
    for(int currX = 1;currX <= n;currX++)
    {
        buffer[currX] = Sb[currX][first];
    }
    for(int currX = 1;currX <= n;currX++)
    {
        Sb[currX][first] = Sb[currX][second];
    }
    for(int currX=1;currX <= n;currX++)
    {
        Sb[currX][second] = buffer[currX];
    }
    delete[] buffer;
}
bool findIsomorphismFunction(int* order_A, int* order_B)
{
    for(int y=1;y <= n;y++)
    {
        for(int x=1;x <= n;x++)
        {
            if(Sb[x][y].code != Sa[x][y].code)
            {
                ///cout<<"different at "<<x<<" "<<y<<" codes "<<Sa[x][y].code<<" "<<Sb[x][y].code<<endl;
                if(x < y)
                {
                    return 0;
                }
                for(int k = x+1;k <= n;k++)
                {
                    bool equal_arr = true;
                    for(int j = 1;j < y;j++)
                    {
                        if(Sb[x][j].code != Sb[k][j].code)
                        {
                            equal_arr=false;
                            break;
                        }
                    }
                    if(equal_arr && Sa[x][y].code == Sb[k][y].code)
                    {
                       /// cout<<"swap "<<x<<" "<<k<<endl;
                        swap(order_B[x], order_B[k]);
                        swapColumns(k,x);
                        swapRolls(k,x);

                        break;
                    }
                    if(k == n)
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
void input_edges_list()
{
    cin>>n>>m;
    for(int i=0;i<m;i++)
    {
        int a, b;
        cin>>a>>b;
        adjacency_list_A[a].push_back(b);
        adjacency_list_A[b].push_back(a);
    }

    for(int i=0;i<m;i++)
    {
        int a, b;
        cin>>a>>b;
        adjacency_list_B[a].push_back(b);
        adjacency_list_B[b].push_back(a);
    }
}
void input_matrix()
{
    cin>>n;
    for(int i = 1;i <= n;i++)
    {
        for(int j = 1;j <= n;j++)
        {
            bool a;
            cin>>a;
            if(a)
            {
                adjacency_list_A[i].push_back(j);
                adjacency_list_A[j].push_back(i);
            }
        }
    }
    cin>>n;
    for(int i = 1;i <= n;i++)
    {
        for(int j = 1;j <= n;j++)
        {
            bool a;
            cin>>a;
            if(a)
            {
                adjacency_list_B[j].push_back(i);
                adjacency_list_B[i].push_back(j);
            }
        }
    }
}
int main(){

    input_matrix();
    calc_S(Sa, adjacency_list_A);
    calc_S(Sb, adjacency_list_B);

    map<long long, int> mapA = make_signs_map(Sa);
    map<long long, int> mapB = make_signs_map(Sb);

    int numberOfSignsA = signs_number(mapA);
    int numberOfSignsB = signs_number(mapB);

    if(numberOfSignsA != numberOfSignsB)
    {
        cout<<"not isomorphic\n";
        return 0;
    }

    int order_A[1024], order_B[1024];

    fillFrequencyMatrix(frequencyA, numberOfSignsA, mapA, Sa);
    fillFrequencyMatrix(frequencyB, numberOfSignsB, mapB, Sb);

    sort(&frequencyA[1], &frequencyA[n+1], compare);
    sort(&frequencyB[1], &frequencyB[n+1], compare);

    for(int i=1;i<=n;i++)
        order_A[i]=frequencyA[i][numberOfSignsA+1];

    for(int i=1;i<=n;i++)
        order_B[i]=frequencyB[i][numberOfSignsB+1];

    reorderS(Sa, frequencyA, numberOfSignsA);
    reorderS(Sb, frequencyB, numberOfSignsB);

    if(!findIsomorphismFunction(order_A, order_B))
    {
        cout<<"not isomorphic\n";
        return 0;
    }

    cout<<"explicit isomorphism:\n";
    for(int i = 1;i <= n;i++)
        cout<<order_A[i]<<" "<<order_B[i]<<endl;
    return 0;
}
