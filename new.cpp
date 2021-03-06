//Inheritence working fine
//TODO: Get all quantities a check for which acc. value>0
//TODO: ADD date of journey to Ticket
//TODO: Add Admin option to edit fare // use slabs to do so
//TODO: Line 1617 PNR()
#include"sock_macro.cpp"
#include<time.h>
#include<math.h>

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
    char from[10];
    char to[10];
    ui SL,A3,A2,A1;
    int mech;                           //Menu choice
    int noStation;
    int adminID;
    int dist;
    ui cost;
    string adminPass;
    char rStat[10];
    fstream train_det;
    int pnr;
    int slf[10],a3f[10],a2f[10],a1f[10];
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

    void print_to_stream()
    {
        train_det.open("trainDetails.txt",ios::out|ios::binary|ios::app);
        fcheck();
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

    int update_train_details()             //*This & associated functions working good
    {
        int tellpoint;
        int mech;
        int search;
        int flag=-1;
        int tno;
        int newSeat;
        train_det.open("trainDetails.txt",ios::in|ios::binary);
        fcheck();
        // cin.ignore(numeric_limits<streamsize>::max(),'\n');      //uncomment it later 
        system("clear");
        cout<<"\n Enter train no.: "; cin>>tno;
        train_det.seekg(0);
        for(int i=0;i<noTrain;i++)
        {
            train_det.seekg(i*35);
            train_det>>search;
            if(tno==search)
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
            cout<<" 7. Train Route "<<endl;
            cout<<"\033[1;32m 8. Return to ADMIN Menu \033[0m"<<endl;
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
                case 7: update_train_route(tno);
                        break;
                case 8: //Return back to ADMIN menu
                        return -1;
                default: cerr<<"\n\033[1;31m Please select a correct choice! \033[0m";
            }
        }while(mech<1 || mech>8);

        return 0;
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
        castToSize(tname,16);

        char A[currSeek+6];
        string C;
        train_det.open("trainDetails.txt",ios::in|ios::binary);
        train_det.seekg(0);
        train_det.getline(A,(currSeek+6));
        train_det.close();

        train_det.open("trainDetails.txt",ios::in|ios::binary);
        train_det.seekg(currSeek+23);
        getline(train_det,C);
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
                    if(noSeat<10)
                        train_det<<X1<<" 0"<<noSeat<<" "<<C;
                    else
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
                    if(noSeat<10)
                        train_det<<X2<<" 0"<<noSeat<<" "<<C;
                    else
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
                    if(noSeat<10)
                        train_det<<X3<<" 0"<<noSeat<<" "<<C;
                    else
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
                    if(noSeat<10)
                        train_det<<X4<<" 0"<<noSeat<<" "<<C;
                    else
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
                    if(noSeat<10)
                        train_det<<X1<<" 0"<<noSeat<<" "<<C;
                    else
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
                    if(noSeat<10)
                        train_det<<X2<<" 0"<<noSeat<<" "<<C;
                    else
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
                    if(noSeat<10)
                        train_det<<X3<<" 0"<<noSeat<<" "<<C;
                    else
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
                    if(noSeat<10)
                        train_det<<X4<<" 0"<<noSeat<<" "<<C;
                    else
                        train_det<<X4<<" "<<noSeat<<" "<<C;
                    train_det.close();

                    break;
            default: cerr<<"\n \033[1;31mCan't Happen! Contact DEV! \033[0m";
                    exit(0);
        }        
    }

    void update_train_route(int tno)
    {
        ifstream troute;
        ofstream tout;
        int location,nost;
        string search;
        troute.open("distance.txt",ios::binary);
        while(!troute.eof())
        {
            troute>>search;
            if(search==to_string(tno))
                break;
        }
        location=troute.tellg();
        troute>>nost;
        location++;
        char A[location];
        troute.seekg(0);
        troute.getline(A,location);
        troute.close();

        troute.open("distance.txt",ios::binary);
        troute.seekg(location+3+(15*nost));
        string C;
        getline(troute,C);
        troute.close();
        tout.open("distance.txt",ios::binary);
        tout<<A<<" ";
        tout.close();

        cout<<"\n Enter no. of stations(including SRC and DEST): "; cin>>noStation; 
        train_det.open("distance.txt",ios::out|ios::binary|ios::app);
        if(noStation<10)
            train_det<<"0"<<noStation<<" ";
        else if(noStation<100)
            train_det<<noStation<<" ";
        else
        {
            cerr<<"\033[1;31m Itni lambi train kon chalata hai bhai! \033[0m"<<endl;
            exit(-1);
        }
        train_det.close();
        
        for(int i=0;i<noStation;i++)
        {
            ignore;
            cout<<"\n Enter station name "<<i+1<<": ";
            cin.getline(rStat,10);
            for(int j=0;rStat[j]!='\0';j++) 
                rStat[j]=toupper(rStat[j]);
            castToSize(rStat,10);
            train_det.open("distance.txt",ios::out|ios::app|ios::binary);
            train_det<<rStat<<" ";
            train_det.close();

            cout<<" Enter distance from source: ";
            cin>>dist;
            dist=abs(dist);
            train_det.open("distance.txt",ios::out|ios::app|ios::binary);
            if(dist<10)
                train_det<<"00"<<dist<<" ";
            else if(dist<100)
                train_det<<"0"<<dist<<" ";
            else if(dist<1000)
                train_det<<dist<<" ";
            else
                cerr<<"\033[1;31m Large distance error xD \033[0m"<<endl;
            train_det.close();
        }

        train_det.open("distance.txt",ios::out|ios::app|ios::binary);
        train_det<<C;
        train_det.close();   //*/

    }

    

    void gen_PNR()
    {
        srand(time(0));
        pnr=rand()%900000+100000;
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

    int seatCheck(int tno,int coach)
    {
        ifstream fin;
        fin.open("trainDetails.txt",ios::binary);
        string stno;
        while(!fin.eof())
        {
            fin>>stno;
            if(stno==to_string(tno))
            break;
        }
        int location;
        location=fin.tellg();
        location+=18;
        int noSeat;
        switch(coach)
        {
            case 1: fin.seekg(location);
                    fin>>noSeat;
                    break;
            case 2: fin.seekg(location+3);
                    fin>>noSeat;
                    break;
            case 3: fin.seekg(location+3+3);
                    fin>>noSeat;
                    break;
            case 4: fin.seekg(location+3+3+3);
                    fin>>noSeat;
                    break;
            default: cout<<"\033[1;31m Lite \033[0m";
        }
        fin.close();

        return noSeat;
    }

    int delete_train()
    {
        system("clear");

        char choice;
        cout<<"Enter train no. whose details you want to wipe out: ";
        cin>>tno;
        do
        {
            cout<<"Are you sure you want to wipe details of "<<tno<<"? (y/n) ";
            cin>>choice;
            choice=toupper(choice);
            if(choice=='Y' || choice=='N')
                break;
            else
            {
                cerr<<" \033[;131mEnter correct choice\033[0m";
                system("clear");
            }
        }while(choice!='Y' || choice!='N');
        if(choice=='Y')
        {
            ifstream fin;
            fin.open("trainDetails.txt",ios::binary);
            string stno;
            while(!fin.eof())
            {
                fin>>stno;
                if(stno==to_string(tno))
                    break;
            }
            int location;
            location=fin.tellg();
            location-=5;
            char A[location];
            fin.seekg(0);
            fin.getline(A,location);
            fin.close();

            fin.open("trainDetails.txt",ios::binary);
            fin.seekg(location+35);
            string C;
            getline(fin,C);
            fin.close();
            
            ofstream fout;
            fout.open("trainDetails.txt",ios::binary);
            fout<<A<<" "<<C;
            fout.close();
        }
    
        return 0;
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

        // * Init fare
        ifstream fare;
        fare.open("fare.txt",ios::binary);
        
        int i=0;
        //* Get SL fare
        while(!fare.eof())
        {
            fare>>slf[i];
            i++;
            if(i==10)
                break;
        }

        i=0;
        //* Get 3A fare
        while(!fare.eof())
        {
            fare>>a3f[i];
            i++;
            if(i==10)
                break;
        }
        
        i=0;
        //* Get 2A fare
        while(!fare.eof())
        {
            fare>>a2f[i];
            i++;
            if(i==10)
                break;
        }
        
        i=0;
        //* Get 1A fare
        while(!fare.eof())
        {
            fare>>a1f[i];
            i++;
            if(i==10)
                break;
        }
    }
    
    int update_fare()
    {
        int uFare,ch,slab;
        do
        {
            system("clear");
            cout<<" Which class' fare do you wish to update? "<<endl;
            cout<<" 1. SL\n 2. 3A\n 3. 2A\n 4. 1A "<<endl;
            cout<<" Enter your choice: "; cin>>ch;
            if(ch<1 || ch>4)
            {
                cerr<<"\033 [1;31m No such coach option \033[0m";
                sleep(1);
            }
        }while(ch<1 || ch>4);
            
        system("clear");
        cout<<" Every 100 km accounts for a slab. "<<endl;;
        cout<<" \033[1;32mSlab 1:\033[0m 0-99km\n \033[1;32mSlab 2:\033[0m 100-199km \n \033[1;32mSlab 3:\033[0m 200-299km \n .\n . \n . \n\033[1;32m Slab 10:\033[0m 1000 km"<<endl;
        cout<<"\n Enter the slab no. whose fare you wish to change: ";
        cin>>slab;
        if(slab<1 || slab>10)
        {
            cout<<"\033[1;31m No such slab \033[0m";
            return -1;
        }    

        slab--;
        fstream fare,fout;
        string C;
        int location;

        //TODO: stack dump.exe created don't know why! Check data type conversions
        //TODO: Check step by step8
        switch(ch)
        {
            case 1://SL
                    cout<<" Current Fare per seat: "<<slf[slab]<<endl;
                    cout<<" Enter updated fare per seat: ";
                    cin>>uFare;
                    uFare=abs(uFare);
                    slf[slab]=uFare;
                    break;
            case 2://3A
                    cout<<" Current Fare per seat: "<<a3f[slab]<<endl;
                    cout<<" Enter updated fare per seat: ";
                    cin>>uFare;
                    uFare=abs(uFare);
                    a3f[slab]=uFare;
                    break;
            case 3://2A
                    cout<<" Current Fare per seat: "<<a2f[slab]<<endl;
                    cout<<" Enter updated fare per seat: ";
                    cin>>uFare;
                    uFare=abs(uFare);
                    a2f[slab]=uFare;
                    break;
            case 4://1A
                    cout<<" Current Fare per seat: "<<a1f[slab]<<endl;
                    cout<<" Enter updated fare per seat: ";
                    cin>>uFare;
                    uFare=abs(uFare);
                    a1f[slab]=uFare;
                    break;
        }

        fare.open("fare.txt",ios::in|ios::binary);
        location=((ch-1)*50)+(slab*5);
        char A[location];
        fare.getline(A,location);
        fare.close();

        fare.open("fare.txt",ios::in|ios::binary);
        fare.seekg(location+5);
        getline(fare,C);
        fare.close();

        fout.open("fare.txt",ios::out|ios::binary);
        if(uFare<1000)
            fout<<A<<" 0"<<uFare<<" "<<C;
        else if(uFare<10000)
            fout<<A<<" "<<uFare<<" "<<C;
        fout.close();
        cout<<"\n\033[1;32m Updated! \033[0m"<<endl;
            // */
        return 0;
    }

    void print()
    {
        cout<<"SL\t3A\t2A\t1A"<<endl;
        for(int i=0;i<10;i++)
        {
            cout<<slf[i]<<"\t"<<a3f[i]<<"\t"<<a2f[i]<<"\t"<<a1f[i]<<endl;
        }
    }
    void def_train_route(int tno)      //! Admin 
    {
        train_det.open("distance.txt",ios::out|ios::binary|ios::app);
        train_det<<tno<<" ";
        train_det.close();

        cout<<"\n Enter no. of stations(including SRC and DEST): "; cin>>noStation; 
        train_det.open("distance.txt",ios::out|ios::binary|ios::app);
        if(noStation<10)
            train_det<<"0"<<noStation<<" ";
        else if(noStation<100)
            train_det<<noStation<<" ";
        else
            cerr<<"\033[1;31m Itni lambi train kon chalata hai bhai! \033[0m"<<endl;
        train_det.close();

        for(int i=0;i<noStation;i++)
        {
            ignore;
            cout<<"\n Enter station name "<<i+1<<": ";
            cin.getline(rStat,10);
            for(int j=0;rStat[j]!='\0';j++) 
                rStat[j]=toupper(rStat[j]);
            castToSize(rStat,10);
            train_det.open("distance.txt",ios::out|ios::app|ios::binary);
            train_det<<rStat<<" ";
            train_det.close();

            cout<<" Enter distance from source: ";
            cin>>dist;
            dist=abs(dist);
            train_det.open("distance.txt",ios::out|ios::app|ios::binary);
            if(dist<10)
                train_det<<"00"<<dist<<" ";
            else if(dist<100)
                train_det<<"0"<<dist<<" ";
            else if(dist<1000)
                train_det<<dist<<" ";
            else
                cerr<<"\033[1;31m Large distance error xD \033[0m"<<endl;
            train_det.close();
        }
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
        castToSize(tname,16);

        def_no_seats();        
        def_train_route(tno);
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
        ifstream troute;
        char ttname[17];
        char tnoseat[13];
        
        //Search operation
        int tno,tellpoint,flag=-1;
        string search;
        cout<<"\n Enter train no. whose details are to be searched: "; cin>>tno;
        
        train_det.open("trainDetails.txt",ios::in|ios::binary);
        fcheck();
        while(!train_det.eof())
        {
            train_det>>search;
            if(search==to_string(tno))
            {
                tellpoint=train_det.tellg();
                flag=1;
                break;
            }
        }
        train_det.close();

        if(flag==1)
        {   
            int searchpoint=tellpoint-5;                    //? Will comeback to it later
            cout<<"Train no: "<<tno<<endl;

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
        
            string rtno;
            char rt[11];
            int location,nost;
            troute.open("distance.txt",ios::binary);
            while(!troute.eof())
            {
                troute>>rtno;
                if(rtno==to_string(tno))
                {
                    location=troute.tellg();
                    break;
                }   
            }

            location++;
            troute>>nost;
            location+=2;
            troute.close();
            cout<<"\033[1;34m ** TRAIN ROUTE ** \033[0m"<<endl;
            for(int i=0;i<nost;i++)
            {
                troute.open("distance.txt",ios::binary);
                troute.seekg(location +(15*i));
                troute>>rt;
                for(int k=0;rt[k]!='\0';k++)
                {
                    if(rt[k]=='0')
                        rt[k]=' ';
                }
                cout<<rt<<endl;
                troute.close();
            }

            troute.close();

        }
        else
        {
            cerr<<"\033[1;31mTrain details not found!\033[0m"<<endl;
            return -1;
        }
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
                cout<<" 5. Delete train details \033[1;31m (Use with CAUTION!!!) \033[0m"<<endl;
                cout<<" 6. Change Password"<<endl;
                cout<<" 7. Update fare"<<endl;
                cout<<" 8. Exit"<<endl; 
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
                    case 5: retval=delete_train();             //* Done
                            break;
                    case 6: retval=change_pass();              //* Done
                            break;
                    case 7: retval=update_fare();
                            break;
                    case 8: return;
                    default:cerr<<"\n \033[1;31mEnter correct choice\033[0m";
                            sleep(1);
                }
            }while(adch<1 || adch>7 || retval==-1);
        }
    }
  
    int check(int tno,char st[])
    {
        ifstream fin;
        fin.open("distance.txt",ios::binary);
        string stno;
        while(!fin.eof())
        {
            fin>>stno;
            if(stno==to_string(tno))
            {
                break;
            }
        }
        int location;
        location=fin.tellg();
        int nost;
        fin.seekg(location);
        fin>>nost;
        location=fin.tellg();
        fin.close();
        char stRoute[11];
        int prlocation;
        location+=1;
        for(int i=0;i<nost;i++)
        {
            fin.open("distance.txt",ios::binary);
            fin.seekg(location+(15*i));
            fin.getline(stRoute,11);
            if(strcmp(stRoute,st)==0)
            {
                return 0;
            }
            fin.close();
        }
        fin.close();
        return -1;
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
            fcheck();
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
                ofstream fout;
                fstream fin;
                int noticket=0,totFare=0;
                int d1,d2,d;
                string search;
                re:
                fin.open("ticket.txt",ios::binary|ios::in);
                fin>>noticket;
                noticket++;

                string c;
                fin.seekg(4);
                getline(fin,c);
                fin.close();
                fout.open("ticket.txt",ios::binary);
                fout.seekp(0);
                if(noticket<10)
                    fout<<"00"<<noticket<<" "<<c;       //Not added a " " after c as c itself contains " " at end
                else if(noticket<100)
                    fout<<"0"<<noticket<<" "<<c;
                else if(noticket<1000)
                    fout<<noticket<<" "<<c;
                else
                {
                    cerr<<"\033[1;31m Stack overloaded, resetting ticket.txt"<<endl;
                    sleep(1);
                    system("rm ticket.txt");
                    goto re;
                }

                gen_PNR();
                //*PNR and TNO
                int flag1=-1,location;
                fout<<pnr<<" "<<tno<<" ";
                ignore;

                //*FROM
                while(flag1==-1)
                {
                    cout<<"From: "; 
                    cin.getline(from,10);                    
                    for(int i=0;from[i]!='\0';i++)
                        from[i]=toupper(from[i]);
                    castToSize(from,10);
                    

                    if(check(tno,from)==-1)
                    {
                        cerr<<"\033[1;31m Station not found in the route! Re-enter \033[0m"<<endl;
                        flag1=-1;
                        sleep(1);
                        system("clear");
                    }
                    else
                        flag1=0;
                }

                fout<<from<<" ";
              
                //* From distance
                fin.open("distance.txt",ios::in|ios::binary);
                while(!fin.eof())
                {
                    fin>>search;
                    if(search==to_string(tno))
                        break;
                }
                location=fin.tellg();
                fin.close();
              
                fin.open("distance.txt",ios::in|ios::binary);
                fin.seekg(location);
                while(!fin.eof())
                {
                    fin>>search;
                    if(search==from)
                        break;
                }
                fin>>d1;
                fin.close();

                flag1=-1;

                //*To
                while(flag1==-1)
                {
                    cout<<"To: "; 
                    cin.getline(to,10);  
                    for(int i=0;to[i]!='\0';i++)
                        to[i]=toupper(to[i]);                  
                    castToSize(to,10);
                    if(check(tno,to)==-1)
                    {
                        cerr<<"\033[1;31m Station not found in the route! Re-enter \033[0m"<<endl;
                        flag1=-1;
                        sleep(1);
                        system("clear");
                    }
                    else
                        flag1=0;
                }
                fout<<to<<" ";
                fout.close();

                //*To distance
                fin.open("distance.txt",ios::in|ios::binary);
                fin.seekg(location);
                while(!fin.eof())
                {
                    fin>>search;
                    if(search==to)
                        break;
                }
                fin>>d2;
                
                //*Distance
                d=d1-d2;
                d=abs(d);
                fin.close();
                
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
                char sex;
                int nopsgr=0;
            
                cout<<"\n \033[1;33m A maximum of \033[1;31m6\033[1;33m seats are allowed per ticket \033[0m"<<endl;
                
                int avSeat;
                fout.open("ticket.txt",ios::binary|ios::app);
                fout<<" ";                  //Was necessarry
                fout.close();
                //* SL
                do
                {  
                    cout<<" Enter no. of seats in SL: "; cin>>SL;
                    avSeat=seatCheck(tno,1);
                    if(avSeat<SL)
                    {
                        cout<<"\033[1;31m Sorry! Only "<<"\033[1;32m"<<avSeat<<" seats\033[1;31m available! \033[0m"<<endl;
                        sleep(1);
                        system("clear");
                    }

                    if(SL > (6-nopsgr))
                    {
                        cerr<<"\033[1;31m Denied! No. of seats exceeded the no. of passangers \033[0m"<<endl;
                        sleep(1);
                        system("clear");
                    }
                }while(SL>6 || avSeat<SL);

                for(int j=0;j<SL;j++)
                {  
                    totFare+=fare(1,d);       //to change this
                    nopsgr++;
                    ignore;
                    fout.open("ticket.txt",ios::binary|ios::app);
                    fout<<"SL"<<j+1<<":- ";
                    cout<<"\n Enter name (<30 char.) of traveller "<<j+1<<": "; 
                    cin.getline(name,30); 
                    castToSize(name,30);
                    fout<<name<<" ";
                    cout<<" Enter age of traveller "<<j+1<<": "; cin>>age;  fout<<age<<" ";
                    do
                    {
                        cout<<" Enter the sex of Passenger(M/F): "; cin>>sex; 
                        sex=toupper(sex); 
                        if(sex=='M' || sex=='F')
                            break;
                        else
                        {
                            cerr<<"\033[1;31m Enter M or F!! \033[0m"<<endl;
                            sleep(1);
                            system("clear");
                        }
                    }while(1);
                    fout<<sex<<" ";
                    fout.close();
                    update_no_seats(i,1);                    
                }
                
                //* 3A
                do
                {   
                    cout<<"\n Enter no. of seats in 3A: "; cin>>A3;
                    avSeat=seatCheck(tno,2);
                    if(avSeat<A3)
                    {
                        cout<<"\033[1;31m Sorry! Only "<<"\033[1;32m"<<avSeat<<" seats\033[1;31m available! \033[0m"<<endl;
                        sleep(1);
                        system("clear");
                    }
                    if(A3>(6-nopsgr))
                    {
                        cerr<<"\033[1;31m Denied! No. of seats exceeded the no. of passangers \033[0m"<<endl;
                        sleep(1);
                        system("clear");
                    }
                }while(A3>(6-nopsgr) || avSeat<A3);
                
                for(int j=0;j<A3;j++)
                {
                    totFare+=fare(2,d);
                    nopsgr++;
                    ignore;
                    fout.open("ticket.txt",ios::binary|ios::app);
                    fout<<"3A"<<j+1<<":- ";
                    cout<<" Enter name of traveller "<<j+1<<": "; 
                    cin.getline(name,30); 
                    castToSize(name,30);
                    fout<<name<<" ";
                    cout<<" Enter age of traveller "<<j+1<<": "; cin>>age; fout<<age<<" ";
                    do
                    {
                        cout<<" Enter the sex of Passenger(M/F): "; cin>>sex; 
                        sex=toupper(sex); 
                        if(sex=='M' || sex=='F')
                            break;
                        else
                        {
                            cerr<<"\033[1;31m Enter M or F!! \033[0m"<<endl;
                            sleep(1);
                            system("clear");
                        }
                    }while(1);
                    fout<<sex<<" ";
                    fout.close();
                    update_no_seats(i,2);                    
                }
                
                //* 2A
                do
                {   
                    cout<<"\n Enter no. of seats in 2A: "; cin>>A2;
                    avSeat=seatCheck(tno,3);
                    if(avSeat<A2)
                    {
                        cout<<"\033[1;31m Sorry! Only "<<"\033[1;32m"<<avSeat<<" seats \033[1;31m available! \033[0m"<<endl;
                        sleep(1);
                        system("clear");
                    }
                    if(A2>(6-nopsgr))
                    {
                        cerr<<"\033[1;31m Denied! No. of seats exceeded the no. of passangers \033[0m"<<endl;
                        sleep(1);
                        system("clear");
                    }
                }while(A2>(6-nopsgr) || avSeat<A2);
                
                for(int j=0;j<A2;j++)
                {
                    totFare+=fare(3,d);
                    nopsgr++;
                    ignore;
                    fout.open("ticket.txt",ios::binary|ios::app);
                    fout<<"2A"<<j+1<<":- ";
                    cout<<" Enter name of traveller "<<j+1<<": "; 
                    cin.getline(name,30);
                    castToSize(name,30);
                    fout<<name<<" ";
                    cout<<" Enter age of traveller "<<j+1<<": "; cin>>age; fout<<age<<" ";
                    do
                    {
                        cout<<" Enter the sex of Passenger(M/F): "; cin>>sex; 
                        sex=toupper(sex); 
                        if(sex=='M' || sex=='F')
                            break;
                        else
                        {
                            cerr<<"\033[1;31m Enter M or F!! \033[0m"<<endl;
                            sleep(1);
                            system("clear");
                        }
                    }while(1);
                    fout<<sex<<" ";
                    fout.close();
                    update_no_seats(i,3);
                }

                //* 1A
                do
                {   
                    cout<<"\n Enter no. of seats in 1A: "; cin>>A1;
                    avSeat=seatCheck(tno,4);
                    if(avSeat<A1)
                    {
                        cout<<"\033[1;31m Sorry! Only "<<"\033[1;32m"<<avSeat<<" seats \033[1;31m available! \033[0m"<<endl;
                        sleep(1);
                        system("clear");
                    }
                    if(A1>(6-nopsgr))
                    {
                        cerr<<"\033[1;31m Denied! No. of seats exceeded the no. of passangers \033[0m"<<endl;
                        sleep(1);
                        system("clear");
                    }
                }while(A1>(6-nopsgr) || avSeat<A1);
                
                for(int j=0;j<A1;j++)
                {
                    totFare+=fare(4,d);
                    nopsgr++;
                    ignore;
                    fout.open("ticket.txt",ios::binary|ios::app);
                    fout<<"1A"<<j+1<<":- ";
                    cout<<" Enter name of traveller "<<j+1<<": "; 
                    cin.getline(name,30); 
                    castToSize(name,30);
                    fout<<name<<" ";
                    cout<<" Enter age of traveller "<<j+1<<": "; cin>>age; fout<<age<<" ";
                    do
                    {
                        cout<<" Enter the sex of Passenger(M/F): "; cin>>sex; 
                        sex=toupper(sex); 
                        if(sex=='M' || sex=='F')
                            break;
                        else
                        {
                            cerr<<"\033[1;31m Enter M or F!! \033[0m"<<endl;
                            sleep(1);
                            system("clear");
                        }
                    }while(1);
                    fout<<sex<<" ";
                    fout.close();
                    update_no_seats(i,4);
                }     

                //*Distance 
                fout.open("ticket.txt",ios::binary|ios::app);          
                if(d<10)
                    fout<<"00"<<d<<" ";
                else if(d<100)
                    fout<<"0"<<d<<" ";
                else if(d<1000)
                    fout<<d<<" ";

                //*Fare
                if(totFare<1000)
                    fout<<"00"<<totFare<<" ";
                else if(totFare<10000)
                    fout<<"0"<<totFare<<" ";
                else if(totFare<100000)
                    fout<<totFare<<" ";

                fout.close();

                // cout<<"\n "<<totFare<<endl;
                //? TODO: Ask for date of travel

                //* Updation of ticket.txt with nopsgr  
                fin.open("ticket.txt",ios::in|ios::binary);
                string searchpnr;
                location=0;
                string pnrString;
                pnrString=to_string(pnr);

                while(!fin.eof())
                { 
                    fin>>searchpnr;
                    if(searchpnr==pnrString)
                        break;
                }      
                location=fin.tellg();
                int writeLocation=location+29;
                char A[writeLocation];
                fin.seekg(0);
                fin.getline(A,writeLocation);
                fin.close();
                
                fin.open("ticket.txt",ios::in|ios::binary);
                fin.seekg(writeLocation+1);
                string C;
                getline(fin,C);
                fin.close();
                
                fout.open("ticket.txt",ios::binary);
                fout<<A<<" "<<nopsgr<<" "<<C;
                fout.close();      //*/
            }
        }
    }

    void PNR()
    {
        system("clear");
        cout<<" Enter your PNR no.: ";  cin>>pnr;
        string strPNR;
        strPNR=to_string(pnr);
        string spnr;
        ifstream fin;
        int location=0;
        int flag=-1;
        char sex1[2];
        fin.open("ticket.txt",ios::binary);
        while(!fin.eof())
        {
            fin>>spnr;
            if(spnr==strPNR)
            {
                flag=0;
                break;
            }
        }

        if(flag==-1)
        {
            cerr<<"\n \033[1;31mCan't find PNR no. !\033[0m"<<endl;
            exit(-1);
        }
        else
        {
            location=fin.tellg();
            location++;             //Location is the start of TNO
            
            //*Get tno
            fin.seekg(location);
            fin>>tno;
            cout<<"Train no.: "<<tno<<endl;

            //* Get from
            fin.seekg(location+6);
            char from[11];
            fin>>from;
            cout<<"From: ";       
            for(int i=0;i<11;i++)
            {
                if(from[i]=='0')
                    cout<<" ";
                else
                    cout<<from[i];
            }
            cout<<endl;

            //*Get to
            char to[11];
            fin>>to;
            cout<<"To: ";       
            for(int i=0;i<11;i++)
            {
                if(to[i]=='0')
                    cout<<" ";
                else
                    cout<<to[i];
            }
            cout<<endl;

            //*Get no. of psgr
            int psgrno;
            fin>>psgrno;
            fin.close();

            //* Get coach
            for(int i=0;i<psgrno;i++)
            {
                fin.open("ticket.txt",ios::binary|ios::in);
                fin.seekg(location+30+(42*i));
                char coach[4];
                fin.getline(coach,4);
                cout<<coach<<"\t\t\t\t\t";
                fin.close();
            }
            cout<<endl;
 
            //* Get Name
            for(int i=0;i<psgrno;i++)
            {
                fin.open("ticket.txt",ios::binary);
                fin.seekg(location+36+(42*i));
                char name1[31];
                fin.getline(name1,31);
                for(int j=0;j<31;j++)
                {
                    if(name1[j]=='0')
                        name1[j]=' ';
                }
                cout<<"NAME: "<<name1<<"\t";
                fin.close();
            }

            cout<<endl;

            //* Get Age
            for(int i=0;i<psgrno;i++)
            {
                fin.open("ticket.txt",ios::binary);
                fin.seekg(location+67+(42*i));
                int age1;
                fin>>age1;
                cout<<"Age: "<<age1<<" yrs.\t\t\t\t";
                fin.close();
            }

            cout<<endl;

            //* Get sex
            for(int i=0;i<psgrno;i++)
            {
                fin.open("ticket.txt",ios::binary);
                fin.seekg(location+70+(42*i));
                fin>>sex1;
                cout<<"Sex: "<<sex1<<"\t\t\t\t\t";
                fin.close();
            }
            
            cout<<endl;
            //? TODO: Get date of jpurney
            //* Get Distance
            int dis;
            fin.open("ticket.txt",ios::in|ios::binary);
            fin.seekg(location+114);
            fin>>dis;
            cout<<"Distance: "<<dis<<" km"<<endl;
            
            //* Get Fare            
            int cost;
            fin.seekg(location+118);
            fin>>cost;
            cout<<"Fare: ₹"<<cost<<endl;

            fin.close();
            //*/
            

        }
    }

    int fare(int coach,int disp)
    {
        cost=0;
        int categ=disp/100;

        switch(coach)
        {
            case 1: //SL
                    cost+=slf[categ];
                    break;
            case 2: //3A
                    cost+=a3f[categ];
                    break;
            case 3: //2A
                    cost+=a2f[categ];
                    break;
            case 4: //1A
                    cost+=a1f[categ];
                    break;
            deafult: cout<<" Lite "<<endl;
        }
        return cost;
    }

    int fare()        //Polymorphism for this also
    {
        string search;
        ifstream fin;
        int location,d1,d2,d,flag=-1;
        cout<<" Enter train no.: ";
        cin>>tno;

        fin.open("distance.txt",ios::binary);
        while(!fin.eof())
        {
            fin>>search;
            if(search==to_string(tno))
            {       
                flag=0;
                break;
            }
        }
        location=fin.tellg();
        if(flag==-1)
        {
            cerr<<"\033[1;31m No train found with this train no. \033[0m";
            sleep(1);
            return -1;
        }        
        
        cout<<" From: ";
        cin>>from;
        for(int i=0;from[i]!='\0';i++)
            from[i]=toupper(from[i]);
        castToSize(from,10);

        while(!fin.eof())
        {
            fin>>search;
            if(search==from)
                break;
        }
        fin>>d1;
        fin.close();
        
        cout<<" To: ";
        cin>>to;
        for(int i=0;to[i]!='\0';i++)
            to[i]=toupper(to[i]);
        castToSize(to,10);

        fin.open("distance.txt",ios::binary);
        fin.seekg(location);
        while(!fin.eof())
        {
            fin>>search;
            if(search==to)
                break;
        }
        fin>>d2;
        fin.close();

        d=abs(d1-d2);
        cout<<" Distance: "<<d<<" km"<<endl;
        cout<<"\033[1;32m\n** FARE ESTIMATE **\033[0m"<<endl;
        cout<<"SL: "<<fare(1,d)<<endl;
        cout<<"3A: "<<fare(2,d)<<endl;
        cout<<"2A: "<<fare(3,d)<<endl;
        cout<<"1A: "<<fare(4,d)<<endl;

        return 0;
    }
    
    void user()
    {
        int usch;
        int retval;
        do
            {
                system("clear");
                cout<<"\n \033[1;32m USER MENU\033[0m"<<endl;
                cout<<" 1. Get all train details"<<endl;
                cout<<" 2. Get particular train details"<<endl;
                cout<<" 3. Book a ticket "<<endl;
                cout<<" 4. PNR Status/See Ticket "<<endl;
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
                    case 5: retval=fare();         //! Under Construction
                            break;
                    case 6: return;
                    default:cerr<<"\n \033[1;31mEnter correct choice\033[0m";
                            sleep(1);
                }
            }while(usch<1 || usch>6 || retval==-1);
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
    system("clear");
    rail obj;
    // obj.init_hint_struct();
    // if(obj.connect_to_server()==-1)
    //     exit(0);
        
    obj.menu(); 
    char a[3];
    ignore;
    // a=cin.get();
    cout<<"Password: ";
    for(int i=0;i<3;i++)
    {
        a[i]=_getch();
        system("clear");
        cout<<"Password:";
        for(int j=0;j<=i;j++)
            cout<<'*';
        
    }

    return 0;
}