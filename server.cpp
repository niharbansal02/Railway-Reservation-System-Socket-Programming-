//Working fine
#include"sock_macro.cpp"
//#include"splug.cpp"
class splug
{
    int listening;
    sockaddr_in hint,client;
    socklen_t clientSize;
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    int clientSocket;
    int result;
    int buf;                 // *! Changed char buf to int
    char passbuf[8];
    int bytesRecv;
    string getString;                           //Test var.

    public:
    int dec=1;
    splug()
    {
        listening=socket(AF_INET,SOCK_STREAM,0);
        if(listening==-1)
            cerr<<"\033[1;31m Can't create socket.\033[0m ";
    }
    
    int bind_socket()
    {
        hint.sin_family=AF_INET;
        hint.sin_port=htons(54000);
        inet_pton(AF_INET,"0.0.0.0",&hint.sin_addr);
        if(bind(listening,(sockaddr*)&hint,sizeof(hint))==-1)
        {
            cerr<<"\033[1;31m Can't bind to IP/PORT\033[0m";
            return -1;
        }
    }

    int listen_in()
    {
        if(listen(listening,SOMAXCONN)==-1)
        {
            cerr<<"\033[1;31m Can't listen!\033[0m";
            return -1;
        }

    }

    int accept_call()
    {
        clientSize=sizeof(client);
        clientSocket=accept(listening,(sockaddr*)&client,&clientSize);
        if(clientSocket==-1)
        {
            cerr<<"\033[1;31m Problem with client connecting!\033[0m";
            return -1;
        }
    }

    void close_listening()
    {
        close(listening);
        memset(host,0,NI_MAXHOST);                                      //Cleanup of host
        memset(svc,0,NI_MAXSERV);  
    }

    void connect_message()
    {
        result=getnameinfo((sockaddr*)&client,clientSize,host,NI_MAXHOST,svc,NI_MAXSERV,0);
        if(result)                                                      //result==0
        {
            cout<<"\033[1;36m"<<host<<"\033[0m connected on "<<"\033[1;33m"<<svc<<"\033[0m"<<endl;
        }
        else
        {
            inet_ntop(AF_INET,&client.sin_addr,host,NI_MAXHOST);
            cout<<"\033[1;36m"<<host<<"\033[0m connected on "<<"\033[1;33m"<<ntohs(client.sin_port)<<"\033[0m"<<endl;
        }
    }
    
    string encrypt(string &pass)
    {
        for(int i=0;pass[i]!='\0';i++)
        {
            pass[i]=pass[i]+(i+1);
        }
        return pass;
    }

    string decrypt(string epass)
    {
        string dpass;
        for(int i=0;epass[i]!='\0';i++)
        {
            epass[i]=(epass[i])-(i+1);
        }
        return epass;
    }
    int id_to_client();
    int pass_to_client();
    int edit_pass();
    int decider()
    {
        int buffer;
        buffer=0;
        bytesRecv=recv(clientSocket,(int*)&buffer,sizeof(buffer),0);
        if(bytesRecv==-1)                                                       
        {
            cerr<<"\033[1;31mThere was a connection issue\033[0m"<<endl;
            // break;
        }
        else if(bytesRecv==0)
        {                   
            cout<<"\033[1;31mThe client disconnected\033[0m"<<endl;
            // break;
        }  
        
        switch(buffer)
        {
            case 1: dec=id_to_client();
                    break;
            case 2: dec=pass_to_client();
                    break;
            case 3: dec=edit_pass();
            case 100:   dec=-1;
                        return -1;
            default: cerr<<"\033[1;33mWaithing for message...\033[0m"<<endl;
        }
    }

    ~splug()
    {
        close(clientSocket);
    }


};

int main()
{
    cout<<0<<endl;
    splug obj;
    if(obj.bind_socket()==-1)
        exit(0);
    if(obj.listen_in()==-1)
        exit(0);
    if(obj.accept_call()==-1)
        exit(0);
    obj.close_listening();
    obj.connect_message();
    while(obj.dec!=-1)
    {
        obj.decider();
    }
    
    // int n;
    // cin>>n;  

    // string pass="pass78\0";
    // cout<<obj.encrypt(pass)<<endl;
    // cout<<obj.decrypt(pass)<<endl;



    return 0;
}

int splug::id_to_client()
{
    // while(true)
    // {
        //Clear the buffer
        // memset(buf,0,4096);
        // strcpy(buf,"0");         
        buf=0;                                  // *!
        
        // Wait for message
        bytesRecv=recv(clientSocket,(int *)&buf,sizeof(buf),0);            //recv() function recieves data from client
        //recv(socket,variable to store,size of variable,flags)
        
        if(bytesRecv==-1)                                                       
        {
            cerr<<"\033[1;31mThere was a connection issue\033[0m"<<endl;
            return -1;
            exit(0);
        }
        else if(bytesRecv==0)
        {                   
            cout<<"\033[1;31mThe client disconnected\033[0m"<<endl;
            return -1;
            exit(0);
        }     
        fstream fin;
        fin.open("id.txt",ios::in|ios::binary);
        if(!fin.is_open())
        {
            cerr<<"\033[1;31m Error opening id file! \033[0m;";
            return -1;
            exit(0);
        }
        int compid;
        fin>>compid;
        fin.close();
        //Displady message 
        if(buf==compid)
        {
            cout<<"\033[1;32mClient: "<<buf<<"\033[0m"<<endl;
            send(clientSocket,corr,sizeof(corr),0);
        }
        else
        {
            cout<<"\033[1;31mClient: "<<buf<<"\033[0m"<<endl;
            send(clientSocket,incorrid,sizeof(incorrid),0);
        }
    // }
    return 1;
}

