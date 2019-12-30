//Inheritence working fine
//TODO: Complete client send and recieve messsages of id, password;
//TODO: Make id,password file
//TODO: Booking system
#include"sock_macro.cpp"
//#include"cplug.cpp"

#define corr "Correct!"
class cplug
{
    int ch;                            //*! int !! Test var. 
    char passkey[50];
    // int sock;
    int port;
    int connectRes;
    string ipAddress;
    char buf[4608];
    string strRec;
    sockaddr_in hint;
    
    public:
    int sock;
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

class rail : public cplug
{
    int tno;
    char tname[16];
    char tempname[16];
    int noTrain;
    int SL;
    int A3;
    int A2;
    int A1;
    int mech;                           //Menu choice
    // vector<string> troute;
    int noStation;
    int adminID;
    string adminPass;
    string rStat;
    fstream train_det;
    // int get_tno;
    // unsigned int count=0;

    void def_no_seats()         //! Admin
    {   
        cout<<"\n Enter no. of seats in Sleeper: ";   cin>>SL; 
        cout<<"\n Enter no. of seats in 3 Tier AC: "; cin>>A3;
        cout<<"\n Enter no. of seats in 2 Tier AC: "; cin>>A2; 
        cout<<"\n Enter no. of seats in 1 Tier AC: "; cin>>A1; 
        print_to_stream();
    }

    void def_train_route()      //! Admin
    {
        train_det.open("trainDetails.txt",ios::out|ios::app|ios::binary);
        if(!train_det.is_open())
            cerr<<"\033 [1;31m Error opening file. [0m";
        cout<<"\n Enter no. of stations(including SRC and DEST): "; cin>>noStation; 
        for(int i=0;i<noStation;i++)
        {
            cout<<"\n Enter station name "<<i+1<<": ";
            // getline(cin,t[count].rStat);
            cin>>rStat;
            train_det<<rStat<<" ";
            // cin>>t[count].rStat; 
            // t[count].troute.push_back(t[count].rStat);    
        }
        train_det.close();
    }

    void print_to_stream()
    {
        train_det.open("trainDetails.txt",ios::out|ios::binary|ios::app);
        
        train_det<<tno<<" ";
        train_det<<tname<<" ";
        train_det<<SL<<" ";
        train_det<<A3<<" ";
        train_det<<A2<<" ";
        train_det<<A1<<" ";

        train_det.close();

    }

    public:
    rail()
    {
        string whole;
        int endpoint;

        train_det.open("trainDetails.txt",ios::in|ios::binary);
        getline(train_det,whole);
        train_det.close();
        
        endpoint=strlen(whole.c_str());
        noTrain=endpoint/35;
    }
    void change_pass()
    {
        string currpass;
        int passvar;
        int aaaa=1;
        do
        {
            cout<<"\n Enter current password: "; getline(cin,currpass);
            passvar=pass_to_server(currpass);
            if(passvar==1)
            {
                // cin.ignore(numeric_limits<streamsize>::max(),'\n');     
                char newpass[8];
                char confpass[8];
                do
                {
                    system("clear");
                    cout<<"\n Enter new password(max. 7 char.): ";  cin.getline(newpass,8);
                    cout<<"\n Confirm new password: "; cin.getline(confpass,8);
                    if(strcmp(newpass,confpass)==0)
                    {
                        fstream fout;
                        fout.open("pass.txt",ios::out|ios::binary);
                        if(!fout.is_open())
                        {
                            cerr<<"\n \033[1;31mError changing password!\n Contact dev. \033[0m"<<endl;
                            exit(0);
                        }
                        fout<<newpass;
                        fout.close();
                        fout.open("pass.txt",ios::in|ios::binary);
                        if(!fout.is_open())
                        {
                            cerr<<"\n \033[1;31mError changing password!\n Contact dev. \033[0m"<<endl;
                            exit(0);
                        }
                        char conf[8];
                        fout>>conf;
                        if(strcmp(conf,newpass)==-1)
                        {
                            cerr<<"\n \033[1;31mError changing password!\n Contact dev. \033[0m"<<endl;
                            exit(0);
                        }
                        aaaa=0;
                        cout<<"\n Password changed successfully!";
                    }
                    else
                    {
                        cerr<<"\n \033[1;31mPasswords do not match!\033[0m";
                        sleep(1);
                    }
                }while(aaaa==1);
            }
        }while(passvar!=1);

    }

