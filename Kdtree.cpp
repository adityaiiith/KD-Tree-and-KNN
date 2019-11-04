#include<iostream>
#include<vector>
#include<algorithm>
#include<math.h>
#include<float.h>
#include<unordered_map>
#include<climits>

using namespace std;

int curr=0;
unordered_map<int,bool> map;
int tcount=0;

struct tree
{
	int *data;
	struct tree *left,*right;
	int dim;
    int cdim;
    int id;
};

struct tree *create(int dim,int id)
{
	struct tree *temp;
	temp=(struct tree*)malloc(sizeof(struct tree));
	temp->data=(int*)malloc(dim*sizeof(int));
	temp->left=NULL;
	temp->right=NULL;
	temp->dim=dim;
    temp->id=id;
	return temp;
}

bool mycmp(struct tree *t1,struct tree *t2)
{
    if(t1->data[curr] < t2->data[curr])
        return true;
    return false;
}

vector<struct tree*> result;
void getnodesorted(vector<struct tree*> da,int start,int end,int cd,int dim)
{
    
    if(end>=start)
    {
        curr=cd;
        if(end!=start)
            sort(da.begin()+start,da.begin()+end+1 ,mycmp);
        int mid=start+((end-start)/2);

        result.push_back(da[mid]);
        getnodesorted(da,start,mid-1,(cd+1)%dim,dim);
        getnodesorted(da,mid+1,end,(cd+1)%dim,dim);
    }
}

struct tree *insert(struct tree *root,struct tree *temp,int d)
{
	if(root==NULL)
	{
		root=create(temp->dim,tcount);
		for(int i=0;i<temp->dim;i++)
			root->data[i]=temp->data[i];
        root->cdim=d;
        map[tcount++]=false;
		return root;
	}
	else
	{
		if(temp->data[d]<=root->data[d])
		{
			root->left=insert(root->left,temp,(d+1)%temp->dim);
			return root;
		}
		else if(temp->data[d]>root->data[d])
		{
			root->right=insert(root->right,temp,(d+1)%temp->dim);
			return root;
		}
	}
}

void traversal(struct tree *root)
{
	if(root!=NULL)
	{
		traversal(root->left);

		for(int i=0;i<root->dim;i++)
			cout<<root->data[i]<<" ";
        //cout<<" id is "<<root->id<<" "<<map[root->id]<<" ";
		cout<<"dimension is "<<root->cdim<<"\n";

		traversal(root->right);
	}
}
struct tree * mini(struct tree *left,struct tree *right,struct tree *root,int sdim)
{
	if(left==NULL && right==NULL)
		return root;

	if(left==NULL)
	{
		if(right->data[sdim]<root->data[sdim])
			return right;
		else
			return root;
	}

	if(right==NULL)
	{
		if(left->data[sdim]<root->data[sdim])
			return left;
		else
			return root;
	}

	if(left->data[sdim]<right->data[sdim]&& left->data[sdim]< root->data[sdim])
		return left;
	
	else if(right->data[sdim]<left->data[sdim]&& right->data[sdim]< root->data[sdim])
		return right;
	else
		return root;
	//cout<<"comparison end\n";
}

bool comp(struct tree * p1,struct tree *p2)
{
	int dim=p1->dim;

	for(int i=0;i<dim;i++)
		if(p1->data[i]!=p2->data[i])
			return false;

	return true;
}

struct tree * findmin(struct tree *root,int sdim,int cd,int dim)
{ 
    if(root==NULL)
        return NULL;
    if(cd==sdim)
    {
        if(root->left==NULL)
            return root;
        else
            return findmin(root->left,sdim,(cd+1)%dim,dim);
    }
    else
        return mini(findmin(root->left,sdim,(cd+1)%dim,dim),findmin(root->right,sdim,(cd+1)%dim,dim),root,sdim);
}

struct tree* deletenode(struct tree *root,struct tree * point, int cd, int dim)
{
	if(root==NULL)
		return NULL;

	struct tree *temp,*minptr;
	if(comp(root,point))
	{
		if(root->left==NULL && root->right==NULL)
		{
			temp=root;
			free(temp);
			return NULL;
		}
		else if(root->right!=NULL)
		{
			minptr=findmin(root->right,cd,(cd+1)%dim,dim);
			for(int i=0;i<dim;i++)
				root->data[i]=minptr->data[i];
			root->right = deletenode(root->right,minptr,(cd+1)%dim,dim);
		}
		else  if(root->left!=NULL)
		{
			minptr=findmin(root->left,cd,(cd+1)%dim,dim);
			for(int i=0;i<dim;i++)
				root->data[i]=minptr->data[i];
			root->right = deletenode(root->left,minptr,(cd+1)%dim,dim);
			root->left=NULL;
		}
		return root;
	}
	else
	{
		if(root->data[cd]>point->data[cd])
			 root->left=deletenode(root->left,point,(cd+1)%dim,dim);

		else if(root->data[cd]<point->data[cd])
			root->right=deletenode(root->right,point,(cd+1)%dim,dim);
	}
	return root;
}

