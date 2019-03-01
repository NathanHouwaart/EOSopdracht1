#include "shell.hh"

int main() {
    std::string input;
    /* Initialiseert een keuzemenu */
    while (true) {
        std::cout << "shell$ ";
        std::getline(std::cin, input);
        if (input == "new_file") new_file();
        if (input == "ls") list();
        if (input == "src") src();
        if (input == "find") find();
        if (input == "python") python();
        if (input == "quit") return 0;
        if (input == "error") return 1;
    }
}


void new_file() {
    /* Maakt een nieuw bestand aan en schrijft er een string in weg */
    std::cout << "----------NEW----------" << std::endl;

    std::string filename;
    std::cout << "Enter a new filename: ";
    std::getline(std::cin, filename);           //user input voor een filename

    std::string path = "/Users/nathanhouwaart/CLionProjects/Opdracht1EOSfinal/" + filename + ".txt";
    char pathname[200];
    strcpy(pathname, path.c_str());             //maakt een nieuwe char array aan met de path, filename en .txt extentie

    mode_t openmode = O_APPEND;
    int fd1 = creat(pathname, 0777);            //maakt een nieuwe file aan. Slaat file descriptor op in variabele fd1.

    open(pathname, openmode);                   //opent de file, weet niet of dit wat toevoegt

    std::string text;
    std::cout << "text: ";


    std::string enddefinition = "<EOF>";

    while (std::getline(std::cin, text)) {  //user kan een string opgeven die in textfile geschreven wordt
        std::size_t found = text.find(enddefinition); //zoekt uit of en op welke index <EOF> zich bevindt.
        if (found != std::string::npos) {
            write(fd1, text.c_str(), (text.size() - enddefinition.size())); //write functie schrijft 'buffer' weg naar fd1. totaal schrijft hij het aantal bytes van buffer weg - de <EOF>
            write(fd1, "\n", 1); //voegt een endline toe aan het einde van de file.
            break;
        }
        write(fd1, text.c_str(), text.size());     //write functie schrijft 'buffer' weg naar fd1. totaal schrijft hij het aantal bytes van buffer weg
        write(fd1, "\n", 1); // voegt een endline toe na elke geschreven regel
    }


    close(fd1);
}


void list() {
    /* Roept ls -la functie aan */
    std::cout << "----------LS----------" << std::endl;

    int pid = fork();                           //maakt een nieuwe fork voor het ls proces.
    if (pid == 0) {
        char *args[] = {
                (char *) "/bin/ls",
                (char *) "-a",
                (char *) "-l",
                (char *) 0};                       //ls parameters worden gedefined
        execv("/bin/ls", args);                   //ls proces wordt gestart met args -a & -l
    } else {
        int exit_status;
        wait(&exit_status);                       //parent wacht tot child proces klaar is
    }
}

void find() {
    /* find functie gepiped met grep:
     * vertaald:
     * >>> find . | grep test */

    std::cout << "----------FIND----------" << std::endl;
    std::cout << "what do you want to grep: ";
    std::string input;
    std::getline(std::cin, input);        //vraagt user input

    pid_t pid;
    int mypipefd[2], status, ret;         //maakt variabelen

    ret = pipe(mypipefd);                 //maakt pipe aan

    if (ret == -1) {                        //Checkt of pipe correct is aangemaakt. Zo niet: exit met error code 1.
        perror("pipe");
        exit(1);
    }

    /* Een find grep pipeline ziet er als volgt uit:
     * >>> find . | grep test
     * We hebben twee processen nodig, dus twee forks. Een links van de pipeline, en een rechts.
     * In onderstaande code worden deze twee processen aangeroepen.
     * Met de functie dup2() wordt de terminal output van het linker proces geredirect naar de input van het proces rechts van de pipeline
     * Daarna worden de pipelines gesloten. Deze sluiten automatisch als de EOF (end of file) is bereikt.
     * Daarna wordt gewacht tot de twee child processen exiten. */

    pid = fork();                       //Forkt proces
    if (pid == 0) {
        /* linker pipeline functie */
        char *args[] = {
                (char *) "find",
                (char *) "./",
                (char *) NULL};;          //find parameters worden gedefined
        close(mypipefd[0]);               //input pipe wordt gesloten
        dup2(mypipefd[1], STDOUT_FILENO); //output gaat naar pipe

        execv("/usr/bin/find", args);     //childproces find wordt gestart met output naar pipe
    }

    pid = fork();
    if (pid == 0) {
        /* rechter pipeline functie */
        char *parm[] = {
                (char *) "grep",
                (char *) input.c_str(),
                (char *) NULL};            //grep parameters worden gedefined
        close(mypipefd[1]);              //output pipe wordt gesloten
        dup2(mypipefd[0], STDIN_FILENO); //pipe gaat naar input

        execv("/usr/bin/grep", parm);    //childproces grep wordt gestart met input van pipe.
    }

    close(mypipefd[0]);
    close(mypipefd[1]);                //pipelines worden gesloten.

    waitpid(-1, &status, 0);
    waitpid(-1, &status, 0);            //wacht tot beide child processen zijn ge-exit
}

void python() {
    /* Start interactieve Python shell */
    std::cout << "----------PYTHON----------" << std::endl;

    int pid = fork();                               //Maakt een nieuwe fork voor het Python proces

    if (pid == 0) {
        char *args[] = {
                (char *) "python3.7",
                (char *) NULL};                        //Python parameters worden gedefined
        execv("/usr/local/bin/python3.7", args);      //Python proces wordt gestart
    } else {
        int exit_status;
        wait(&exit_status);                           //parent wacht tot child proces klaar is
    }
}


void src() {
    int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
    char byte[1];                                           // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
    while (syscall(SYS_read, fd, byte,
                   1))                   // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
        std::cout << byte;
}                                  //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.