    void def_train_det()        //! Admin
    {
        cout<<"\n \t\t\t**** Enter Train Details ***"<<endl;    
        cout<<"\n Enter train no.: ";  cin>>tno;      
        cout<<" Enter train name: "; 
        cin.getline(tname,16);
        cin.getline(tname,16);
        for(int i=0;i<strlen(tname)+1;i++)
        {
            if(tname[i]==' ')
                tname[i]='0';
        }
        for(int i=strlen(tname);i<16;i++)
            tname[i]='0';
        def_no_seats();
        // def_train_route();
    }
    
    void get_all_trains()
    {
        system("clear");
        char ttno[6];
        char ttname[17];
        char tnoseat[13];
        int prseekg;

        for(int i=0;i<noTrain;i++)
        {
            prseekg=i*35;
            //*Train no.
            train_det.open("trainDetails.txt",ios::in|ios::binary);
            fcheck();
            train_det.seekg(prseekg);
            train_det.getline(ttno,6);
            cout<<"Train no: "<<ttno<<endl;
            train_det.close();

            //*Train Name
            train_det.open("trainDetails.txt",ios::in|ios::binary);
            fcheck();
            train_det.seekg(prseekg+6);
            train_det.getline(ttname,17);
            for(int i=0;i<16;i++)
            {   
                if(ttname[i]=='0')
                    ttname[i]=' ';
            }
            cout<<"Train name: "<<ttname<<endl;
            train_det.close();
            
            //*No of seats
            train_det.open("trainDetails.txt",ios::in|ios::binary);
            fcheck();
            train_det.seekg(prseekg+23);
            train_det.getline(tnoseat,13);
            cout<<"\n\033[1;32mNO. OF SEATS AVAILABLE\033[0m"<<endl;
            cout<<"SL 3A 2A 1A"<<endl;
            cout<<tnoseat<<endl;
            train_det.close();
            cout<<"---------------------------------------------------"<<endl;
        }
    }

    void get_train_details()
    {
        char ttno[6];
        char ttname[17];
        char tnoseat[13];
        
        //Search operation
        char stno[6];
        cout<<"\n Enter train no. whose details are to be searched: "; cin>>stno;
        int tellpoint;
        string search,whole;
        int endpoint;
        train_det.open("trainDetails.txt",ios::in|ios::binary);
        fcheck();
        getline(train_det,whole);
        endpoint=strlen(whole.c_str());
        // cout<<whole<<endl<<endl;
        train_det.seekg(0);
        while(!train_det.eof())
        {
            train_det>>search;
            if(strcmp(search.c_str(),stno)==0)
            {
                // cout<<search<<" "<<"FOUND!"<<endl;
                tellpoint=train_det.tellg();
                // cout<<tellpoint<<endl;
                break;
            }
        }
        train_det.close();
        // cout<<tellpoint<<endl<<endl;
        if(tellpoint<endpoint && tellpoint>4)
        {   
            int searchpoint=tellpoint-5;
            train_det.open("trainDetails.txt",ios::in|ios::binary);
            fcheck();
            train_det.seekg(searchpoint);
            train_det.getline(ttno,6);
            cout<<"Train no: "<<ttno<<endl;
            train_det.close();

            train_det.open("trainDetails.txt",ios::in|ios::binary);
            fcheck();
            train_det.seekg(searchpoint+6);
            train_det.getline(ttname,17);
            for(int i=0;i<16;i++)
            {   
                if(ttname[i]=='0')
                    ttname[i]=' ';
            }
            cout<<"Train name: "<<ttname<<endl;
            train_det.close();
            
            train_det.open("trainDetails.txt",ios::in|ios::binary);
            fcheck();
            train_det.seekg(searchpoint+23);
            train_det.getline(tnoseat,13);
            cout<<"\n\033[1;32mNO. OF SEATS AVAILABLE\033[0m"<<endl;
            cout<<"SL 3A 2A 1A"<<endl;
            cout<<tnoseat<<endl;
            train_det.close();
        }
        else
        {
            cerr<<"\033[1;31mTrain details not found!\033[0m"<<endl;
        }
    }