double dist(struct tree* a, struct tree* b)
{
	double d = 0;
	for(int i = 0 ; i < a -> dim ; i++)
	{
		double t = a->data[i] - b->data[i];
		d += t*t;
	}
	return sqrt(d);
}

double min_dist = DBL_MAX;

struct tree* nearest(struct tree* root, struct tree* goal ,struct tree* best, int cd)
{
	struct tree* goodside, *badside;
	if(!root)
	{
		return best;
	}
	if(dist(root,goal) < dist(best,goal))
	{
		min_dist = dist(root,goal);
		for(int i = 0 ; i < root->dim ; i++)
		{
			best -> data[i]=root->data[i];
		}
		best->left=root->left;
		best->right=root->right;
		best->dim=root->dim;
		best->cdim=root->cdim;
	}
	if(goal -> data[cd] < root -> data[cd])
	{
		goodside=root->left;
		badside=root->right;
	}
	else
	{
		goodside=root->right;
		badside=root->left;
	}
	best=nearest(goodside,goal,best,(cd+1)%(root -> dim));
	if(abs(root->data[cd]-goal->data[cd])<min_dist)
		best=nearest(badside,goal,best,(cd+1)%(root->dim));

	return best;

}

struct tree* nearestk(struct tree* root, struct tree* goal ,struct tree* best, int cd,
			vector <pair<double,struct tree *>> &mheap)
{
	struct tree* goodside, *badside;
	if(!root)
	{
		return best;
	}
	if(dist(root,goal)<dist(best,goal))
	{
		min_dist = dist(root,goal);
		for(int i = 0 ; i < root->dim ; i++)
		{
			best->data[i]=root->data[i];
		}
		best->left=root->left;
		best->right=root->right;
		best->dim=root->dim;
		best->cdim=root->cdim;
	}

	struct tree *temp=create(root->dim,0);
	double dis=dist(root,goal);

	for(int i = 0 ; i < root->dim ; i++)
	{
		temp->data[i]=root->data[i];
		
	}
	
	temp->left=root->left;
	temp->right=root->right;
	temp->dim=root->dim;
	temp->cdim=root->cdim;
    temp->id=root->id;

	mheap.push_back(make_pair(dis,temp));
    map[temp->id]=true;

	if(goal->data[cd]<root ->data[cd])
	{
		goodside=root->left;
		badside=root->right;
	}
	else
	{
		goodside=root->right;
		badside=root->left;
	}
	best=nearestk(goodside,goal,best,(cd+1)%(root -> dim),mheap);
	if(abs(root->data[cd]-goal->data[cd])<min_dist)
		best=nearestk(badside,goal,best,(cd+1)%(root->dim),mheap);

	return best;

}


void minheapify( vector <pair<double,struct tree *>> &mheap,int i)
{
    pair<double,struct tree *> temp;
    int l,r,small=i;

    int hsize=mheap.size();

    l=(2*i)+1;
    r=(2*i)+2;

    if(l<hsize && mheap[l].first< mheap[i].first)
        small=l;
    else
        small=i;

    if(r<hsize && mheap[r].first < mheap[small].first)
        small=r;

    if(small!=i)
    {
        temp=mheap[small];
        mheap[small]=mheap[i];       
        mheap[i]=temp;

        minheapify(mheap,small);

    }

}

void buildminheap(vector <pair<double,struct tree *>> &mheap)
{
    int hsize=mheap.size();

    int i=(hsize-1);

    while(i>=0)
    {
        minheapify(mheap,i);
        i--;
    }
}

void adjust(vector <pair<double,struct tree *>> &mheap,int pos)
{
    int child=pos,parent;
    pair<double,struct tree *> temp;
   
    parent=(child-1)/2;
    while(parent>=0 && mheap[parent].first>mheap[child].first)
    {
        temp=mheap[child];
        mheap[child]=mheap[parent];
        mheap[parent]=temp;

        child=parent;
        parent=(child-1)/2;
    }
}

