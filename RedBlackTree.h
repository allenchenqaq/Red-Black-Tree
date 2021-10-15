#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

template <typename T>
class NodeT{
   //… includes constructor definitions …
  public:
   T data;
   NodeT* left;
   NodeT* right;
   NodeT* parent;
   bool isBlack;
   NodeT(T value) : data(value), left(nullptr), right(nullptr), parent(nullptr), isBlack(false) {};
};


// template declaration
template <typename T>
class RedBlackTree{
  public:
   //template <class T> 
   friend NodeT<T>* JWMEgetRoot(const RedBlackTree<T> & rbt);

   RedBlackTree();

   RedBlackTree(const RedBlackTree &rbt);

   RedBlackTree & operator=(const RedBlackTree &rbt);

   ~RedBlackTree();

   bool insert(T value);

   bool remove(T value);

   bool search(T value) const;

   vector<T> search(T first, T second);

   T closestLess(T value);

   T closestGreater(T value);

   vector<T> values();

   int size() const;
  private:
   NodeT<T>* root;

   int length;

   void clear(NodeT<T>* nd);

   void copy(NodeT<T>* nd,NodeT<T> *rbt);

   void insertHelper(NodeT<T>* value);

   void searchHelper(vector<T>& vals,NodeT<T>* root,T first,T second);

   void left_rotate(NodeT<T>* x);

   void right_rotate(NodeT<T>* y);

   NodeT<T>* removeHelper(T x) const;

   void rbFix(NodeT<T>* x);

   NodeT<T>* successor(NodeT<T> * x) const;

   void valuehelper(vector<T>& vectorT, NodeT<T>* root);
};


//creates an empty tree whose root is a null pointer
template <typename T>
RedBlackTree<T>::RedBlackTree(){
  root=nullptr;
  length=0;
}


//a constructor that creates a deep copy of its RedBlackTree reference parameter
template <typename T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree<T> &rbt){
  if(rbt.root==nullptr) root=nullptr;
  root=new NodeT<T>(rbt.root->data);
  length=rbt.length;
	copy(root,rbt.root);
}


//overloads the assignment operator for RedBlackTree objects
template <typename T>
RedBlackTree<T> &RedBlackTree<T>::operator=(const RedBlackTree<T> &rbt){
  if(root==rbt.root) return *this;
  else{
    clear(rbt.root);
    copy(root,rbt.root);
  }
	return *this;
}


//helper method for copy
template <typename T>
void RedBlackTree<T>::copy(NodeT<T>* nd,NodeT<T>* rbt){
  if(rbt!=nullptr){ 
    nd->data=rbt->data;
    nd->isBlack=rbt->isBlack;
    if(rbt->left != nullptr){
      nd->left=new NodeT<T>(rbt->left->data);
      nd->left->isBlack=rbt->left->isBlack;
      nd->left->parent=nd;
      copy(nd->left,rbt->left);
    }
    else nd->left = nullptr;
    if(rbt->right!=nullptr){
      nd->right=new NodeT<T>(rbt->right->data);
      nd->right->isBlack=rbt->right->isBlack;
      nd->right->parent=nd;
      copy(nd->right,rbt->right);
    }
    else nd->right=nullptr;
  }
}


//deletes dynamic memory allocated by the tree
template <typename T>
RedBlackTree<T>::~RedBlackTree(){
	clear(root);
}


//helper method for destructor
template <typename T>
void RedBlackTree<T>::clear(NodeT<T>* nd){
	if (nd != nullptr) {
		clear(nd->left);
		clear(nd->right);
		delete nd;
	}
  root=nullptr;
  length=0;
}


