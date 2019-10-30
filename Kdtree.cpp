#include<iostream>
#include<vector>
#include<algorithm>
#include<math.h>
#include<float.h>

using namespace std;

int curr=0;

struct tree
{
	int *data;
	struct tree *left,*right;
	int dim;
    int cdim;
};

struct tree *create(int dim)
{
	struct tree *temp;
	temp=(struct tree*)malloc(sizeof(struct tree));
	temp->data=(int*)malloc(dim*sizeof(int));
	temp->left=NULL;
	temp->right=NULL;
	temp->dim=dim;
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

        //cout<<"Middle is "<<da[mid]->data[0]<<" "<<da[mid]->data[1]<<"\n";

        result.push_back(da[mid]);
        getnodesorted(da,start,mid-1,(cd+1)%dim,dim);
        getnodesorted(da,mid+1,end,(cd+1)%dim,dim);
    }
}

struct tree *insert(struct tree *root,struct tree *temp,int d)
{
	if(root==NULL)
	{
		root=create(temp->dim);
		for(int i=0;i<temp->dim;i++)
			root->data[i]=temp->data[i];
        root->cdim=d;
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
		cout<<root->cdim<<"\n";

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
			best -> data[i] = root -> data[i];
		}
		best -> left = root -> left;
		best -> right = root -> right;
		best -> dim = root -> dim;
		best -> cdim = root -> cdim;
	}
	if(goal -> data[cd] < root -> data[cd])
	{
		goodside = root-> left;
		badside = root -> right;
	}
	else
	{
		goodside = root-> right;
		badside = root -> left;
	}
	best = nearest (goodside,goal,best,(cd+1)%(root -> dim));
	if(abs(root -> data[cd] - goal -> data[cd]) < min_dist)
		best = nearest(badside,goal,best,(cd+1)%(root -> dim));

	return best;

}

void k_nearest_neighbours(int k, tree* root,int cd,int dim)
{
	tree *best = create(dim);
	
	struct tree * goal = create(dim);

	 for(int i=0;i<dim;i++)
	 	cin>>goal->data[i];
	
	vector <tree*> store;
	for(int i = 0 ; i < k ; i++)
	{
		tree *temp;
		tree *temp2=create(dim);
		for(int i = 0 ; i < root->dim ; i++)
		{
			best -> data[i] = root -> data[i];
		}
		best -> left = root -> left;
		best -> right = root -> right;
		best -> dim = root -> dim;
		best -> cdim = root -> cdim;
		
		temp = nearest(root,goal,best,0);
		

		for(int i = 0 ; i < temp->dim ; i++)
		{
			temp2->data[i] = temp->data[i];
		}
		temp2->left = temp->left;
		temp2->right = temp->right;
		temp2->dim = temp->dim;
		temp2->cdim = temp->cdim;

		// for(int i=0;i<dim;i++){
		// 	cout<<tempo->data[i]<< " ";
		// }
		store.push_back(temp2);
		
		root = deletenode(root,best,cd,dim);
		
	}
	
	for(auto x : store)
    {
        root=insert(root,x,0);
    }
    
    for(auto l : store)
    {
		for(int i=0;i<dim;i++){
			cout<<l->data[i]<< " ";
		}
		cout<<endl;
	}	
}

int main()
{
    int size=100;
    struct tree nodes[size];
    int dim=3;
    
    struct tree *root=NULL;
    vector<struct tree*> da;
    
	for(int i=0;i<5;i++)
	{
        struct tree *temp=create(dim);
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

	struct tree * temp=findmin(root,0,0,dim);

    cout<<"Minimum in x dimension is ";

	for(int i=0;i<dim;i++)
		cout<<temp->data[i]<<" ";
	cout<<"\n";


	temp=findmin(root,1,0,dim);

    cout<<"Minimum in y dimension is ";

	for(int i=0;i<dim;i++)
		cout<<temp->data[i]<<" ";
	cout<<"\n";

	struct tree * point=create(dim);
	for(int i=0;i<dim;i++)
		cin>>point->data[i];

	root=deletenode(root,point,0,dim);
	
	traversal(root);

	struct tree * best=create(dim);

	for(int i = 0 ; i < root->dim ; i++)
		{
			best -> data[i] = root -> data[i];
		}
		best -> left = root -> left;
		best -> right = root -> right;
		best -> dim = root -> dim;
		best -> cdim = root -> cdim;


	struct tree * goal = create(dim);

	 for(int i=0;i<dim;i++)
	 	cin>>goal->data[i];

	min_dist = dist(best,goal);
	best = nearest(root,goal,best,0);
	cout<<"nearest node is : "<<endl;		
	for(int i=0;i<dim;i++)
		cout<<best->data[i]<< " ";


 	// cout<<"nearest k_nearest_neighbours are : "<<endl;
	// k_nearest_neighbours(3,root,0,dim);

	// traversal(root);
	return 0;
}