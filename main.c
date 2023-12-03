#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>


struct pipe_data {
    char szolofajta[50];
    int mennyiseg;
};

const char * acceptedTypes[] =  {
    "Irsai",
    "Otellou",
    "Szurkebarat"
};

const int idealWeight = 500;


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

int isInAccepted(const char* tested) {
    for(int i = 0; i < 3; i++) {
        if(strcmp(tested, acceptedTypes[i]) == 0) {
            return 1;
        }
    }
    printf("szolofajta nem talalhato az elfogadottak listajan...\n");
    printf("elfogadott tipusok: Irsai, Otellou, Szurkebarat\n");
    return 0;
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
    do {
        printf("\nszolofajta: ");
        scanf("%s", inserted.szolofajta);
        getchar();
    } while (!isInAccepted(inserted.szolofajta));
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
                    do {
                        printf("\nuj adat: ");
                        fgets(db[i].szolofajta, sizeof(db[i].szolofajta), stdin);
                        db[i].szolofajta[strcspn(db[i].szolofajta, "\n")] = 0;
                        break;
                    } while(!isInAccepted(db[i].szolofajta));
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

void sumByType(struct pipe_data * ret, struct Szallitmany * db, int size) {
    strcpy(ret[0].szolofajta, "Irsai");
    strcpy(ret[1].szolofajta, "Otellou");
    strcpy(ret[2].szolofajta, "Szurkebarat");

    for(int i = 0; i < 3; i++) {
        ret[i].mennyiseg = 0;
    }

    for(int i = 0; i < size; i++) {
        if(strcmp(db[i].szolofajta, "Irsai") == 0)
            ret[0].mennyiseg += db[i].mennyiseg;
        else if(strcmp(db[i].szolofajta, "Otellou") == 0)
            ret[1].mennyiseg += db[i].mennyiseg;
        else
            ret[2].mennyiseg += db[i].mennyiseg;
    }
}


void processSzolo(struct Szallitmany * db, int size) {
    system("clear"); 
    struct pipe_data summedByType[3];
    sumByType(summedByType, db, size);

    if( summedByType[0].mennyiseg >= idealWeight ||
    summedByType[1].mennyiseg >= idealWeight ||
    summedByType[2].mennyiseg >= idealWeight) {

    
    pid_t pid;
    int pipefd[2];
    int pipe2fd[2];
    if( pipe(pipefd) == -1 || pipe(pipe2fd) == -1 ) {
    perror("hiba a pipe nyitasakor\n");
    exit(EXIT_FAILURE);
    }
    pid = fork();
    if( pid == -1 ) {
    perror("fork error..\n");
    exit(EXIT_FAILURE);
    }

    if(pid == 0) {
    //child prcss
    srand (time ( NULL));
            
    // ready to read pipe 
    kill(getppid(), SIGUSR1);
    int size_of_toProcess;
    read(pipefd[0], &size_of_toProcess, sizeof(int));
    struct pipe_data to_process[size_of_toProcess];

    sleep(1);
    close(pipefd[1]);
    for(int i = 0; i < size_of_toProcess; i++) { 
    read(pipefd[0], &to_process[i], sizeof(struct pipe_data));
    }
    close(pipefd[0]);

    // started processing
    kill(getppid(), SIGUSR2);

    unsigned int waitTime = 1 + (rand() % 2);
    sleep(waitTime);

    // actual processing
    double lpkg = 0.6 + (rand() / (RAND_MAX / 0.2));
    int summed_kg = 0;
    for (int i = 0; i < size_of_toProcess; i++) 
    summed_kg += to_process[i].mennyiseg;
    int summed_l = summed_kg * lpkg; 

    close(pipe2fd[0]);
    char answer_buffer[150];
    sprintf(answer_buffer, "szolo megviszgalva. Minoseg alapjan %f l/kg kozotti bor varhato.\nMegadott mennyisegbol (%d kg) osszesen %d l fog eloallni\n", lpkg, summed_kg, summed_l); 
    write(pipe2fd[1], answer_buffer, sizeof(answer_buffer));
    close(pipe2fd[1]);
    exit(EXIT_SUCCESS);
    }
    else {
    // never use these in parent
    close(pipefd[0]);
    close(pipe2fd[1]);

    struct pipe_data to_send[3];
    int to_send_len = 0;
    for(int i = 0; i < 3; i++) {
    if(summedByType[i].mennyiseg >= idealWeight) {
    to_send[to_send_len].mennyiseg = summedByType[i].mennyiseg;
    strcpy(to_send[to_send_len].szolofajta, summedByType[i].szolofajta);
    to_send_len++;
    }
    } 
    write(pipefd[1], &to_send_len, sizeof(int));

    // wait for child to read
    pause();
    for(int i = 0; i < to_send_len; i++) {
    write(pipefd[1], &to_send[i], sizeof(struct pipe_data));
    //printf("k: %s : %d\n", summedByType[i].szolofajta, summedByType[i].mennyiseg);
    }
                
    // receive prompt abt processing
    pause(); 

    char pipe_buffer[150]; 

    read(pipe2fd[0], pipe_buffer, sizeof(pipe_buffer));
    printf("%s", pipe_buffer);
    close(pipefd[1]);
    close(pipe2fd[0]);
    }

    fflush(NULL);
    } 
    else {
    printf("egyik szolofajtabol sincsen elegendo mennyiseg\n");
    }
}


void handler_ready_to_receive() {
    printf("SIGNAL: gyerek kesz az olvasasra\n");
}

void handler_processing() {
    printf("SIGNAL: gyerek feldolgozza a kapott adatokat..\n");
}

int main() {

    signal(SIGUSR1, handler_ready_to_receive);
    signal(SIGUSR2, handler_processing);

    struct Szallitmany db[100];
    FILE *f = fopen("./szolo.dat", "r");
    if( f == NULL) {
        printf("failed to open file");
        return 1;
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
        printf("6. keszlet feldolgozasa\n");
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
                processSzolo(db, size);
                break;
            case 0:
                dbToFile(db, size); 
            default:
                break;
        }
    }
    return 0;
}
