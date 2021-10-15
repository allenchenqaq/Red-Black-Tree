#include "RedBlackTree.h"

// int main()
// {
// 	// Int Tree Tests	
// 	RedBlackTree<int> rb1;
// 	if (rb1.insert(42)) cout << "inserted 42" << endl;
// 	rb1.insert(71);
//   cout << "aaa" << endl;
// 	rb1.insert(13);
//   cout << "bbb" << endl;
// 	RedBlackTree<int> rb2(rb1);
//   cout << "ccc" << endl;
// 	if (rb1.remove(13)) cout << "removed 13" << endl;
// 	if (rb1.search(42)) cout << "found 42" << endl;
 
// 	vector<int> v1 = rb1.search(50, 100); //should contain 71 ???
 
// 	vector<int> v2 = rb1.values(); //should contain {42,71} ???
//    for(auto itr = v2.begin(); itr < v2.end(); itr++) {
//     cout << *itr << endl;
//   }
//   cout << v2.size() << endl;
 
// 	cout << "closestLess(71) = " << rb1.closestLess(71) << endl;
// 	cout << "closestGreater(42) = " << rb1.closestGreater(42) << endl;
 
// 	cout << "size = " << rb1.size() << endl; //should be 2
//   rb1.~RedBlackTree();
// 	cout << "size after destruct = " << rb1.size();
//   return 0;
// }

int main()
{
 //Case 1: print an empty tree
 //Case 2: print an filled tree

 //Test for Case 1
 RedBlackTree<int> case1;
 vector<int> v1 = case1.values();
 vector<int>::iterator it = v1.begin();
 if (!v1.empty()) {
  cout << "{";
  while (it != v1.end() - 1) {
   cout << *it << ", ";
   it++;
  }
  cout << *it << "}" << endl;
 }
 else {
  cout << "There is no value in the tree" << endl;
 }


 RedBlackTree<int> case2;
 case2.insert(100);
 case2.insert(50);
 case2.insert(20);
 case2.insert(10);
 case2.insert(110);
 case2.insert(5);
 case2.insert(40);
 case2.insert(41);
 case2.remove(100);

 vector<int> v2 = case2.values();
 vector<int>::iterator it2 = v2.begin();
 if (!v2.empty()) {
  cout << "{";
  while (it2 != v2.end() - 1) {
   cout << *it2 << ", ";
   it2++;
  }
  cout << *it2 << "}" << endl;
 }
 else {
  cout << " " << endl;
 }

 cout << "closestLess(10) = " << case2.closestLess(10) << endl;
 cout << "closestGreater(100) = " << case2.closestGreater(100) << endl;

 return 0;
 }