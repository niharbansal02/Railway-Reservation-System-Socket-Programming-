#include"sock_macro.cpp"
#include"cplug.cpp"

int main()
{   
    char choice='y';
    cplug obj;
    obj.init_hint_struct();
    if(obj.connect_to_server()==-1)
        exit(0);
    while(choice=='y')   
    {
        system("clear");
        obj.game();
        cout<<"\n Continue? ";
        cin>>choice;
        choice=tolower(choice);
    }

    return 0;
}

void cplug::data_to_server(int ch)
{             
    if(send(sock,(int*)&ch,sizeof(ch),0)==-1)
    {
        cerr<<"\033[1;31mCoudnot send data to server \033[0m";
        exit(0);
    }

    strcpy(buf,"0");
    int bytesRecieved=recv(sock,buf,sizeof(buf),0);
    strRec=string(buf,bytesRecieved);
    if(strcmp(strRec.c_str(),corr)==0)
        cout<<"\033[1;32m"<<strRec<<"\033[0m"<<endl;
    else 
        cout<<"\033[1;31m"<<strRec<<"\033[0m"<<endl;

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
    
} 

void cplug::game()
{
    
    cout<<"\n Welcome to quiz game";
    cout<<"\n What is 2+2 "<<endl;
    cout<<" a. 4\t b.6\n c. 8\t d. 10"<<endl;
    // getline(cin,ch);
    cin>>ch;
    // for_each(ch.begin(),ch.end(),[](char &c)
    // {
    //     c=tolower(c);
    // });
    data_to_server(ch);
}

/**

*TODO: Create two types of Users => 1. Admin 2. User 
*TODO: Initialize no. of seats, fare, train name, train no., coaches
** https://app.box.com/s/5f023fb64f685a2c88e1/file/567749143
** https://app.box.com/s/f4gpw7ghp1br9e13z8n5
*TODO: Use DATA FILE HANDLING
*? Learn DATA FILE HANDLING
*! PASS adequeate data, min data to server
*! SEE hide pass
* @param socket 

*/