int splug::pass_to_client()
{
    // while(true)
    // {
        //Clear the buffer
        // memset(buf,0,4096);
        strcpy(passbuf,"0");
        string str;
        // Wait for message
        bytesRecv=recv(clientSocket,(void *)&str,sizeof(str),0);            //recv() function recieves data from client
        //recv(socket,variable to store,size of variable,flags)

        if(bytesRecv==-1)                                                       
        {
            cerr<<"\033[1;31mThere was a connection issue\033[0m"<<endl;
            return -1;
            exit(0);
        }
        else if(bytesRecv==0)
        {                   
            cout<<"\033[1;31mThe client disconnected\033[0m"<<endl;
            return -1;
            exit(0);
            
        }     

        //* Get pass from file
        fstream fin;
        fin.open("pass1.txt",ios::in|ios::binary);
        if(!fin.is_open())
        {
            cerr<<"\033[1;31m Error opening pass file! \033[0m;";
            return -1;
            exit(0);
        }
        // char compPass[8];
        string copass;
        fin>>copass;
        decrypt(copass);
        // fin>>compPass;                  //This is working fine!
        fin.close();
        //Displady message 
        if(passbuf==copass)
        {
            cout<<"\033[1;32mClient: "<<passbuf<<"\033[0m"<<endl;
            send(clientSocket,corr,sizeof(corr),0);
        }
        else
        {
            cout<<"\033[1;31mClient: "<<passbuf<<"\033[0m"<<endl;
            send(clientSocket,incorrpass,sizeof(incorrpass),0);
        }
        return 1;
    // }
}

int splug::edit_pass()
{
    //MEMSET
    strcpy(passbuf,"0");

    //Wait for message
    bytesRecv=recv(clientSocket,passbuf,sizeof(passbuf),0);

    cout<<"\033[1;33m"<<passbuf<<"\033[0m"<<endl;           //remove it later

    if(bytesRecv==-1)                                                       
    {
        cerr<<"\033[1;31mThere was a connection issue\033[0m"<<endl;
        return -1;
        exit(0);
    }
    else if(bytesRecv==0)
    {                   
        cout<<"\033[1;31mThe client disconnected\033[0m"<<endl;
        return -1;
        exit(0);
        
    }   

    fstream fout;
    fout.open("pass.txt",ios::binary|ios::out);
    if(!fout.is_open())
    {
        cerr<<"\033[1;31m Error opening pass file! \033[0m;";
        return -1;
        exit(0);
    }
    fout<<passbuf;
    fout.close();

    fstream fin;
    fin.open("pass.txt",ios::in|ios::binary);
    if(!fin.is_open())
    {
        cerr<<"\n \033[1;31mError changing password!\n Contact dev. \033[0m"<<endl;
        exit(0);
    }

    char conf[8];
    fin>>conf;
    if(strcmp(conf,passbuf)==0)
    {
        if(send(clientSocket,corr,sizeof(corr),0)==-1)
        {
            cerr<<"\n \033[1;31mSending error!! \033[0m";
            return -1;
        }
    }
    else
    {
        send(clientSocket,incorrid,sizeof(incorrid),0);
        cerr<<"\n \033[1;31mError changing password!\n Contact dev. \033[0m"<<endl;
        return -1;
        exit(0);
    }
    
    fout.close();
    return 0;    
}



// switch(buf)
        // {
        //     case 1:send(clientSocket,corr,sizeof(corr),0);
        //             break;
        //     case 2:send(clientSocket,incorr,sizeof(incorr),0);
        //             break;
        //     case 3:send(clientSocket,incorr,sizeof(incorr),0);
        //             break;
        //     case 4:send(clientSocket,incorr,sizeof(incorr),0);
        //             break;
        //     default:send(clientSocket,def,sizeof(def),0);
        //             break;       
        // }
        // cout<<"\033[1;32mClient: "<<string(buf,0,bytesRecv)<<"\033[0m"<<endl
        // if(strcmp(buf,"a")==0)
        //     send(clientSocket,corr,sizeof(corr),0);
        // else if(strcmp(buf,"b")==0)
        //     send(clientSocket,incorr,sizeof(incorr),0);
        // else if(strcmp(buf,"c")==0)
        //     send(clientSocket,incorr,sizeof(incorr),0);
        // else if(strcmp(buf,"d")==0)
        //     send(clientSocket,incorr,sizeof(incorr),0);
        // else
        //     send(clientSocket,def,sizeof(def),0);
        // send(clientSocket,(int*)&buf,sizeof(buf),0);