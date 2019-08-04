#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#define PORT 2024

char msgrasp[10000]=" ",msg[200];      
const char *userStatus = "Nelogat";
const char *voteStatus = "Nelogat";
int i,j,k=0,l=0;
sqlite3_stmt * stmt;
sqlite3 *bd;
char *tables;
char *err_msg=0;
int bdaux,err;
char *sql;
char *tok;
const char s[2]=" ";
int argc;
char aux[5];
extern int errno;
char gen[20],nume3[100],nume2[100],comentariu[300],nume[100],descriere[300],gen1[20],gen2[20],gen3[20],linkk[50];
int nrCom, ok;
char argv[20][100];
const char *numeAux = "";

static int callback(void *data, int argc, char **argv, char **numeColoana)
{
    int i;
    for(i = 0; i<argc; i++)
    {
        strcat(msgrasp, numeColoana[i]);
        strcat(msgrasp, " = ");
        strcat(msgrasp, argv[i] ? argv[i]: " ");
        strcat(msgrasp, "\n");
        //printf("%s = %s\n", numeColoana[i], argv[i] ? argv[i] : "NULL");
    }
    strcat(msgrasp, "\n");
    return 0;
}

void inregistrare()
{
    strcat(msgrasp,"Comanda de inregistrare a fost apelata cu succes\n");
    ok=1;
    tables = "SELECT Username FROM UserInfo";
    err=sqlite3_prepare_v2(bd,tables,-1,&stmt,NULL);
    do
    {
        err=sqlite3_step(stmt);
        if(err==SQLITE_ROW && !strcmp(argv[2],sqlite3_column_text(stmt,0)))
            ok=0;
    }
    while(err==SQLITE_ROW);
    if(ok==0) strcat(msgrasp,"Username-ul ales deja exista\n");
    if(ok==1)
    {
        tables = "INSERT INTO UserInfo (Username, Password, UserStatus, VoteStatus) VALUES (?, ?, 'User', 'Yes');";
        err=sqlite3_prepare_v2(bd,tables,-1,&stmt, NULL);
        sqlite3_bind_text(stmt, 1, argv[2], -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, argv[3], -1, SQLITE_TRANSIENT);
        err=sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        strcat(msgrasp,"Inregistrare realizata cu succes.\n");
    }
}

void conturi()
{
    if(k==2) strcat(msgrasp,"Actiune interzisa\n");
    else if(k==1)
    {
        strcat(msgrasp,"Comanda de afisare a conturilor a fost apelata cu succes\n");
        sql = "SELECT * FROM UserInfo";
        bdaux=sqlite3_exec(bd,sql,callback,0,&err_msg);
    }
}

void login()
{
    strcat(msgrasp,"Comanda de login a fost apelata cu succes\n");
    ok=0;
    tables = "SELECT Username, Password, UserStatus, VoteStatus FROM UserInfo";
    err=sqlite3_prepare_v2(bd,tables,-1,&stmt,NULL);
    do
    {
        err=sqlite3_step(stmt);
        if(err==SQLITE_ROW && !strcmp(argv[2],sqlite3_column_text(stmt,0)) && !strcmp(argv[3],sqlite3_column_text(stmt,1)))
        {
            userStatus = sqlite3_column_text(stmt,2);
            voteStatus = sqlite3_column_text(stmt,3);
            if(!strcmp(userStatus,"Admin")) k=1;
            else k=2;
            if(!strcmp(voteStatus,"Yes")) l=1;
            else l=2;
            //printf("[%s]\n[%s]\n", userStatus, voteStatus);
            strcat(msgrasp,"Login reusit.\n");
            strcat(msgrasp,"Status user ");
            strcat(msgrasp,userStatus);
            strcat(msgrasp,"\n");
            strcat(msgrasp,"Vote status ");
            strcat(msgrasp,voteStatus);
            strcat(msgrasp,"\n");
            ok=1;
        }
    }
    while(err==SQLITE_ROW);
    sqlite3_finalize(stmt);
    //printf("[%s]\n[%s]\n", userStatus, voteStatus);
    if(ok==0) strcat(msgrasp,"Login esuat.\n");
}

