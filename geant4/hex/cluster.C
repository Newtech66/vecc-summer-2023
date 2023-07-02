//Assume a list of cell and row data is passed
//return a list of numbers where i-th number is label of cluster it belongs to (-1 for noise)
//this approach is insensitive to Edep data

const int dx[]={1,0,-1,1,0,-1};
const int dy[]={-1,-2,-1,1,2,1};
const int hexcols=20;
const int hexrows=10;
const int minPts=2;

bool ok(int r,int c){
    return r>=0 && r<hexrows && c>=0 && c<hexcols;
}

vector<Int_t> rangeQuery(int r,int c,map<pair<Int_t,Int_t>,Int_t>& idx){
    vector<Int_t> N;
    for(int i=0;i<6;i++){
        if(ok(r+dx[i],c+dy[i]) && idx.find({r+dx[i],c+dy[i]})!=idx.end()){
            N.push_back(idx[{r+dx[i],c+dy[i]}]);
        }
    }
    N.push_back(idx[{r,c}]);
    return N;
}

Int_t cluster(vector<pair<Int_t,Int_t>>& data){
    //how to get the neighbourhood of a hexagonal array
    map<pair<Int_t,Int_t>,Int_t> idx;
    for(int i=0;i<(int)data.size();i++){
        idx[data[i]]=i;
    }
    vector<Int_t> clust(data.size(),-2);
    Int_t cl=0;
    for(int pt=0;pt<(int)data.size();pt++){
        if(clust[pt]!=-2)   continue;
        Int_t r,c;
        tie(r,c)=data[pt];
        vector<Int_t> N=rangeQuery(r,c,idx);
        if(N.size()<minPts){
            clust[pt]=-1;
            continue;
        }
        cl++;
        clust[pt]=cl;
        vector<Int_t> seedSet{N.begin(),N.end()};
        for(auto q:seedSet){
            if(q==pt)   continue;
            if(clust[q]==-1)    clust[q]=cl;
            if(clust[q]!=-2)    continue;
            clust[q]=cl;
            tie(r,c)=data[q];
            N=rangeQuery(r,c,idx);
            if(N.size()>=minPts){
                for(auto p:N)   seedSet.push_back(p);
            }
        }
    }
    return cl;
    // return clust;
}