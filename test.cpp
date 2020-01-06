#include<iostream>
#include<fstream>

using namespace std;

int main()
{
    system("clear");
    fstream fin;
    string asd;
    fin.open("test.txt",ios::binary|ios::in);
    fin.seekg(1);
    getline(fin,asd);
    cout<<asd<<" ";
    
    return 0;
}