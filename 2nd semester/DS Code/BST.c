#include<stdio.h>
#include<stdlib.h>
struct nodd
{
    int data;
    struct nodd *left;
    struct nodd *right;
};

typedef struct nodd N;

N*create(int data)

{
    N*newnode=(N*)malloc(sizeof(N));
    newnode->data=data;
    newnode->left=NULL;
    newnode->right=NULL;
    return newnode;
}
void PrintInOrder(N*root)
{
    if(root==NULL)
    return;


    PrintInOrder(root->left);
    printf("%d\n",root->data);
    PrintInOrder(root->right);
}

N*findmin(N*root)
{
    while(root->left!=NULL)
    {
        root=root->left;
    }

    return root;
}


N*deleteBST(N*root, int value)
{
    if(root==NULL)
        return root;
    if(value>root->data)
        root->right=deleteBST(root->right, value);
    else if(value<root->data)
        root->left=deleteBST(root->left, value);
    else
    {
        if(root->left==NULL)
        {
            N*temp=root->right;
            free(root);
            return temp;
        }

        else if(root->right==NULL)
        {
            N*temp=root->left;
            free(root);
            return temp;
        }

        else
        {
            N*temp=findmin(root->right);
            root->data= temp->data;
            root->right=deleteBST(root->right, temp->data);

        }



    }

    return root;


}



N*insertBST(N*root, int key)
{
    if(root==NULL)
    return create(key);

    if(key<root->data)
    root->left=insertBST(root->left,key);
    else if(key>root->data)
    root->right=insertBST(root->right,key);
    return root;



}

int main()
{
    N*root=NULL;
    root=insertBST(root,10);
    insertBST(root,20);
    insertBST(root,2);
    insertBST(root,3);
    insertBST(root,2);
    insertBST(root,3);
    deleteBST(root,660);

    PrintInOrder(root);



    return 0;
}
