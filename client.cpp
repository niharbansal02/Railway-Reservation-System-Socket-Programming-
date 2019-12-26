#include"sock_macro.cpp"

using namespace std;
class cplug
{
    string ch;                            //Test var.
    int sock;
    int port;
    int connectRes;
    string ipAddress;
    char buf[4608];
    string strRec;
    sockaddr_in hint;
    
    public:
    cplug()                                                          //Sorted
    {
        sock=socket(AF_INET,SOCK_STREAM,0);
        if(sock==-1)
            cerr<<"\033[1;31mCan't create socket\033[0m";
    }
    
    void init_hint_struct()                                         //Sorted
    {
        port=54000;
        ipAddress="127.0.0.1";
        hint.sin_family=AF_INET;
        hint.sin_port=htons(port);
        inet_pton(AF_INET,ipAddress.c_str(),&hint.sin_addr);
    }
    
    int connect_to_server()
    {
        connectRes=connect(sock,(sockaddr*)&hint,sizeof(hint));
        if(connectRes==-1)
        {
            cerr<<"\033[1;31mConnection failed\033[0m";
            return -1;
        }
    }
    
    void data_to_server(string);
    void game();
    
    ~cplug()
    {
        close(sock);
    }
};

int main()
{
    cplug obj;
    obj.init_hint_struct();
    if(obj.connect_to_server()==-1)
        exit(0);
    while(1)   
    obj.game();

    return 0;
}

void cplug::data_to_server(string ch)
{
    if(send(sock,ch.c_str(),sizeof(ch)+1,0)==-1)                        //Send_to_server              
    {
        cerr<<"\033[1;31mCoudnot send data to server \033[0m";
        exit(0);
    }
    strcpy(buf,"0");                                                    //Clear_the_buffer;
    int bytesRecieved=recv(sock,buf,4096,0);
    strRec=string(buf,bytesRecieved);
    // display response
    if(strcmp(corr,strRec.c_str())==0)                              //strRec.c_srt() is v. imp.
        cout<<"\033[1;32m"<<strRec<<" \033[0m\r\n\n";
    else
        cout<<"\033[1;31m"<<strRec<<" \033[0m\r\n\n";
    
} 

void cplug::game()
{
    cout<<"\n Welcome to quiz game";
    cout<<"\n What is 2+2 "<<endl;
    cout<<" a. 4\t b.6\n c. 8\t d. 10"<<endl;
    getline(cin,ch);
    for_each(ch.begin(),ch.end(),[](char &c)
    {
        c=tolower(c);
    });
    data_to_server(ch);
}