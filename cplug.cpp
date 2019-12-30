#define corr "Correct!"
class cplug
{
    int ch;                            //*! int !! Test var. 
    char passkey[50];
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
    
    int id_to_server(int ch)
    {   
        int ID=1;
        if(send(sock,(int*)&ID,sizeof(ID),0)==-1)
        {
            cerr<<"\033[1;31mCoudnot send data to server \033[0m";
            return -1;
        }
        
        //*SEND        
        if(send(sock,(int*)&ch,sizeof(ch),0)==-1)
        {
            cerr<<"\033[1;31mCoudnot send data to server \033[0m";
            return -1;
        }

        //*RECV
        strcpy(buf,"0");
        int bytesRecieved=recv(sock,buf,sizeof(buf),0);
        strRec=string(buf,bytesRecieved);
        if(strcmp(strRec.c_str(),corr)==0)
            cout<<"\033[1;32m"<<strRec<<"\033[0m"<<endl;
        else 
        {
            cout<<"\033[1;31m"<<strRec<<"\033[0m"<<endl;
            sleep(1);
            return -1;
        }

        /*  
            // FOR STRINGS
            if(send(sock,ch.c_str(),sizeof(ch)+1,0)==-1)
            {
                cerr<<"\033[1;31mCoudnot send data to server[0m";
                exit(0);
            }
            strcpy(buf,"0");                                             //memset
            int bytesRecieved=recv(sock,ch,sizeof(ch),0);
            //Display Response
            cout<<" "<<strRec<<endl;
        */
        return 1;
    } 

    int pass_to_server(string str)
    {
        int PASS=2;
        
        if(send(sock,(int*)&PASS,sizeof(PASS),0)==-1)
        {
            cerr<<"\033[1;31mCoudnot send data to server[0m";
            return -1;
        }

        if(send(sock,str.c_str(),sizeof(str)+1,0)==-1)
        {
            cerr<<"\033[1;31mCoudnot send data to server[0m";
            return -1;
        }
        strcpy(buf,"0");                                             //memset
        int bytesRecieved=recv(sock,passkey,sizeof(passkey),0);
        strRec=string(passkey,bytesRecieved);
        if(strcmp(strRec.c_str(),corr)==0)
            cout<<"\033[1;32m"<<strRec<<"\033[0m"<<endl;
        else 
        {
            cout<<"\033[1;31m"<<strRec<<"\033[0m"<<endl;
            sleep(1);
            return -1;
        }
        return 1;
    }

    
    ~cplug()
    {
        int exitsignal=100;
        send(sock,(int *)&exitsignal,sizeof(exitsignal),0);
        close(sock);
    }
};