void melodii()
{
    strcat(msgrasp,"Comanda de afisare a melodiilor a fost apelata cu succes\n");
    tables = "SELECT Name, Desc, Gen1, Gen2, Gen3, Link, NrVoturi, NrComentarii, Comment1, Comment2, Comment3, Comment4, Comment5 FROM Top";
    bdaux=sqlite3_exec(bd,tables,callback,0,&err_msg);
}

void adaugare_melodie()
{
    strcat(msgrasp,"Comanda de adaugare a unei melodii a fost apelata cu succes\n");
    strcpy(nume,"");
    strcpy(descriere,"");
    strcpy(gen1,"");
    strcpy(gen2,"");
    strcpy(gen3,"");
    strcpy(linkk,"");
    i=3;
    while(i<argc)
    {
        if(!strcmp(argv[i],"nume:"))
        {
            i++;
            while(strcmp(argv[i],"descriere:"))
            {
                strcat(nume, argv[i]);
                strcat(nume, " ");
                i++;
            }
        }
        else if(!strcmp(argv[i],"descriere:"))
        {
            i++;
            while(strcmp(argv[i],"gen1:"))
            {
                strcat(descriere, argv[i]);
                strcat(descriere, " ");
                i++;
            }
        }
        else if(!strcmp(argv[i],"gen1:"))
        {
            strcat(gen1, argv[i+1]);
            i=i+2;
        }
        else if(!strcmp(argv[i],"gen2:"))
        {
            strcat(gen2, argv[i+1]);
            i=i+2;
        }
        else if(!strcmp(argv[i],"gen3:"))
        {
            strcat(gen3, argv[i+1]);
            i=i+2;
        }
        else if(!strcmp(argv[i],"link:"))
        {
            strcat(linkk, argv[i+1]);
            i=i+2;
        }
        else i++;

    }
    nume[strlen(nume)-1]='\0';
    if(strlen(gen2)==0) strcat(gen2,"NULL");
    if(strlen(gen3)==0) strcat(gen3,"NULL");
    tables = "INSERT INTO Top (Name,Desc,Gen1,Gen2,Gen3,Link,NrVoturi,NrComentarii) VALUES(?, ?, ?, ?, ?, ?, 1, 0);";
    err=sqlite3_prepare_v2(bd,tables,-1,&stmt,NULL);
    sqlite3_bind_text(stmt, 1, nume, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, descriere, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, gen1, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, gen2, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, gen3, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, linkk, -1, SQLITE_TRANSIENT);
    err=sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void adaugare_comentariu()
{
    strcat(msgrasp,"Comanda de adaugare comentariu a fost apelata cu succes\n");
    i=3;
    nrCom=0;
    strcpy(nume2,"");
    strcpy(comentariu,"");
    while(i<argc)
    {
        if(!strcmp(argv[i],"nume:"))
        {
            i++;
            while(strcmp(argv[i],"comentariu:"))
            {
                strcat(nume2, argv[i]);
                strcat(nume2, " ");
                i++;
            }
        }
        else if(!strcmp(argv[i],"comentariu:"))
        {
            i++;
            while(i<argc)
            {
                strcat(comentariu, argv[i]);
                strcat(comentariu, " ");
                i++;
            }
        }
        else i++;
    }
    nume2[strlen(nume2)-1]='\0';
    tables = "SELECT NrComentarii FROM Top WHERE Name LIKE ?";
    err=sqlite3_prepare_v2(bd,tables,-1,&stmt,NULL);
    sqlite3_bind_text(stmt, 1, nume2, -1, SQLITE_TRANSIENT);
    err=sqlite3_step(stmt);
    nrCom = sqlite3_column_int(stmt,0);
    sqlite3_finalize(stmt);
    //printf("Numarul de comentarii este %d\n",nrCom);
    //printf("[%s]\n",nume2);
    if(nrCom==0) tables = "UPDATE Top set Comment1 = ? WHERE Name LIKE ?";
    else if(nrCom==1) tables = "UPDATE Top set Comment2 = ? WHERE Name LIKE ?";
    else if(nrCom==2) tables = "UPDATE Top set Comment3 = ? WHERE Name LIKE ?";
    else if(nrCom==3) tables = "UPDATE Top set Comment4 = ? WHERE Name LIKE ?";
    else if(nrCom==4) tables = "UPDATE Top set Comment5 = ? WHERE Name LIKE ?";
    err=sqlite3_prepare_v2(bd,tables,-1,&stmt,NULL);
    sqlite3_bind_text(stmt, 1, comentariu, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, nume2, -1, SQLITE_TRANSIENT);
    err=sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if(nrCom <5)
    {
        tables = "UPDATE Top set NrComentarii = NrComentarii+1 WHERE Name LIKE ?";
        err=sqlite3_prepare_v2(bd,tables,-1,&stmt,NULL);
        sqlite3_bind_text(stmt, 1, nume2, -1, SQLITE_TRANSIENT);
        err=sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

void vot()
{
    strcat(msgrasp,"Comanda de vot a fost apelata cu succes\n");
    if(!strcmp(voteStatus,"No")) strcat(msgrasp,"Nu aveti drept de vot.\n");
    else
    {
        strcpy(nume3,"");
        i=2;
        while(i<argc)
        {
            if(!strcmp(argv[i],"nume:"))
            {
                i++;
                while(i<argc)
                {
                    strcat(nume3, argv[i]);
                    strcat(nume3, " ");
                    i++;
                }
            }
            i++;
        }
        //printf("[%s]\n",nume3);
        nume3[strlen(nume3)-1]='\0';
        tables = "UPDATE Top set NrVoturi = NrVoturi+1 WHERE Name LIKE ?";
        err=sqlite3_prepare_v2(bd,tables,-1,&stmt,NULL);
        sqlite3_bind_text(stmt, 1, nume3, -1, SQLITE_TRANSIENT);
        err=sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        strcat(msgrasp,"Votarea a fost realizata cu succes.\n");
    }
}

void afisare_dupa_voturi()
{
    strcat(msgrasp,"Comanda de afisare top dupa voturi a fost apelata cu succes\n");
    tables = "SELECT Name, NrVoturi FROM Top ORDER BY NrVoturi DESC";
    bdaux = sqlite3_exec(bd,tables,callback,0,&err_msg);
}

void afisare_dupa_gen()
{
    strcat(msgrasp,"Comanda de afisare top dupa un anumit gen a fost apelata cu succes\n");
    strcpy(gen,"");
    strcat(gen,argv[5]);
    //printf("[%s]\n",gen);
    //gen[strlen(gen)]=' ';
    tables = "SELECT Name, NrVoturi FROM Top WHERE (Gen1 LIKE ? OR Gen2 LIKE ? OR Gen3 LIKE ?) ORDER BY NrVoturi DESC";
    err=sqlite3_prepare_v2(bd,tables,-1,&stmt,NULL);
    sqlite3_bind_text(stmt, 1, gen, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, gen, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, gen, -1, SQLITE_TRANSIENT);
    do
    {
        err=sqlite3_step(stmt);
        if(err==SQLITE_ROW)
        {
            numeAux = sqlite3_column_text(stmt,0);
            i = sqlite3_column_int(stmt,1);
            strcat(msgrasp,"Nume melodie: ");
            strcat(msgrasp,numeAux);
            strcat(msgrasp,"\n");
            strcpy(aux,"");
            //aux[0]=i + '0';
            //aux[1]='\0';
            j=snprintf(aux, strlen(aux)-1, "%d", i);
            strcat(msgrasp,"Numar voturi: ");
            strcat(msgrasp,aux);
            strcat(msgrasp,"\n\n");
            //printf("Nume: %s  \nNumar Voturi: %d\n\n",numeAux,i);
        }
    }
    while(err==SQLITE_ROW);
    sqlite3_finalize(stmt);
}

void stergere()
{
    strcat(msgrasp,"Comanda de stergere a fost apelata cu succes\n");
    if(k==2) strcat(msgrasp,"Actiune interzisa\n");
    else if(k==1)
    {
        strcpy(nume,"");
        i=2;
        while(i<argc)
        {
            strcat(nume, argv[i]);
            strcat(nume, " ");
            i++;
        }
        //printf("[%s]\n",nume);
        nume[strlen(nume)-1]='\0';
        tables = "DELETE FROM Top WHERE Name LIKE ?";
        err=sqlite3_prepare_v2(bd,tables,-1,&stmt,NULL);
        sqlite3_bind_text(stmt, 1, nume, -1, SQLITE_TRANSIENT);
        err=sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

void restrictionare()
{
    strcat(msgrasp,"Comanda de restrictionare a fost apelata cu succes\n");
    if(k==2) strcat(msgrasp,"Actiune interzisa\n");
    else if(k==1)
    {
        strcpy(nume, "");
        strcat(nume, argv[2]);
        //nume[strlen(nume)-1]='\0';
        tables= "UPDATE UserInfo set VoteStatus = 'No' WHERE Username LIKE ? AND UserStatus NOT LIKE 'Admin'";
        err=sqlite3_prepare_v2(bd,tables,-1,&stmt,NULL);
        sqlite3_bind_text(stmt, 1, nume, -1, SQLITE_TRANSIENT);
        err=sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

void permitere()
{
    strcat(msgrasp,"Comanda de permitere a fost apelata cu succes\n");
    if(k==2) strcat(msgrasp,"Actiune interzisa\n");
    else if(k==1)
    {
        strcpy(nume, "");
        strcat(nume, argv[2]);
        //nume[strlen(nume)-1]='\0';
        tables= "UPDATE UserInfo set VoteStatus = 'Yes' WHERE Username LIKE ?";
        err=sqlite3_prepare_v2(bd,tables,-1,&stmt,NULL);
        sqlite3_bind_text(stmt, 1, nume, -1, SQLITE_TRANSIENT);
        err=sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

void logout()
{
    strcat(msgrasp,"Comanda de logout a fost apelata cu succes\n");
    userStatus="Nelogat";
    voteStatus="Nelogat";
    k=0;
    l=0;
}

void promovare()
{
    strcat(msgrasp,"Comanda de promovare a fost apelata cu succes\n");
    if(k==2) strcat(msgrasp,"Actiune interzisa\n");
    else if(k==1)
    {
        strcpy(nume, "");
        strcat(nume, argv[2]);
        tables= "UPDATE UserInfo set UserStatus = 'Admin' WHERE Username LIKE ?";
        err=sqlite3_prepare_v2(bd,tables,-1,&stmt,NULL);
        sqlite3_bind_text(stmt, 1, nume, -1, SQLITE_TRANSIENT);
        err=sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

void construire_argv()
{
    argc=1;
    i=1;
    tok=strtok(msg,s);
    while(tok!=NULL)
    {
        argc++;
        strcpy(argv[i++],tok);
        tok=strtok(NULL,s);
    }
    strtok(argv[argc-1],"\n");
}

void initializare_bd()
{
    bdaux = sqlite3_open("topmusic.db",&bd);
    if(bdaux != SQLITE_OK) printf("Nu se poate deschide baza de date: %s\n", sqlite3_errmsg(bd));
    tables =           "CREATE TABLE UserInfo(" \
                       "Username TEXT NOT NULL," \
                       "Password TEXT NOT NULL," \
                       "UserStatus TEXT NOT NULL," \
                       "VoteStatus TEXT NOT NULL);" \
                       "INSERT INTO UserInfo (Username, Password, UserStatus, VoteStatus) " \
                       "VALUES ('Admin','Admin','Admin','Yes'); " \
                       "INSERT INTO UserInfo (Username, Password, UserStatus, VoteStatus) " \
                       "VALUES ('Tiberiu','Shrek','User','Yes'); " \
                       "INSERT INTO UserInfo (Username, Password, UserStatus, VoteStatus) " \
                       "Values ('Tiberiu2','Ogre','User','No');";

    bdaux=sqlite3_exec(bd,tables,0,0,&err_msg);

    tables =           "CREATE TABLE Top(" \
                       "Name TEXT NOT NULL," \
                       "Desc TEXT NOT NULL," \
                       "Gen1 TEXT NOT NULL," \
                       "Gen2 TEXT," \
                       "Gen3 TEXT," \
                       "Link TEXT NOT NULL," \
                       "NrVoturi INT NOT NULL," \
                       "NrComentarii INT NOT NULL," \
                       "Comment1 TEXT," \
                       "Comment2 TEXT," \
                       "Comment3 TEXT," \
                       "Comment4 TEXT," \
                       "Comment5 TEXT );" \
                       "INSERT INTO Top (Name, Desc, Gen1, Link, NrVoturi, NrComentarii, Comment1, Comment2) " \
                       "VALUES ('Sonata','No description','Classic','www.youtube.com/tratatava424m',1,2,'Very good','Yes'); " \
                       "INSERT INTO Top (Name, Desc, Gen1, Link, NrVoturi, NrComentarii, Comment1, Comment2) " \
                       "VALUES ('Smash Mouth by All Star','SomeBODY once told me','Rock','www.youtube.com/131231231aaa',6,2,'life was gonna roll me','nimic'); " \
                       "INSERT INTO Top (Name, Desc, Gen1, Link, NrVoturi, NrComentarii, Comment1, Comment2, Comment3, Comment4) " \
                       "VALUES ('Architects by Rise Against','1st song of Endgame album','Rock','www.youtube.com/r34awra4wr',2,4,'Best','i LiKEd MiNUTe 3 MOSt','good song','filler comment'); " \
                       "INSERT INTO Top (Name, Desc, Gen1, Gen2, Link, NrVoturi, NrComentarii, Comment1, Comment2) " \
                       "Values ('Deteriorate by Demon Hunter','5th song of The Triptych album','Metalcore','Metal','www.youtube.com/a3era33a2222666',5,2,'ceva','altceva');";
    bdaux=sqlite3_exec(bd,tables,0,0,&err_msg);
}

int main ()
{
    initializare_bd();

    struct sockaddr_in server;
    struct sockaddr_in from;
    //char msgrasp[10000]=" ";
    int sd;

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("[server]Eroare la socket().\n");
        return errno;
    }

    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);

    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
        perror ("[server]Eroare la bind().\n");
        return errno;
    }

    if (listen (sd, 1) == -1)
    {
        perror ("[server]Eroare la listen().\n");
        return errno;
    }

    while (1)
    {
        int client;
        int length = sizeof (from);

        printf ("Asteptam la portul %d...\n",PORT);
        fflush (stdout);
        client = accept (sd, (struct sockaddr *) &from, &length);
        if (client < 0)
        {
            perror ("Eroare la accept().\n");
            continue;
        }
        int pid;
        if ((pid = fork()) == -1)
        {
            close(client);
            continue;
        }
        else if (pid > 0)
        {
            close(client);
            while(waitpid(-1,NULL,WNOHANG));
            continue;
        }
        else if (pid == 0)
        {
            close(sd);
            while(1)  //permite procesarea mai multor comenzi
            {
                bzero (msg, 200);
                printf ("Asteptam mesajul...\n");
                fflush (stdout);

                if (read (client, msg, 200) <= 0)
                {
                    perror ("Eroare la read() de la client.\n");
                    close (client);
                    continue;
                }

                printf ("Mesajul a fost receptionat...%s\n", msg);

                construire_argv();
                bzero(msgrasp,2500);

                if((!strcmp(argv[1],"Inregistrare") || !strcmp(argv[1],"inregistrare")) && argc!=4) strcat(msgrasp,"Numar incorect de argumente.\n");
                if((!strcmp(argv[1],"Inregistrare") || !strcmp(argv[1],"inregistrare")) && strcmp(userStatus,"Nelogat")) strcat(msgrasp,"User deja logat.\n");
                if((!strcmp(argv[1],"Inregistrare") || !strcmp(argv[1],"inregistrare") || !strcmp(argv[1],"INREGISTRARE")) && argc==4 && !strcmp(userStatus,"Nelogat"))
                    inregistrare();
                if((!strcmp(argv[1],"Conturi") || !strcmp(argv[1],"conturi")) && strcmp(userStatus,"Nelogat"))
                    conturi();
                if((!strcmp(argv[1],"Login") || !strcmp(argv[1],"login") || !strcmp(argv[1],"LOGIN")) && argc!=4)
                    strcat(msgrasp,"Numar incorect de cuvinte introduse de la tastatura. Formatul este: Login Nume Parola\n");
                if((!strcmp(argv[1],"Login") || !strcmp(argv[1],"login")) && strcmp(userStatus,"Nelogat")) strcat(msgrasp,"User deja logat.\n");
                else if((!strcmp(argv[1],"Login") || !strcmp(argv[1],"login") || !strcmp(argv[1],"LOGIN")) && argc==4 && !strcmp(userStatus,"Nelogat"))
                    login();
                if((!strcmp(argv[1],"Melodii") || !strcmp(argv[1],"melodii")) && strcmp(userStatus,"Nelogat"))
                    melodii();
                if((!strcmp(argv[1],"Adaugare") || !strcmp(argv[1],"adaugare") || !strcmp(argv[1],"ADAUGARE")) && argc==2 && strcmp(userStatus,"Nelogat"))
                {
                    strcat(msgrasp,"Formatul pentru adaugarea unei melodii este:\nAdaugare melodie nume: <nume> descriere: <desc> gen1: <gen1> gen2: <gen2> gen3: <gen3> link: <link>\nGen2: <gen2> si gen3: <gen3> pot sa lipseasca.\n");
                    strcat(msgrasp,"Formatul pentru adaugarea unui comentariu este:\nAdaugare comentariu nume: <nume_melodie> comentariu: <comentariu>\n");
                }
                if(argc>12 && (!strcmp(argv[1],"Adaugare") || !strcmp(argv[1],"adaugare") || !strcmp(argv[1],"ADAUGARE")) && (!strcmp(argv[2],"Melodie") || !strcmp(argv[2],"melodie") || !strcmp(argv[2],"MELODIE")) && strcmp(userStatus,"Nelogat"))
                    adaugare_melodie();
                if(argc>4 && (!strcmp(argv[1],"Adaugare") || !strcmp(argv[1],"adaugare") || !strcmp(argv[1],"ADAUGARE")) && (!strcmp(argv[2],"Comentariu") || !strcmp(argv[2],"comentariu") || !strcmp(argv[2],"COMENTARIU")) && strcmp(userStatus,"Nelogat"))
                    adaugare_comentariu();
                if((!strcmp(argv[1],"Vot") || !strcmp(argv[1],"vot") || !strcmp(argv[1],"VOT")) && argc==2 && strcmp(userStatus,"Nelogat"))
                    strcat(msgrasp,"Format pentru comanda vot: Vot nume: <nume_melodie>\n");
                if((!strcmp(argv[1],"Vot") || !strcmp(argv[1],"vot") || !strcmp(argv[1],"VOT")) && argc!=2 && strcmp(userStatus,"Nelogat"))
                    vot();
                if((!strcmp(argv[1],"Afisare") || !strcmp(argv[1],"afisare") || !strcmp(argv[1],"AFISARE")) && argc==2 && strcmp(userStatus, "Nelogat"))
                    strcat(msgrasp,"Format afisare top: Afisare top dupa voturi SAU Afisare top dupa genul <gen>\n");
                if(argc>4 && (!strcmp(argv[1],"Afisare") || !strcmp(argv[1],"afisare")) && !strcmp(argv[2],"top") && !strcmp(argv[3],"dupa") && !strcmp(argv[4],"voturi") && strcmp(userStatus,"Nelogat"))
                    afisare_dupa_voturi();
                if(argc>5 && (!strcmp(argv[1],"Afisare") || !strcmp(argv[1],"afisare")) && !strcmp(argv[2],"top") && !strcmp(argv[3],"dupa") && !strcmp(argv[4],"genul") && strcmp(userStatus,"Nelogat"))
                    afisare_dupa_gen();
                if((!strcmp(argv[1],"Sterge") || !strcmp(argv[1],"sterge")) && strcmp(userStatus,"Nelogat"))
                    stergere();
                if((!strcmp(argv[1],"Restrictionare") || !strcmp(argv[1],"restrictionare")) && strcmp(userStatus,"Nelogat"))
                    restrictionare();
                if((!strcmp(argv[1],"Permitere") || !strcmp(argv[1],"permitere")) && strcmp(userStatus,"Nelogat"))
                    permitere();
                if((strcmp(argv[1],"Login") && strcmp(argv[1],"login") && strcmp(argv[1],"inregistrare") && strcmp(argv[1],"Inregistrare") && strcmp(argv[1],"help") && strcmp(argv[1],"Help")) && !strcmp(userStatus,"Nelogat"))
                    strcat(msgrasp,"Incercati sa accesati comenzi la care nu aveti acces. Pentru o lista de comenzi folositi comanda help.\n");
                if(!strcmp(argv[1],"help") || !strcmp(argv[1],"Help"))
                    strcat(msgrasp,"Lista de comenzi:\nComenzi pentru useri nelogati\n1.login <user> <parola>\n2.inregistrare <user> <parola>\n3.quit\nComenzi pentru useri logati (fara drept de administrator):\n1.melodii - afiseaza toate melodiile din baza de date\n2.adaugare melodie nume: <nume> descriere: <descriere> gen1: <gen1> gen2: <gen2> gen3: <gen3> link: <link> - Campurile gen2: <gen2> si gen3: <gen3> sunt optionale.\n3.adaugare comentariu nume: <nume_melodie> comentariu: <comentariu>\n4.vot nume: <nume_melodie>\n5.afisare top dupa voturi\n6.afisare top dupa genul <gen>\n7.logout\nComenzi pentru useri cu drept de administrator:\n1.sterge <nume_melodie> - va sterge melodia din baza de date\n2.restrictionare <nume_utilizator> - va restrictiona dreptul de vot al unui utilizator obisnuit\n3.permitere <nume_utilizator> - va permite dreptul de vot al unui utilizator obisnuit\n4.promovare <nume> - promoveaza user-ul la statusul de Admin\n5.conturi - afiseaza baza de date a userilor\n");
                if(!strcmp(argv[1],"logout") || !strcmp(argv[1],"Logout"))
                    logout();
                if(!strcmp(argv[1],"promovare") || !strcmp(argv[1],"Promovare"))
                    promovare();
                if(!strcmp(argv[1],"quit") || !strcmp(argv[1],"Quit"))
                {
                    close (client);
                    exit(0);
                }
                printf("Trimitem mesajul inapoi...%s\n",msgrasp);

                if (write (client, msgrasp, 2500) <= 0)
                {
                    perror ("Eroare la write() catre client.\n");
                    continue;
                }
                else
                    printf ("Mesajul a fost trasmis cu succes.\n");
            }
            close (client);
            exit(0);
        }

    }				// while
}				// main
