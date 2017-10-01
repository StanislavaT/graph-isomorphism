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
int pathsA[1024][1024], pathsB[1024][1024], valuesA[1024][1024], valuesB[1024][1024];
void floyd(int values [][1024], int paths [][1024])
{
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if(values[i][j] > 0)
            {
                paths[i][j] = values[i][j];
                paths[j][i] = values[j][i];
            }
            else
            {
                paths[i][j] = 1000000;
                paths[j][i] = 1000000;
            }
        }
        paths[i][i]=0;
    }

    for(int mid = 1;mid <= n;mid++)
    {
        for(int from = 1;from <= n;from++)
        {
            for(int to = 1;to <= n;to++)
            {
                if( from!=to && paths[from][to] > paths[from][mid] + paths[mid][to])
                {
                    paths[from][to] = paths[from][mid] + paths[mid][to];
                }
            }
        }
    }
}
cell  cell_info(int from, int to, vector<int>* adjacency_list, int value [][1024], int paths [][1024])
{

    bool fromPath[1024];

    for(int i = 0;i <= n;i++)
        fromPath[i]=false;

    queue<int> q;
    q.push(to);
    fromPath[to]=true;
    while(!q.empty())
    {
        int curr = q.front();
        q.pop();
        for(int i = 0;i < adjacency_list[curr].size();i++)
        {
            if(paths[adjacency_list[curr][i]] == paths[curr] - value[curr][adjacency_list[curr][i]])
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
    res.distance=paths[from][to];
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

void calc_S( cell S[][1024], vector<int>* adjList, int value [][1024], int paths [][1024])
{
    for(int y = 1;y <= n;y++)
    {
        for(int x = 1;x <= n;x++)
        {
            S[x][y] = cell_info(x, y, adjList, value, paths);
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
void input_edges_list(vector<int>* adjacency_list, int values [][1024])
{
    cin>>n>>m;
    for(int i = 0;i < m;i++)
    {
        int a, b, v;
        cin>>a>>b>>v;
        adjacency_list[a].push_back(b);
        adjacency_list[b].push_back(a);
        values[a][b] = v;
        values[b][a] = v;
    }
}
void input_matrix(vector<int>* adjacency_list, int values [][1024])
{
    cin>>n;
    for(int i = 1;i <= n;i++)
    {
        for(int j = 1;j <= n;j++)
        {
            int a;
            cin>>a;
            if(a)
            {
                adjacency_list[i].push_back(j);
                adjacency_list[j].push_back(i);
                values[i][j] = a;
                values[j][i] = a;

            }
        }
    }

}
int main(){

    input_edges_list(adjacency_list_A, valuesA);
    input_edges_list(adjacency_list_B, valuesB);

    floyd(valuesA, pathsA);
    floyd(valuesB, pathsB);

    calc_S(Sa, adjacency_list_A, valuesA, pathsA);
    calc_S(Sb, adjacency_list_B, valuesB, pathsB);

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