// if the tree does not contain the method's single template parameter, inserts the parameter and returns true; otherwise does not insert the parameter and returns false
template <typename T>
bool RedBlackTree<T>::insert(T n){
  if(!search(n)){
    NodeT<T>* x=new NodeT<T>(n);
    insertHelper(x);
    //color is red
    while(x!=root &&x->parent->isBlack==false){
      if(x->parent==x->parent->parent->left){
        //x's "uncle"
        NodeT<T>* y=x->parent->parent->right;
        if(y!=NULL&&y->isBlack==false){
          x->parent->isBlack=true;
          y->isBlack=true;
          x->parent->parent->isBlack=false;
          x=x->parent->parent;
        }
        else{ //y->isBlack==true
          if(x==x->parent->right){
            x=x->parent;
            left_rotate(x);
          }
          x->parent->isBlack=true;
          x->parent->parent->isBlack=false;
          if(x->parent->parent!=NULL)
            right_rotate(x->parent->parent);
        }
      }
      else{//symmetric to if
        NodeT<T>* y=x->parent->parent->left;
        if(y!=NULL&&y->isBlack==false){
          x->parent->isBlack=true;
          y->isBlack=true;
          x->parent->parent->isBlack=false;
          x=x->parent->parent;
        }
        else{
          if(x==x->parent->left){
            x=x->parent;
            right_rotate(x);
          }
          x->parent->isBlack=true;
          x->parent->parent->isBlack=false;
          if(x->parent->parent!=NULL)
            left_rotate(x->parent->parent);
        } 
      } 
    }
    //end while
    root->isBlack=true;
  }
  return true;
}


//helper method for insert
//refering to ex8
template <typename T>
void RedBlackTree<T>::insertHelper(NodeT<T>* value){
	NodeT<T>* temp1 = root;
	NodeT<T>* temp2 = root;
	if (root==nullptr) {
		root = value;
	}
	else { //non-empty
		   // Find parent of new node
		while (temp1 != nullptr) {
			temp2 = temp1;
			if (value->data < temp1->data) {
				temp1 = temp1->left;
			}
			else {
				temp1 = temp1->right;
			}
		}
    value->parent=temp2;
		// Insert new node
		if (value->data < temp2->data) {
			temp2->left = value;
		}
		else {
			temp2->right = value;
		}
	}
  length++;
}

//left rotation
template <typename T>
void RedBlackTree<T>::left_rotate(NodeT<T>* x){//x is the node to be rotated
  NodeT<T>* y=nullptr;
  y=x->right;
  x->right=y->left;
  //set nodes' parent references
  //y's left child
  if(y->left!=nullptr) 
    y->left->parent=x;
  //y
  y->parent=x->parent;
  //set child reference of x's parent
  if(x->parent==nullptr) //x was root
    root=y;
  else if(x==x->parent->left) //left child
    x->parent->left=y;
  else 
    x->parent->right=y;
  //make x y's left child
  y->left=x;
  x->parent=y;
}

//right rotation
template <typename T>
void RedBlackTree<T>::right_rotate(NodeT<T>* y){
  NodeT<T>* x=nullptr;
  x=y->left;
  y->left=x->right;
  if(x->right!=nullptr) 
  x->right->parent=y; 
  x->parent=y->parent;
  if(y->parent==nullptr)
    root=x;
  else if(y==y->parent->left)
    y->parent->left=x;
  else 
    y->parent->right=x;
  x->right=y;
  y->parent=x;
}


//searches the tree for the method's single template parameter and returns true if it is found and false otherwise
template <typename T>
bool RedBlackTree<T>::search(T value) const{
  //return searchHelper(d,root);
  NodeT<T>* newNode=root;
	while(newNode!=nullptr){
		if(newNode->data==value)
			return true;
		else if(newNode->data>value)
			newNode=newNode->left;
		else
			newNode=newNode->right;
	}
  return false;
}

//returns a vector that contains all of the values between the method's first and second template parameters, including both parameter values if they are in the tree
template<typename T>
vector<T> RedBlackTree<T>::search(T first,T second){
	vector<T> result;
	if(search(first) && search(second))
		searchHelper(result, 
    root,first,second);
	return result;
}

