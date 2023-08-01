#include <iostream> 
#include <string> 

using namespace std; 

int main(){

  char *str= "abcdefgcccc"; 
  char *str1= "c"; 
  char* p1; 
  while ((p1= strstr(str, str1) )!= nullptr ){
    cout<<p1<<endl; 
    cout<<p1-str<<endl; 
    str= p1+1;
  }
 return 0; 
}
