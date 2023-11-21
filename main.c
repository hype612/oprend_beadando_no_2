#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

struct Szallitmany {
    int ID;
    char borvidek[50];
    char termelo[50];
    int mennyiseg;
    char szolofajta[50]; 
};

void printOne(struct Szallitmany sz) {
    printf("%d; %s; %s; %d; %s", sz.ID, sz.borvidek, sz.termelo, sz.mennyiseg, sz.szolofajta);
    printf("\n");
}

void listDb(struct Szallitmany *db, int size) {
    if(size > 0) {
        printf("ID; borvidek; termelo; mennyiseg; szolofajta\n");
        for(int i = 0; i < size; i++) {
            printOne(db[i]);
        }
    }
    else {
        printf("az adatbazis ures\n");
    }
    printf("==========\n");
}

void filterDb(struct Szallitmany *db, int size,  int column, char filter[50]) {
    struct Szallitmany ret[100];
    filter[strcspn(filter, "\n")] = 0;
    filter[strcspn(filter, "\n")] = 0;
    
    int retSize = 0;
    for(int i = 0; i < size; i++) {
        switch (column) {
            case 1:
                printf("%d, %d\n", strlen(filter), strlen(db[i].borvidek));
                if((strcmp(filter, db[i].borvidek) == 0)) {
                    ret[retSize] = db[i];
                    retSize++;
                }
                break;
            case 2:
                if((strcmp(filter, db[i].termelo) == 0)) {
                    ret[retSize] = db[i];
                    retSize++;
                }
                break;
            case 3:
                if(atoi(filter) == db[i].mennyiseg) {
                    ret[retSize] = db[i];
                    retSize++;
                }
                break;
            case 4:
                if((strcmp(filter, db[i].szolofajta) == 0)) {
                    ret[retSize] = db[i];
                    retSize++;
                }
                break;
            default:
                break;
        }
    }
    listDb(ret, retSize);
}

void listFilteredDb(struct Szallitmany *db, int size) {
    system("clear");
    char *buffer = NULL;
    size_t buffersize = 50;
    printf("hanyadik oszlopban szeretnel szurni? (1: borvidek, 2: termelo, 3: mennyiseg, 4: szolotipus): ");
    int c;
    scanf("%d", &c);
    getchar(); 
    switch (c) {
        case 1:
            printf("\nkeresett borvidek: ");
            getline(&buffer, &buffersize, stdin); 
            filterDb(db, size, c, buffer);
            break;
        case 2:
            printf("\nkeresett termelo: ");
            getline(&buffer, &buffersize, stdin); 
            filterDb(db, size, c, buffer);
            break;
        case 3:
            printf("\nkeresett mennyiseg: ");
            getline(&buffer, &buffersize, stdin); 
            filterDb(db, size, c, buffer);
            break;
        case 4:
            printf("\nkeresett szolotipus: ");
            getline(&buffer, &buffersize, stdin); 
            filterDb(db, size, c, buffer);
            break;
        default: 
            printf("\nhelytelen ertek %d. visszairanyitas a menube...\n",c);
            break;
    } 

}


struct Szallitmany insertInDb(struct Szallitmany *db, int size) {
    system("clear");
    getchar();
    struct Szallitmany inserted;
    int s = 50;
    char buffer[20];
    inserted.ID = size;
    printf("borvidek: ");
    scanf("%s", inserted.borvidek);
    getchar();
    printf("\ntermelo: ");
    fgets(inserted.termelo, sizeof(inserted.termelo), stdin);
    inserted.termelo[strcspn(inserted.termelo, "\n")] = 0;
    printf("\nmennyiseg (kg): ");
    scanf("%d", &inserted.mennyiseg);
    getchar();
    printf("\nszolofajta: ");
    scanf("%s", inserted.szolofajta);
    getchar();

    printOne(inserted);
    return inserted;
}

void deleteFromDb(struct Szallitmany *db, int size) {
    system("clear");
    listDb(db,size);
    printf("adja meg a torolni kivant szallitmany azonositojat: ");
    int c;
    scanf("%d", &c);
    getchar(); 
    for(int i = 0; i < size; i++) {
        if(db[i].ID == c) {
            for(int j = i+1; i < size; i++) {
                db[i] = db[j];
            }
            return;
        }
    }

}


void editInDb(struct Szallitmany *db, int size) {
    system("clear");
    listDb(db,size);
    printf("adja meg a modositani kivant szallitmany azonositojat: ");
    int c;
    scanf("%d", &c);
    getchar(); 
    for(int i = 0; i < size; i++) {
        if(db[i].ID == c) {
            printf("adja meg a modositani kivant oszlopot (1: borvidek, 2: termelo, 3: mennyiseg, 4: szolotipus): ");
            scanf("%d", &c);
            getchar();
            switch (c) {
                case 1:
                    printf("\nuj adat: ");
                    fgets(db[i].borvidek, sizeof(db[i].borvidek), stdin);
                    db[i].borvidek[strcspn(db[i].borvidek, "\n")] = 0;
                    break;
                case 2:
                    printf("\nuj adat: ");
                    fgets(db[i].termelo, sizeof(db[i].termelo), stdin);
                    db[i].termelo[strcspn(db[i].termelo, "\n")] = 0;
                    break;
                case 3: 
                    printf("\nuj adat: ");
                    scanf("%d", &db[i].mennyiseg);
                    getchar();
                    break;
                case 4:
                    printf("\nuj adat: ");
                    fgets(db[i].szolofajta, sizeof(db[i].szolofajta), stdin);
                    db[i].szolofajta[strcspn(db[i].szolofajta, "\n")] = 0;
                    break;
                default:
                    printf("helytelen mezo. Visszairanyitas a menube...");
                    system("clear");
                    break;

            }
            return;
        }
    }
}

