//Working fine
#include"sock_macro.cpp"
#include"splug.cpp"


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
    obj.data_to_client();

    return 0;
}

void splug::data_to_client()
{
    while(true)
    {
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
            break;
        }
        else if(bytesRecv==0)
        {                   
            cout<<"\033[1;31mThe client disconnected\033[0m"<<endl;
            break;
        }     

        //Displady message 
        if(buf==1)
            cout<<"\033[1;32mClient: "<<buf<<"\033[0m"<<endl;
        else
            cout<<"\033[1;31mClient: "<<buf<<"\033[0m"<<endl;
        switch(buf)
        {
            case 1:send(clientSocket,corr,sizeof(corr),0);
                    break;
            case 2:send(clientSocket,incorr,sizeof(incorr),0);
                    break;
            case 3:send(clientSocket,incorr,sizeof(incorr),0);
                    break;
            case 4:send(clientSocket,incorr,sizeof(incorr),0);
                    break;
            default:send(clientSocket,def,sizeof(def),0);
                    break;       
        }
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
    }
}