    void update_train_details()
    {
        int tellpoint;
        int search;
        string B,C;
        int flag=-1;
        int stno;
        train_det.open("trainDetails.txt",ios::in|ios::binary);
        fcheck();
        // cin.ignore(numeric_limits<streamsize>::max(),'\n');      //uncomment it later 
        cout<<"\n Enter train no.: "; cin>>stno;
        train_det.seekg(0);
        for(int i=0;i<noTrain;i++)
        {
            train_det.seekg(i*35);
            train_det>>search;
            if(stno==search)
            {
                flag=i;
                train_det.close();
                break;
            }    
        }
        if(flag!=-1)
        {
            train_det.open("trainDetails.txt",ios::in|ios::binary);
            fcheck();
            train_det.seekg(35*flag);
            char uptno[6];
            cout<<" Enter updated train no.: ";
            cin>>uptno;
            char A[35*flag];
            train_det.seekg(0);
            train_det.getline(A,(35*flag));
            train_det.close();

            train_det.open("trainDetails.txt",ios::in|ios::binary);
            train_det.seekg(35*flag + 6);
            getline(train_det,C);
            train_det.close();

            // cout<<A<<" "<<uptno<<" "<<C;

            train_det.open("trainDetails.txt",ios::out|ios::binary);
            train_det<<A<<" "<<uptno<<" "<<C;
        }
    }
    void fcheck()
    {
        if(!train_det.is_open())
        {
            cerr<<"\033[1;31m Error opening file! [0m;";
            // return -1;
        }
    }
    void admin()
    {   
        int adch;
        int passvar=-1;
        do
        {
            system("clear");
            cout<<"\n \033[1;32m ADMIN MODE\033[0m"<<endl;
            cout<<"Enter your id: "; cin>>adminID;
            cin.ignore(numeric_limits<streamsize>::max(),'\n');     
            if(id_to_server(adminID)==1)
            {
                cout<<"Enter password: "; getline(cin,adminPass);
                passvar=pass_to_server(adminPass);
            }
        }while(passvar!=1);

        if(passvar==1)
        {
            do
            {
                system("clear");
                cout<<"\n \033[1;32m ADMIN MENU\033[0m"<<endl;
                cout<<" 1. Add train details"<<endl;
                cout<<" 2. Update train details"<<endl;
                cout<<" 3. Get all train details"<<endl;
                cout<<" 4. Get particular train details"<<endl;
                cout<<" 5. Change Password"<<endl;
                cout<<" 6. Exit"<<endl; 
                // cout<<" 7. Add admin id"<<endl;
                cout<<" Enter your choice: "<<"\033[1;32m"; cin>>adch;
                cout<<"\033[0m";
                switch(adch)
                {
                    case 1: def_train_det();            //* Done
                            break;
                    case 2: update_train_details();     //! Under construction
                            break;
                    case 3: get_all_trains();           //* Done
                            break;
                    case 4: get_train_details();        //* Done
                            break;
                    case 5: change_pass();              //* Done
                            break;
                    case 6: return;
                    default:cerr<<"\n \033[1;31mEnter correct choice\033[0m";
                            sleep(1);
                }
            }while(adch<1 || adch>6);
        }
    }
    
    void user()
    {
        cout<<"\n Under construction"<<endl;
    }

