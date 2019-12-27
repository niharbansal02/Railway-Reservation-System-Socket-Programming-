class cplug
{
    int ch;                            //*! int !! Test var. 
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
    
    void data_to_server(int);
    void game();
    
    ~cplug()
    {
        close(sock);
    }
};