pair<double,struct tree *> removemin(vector <pair<double,struct tree *>> &mheap)
{
    pair<double,struct tree *> temp=mheap[0];
    
    int size=mheap.size()-1;

    mheap[0]=mheap[size];
    mheap.pop_back();

    minheapify(mheap,0);
    return temp;
}

void populateheap(struct tree *root,vector<pair<double,struct tree *> > &mheap,int dim,struct tree *goal)
{
	if(root==NULL)
		return;

	struct tree *tp=create(dim,root->id);

	for(int i=0;i<dim;i++)
		tp->data[i]=root->data[i];
	
	tp->left=root->left;
	tp->right=root->right;
	tp->cdim=root->cdim;
	tp->dim=root->dim;
	map[root->id]=true;
	mheap.push_back(make_pair(dist(tp,goal),tp));
	adjust(mheap,mheap.size()-1);

	if(root->left!=NULL&&map[root->left->id]==false)
		populateheap(root->left,mheap,dim,goal);
	if(root->right!=NULL&&map[root->right->id]==false)
		populateheap(root->right,mheap,dim,goal);
}

void knn(int k,struct tree *root,int dim)
{
	struct tree *best = create(dim,0);
    
	vector<pair<double,struct tree *> > mheap;

	struct tree *goal = create(dim,0);

	cout<<"Enter the search point\n";
	for(int i=0;i<dim;i++)
	 	cin>>goal->data[i];

	for(int i=0;i<root->dim;i++)
	{
		best->data[i]=root->data[i];
	}
	best->left=root->left;
	best->right=root->right;
	best->dim=root->dim;
	best->cdim=root->cdim;

	min_dist=dist(best,goal);
	//pq.push(make_pair(min_dist,temp));
	
	nearestk(root,goal,best,0,mheap);
    buildminheap(mheap);

	cout<<"Knn is \n";
	
	while(k--&&mheap.size()>0)
	{
		struct tree *temp;
        pair<double,struct tree *> pt=removemin(mheap);
		cout<<pt.first<<" ";
        temp=pt.second;

		for(int i=0;i<temp->dim;i++)
			cout<<temp->data[i]<<" ";
		cout<<"\n";

		//arr.push_back(temp);
        if(temp->left!=NULL&&map[temp->left->id]==false)
        {
            populateheap(temp->left,mheap,dim,goal);
        }
        if(temp->right!=NULL&&map[temp->right->id]==false)
        {
            populateheap(temp->right,mheap,dim,goal);
        }
	}
}

int main()
{
    int size=200000;
    struct tree nodes[size];
    int dim,no;
	cout<<"Enter the dimension\n";
	cin>>dim;
	
	cout<<"Enter no of nodes\n";
	cin>>no;

    struct tree *root=NULL;
    vector<struct tree*> da;
    
	for(int i=0;i<no;i++)
	{
        struct tree *temp=create(dim,0);
		cout<<"Enter node\n";

		for(int i=0;i<dim;i++)
		{
			cin>>temp->data[i];
		}

		da.push_back(temp);
	}
    getnodesorted(da,0,da.size()-1,0,dim);
    for(auto x:result)
    {
        root=insert(root,x,0);
    }

    traversal(root);

	int sdim;
	cout<<"Enter the dim for minimum point\n";
	cin>>sdim;
	struct tree * temp=findmin(root,sdim,0,dim);

    cout<<"Minimum in "<<sdim<<" dimension is ";

	for(int i=0;i<dim;i++)
		cout<<temp->data[i]<<" ";
	cout<<"\n";

	struct tree * best=create(dim,0);

	for(int i = 0 ; i < root->dim ; i++)
	{
		best->data[i]=root->data[i];
	}
	best->left=root->left;
	best->right=root->right;
	best->dim=root->dim;
	best->cdim=root->cdim;

	struct tree * goal = create(dim,0);

	cout<<"Enter the search point\n";
	for(int i=0;i<dim;i++)
	 	cin>>goal->data[i];

	min_dist = dist(best,goal);
	best = nearest(root,goal,best,0);
	cout<<"nearest node is : "<<endl;		
	for(int i=0;i<dim;i++)
		cout<<best->data[i]<< " ";

	int k;
	cout<<"Enter the value of k\n";
	cin>>k;
	knn(k,root,dim);

	cout<<"Enter the point to delete\n";
	struct tree * point=create(dim,0);
	for(int i=0;i<dim;i++)
		cin>>point->data[i];

	cout<<"Before deleting\n";
	traversal(root);
	root=deletenode(root,point,0,dim);
	cout<<"After deleting\n";
	traversal(root);

	return 0;
}