    void menu()
    {
        do
        {
            system("clear");
            cout<<"\n \033[1;36m\t\t\t WELCOME TO INDIAN RAILWAYS\033[0m"<<endl;
            cout<<"_________________________________________________________________________________________________"<<endl;
            cout<<"\n\n \033[1;32mSELECTION MENU\033[0m"<<endl;
            cout<<"\n 1. Admin"<<endl<<" 2. User"<<endl<<" 3. Exit"<<endl<<" Enter your choice: "; cin>>mech;
            if(mech<1 || mech>3)
            {
                cerr<<"\n \033[1;31m Enter correct choice!\033[0m";
                sleep(1);
            }
        }while(mech<1 || mech>3);
        switch(mech)
        {
            case 1: admin();
                    break;
            case 2: user();
                    break;
            case 3: return;
            default: cerr<<"\n\033[1;31mCan't occur; Contact dev";
                    break;
        }
    }

    ~rail()
    {
        int exitsignal=100;
        send(sock,(int *)&exitsignal,sizeof(exitsignal),0);
        close(sock);
    }
};

int main()
{
    char choice='y';
    rail obj;
    // obj.init_hint_struct();
    // if(obj.connect_to_server()==-1)
    //     exit(0);
    // // while(choice=='y')   
    // // {
    //     system("clear");
    //     obj.menu();
        // cout<<"\n Continue? ";
        // cin>>choice;
        // choice=tolower(choice);
    // }

    system("clear");
    obj.update_train_details();
    
    /*
    fstream fin;
    string str;
    fin.open("trainDetails.txt",ios::in|ios::binary);
    fin>>str;
    cout<<endl<<str;
    */

    //TODO: test function seekg to the train no.
    //TODO: admin login password pass to the server
    //TODO: pass on to client

    return 0;
}


//// void get_train_route()
    // {
    //     cout<<"\n Enter train no.: ";
    //     cin>>get_tno;
    //     int search;
    //     // int fcount;
    //     for(search=1;search<50;search++)
    //     {
    //         if(t[search].tno==get_tno)
    //             break;
    //     }

    //     // cout<<"\033[1;32m"<<*(t[search].troute.begin())<<"\033[0m"<<endl;
    //     for(auto i=t[search].troute.begin();i<t[search].troute.end();i++)
    //         cout<<*i<<endl;
    //     // auto s=t[search].troute.end();
    //     // cout<<*s<<endl;
    // // }

     // string ch;
    // string search="00";
    // long  pos;
    // while(!fin.eof())
    // {
    //     // getline(fin,ch);
    //     fin>>ch;
    //     pos=ch.find(search);
    //     // cout<<ch<<endl;
    //     if(pos!=string::npos)
    //     {
    //         // cout<<"Found at "<<(fin.tellg()-sizeof(ch)-strlen((char*)&ch));
    //         cout<<pos<<endl<<fin.tellg();

    //         break;
    //     }
    // }
    
    // fstream fin;
    // fin.open("trainDetails.txt",ios::in|ios::binary);
    // if(!fin.is_open())
    // {
    //     cerr<<"\033[1;31m Error opening file! [0m;";
    //     return -1;
    // }

    // string search;
    // fin.seekg(0);
    // while(!fin.eof())
    // {

    //     fin>>search;
    //     if(strcmp(search.c_str(),"19002")==0)
    //     {
    //         cout<<search<<" "<<"FOUND!"<<endl;
    //         cout<<fin.tellg();
    //         break;
    //     }
    // }

    // fin.seekg(6);
    // char asd[16];
    // fin.getline(asd,16);
    // for(int i=0;i<16;i++)
    // {
    //     if(asd[i]=='0')
    //         asd[i]=' ';
    // }
    // cout<<asd;


    // char asd[50];
    // cin.getline(asd,50);
    // for(int i=0;i<strlen(asd);i++)
    // {
    //     cout<<i<<' ';
    //     if(asd[i]==' ' || asd[i]==NULL)
    //         asd[i]='0';
    // }

    // cout<<endl<<asd;