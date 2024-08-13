#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
#include<set>
#include<queue>
using namespace std;

//Global Declaration
const int V=50;
vector<int> wareHouses{1,48,29,36};
int property[V] = {0};
vector<int> GasStation{25,17,27,38};
vector<pair<int,int>> adj[V];
vector<vector<int>> dist(V,vector<int>(V,1e7));
int Tankcap = 250;

// custom camparator function
bool comp(pair<int,int> &a,pair<int,int> &b) {
    return a.second < b.second;
}
void floydwarshall(){   // Time complexity => O(n^3)

    // Initialization   O(n^2)
    for(int i=0;i<V;i++){
        for(auto adjNode:adj[i]){
            dist[i][adjNode.first] = adjNode.second;
        }
    }

    // Self loop -> dist=0;    O(n^2)
    for(int i=0;i<V;i++){
        for(int j=0;j<V;j++){
            if(i==j){
                dist[i][j]=0;
            }
        }
    }

    // Relaxation O(n^3)
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][j] > (dist[i][k] + dist[k][j])
                    && (dist[k][j] != 1e7
                        && dist[i][k] != 1e7))
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }
    return;
} 


// Gives the FinalList of items to be delivered using kanpsack algorithm (greedy approach)
vector<pair<int,int>> SystematicPackaging(vector<pair<int,int>> List,int capacity){ // O(n)
    vector<pair<int,int>> finalListOfItems;
    //First integer Represent wt of Item and second integer represent ID of that item...
    sort(List.begin(),List.end(),comp);
    //Sort the Given List according to their wt of items
    //since we are assuming that each and every package will be deliver in one Go
    //Push the smallest wt Element first according to capacity of truck
    int i=0;
    while(capacity>=0 && i < List.size()){
        capacity-=List[i].second;
        finalListOfItems.push_back(List[i]);
        i++;
    }
    if(capacity<0){
        finalListOfItems.pop_back();
    }
    return finalListOfItems;
}

// Gives the warehouse 
int wareHouseSelection(vector<pair<int,int>> List) {
    vector<pair<int,int>> data;
    
    // Calculating the Distances of all houses present in the list from all the warehouses and storing that warehouse which is closer to the house  ->  O(n^2)
    for(int i=0;i<List.size();i++) {
        int mini = 1e7;
        int wareHouse = 0;
        int house = List[i].first;
        for(int j=0;j<wareHouses.size();j++){
            if(dist[wareHouses[j]][List[i].first] < mini){
                mini = dist[wareHouses[j]][List[i].first]; 
                wareHouse = wareHouses[j];
            }
        }
        data.push_back({wareHouse,mini});
    }
    
    // Couting that how many times the warehouses are repeating  ->  O(n^2)
    vector<pair<int,int>> counters;
    for(int i=0;i<wareHouses.size();i++){
        int cnt = 0; 
        for(int j=0;j<data.size();j++){
            if(data[j].first == wareHouses[i]){
                cnt++;
            }
        }
        counters.push_back({cnt,wareHouses[i]});
    }
    int result = 0; 
    
    // selecting that warehouse which is repeating maximum times  ->  O(n)
    for(int i=1;i<counters.size();i++){
        if(counters[i].first>counters[result].first){
            result = i;
        }
    }
    return counters[result].second;
}

// Gives the Shortest Path between any two nodes    ->  O[n.Elog(V)]
vector<int> shortestPath(int srcNode,int lastNode) {

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int,int>>> pq;

        vector<int> dist(V, 1e9), parent(V);
        
        // Initializing the parents with themseleves
        for (int i = 0; i <= V; i++){
            parent[i] = i;
        }

        dist[srcNode] = 0;

        pq.push({0, srcNode});  // {dist, Node}
        while (!pq.empty())
        {
            auto it = pq.top();
            pq.pop();
            int node = it.second;
            int dis = it.first;

            for (auto it : adj[node])
            {
                int adjNode = it.first;
                int edW = it.second;

                if (dis + edW < dist[adjNode])
                {
                    dist[adjNode] = dis + edW;
                    pq.push({dis + edW, adjNode});

                    parent[adjNode] = node;
                }
            }
        }

        if (dist[lastNode] == 1e9)
            return {-1};

        vector<int> path;
        int node = lastNode;

        while (parent[node] != node)
        {
            path.push_back(node);
            node = parent[node];
        }
        path.push_back(srcNode);
        reverse(path.begin(), path.end());
        return path;
}

  
// bool comp2(vector<int> a,vector<int> b){
//     return a[2] > b[2];
// }

// void PathPrinting(vector<pair<int,int>> list){
//     vector<pair<int,int>> FinalList = SystematicPackaging(list,296);
//     int srcNode = wareHouseSelection(list);
//     vector<int> path;