//helper method for search
template<typename T>
void RedBlackTree<T>::searchHelper(vector<T>& tree,NodeT<T>* root,T first,T second){
	NodeT<T>* newNode=root;
	T begin,end;
  //the order of the parameters is not important
	if(second>first){
		begin=first;
		end=second;
	}
	else{
		begin=second;
		end=first;
	}
	if(newNode==nullptr) return;
	else if(newNode->data>end)
		searchHelper(tree, 
    newNode->left,first,second);
	else if(newNode->data<begin)
		searchHelper(tree, newNode->right,first,second);
	else{	
		searchHelper(tree,newNode->left,first,second);
		tree.push_back(newNode->data);
		searchHelper(tree,newNode->right,first,second);
	}
}

//removes the method's template parameter from the tree and returns true; if the tree does not contain the parameter returns false
template <typename T>
bool RedBlackTree<T>::remove(T value){
  if(!search(value))
    return false;
  NodeT<T> *z=removeHelper(value);
  NodeT<T>* x=NULL;
  NodeT<T>* y=NULL;
  if(root==z&&(root->left==NULL&&root->right==NULL)){
    delete root;
    root=NULL;
    length=0;
    return true;
  }
  length--;
  if(z->left==NULL||z->right==NULL)
    y=z;//node to be removed
  else
    y=successor(z);
  if((z->left==NULL&&z->right!=NULL)||(z->right==NULL&&z->left!=NULL)){
    if(y->left!=NULL)
      x=y->left;
    else
      x=y->right;
      x->parent=y->parent;//detach x from y; if not null
    if(y->parent==NULL) //y is the root
      root=x;
    else
      if(y==y->parent->left)
        y->parent->left=x;
      else
        y->parent->right=x;
  } 
  if(z->left==NULL&&z->right==NULL){
    if(y->parent==NULL){
      root=NULL;
      x=NULL;
    }
    if(y==y->parent->left){
      x=NULL;
      y->parent->left=NULL;
    }
    else{
      x=NULL;
      y->parent->right=NULL;
    }
  }
  if(y!=z){
    z->data=y->data;
    if(y->right==NULL){
      if(y==y->parent->left)
        y->parent->left=NULL;
      else
        y->parent->right=NULL;
        x=NULL;
    }
    else{
      x=y->right;
      if(y==y->parent->left)
        y->parent->left=y->right;
      else
        y->parent->right=y->right;
    }
  }
  if(y->isBlack&&x!=NULL)
    rbFix(x); 
  return true;
}

//helper method for remove
template <typename T>
NodeT<T>* RedBlackTree<T>::removeHelper(T x) const{
  NodeT<T>* temp=root;
  if(root->data==x)
    return temp;
  else{
    while(temp!=NULL){
      if(x<temp->data)
        temp=temp->left;
      else if(x>temp->data)
        temp=temp->right;
      else
        return temp;
    }
    return temp;
  }
}


//successor for remove
template <typename T>
NodeT<T>* RedBlackTree<T>::successor(NodeT<T>* x) const{
		if(x->right != NULL){
			while(x!=NULL){
        x->right=x;
      }
      return x->right;
		}
		NodeT<T>* y=x->parent;
		while (y!=NULL && x==y->right){
			x=y;
			y=y->parent;
		}
		return y;
}


//tree fix algorithm
template <typename T>
void RedBlackTree<T>::rbFix(NodeT<T>* x){
  NodeT<T>* y=nullptr;
  while(x!=root&&x->isBlack==true){
    //x is left child
    if(x==x->parent->left){
      //x's sibling
      y=x->parent->right;
      if(y!=nullptr&&y->isBlack==false){
        //makes x's sibling black or pushes problem up tree
        y->isBlack=true;
        y->parent->isBlack=false;
        left_rotate(x->parent);
        y=x->parent->right;
      }
      if((y->left!=nullptr&&y->right!=nullptr)&&(y->left->isBlack==true&&y->right->isBlack==true)){
        y->isBlack=false;
        x=x->parent;
      }
      else{
        if((y->right!=nullptr)&&y->right->isBlack==true){
          if(y->left!=nullptr)
            y->left->isBlack=true;
          y->isBlack=false;
          right_rotate(y);
          y=x->parent->right;
        }
        y->isBlack=x->parent->isBlack;
        x->parent->isBlack=true;
        if(y->right!=nullptr)
          y->right->isBlack=true;
        left_rotate(x->parent);
        x=root;
      }
    }
    else{//symmetric to if
      y=x->parent->left;
      if(y!=nullptr&&y->isBlack==false){
        y->isBlack=true;
        y->parent->isBlack=false;
        right_rotate(x->parent);
        y=x->parent->left;
      }
      if((y->left!=nullptr&&y->right!=nullptr)&&(y->right->isBlack==true&&y->left->isBlack==true)){
        y->isBlack=false;
        x=x->parent;
      }
      else{ 
        if((y->left!=nullptr)&&y->left->isBlack==true){
          if(y->right!=nullptr)
            y->right->isBlack=true;
          y->isBlack=false;
          left_rotate(y);
          y=x->parent->left;
        }
        y->isBlack=x->parent->isBlack;
        x->parent->isBlack=true;
        if(y->left!=nullptr)
          y->left->isBlack=true;
        right_rotate(x->parent);
        x=root;
      }
    }
  }
  x->isBlack=true;
}