int dbToFile(struct Szallitmany *db, int size) {
    FILE* f;
    f = fopen("./szolo.dat", "w");
    if(f == NULL) {
        printf("a file nem nyilt meg");
    }
    else {
        for(int i = 0; i < size; i++) {
            fprintf(f, "%s;%s;%d;%s;\n", db[i].borvidek, db[i].termelo, db[i].mennyiseg, db[i].szolofajta);
        }
        fclose(f);
    }
    
}


int main() {
    int pid, fd, fd2;

    char pipename[30];
    char processed_pipe[30];
    sprintf(pipename,"/tmp/%d",getpid());
    sprintf(processed_pipe, "/tmp/%d",getpid() * 2);
    int fid = mkfifo(pipename, S_IRUSR|S_IWUSR);
    int fid2 = mkfifo(processed_pipe, S_IRUSR|S_IWUSR);

    if(fid == -1 || fid2 == -1) {
        printf("error_no: %i", errno);
        perror("hiba lepett fel vmelyik pipe nyitasa soran\n");
        exit(EXIT_FAILURE);
    }


    if (pid > 0) {  // parent - sends

        struct Szallitmany db[100];
        FILE *f = fopen("./szolo.dat", "r");
        if( f == NULL) {
            printf("failed to open file");
            exit(EXIT_FAILURE);
        }
        
        char ch;
        char buffer[50];
        memset(buffer, 0, sizeof(buffer));
        int state = 0;
        int size = 0;
        do {
            ch = fgetc(f);
            if( ch == '\n') { continue; }
            if(ch == ';') {
                switch (state) {
                    case 0:
                        db[size].ID = size;
                        strcpy(db[size].borvidek, buffer);
                        state++;
                        break;
                    case 1:
                        strcpy(db[size].termelo, buffer);
                        state++;
                        break;
                    case 2:
                        db[size].mennyiseg = atoi(buffer);
                        state++;
                        break;
                    case 3:
                        strcpy(db[size].szolofajta, buffer);
                        state = 0;
                        size++;
                        break;
                    default:
                        break;
                }
                memset(buffer, 0, sizeof(buffer));
            }
            else {
                buffer[strlen(buffer)] = ch;
                buffer[strlen(buffer) + 1] = '\0';
            }

        } while ( ch != EOF);
        fclose(f);

        int input = 1;
        while (input != 0) {
            printf("1. teljes adatbazis listazasa\n");
            printf("2. szures es listazas\n");        
            printf("3. hozzaadas adatbazishoz\n");
            printf("4. torles adatbazisbol\n");
            printf("5. szallitmany modositasa\n");
            printf("6. szolo feldolgozasa\n");
            printf("0. program bezarasa\n");
            scanf("%d", &input);
            switch(input) {
                case 1:
                    system("clear");
                    listDb(db, size);
                    break;
                case 2:
                    listFilteredDb(db, size);
                    break;
                case 3:
                    db[size] = insertInDb(db, size);
                    size++;
                    break;
                case 4:
                    deleteFromDb(db, size);
                    size--;
                    break;
                case 5:
                    editInDb(db, size);
                    break;
                case 6:
                    pid = fork();
                    fd = open(pipename, O_WRONLY);
                    /*struct pipeData temp;

                    //wait for signal1 b4 sending
                    for(int i = 0; i < size; i++) {
                        write(fd, temp);
                    }*/
                    //signal2: child started processing
                    close(fd);

                    char receive_buffer[1024];
                    fd2 = open(processed_pipe, O_RDONLY);
                    read(fd, receive_buffer, sizeof(receive_buffer));
                    printf("%s", receive_buffer);
                    break;
                case 0:
                    dbToFile(db, size); 
                default:
                    break;
            }
        }
    }
    else { // child - receives
        // prepare for receiving

        // send signal that child is ready
        kill(getppid(), SIGUSR1);

        // receive data
        fd = open(pipename, O_RDONLY);
            // other code
        close(fd);

        // process data and send sig to parent
        kill(getppid(), SIGUSR2);
        sleep(1); // TODO: SET TO 5-10
            // other code 
        // send back string through pipe
        char send_buffer[200]; 
            // calculate exact data 4 string
        fd2 = open(processed_pipe, O_WRONLY);
        write(processed_pipe, send_buffer, strlen(send_buffer + 1));
        close(fd2); 
    }
    return 0;
}