//     // for(auto it : FinalList){
//     //     cout<<it.first<<" "<<it.second<<endl;
//     // }

//     vector<int> vis(V,0);

//     for(int i=0;i<FinalList.size();i++){
//         vis[FinalList[i].first] = 1;
//     }
//     vector<int> arr;
//     for(int i=0;i<FinalList.size();i++){
//         arr.push_back(FinalList[i].first);
//     }

//     vector<pair<int,int>> pairs;
//     for(int i=0;i<arr.size();i++){
//         for(int j=0;j<arr.size();j++){
//             if(arr[i]!=arr[j])
//             pairs.push_back({arr[i],arr[j]});
//         }
//     }
//     for(int i=0;i<arr.size();i++){
        
//         pairs.push_back({srcNode,arr[i]});
//     }

//     vector<vector<int>> data;
//     for(int i=0;i<pairs.size();i++){
//         vector<int> res(3);
//         res[0] = pairs[i].first;
//         res[1] = pairs[i].second;
//         int cnt = 0;
//         vector<int> path;
//         path = shortestPath(res[0],res[1]);
//         for(int i=0;i<path.size();i++){
//             if(vis[path[i]]==1){
//                 cnt++;
//             }
//         } 
//         res[2] = cnt;
//         data.push_back(res);
//     }
    
//     sort(data.begin(),data.end(),comp2);
//     vis[V]={0};
//     // for(int i=0;i<data.size();i++){
//     //     for(int j=0;j<data[i].size();j++){
//     //         cout<<data[i][j]<<" ";
//     //     }
//     //     cout<<endl;
//     // }
    
//     for(int i=0;i<data.size();i++){
//         if(data[i][0]==1){
//             continue;
//         }
//         vector<int> path;
//         path = shortestPath(srcNode,data[i][0]);
//         for(int j=0;j<path.size()-1;j++){
//             cout<<path[j]<<"-> ";
//             if(vis[path[j]]==0){
//                 vis[path[j]]=1;
//             }
//         }
//         srcNode = data[i][1];
//     }
// }

// Gives the total Distance travelled by the van    ->  O(n)
int TotalDistance(vector<pair<int,int>> FinalList){
    long int Distance = 0;
    int srcNode = wareHouseSelection(FinalList);
    
    for(int i = 0 ; i<FinalList.size() ; i++){
        Distance += dist[srcNode][FinalList[i].first];
        srcNode = FinalList[i].first;
    }
    return Distance;
}

// Gives the Nearest Gas Station from the Node where the Van Runs out of fuel   ->  O(n)
int NearGasStation(int srcNode){
    int mini = 1e9;
    int id = -1;
    // 25,17,27,38
    for(int i=0;i<GasStation.size();i++){
        if(mini > dist[srcNode][GasStation[i]]){
            mini = dist[srcNode][GasStation[i]];
            id = i;
        }
    }
    return GasStation[id];
}

// Gives the Total number of times the van go for a tank refill ->  O(n^2)
int NumberOfRefill(vector<pair<int,int>> FinalList){
    int cnt = 0;
    int cap = Tankcap;
    int srcNode = wareHouseSelection(FinalList);
    vector<int> path;
    for(int i = 0 ; i<FinalList.size() ; i++){
        path = shortestPath(srcNode,FinalList[i].first);
        for(int j=0 ; j<path.size()-1 ; j++){
                if(cap - dist[path[j]][path[j+1]] <= 0){
                    cap = Tankcap;
                    cnt++;
                }
                else{
                    cap = cap - dist[path[j]][path[j+1]];
                    
                }
        }
        srcNode = FinalList[i].first;
    }
    return cnt;
}


// Prints the Final Path of the Van to deliver all the items of FinalList   ->  O(n^2)
void PathPrinting(vector<pair<int,int>> FinalList){
    long int totalDistance = 0;
    int srcNode = wareHouseSelection(FinalList);
    vector<int> path;
    int cap = Tankcap;
    int pump;
    // int cnt = 0;
    totalDistance = TotalDistance(FinalList);
    for(int i = 0 ; i<FinalList.size() ; i++){
        path = shortestPath(srcNode,FinalList[i].first);
        for(int j=0 ; j<path.size()-1 ; j++){
                if(cap - dist[path[j]][path[j+1]] <= 0){
                    pump = NearGasStation(path[j]);
                    cout<<pump<<"(*) -> ";
                    totalDistance += dist[path[j]][pump] + dist[pump][path[j+1]];
                    cap = Tankcap;
                    // cnt++;
                }
                else{
                    cap = cap - dist[path[j]][path[j+1]];
                    cout<<path[j]<<" -> ";
                }
        }
        srcNode = FinalList[i].first;
    }
    cout<<path[path.size()-1]<<endl;
    cout<<endl<<"Total Distance Covered in this Path: "<<totalDistance<<endl;
    cout<<endl<<"Number of refills required to cover the Path: "<<NumberOfRefill(FinalList);
}
 