//returns a vector that contains all of the values in the tree; the contents of the vector are in ascending order
template<typename T>
vector<T> RedBlackTree<T>::values(){
	vector<T> result;
	valuehelper(result, root);
	return result;
}

//helper method for values
template<typename T>
void RedBlackTree<T>::valuehelper(vector<T>& tree, NodeT<T>* nd){
	if(nd!=nullptr){
    tree.push_back(nd->data);
	  valuehelper(tree, nd->left);
	  valuehelper(tree, nd->right);
  }
}

//returns the largest value stored in the tree that is less than the method's single template parameter
//n is the value selected
template<typename T>
T RedBlackTree<T>::closestLess(T n){
	NodeT<T>* newNode = root;
	T diff=n-newNode->data;
	while(newNode!=nullptr)
	{
		if(diff<0 && newNode->data-n>0)
			diff=newNode->data-n;
		if(n-newNode->data>0 && (n-newNode->data < diff ))
			diff=n-newNode->data;
		if (newNode->data < n) {
			newNode=newNode->right;
		}
		else{
			newNode=newNode->left;
		}
	}
	if(diff<=0)
		return n;
	else
		return n-diff;
}

//returns the smallest value stored in the tree that is greater than the method's single template parameter
//n is the value selected
template<typename T>
T RedBlackTree<T>::closestGreater(T n){
	NodeT<T>* newNode=root;
	T diff=newNode->data-n;
	while (newNode != nullptr){
		if(diff<0 && newNode->data-n>0)
			diff=newNode->data-n;
		else if(newNode->data-n>0 && n-newNode->data<diff )
			diff=newNode->data-n;
		if(newNode->data<=n){
			newNode=newNode->right;
		}
		else{
			newNode=newNode->left;
		}
	}
	if(diff<=0)
		return n;
	else
		return n+diff;  
}

//returns the number of values stored in the tree
template <typename T>
int RedBlackTree<T>::size() const{
  return length;
}

//Part 2 - Statistics Function
void statistics(string fname){
	ifstream fin(fname);
  RedBlackTree<double> rbt;
	while(!fin.eof()){
		double num;
		while(fin >> num){
		  rbt.insert(num);
    }
    fin.close();
	}
  //calculating the average
	double total=0;
	vector<double>rbt_values=rbt.values();
	int size=rbt_values.size();
	for(vector<double>::iterator itr1=rbt_values.begin();itr1 < rbt_values.end();itr1++){
		total+=*itr1;
	}
	double average=total/size;
  //calculating the median
  double median=0;
	vector<double>::iterator itr2 = rbt_values.begin();
	for (int i=0;i<size/2;i++){
		if(size%2==0)
			median=(*(itr2+1)+*itr2) / 2;
		else
			median=*(itr2+1);

		itr2++;
	}
  //number of unique values
	cout << " # of values: " << size << endl;
	cout << "average : " << average << endl;
	cout << "median : " << median << endl;
  //closestLess
	cout << "closest < 42 : " << rbt.closestLess(42) << endl;
  //closestGreater
	cout << "closest > 42 : " << rbt.closestGreater(42) << endl;
}