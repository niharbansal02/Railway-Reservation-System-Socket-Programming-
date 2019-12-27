#include<iostream>
#include<fstream>

using namespace std;

int main()
{
    char arr[100];
    cin.getline(arr,100);
    ofstream myfile("xyz.txt",ios::out|ios::app);
    // myfile.open("xyz.txt");
    myfile<<arr;
    myfile.close();
    cout<<"\n File write operation complete.";

    ifstream obj("xyz.txt",ios::in);
    char arr1[100];
    obj.getline(arr1,100);
    cout<<"\n\n Array: "<<arr1;
    obj.close();

    return 0;
}