struct node{
    int v,i; // value and index
    node* left,*right;
    node(int x, int index){v=x;i=index;}
    static void insert(node* x, node*& t){
        if(!t) t=x;
        else if (t->v==x->v) t->i=x->i;
        else split(t,x->left,x->right,x->v), t=x;
    }
    static void split(node*p, node*& l, node*& r, int val){
        if(!p) l=r=p;
        else if(p->v>val) split(p->left,l,p->left,val), r=p;
        else split(p->right,p->right,r,val), l=p;
    }
};

// read input in vector a;
node* treapL = nullptr , *treapR = nullptr;
for (int i=0, j=n-1; i<n; i++, j--){
    node::insert(new node(a[i], i+1), treapL);
    node::insert(new node(a[j], j+1), treapR);
    l[i]=treapL->right ? treapL->right->i : 0; // 0 if no right child
    r[j]=treapR->right ? treapR->right->i : 0;
}
for(int i=0;i<n;i++) ans=max(ans, (int64_t)l[i]*r[i]);
cout<<ans<<'\n';
