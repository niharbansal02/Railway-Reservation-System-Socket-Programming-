//Inheritence working fine
//? PNR:- 6 digit TNO:- 5 digit TNAME:- 15 char.
//TODO: Get all quantities a check for which acc. value>0
//TODO: Booking system
//TODO: Add a delete train option to admin menu
//TODO: ADD date of journey to Ticket
//TODO: ADD Troute and get it functional
#include"sock_macro.cpp"
#include<time.h>
#define ignore cin.ignore(numeric_limits<streamsize>::max(),'\n')

//#include"cplug.cpp"
typedef unsigned int ui;
#define corr "Correct!"
class cplug
{
    int ch;                            //*! int !! Test var. 
    char passkey[50];
    // int sock;
    int port;
    int sendInt;                //!Replace the ID,PASS,CHANGE to sendInt later
    int connectRes;
    string ipAddress;
    int bytesRecieved;
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
        bytesRecieved=recv(sock,buf,sizeof(buf),0);
        strRec=string(buf,bytesRecieved);
        if(strcmp(strRec.c_str(),corr)==0)
            return 1;
        else 
        {
            cout<<"\033[1;31mEnter correct ID\033[0m"<<endl;
            sleep(1);
            return -1;
        }

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
        bytesRecieved=recv(sock,passkey,sizeof(passkey),0);
        // strRec=string(passkey,bytesRecieved);
        if(strcmp(passkey,corr)==0)
        {
            cout<<"\033[1;32m Access Granted\033[0m"<<endl;
            sleep(1);
        }        
        else 
        {
            cout<<"\033[1;31m Access Denied\033[0m"<<endl;
            sleep(1);
            return -1;
        }
        return 1;
    }

    int change_pass_req(string pass)
    {
        int CHANGE=3;
        if(send(sock,(int *)&CHANGE,sizeof(CHANGE),0)==-1)
        {
            cout<<"\n \033Coudnot send data to server!! \033[0m";
            sleep(1);
            return -1;
        }
        
        if(send(sock,pass.c_str(),sizeof(pass),0)==-1)
        {
            cout<<"\n \033Could not send data to server!! \033[0m";
            sleep(1);
            return -1;
        }

        //? Checked till here, works fine

        strcpy(buf,"0");
        bytesRecieved=recv(sock,buf,sizeof(buf),0);
        // strRec=string(buf,bytesRecieved);

        if(strcmp(buf,corr)==0)
        {
            cout<<"\n \033[1;32m Password changed successfully!! \033[0m"<<endl;
            sleep(1);
            return 1;
        }
        else
        {
            cerr<<"\n \033[1;31m Error in changing password!! \033[0m"<<endl;
            sleep(1);
            return -1;
        }        
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
    ui tno;
    char tname[16];
    char tempname[16];
    int noTrain;
    char from[8];
    char to[8];
    ui SL,A3,A2,A1;
    int SLp=400;            //Yet to be used;
    int A3p=600;
    int A2p=800;
    int A1p=1000;           //till here
    int mech;                           //Menu choice
    // vector<string> troute;
    int noStation;
    int adminID;
    string adminPass;
    string rStat;
    fstream train_det;
    int pnr;
    // int get_tno;
    // unsigned int count=0;

    void def_no_seats()         //! Admin
    {   
        do
        {
            system("clear");
            cout<<"\n \033[36mNo. of seats must be less than 100\033[0m"<<endl;
            cout<<"\n Enter no. of seats in Sleeper: ";   cin>>SL; 
            cout<<"\n Enter no. of seats in 3 Tier AC: "; cin>>A3;
            cout<<"\n Enter no. of seats in 2 Tier AC: "; cin>>A2; 
            cout<<"\n Enter no. of seats in 1 Tier AC: "; cin>>A1; 
            if(SL<0 || A3<0 || A2<0 || A1<0)
            {    
                cerr<<"\n \033[1;31m How can no. of seats be negative!! Re-enter! \033[0m";
                sleep(1);
            }
            if(SL>99 || A3>99 || A2>99 || A1>99)
            {
                cerr<<"\n \033[1;31mSeats must be less than 100!!! Re-enter! \033[0m";
                sleep(1);
            }
        }while(SL<0 || A3<0 || A2<0 || A1<0 || SL>99 || A3>99 || A2>99 || A1>99);
        print_to_stream();
    }

    void def_train_route()      //! Admin   //!Broken code
    {
        train_det.open("trainDetails.txt",ios::out|ios::app|ios::binary);
        if(!train_det.is_open())
            cerr<<"\033 [1;31m Error opening file. [0m";
        cout<<"\n Enter no. of stations(including SRC and DEST): "; cin>>noStation; 
        for(int i=0;i<noStation;i++)
        {
            cout<<"\n Enter station name "<<i+1<<": ";
            cin>>rStat;
            train_det<<rStat<<" ";
        }
        train_det.close();
    }

    void print_to_stream()
    {
        train_det.open("trainDetails.txt",ios::out|ios::binary|ios::app);
        
        train_det<<tno<<" ";
        train_det<<tname<<" ";

        if(SL<10)
            train_det<<"0"<<SL<<" ";
        else
            train_det<<SL<<" ";

        if(A3<10)
            train_det<<"0"<<A3<<" ";
        else
            train_det<<A3<<" ";

        if(A2<10)
            train_det<<"0"<<A2<<" ";
        else
            train_det<<A2<<" ";
        
        if(A1<10)
            train_det<<"0"<<A1<<" ";
        else
            train_det<<A1<<" ";

        train_det.close();

    }

    int update_train_no(int flag)
    {
        string B,C;
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
        return 0;
    }

    void update_train_name(int trainIndex) 
    {
        int currSeek=trainIndex*35;
        cout<<" Enter (new) train name: "; 
        ignore;
        cin.getline(tname,16);
        for(int i=0;i<strlen(tname);i++)
        {
            if(tname[i]==' ')
                tname[i]='0';
        }
        for(int i=strlen(tname);i<16;i++)
            tname[i]='0';
        tname[16]='\0';

        char A[currSeek+6];
        string C;
        train_det.open("trainDetails.txt",ios::in|ios::binary);
        train_det.seekg(0);
        train_det.getline(A,(currSeek+6));
        train_det.close();

        train_det.open("trainDetails.txt",ios::in|ios::binary);
        train_det.seekg(currSeek+23);
        getline(train_det,C);
        cout<<A<<endl;
        cout<<tname<<endl;;
        cout<<C;
        train_det.close();

        train_det.open("trainDetails.txt",ios::out|ios::binary);
        train_det.seekg(0);
        train_det<<A<<" "<<tname<<" "<<C;
    }

    void update_no_seats(int trainIndex, int coach, int upSeat)     //For direct update
    {
        int currSeek=trainIndex*35;
        int noSeat;
        char X1[currSeek+23];
        char X2[currSeek+26];
        char X3[currSeek+29];
        char X4[currSeek+32];
        string C;

        switch(coach)
        {
            case 1: //SL
                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    if(!train_det.is_open());           //!ADD Statement
                    train_det.seekg(currSeek+23);
                    train_det>>noSeat;
                    noSeat=upSeat;
                    train_det.seekg(0);
                    train_det.getline(X1,(currSeek+23));
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    train_det.seekg(currSeek+23+3);
                    getline(train_det,C);
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::out|ios::binary);
                    train_det<<X1<<" "<<noSeat<<" "<<C;
                    train_det.close();
                    break;
            case 2: //3A
                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    if(!train_det.is_open());           //!ADD Statement
                    train_det.seekg(currSeek+26);
                    train_det>>noSeat;          
                    noSeat=upSeat;                   
                    train_det.seekg(0);
                    train_det.getline(X2,(currSeek+26));        
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    train_det.seekg(currSeek+26+3);
                    getline(train_det,C);
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::out|ios::binary);
                    train_det<<X2<<" "<<noSeat<<" "<<C;
                    train_det.close();
                    break;
            case 3: //2A
                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    if(!train_det.is_open());           //!ADD Statement
                    train_det.seekg(currSeek+29);
                    train_det>>noSeat;
                    noSeat=upSeat;
                    train_det.seekg(0);
                    train_det.getline(X3,(currSeek+29));
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    train_det.seekg(currSeek+29+3);
                    getline(train_det,C);
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::out|ios::binary);
                    train_det<<X3<<" "<<noSeat<<" "<<C;
                    train_det.close();

                    break;
            case 4: //1A
                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    if(!train_det.is_open());           //!ADD Statement
                    train_det.seekg(currSeek+32);
                    train_det>>noSeat;
                    noSeat=upSeat;
                    train_det.seekg(0);
                    train_det.getline(X4,(currSeek+32));
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    train_det.seekg(currSeek+32+3);
                    getline(train_det,C);
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::out|ios::binary);
                    train_det<<X4<<" "<<noSeat<<" "<<C;
                    train_det.close();

                    break;
            default: cerr<<"\n \033[1;31mCan't Happen! Contact DEV! \033[0m";
                    exit(0);
        } 
    }

    void update_no_seats(int trainIndex,int coach)                 //For ticket booking update
    {
        int currSeek=trainIndex*35;
        int noSeat;
        char X1[currSeek+23];
        char X2[currSeek+26];
        char X3[currSeek+29];
        char X4[currSeek+32];

        string C;
        switch(coach)
        {
            case 1: //SL
                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    if(!train_det.is_open());           //!ADD Statement
                    train_det.seekg(currSeek+23);
                    train_det>>noSeat;
                    noSeat--;
                    train_det.seekg(0);
                    train_det.getline(X1,(currSeek+23));
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    train_det.seekg(currSeek+23+3);
                    getline(train_det,C);
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::out|ios::binary);
                    train_det<<X1<<" "<<noSeat<<" "<<C;
                    train_det.close();
                    break;
            case 2: //3A
                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    if(!train_det.is_open());           //!ADD Statement
                    train_det.seekg(currSeek+26);
                    train_det>>noSeat;          
                    noSeat--;                   
                    train_det.seekg(0);
                    train_det.getline(X2,(currSeek+26));        
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    train_det.seekg(currSeek+26+3);
                    getline(train_det,C);
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::out|ios::binary);
                    train_det<<X2<<" "<<noSeat<<" "<<C;
                    train_det.close();
                    break;
            case 3: //2A
                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    if(!train_det.is_open());           //!ADD Statement
                    train_det.seekg(currSeek+29);
                    train_det>>noSeat;
                    noSeat--;
                    train_det.seekg(0);
                    train_det.getline(X3,(currSeek+29));
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    train_det.seekg(currSeek+29+3);
                    getline(train_det,C);
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::out|ios::binary);
                    train_det<<X3<<" "<<noSeat<<" "<<C;
                    train_det.close();

                    break;
            case 4: //1A
                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    if(!train_det.is_open());           //!ADD Statement
                    train_det.seekg(currSeek+32);
                    train_det>>noSeat;
                    noSeat--;
                    train_det.seekg(0);
                    train_det.getline(X4,(currSeek+32));
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::in|ios::binary);
                    train_det.seekg(currSeek+32+3);
                    getline(train_det,C);
                    train_det.close();

                    train_det.open("trainDetails.txt",ios::out|ios::binary);
                    train_det<<X4<<" "<<noSeat<<" "<<C;
                    train_det.close();

                    break;
            default: cerr<<"\n \033[1;31mCan't Happen! Contact DEV! \033[0m";
                    exit(0);
        }        
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
        SL=A3=A2=A1=0;

    }

    int change_pass()
    {
        string currpass;
        int passvar;
        int flag=1;
        
        ignore;
        do
        {
            cout<<"\n Enter current password: "; getline(cin,currpass);
            passvar=pass_to_server(currpass);
            if(passvar==1)
            {
                string newpass;   
                char confpass[8];
                do
                {
                    do
                    {
                        system("clear");
                        cout<<"\n Enter new password(max. 7 char.): "; getline(cin,newpass); 
                        if(strlen(newpass.c_str())>7)
                        {
                            cerr<<"\n \033[1;31m MAX 7 CHARACTERS!!! \033[0m"<<endl;
                            sleep(1);
                        }
                    }while(strlen(newpass.c_str())>7);

                    cout<<"\n Confirm new password: "; cin.getline(confpass,8);
                    if(strcmp(newpass.c_str(),confpass)==0)
                    {
                        change_pass_req(newpass);
                        flag=0;
                        break;
                    }
                    else
                    {
                        cerr<<"\n \033[1;31mPasswords do not match!\033[0m";
                        sleep(1);
                    }
                }while(flag==1);
            }
        }while(passvar!=1);

        return 0;
    }

    int def_train_det()        //! Admin
    {
        do
        {
            system("clear");
            cout<<"\n \t\t\t**** Enter Train Details ***"<<endl;    
            cout<<"\n Enter 5 digit train no.: ";  cin>>tno;   
            if(tno<10000 || tno>99999)
            {   
                cerr<<"\n \033[1;31m Check the train no.! Re-enter\033[0m";
                sleep(1);
            }
        }while(tno<10000 || tno>99999);

        cout<<" Enter train name(less than 16 char.): ";        //Tname is already adjusted to 15 char. because of size;
        ignore;
        cin.getline(tname,16);

        //Casting tname to tname of 15 characters.
        for(int i=0;i<strlen(tname);i++)              
        {                                         
            if(tname[i]==' ')
                tname[i]='0';
        }
        for(int i=strlen(tname);i<16;i++)
            tname[i]='0';
        tname[16]='\0';

        def_no_seats();        
        // def_train_route();
        return 0;
    }
    
    int get_all_trains()
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
        return 0;
    }

    int get_train_details()
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
        train_det.seekg(0);
        while(!train_det.eof())
        {
            train_det>>search;
            if(strcmp(search.c_str(),stno)==0)
            {
                tellpoint=train_det.tellg();
                break;
            }
        }
        train_det.close();

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
            return -1;
        }
        return 0;
    }

    int update_train_details()             //*This & associated functions working good
    {
        int tellpoint;
        int mech;
        int search;
        int flag=-1;
        int stno;
        int newSeat;
        train_det.open("trainDetails.txt",ios::in|ios::binary);
        fcheck();
        // cin.ignore(numeric_limits<streamsize>::max(),'\n');      //uncomment it later 
        system("clear");
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
        train_det.close();
        if(flag==-1)
        {
            cerr<<"\n \033[1;31m Error! Train not found!\033[0m";
            sleep(1);
            return -1;
        }
        system("clear");
        do
        {
            cout<<"\n What do you want to update? "<<endl;
            cout<<" 1. Train No. "<<endl;
            cout<<" 2. Train Name "<<endl;
            cout<<" 3. No. of Seats SL "<<endl;
            cout<<" 4. No. of Seats in 3A "<<endl;
            cout<<" 5. No. of Seats in 2A "<<endl;
            cout<<" 6. No. of seats in 1A "<<endl;
            cout<<"\033[1;32m 7. Return to ADMIN Menu \033[0m"<<endl;
            cout<<" Enter your choice: "; cin>>mech;

            switch(mech)
            {
                case 1: update_train_no(flag);
                        break;
                case 2: update_train_name(flag);
                        break;
                case 3: // 1
                        cout<<"\n Enter new no. of seats(<100): ";
                        cin>>newSeat;
                        update_no_seats(flag,1,newSeat);
                        break;
                case 4: //2
                        cout<<"\n Enter new no. of seats(<100): ";
                        cin>>newSeat;
                        update_no_seats(flag,2,newSeat);
                        break;
                case 5: //3
                        cout<<"\n Enter new no. of seats(<100): ";
                        cin>>newSeat;
                        update_no_seats(flag,3,newSeat);
                        break;
                case 6: //4
                        cout<<"\n Enter new no. of seats(<100): ";
                        cin>>newSeat;
                        update_no_seats(flag,4,newSeat);
                        break;
                case 7: //Return back to ADMIN menu
                        return -1;
                default: cerr<<"\n\033[1;31m Please select a correct choice! \033[0m";
            }
        }while(mech<1 || mech>6);

        return 0;
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
        int retval;
        do
        {
            system("clear");
            cout<<"\n \033[1;32m ADMIN MODE\033[0m"<<endl;
            cout<<"Enter your id: "; cin>>adminID;
            ignore;
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
                    case 1: retval=def_train_det();            //* Done
                            break;
                    case 2: retval=update_train_details();     //* Done
                            break;
                    case 3: retval=get_all_trains();           //* Done
                            break;
                    case 4: retval=get_train_details();        //* Done
                            break;
                    case 5: retval=change_pass();              //* Done
                            break;
                    case 6: return;
                    default:cerr<<"\n \033[1;31mEnter correct choice\033[0m";
                            sleep(1);
                }
            }while(adch<1 || adch>6 || retval==-1);
        }
    }

    void gen_PNR()
    {
        srand(time(0));
        pnr=rand()%1000000;
    }

    void castToSize(char str[],int size)
    {
        for(int i=0;i<strlen(str);i++)              
        {                                         
            if(str[i]==' ')
                str[i]='0';
        }
        for(int i=strlen(str);i<size;i++)
            str[i]='0';
        str[size]='\0';
    }
  
    void ticket()
    {
        int flag=-1;
        int i;

        while(flag==-1)
        {      
            system("clear");
            cout<<"\n Welcome to ticket booking section";
            cout<<"\n Enter train no. you want to book ticket for: ";
            cin>>tno;
            train_det.open("trainDetails.txt",ios::in|ios::binary);
            if(!train_det.is_open())
            {
                cerr<<"\n \033[1;31mError getting train details\033[0m";
                exit(-1);
            }
            train_det.seekg(0);
            int stno;
            for(i=0;i<noTrain;i++)
            {
                train_det.seekg(35*i);
                train_det>>stno;
                if(stno==tno)
                {
                    flag=1;
                    break;
                }
            }
            train_det.close();
            
            if(flag==-1)
            {
                cerr<<"\n \033[1;31mCan't find train no.! Re-enter\033[0m";
                sleep(1);
            }
            else
            {
                fstream fout;
                gen_PNR();
                fout.open("ticket.txt",ios::out|ios::binary|ios::app);      // add ios::app later
                fout<<pnr<<" "<<tno<<" ";
                
                ignore;
                cout<<"From: "; 
                cin.getline(from,8);                    
                castToSize(from,8);
                fout<<from<<" ";

                cout<<"To: ";
                cin.getline(to,8);
                castToSize(to,8);
                fout<<to<<" ";
                fout.close();
                
                //TODO: check whether from and to are in tno route or not
                train_det.open("trainDetails.txt",ios::in|ios::binary);
                fcheck();
                train_det.seekg(35*i + 23);
                cout<<"Available Seats "<<endl;
                cout<<"SL 3A 2A 1A";
                char getd[12];
                train_det.getline(getd,12);
                cout<<"\n"<<getd<<endl;
                train_det.close();
                
                char name[30];
                int age;
                
                cout<<"\n \033[1;33m A maximum of six seats are allowed per ticket \033[0m"<<endl;

                do
                {  
                    cout<<" Enter no. of seats in SL: "; cin>>SL;
                    if(SL>6)
                    {
                        cerr<<"\033[1;31m Denied! No. of seats exceeded the max. limit per ticekt \033[0m"<<endl;
                        sleep(1);
                        system("clear");
                    }
                }while(SL>6);

                for(int j=0;j<SL;j++)
                {   
                    ignore;
                    fout.open("ticket.txt",ios::out|ios::binary|ios::app);
                    fout<<"SL"<<j+1<<":- ";
                    cout<<"\n Enter name of traveller "<<j+1<<": "; 
                    cin.getline(name,30); 
                    castToSize(name,30);
                    fout<<name<<" ";
                    cout<<"\n Enter age of traveller "<<j+1<<": "; cin>>age;  fout<<age<<" ";
                    fout.close();
                    update_no_seats(i,1);                    
                }
                
                
                do
                {   
                    cout<<"\n Enter no. of seats in 3A: "; cin>>A3;
                    if((SL+A3)>6)
                    {
                        cerr<<"\033[1;31m Denied! No. of seats exceeded the max. limit per ticekt \033[0m"<<endl;
                        sleep(1);
                        system("clear");
                    }
                }while((SL+A3)>6);
                
                for(int j=0;j<A3;j++)
                {
                    ignore;
                    fout.open("ticket.txt",ios::out|ios::binary|ios::app);
                    fout<<"3A"<<j+1<<":- ";
                    cout<<"\n Enter name of traveller "<<j+1<<": "; 
                    cin.getline(name,30); 
                    castToSize(name,30);
                    fout<<name<<" ";
                    cout<<"\n Enter age of traveller "<<j+1<<": "; cin>>age; fout<<age<<" ";
                    fout.close();
                    update_no_seats(i,2);                    
                }
                
                
                do
                {   
                    cout<<"\n Enter no. of seats in 2A: "; cin>>A2;
                    if((SL+A3+A2)>6)
                    {
                        cerr<<"\033[1;31m Denied! No. of seats exceeded the max. limit per ticekt \033[0m"<<endl;
                        sleep(1);
                        system("clear");
                    }
                }while((SL+A3+A2)>6);
                
                for(int j=0;j<A2;j++)
                {
                    ignore;
                    fout.open("ticket.txt",ios::out|ios::binary|ios::app);
                    fout<<"2A"<<j+1<<":- ";
                    cout<<"\n Enter name of traveller "<<j+1<<": "; 
                    cin.getline(name,30);
                    castToSize(name,30);
                    fout<<name<<" ";
                    cout<<"\n Enter age of traveller "<<j+1<<": "; cin>>age; fout<<age<<" ";
                    fout.close();
                    update_no_seats(i,3);
                }

                
                do
                {   
                    cout<<"\n Enter no. of seats in 1A: "; cin>>A1;
                    if((SL+A2+A3+A1)>6)
                    {
                        cerr<<"\033[1;31m Denied! No. of seats exceeded the max. limit per ticekt \033[0m"<<endl;
                        sleep(1);
                        system("clear");
                    }
                }while((SL+A3+A2+A1)>6);
                
                for(int j=0;j<A1;j++)
                {
                    ignore;
                    fout.open("ticket.txt",ios::out|ios::binary|ios::app);
                    fout<<"1A"<<j+1<<":- ";
                    cout<<"\n Enter name of traveller "<<j+1<<": "; 
                    cin.getline(name,30); 
                    castToSize(name,30);
                    fout<<name<<" ";
                    cout<<"\n Enter age of traveller "<<j+1<<": "; cin>>age; fout<<age<<" ";
                    fout.close();
                    update_no_seats(i,4);
                }               //*/
            }
        }
    }
    void PNR();
    void fare();
    void user()
    {
        int usch;
        do
            {
                system("clear");
                cout<<"\n \033[1;32m USER MENU\033[0m"<<endl;
                cout<<" 1. Get all train details"<<endl;
                cout<<" 2. Get particular train details"<<endl;
                cout<<" 3. Book a ticket "<<endl;
                cout<<" 4. PNR Status "<<endl;
                cout<<" 5. Get fare estimate"<<endl;
                cout<<" 6. Exit"<<endl; 
                cout<<" Enter your choice: "<<"\033[1;32m"; cin>>usch;
                cout<<"\033[0m";
                switch(usch)
                {
                    case 1: get_all_trains();            //* Done
                            break;
                    case 2: get_train_details();     
                            break;
                    case 3: ticket();           //! Under construction
                            break;
                    case 4: PNR();        //! Under construction
                            break;
                    case 5: fare();         //! Under Construction
                            break;
                    case 6: return;
                    default:cerr<<"\n \033[1;31mEnter correct choice\033[0m";
                            sleep(1);
                }
            }while(usch<1 || usch>6);
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
    obj.ticket(); 

    /*
    fstream fin;
    string str;
    fin.open("trainDetails.txt",ios::in|ios::binary);
    fin>>str;
    cout<<endl<<str;
    */

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