int main() {
    //  warehouse -> 2
    //  house -> 0
    //  petrol -> 1
    
    //Warehouses..
    property[1] = 2;
    property[48] = 2;
    property[29] = 2;
    property[36] = 2;
    
    //Petrol Pumps..
    property[25] = 1;
    property[17] = 1;
    property[27] = 1;
    property[38] = 1;

    adj[0]  = {{22,73},{43,80},{44,72}};
    adj[1]  = {{2,2},{3,1},{42,8},{7,5},{44,6},{43,7},{9,4}};
    adj[2]  = {{2,2},{5,9},{10,3},{8,14},{9,13}};
    adj[3]  = {{4,11},{5,10},{1,1}};
    adj[4]  = {{5,12},{3,11},{34,55},{38,54}};
    adj[5]  = {{3,10},{4,12},{38,53},{2,9},{10,50},{6,51}};
    adj[6]  = {{37,63},{5,51},{11,49},{38,52}};
    adj[7]  = {{1,5},{9,18},{44,107}};
    adj[8]  = {{2,14},{9,15},{21,16},{17,74},{10,79}};
    adj[9]  = {{2,13},{1,4},{7,18},{23,19},{21,17},{8,15}};
    adj[10] = {{2,3},{8,79},{5,50},{11,47},{15,30},{8,79}};
    adj[11] = {{10,47},{6,49},{12,48},{28,94}};
    adj[12] = {{11,48},{28,43},{24,44},{25,97}};
    adj[13] = {{15,32},{28,31},{14,33},{32,93}};
    adj[14] = {{13,33},{29,101},{32,34}};
    adj[15] = {{13,32},{17,29},{10,30}};
    adj[16] = {{17,28},{18,27},{32,92},{33,100}};
    adj[17] = {{16,28},{18,78},{15,29},{8,74},{19,75}};
    adj[18] = {{16,27},{19,26},{17,78},{20,25}};
    adj[19] = {{18,26},{17,75},{20,24},{21,23},{22,76}};
    adj[20] = {{18, 25},{22, 22},{19, 24}};
    adj[21] = {{9, 17},{23, 20},{22, 77},{8, 16},{19, 23}};
    adj[22] = {{19, 76},{20, 22},{23, 21},{21, 77},{0, 73}};
    adj[23] = {{21, 20},{22, 21},{44, 81},{9, 19}};
    adj[24] = {{25, 45},{26, 41},{12, 44},{29, 42}};
    adj[25] = {{12, 97},{39, 46},{40, 102},{24, 45}};
    adj[26] = {{27, 40},{24, 41},{30, 98}};
    adj[27] = {{30, 39},{26, 40}};
    adj[28] = {{29, 95},{12, 43},{11, 94},{13, 31}};
    adj[29] = {{30, 96},{32, 38},{14, 101},{28, 95},{24,42}};
    adj[30] = {{29, 96},{27, 39},{31, 99},{26, 98}};
    adj[31] = {{30, 99},{33, 36},{32, 37}};
    adj[32] = {{31, 37},{33, 35},{29, 38},{14, 34},{16, 92},{13, 93}};
    adj[33] = {{31, 36},{32, 35},{16, 100}};
    adj[34] = {{38, 56},{4, 55},{45, 87}};
    adj[35] = {{36,58},{37,61},{39,59},{41,89}};
    adj[36] = {{35,58},{39,57},{40,91}};
    adj[37] = {{35,61},{39,60},{38,62},{6,63}};
    adj[38] = {{4,54},{5,53},{6,52},{34,56},{37,62},{41,88},{45,64}};
    adj[39] = {{25,46},{36,57},{35,59},{37,60}};
    adj[40] = {{36,91},{41,90},{25,102}};
    adj[41] = {{35,89},{38,88},{40,90}};
    adj[42] = {{1,8},{45,86},{48,85}};
    adj[43] = {{1,7},{44,71},{0,80},{48,82},{46,69}};
    adj[44] = {{1,6},{7,107},{23,81},{0,72},{43,71}};
    adj[45] = {{38,64},{34,87},{42,86},{48,66},{49,65}};
    adj[46] = {{43,69},{48,70},{47,68}};
    adj[47] = {{46,68},{48,83},{49,67}};
    adj[48] = {{43,82},{46,70},{47,83},{49,84},{45,66},{42,85}};
    adj[49] = {{47,67},{48,84},{45,65}};
    
    //List of Items
    floydwarshall();
    int cap[2];
    vector<pair<int,int>> List[2];
    List[0] = {{4,95},{35,4},{31,60},{5,32},{19,23},{45,72},{32,80},{19,65},{2,49}};
    cap[0] = 269;
    List[1] = {{39,59},{25,102},{41,90},{38,58},{10,50},{45,30},{28,25},{20,95},{0,107},{46,12}};
    cap[1] = 400;
    system("CLS");
    int cargocap;

    cout<<endl<<"Addresses of All WareHouses present in the map : ";
    for(auto i : wareHouses){
        cout<<" "<<i;
    }

    cout<<endl<<"Addresses of All Gas Stations present in the map : ";
    for(auto j : GasStation){
        cout<<" "<<j;
    }
    cout<<endl;
    int i;
    bool Entered = false;
    cout<<endl<<"-----------------------------------------------------------";
    cout<<endl<<"\tPress 1 to See Existing Lists ";
    cout<<endl<<"\tPress 2 to Enter a new List of items ";
    cout<<endl<<"-----------------------------------------------------------";
    cout<<endl<<"Enter your choice : ";
    cin>>i;
    int a,b,n,k=0;
    char ans='y';
    vector<pair<int,int>> newList;
    switch(i){
        case 1:
            do{
                cout<<endl<<"List"<<k+1<<" with cargo capacity "<<cap[k]<<" : ";
                cout<<endl<<"House No.\tWeight"<<endl;
                for(auto i : List[k]){
                    cout<<" "<<i.first<<"\t\t"<<i.second<<endl;
                }
                k++;
                cout<<endl<<"Press Y to see another list: ";
                cin>>ans;
                if(k>=2 && (ans=='y'||ans=='Y')){
                    cout<<endl<<"Oops!No more list present.."<<endl;
                    break;
                }
            }while((ans == 'Y' || ans == 'y') && k<2);
            break;
        case 2:
            Entered = true;
            cout<<endl<<"Enter the capacity of the van: ";
            cin>>cargocap;
            cout<<endl<<"Enter the number of items to Deliver: ";
            cin>>n;
            cout<<endl<<"Enter the adderess and weights of all the items ";
            for(int i=0;i<n;i++){
                cin>>a>>b;
                newList.push_back({a,b});
            }
            break;
        default:
            cout<<endl<<"Invalid Input!";
            exit(0);
    }
    int ch,wareHouse;
    cout<<endl<<"-----------------------------------------------";
    cout<<endl<<"\tPress 1 to select List1";
    cout<<endl<<"\tPress 2 to select List2";
    cout<<endl<<"\tPress 3 to select your Entered List";
    cout<<endl<<"-----------------------------------------------";
    cout<<endl<<"Enter your choice: ";
    cin>>ch;
    system("cls");
    vector<pair<int,int>> FinalList;
    switch(ch){
        case 1 : 
            FinalList = SystematicPackaging(List[0],cap[0]);
            wareHouse = wareHouseSelection(List[0]);
            cout<<endl<<"Final list of items is: ";
            cout<<endl<<"-----------------------------------------------";
            cout<<endl<<"House No.\tWeight"<<endl;
            for(auto it : FinalList){
                cout<<" "<<it.first<<"\t\t"<<it.second<<endl;
            }
            cout<<endl<<"-----------------------------------------------";
            cout<<endl<<"Van will Depart from Warehouse Node "<<wareHouse<<endl;
            cout<<endl<<"Van will Follow the given path: "<<endl;
            PathPrinting(FinalList);
            break;
        case 2 : 
            Entered = true;
            FinalList = SystematicPackaging(List[1],cap[1]);
            wareHouse = wareHouseSelection(List[1]);
            cout<<endl<<"Final list of items is: ";
            cout<<endl<<"House No.\tWeight"<<endl;
            for(auto it : FinalList){
                cout<<" "<<it.first<<"\t\t"<<it.second<<endl;
            }
            cout<<endl<<"Van will Start from Warehouse Node "<<wareHouse;
            cout<<endl<<endl<<"Van will Follow the given path: "<<endl;
            PathPrinting(FinalList);
            break;
        case 3 : 
            if(!Entered){
                cout<<endl<<"No List entered!"<<endl;
                break;
            }
            
            FinalList = SystematicPackaging(newList,cargocap);
            wareHouse = wareHouseSelection(newList);
            cout<<endl<<"Final list of items is: ";
            cout<<endl<<"House No.\tWeight"<<endl;
            for(auto it : FinalList){
                cout<<"  "<<it.first<<"\t\t"<<it.second<<endl;
            }
            cout<<endl<<"Van will Start from Warehouse Node "<<wareHouse;
            cout<<endl<<"Van will Follow the given path: "<<endl;
            PathPrinting(FinalList);
            break;
        default:
            cout<<endl<<"Invalid Input!";
            exit(0);
    }
    return 0;
}