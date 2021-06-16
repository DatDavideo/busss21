#define _GNU_SOURCE

#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>


void fahreAufPlattform(pid_t wagenNummer) {
    fprintf(stdout, "Wagen %d fährt auf Plattform\n", wagenNummer);
}

void oeffneTueren(pid_t wagenNummer) {
    fprintf(stdout, "Wagen %d öffnet die Türen\n", wagenNummer);
}

void schliesseTueren(pid_t wagenNummer) {
    fprintf(stdout, "Wagen %d schließt die Türen\n", wagenNummer);
}

void fahreVonPlattform(pid_t wagenNummer) {
    fprintf(stdout, "Wagen %d verlässt die Plattform\n\n", wagenNummer);
}

sem_t* sitze_lock;
sem_t* wagen_lock;
sem_t* besucher_lock;

void AnkunftWagen() {
    pid_t wagen = getpid();
    fprintf(stdout, "Wagen %d kommt an\n", wagen);
    sem_wait(wagen_lock);    
    /* Auf die Plattform fahren, wenn diese frei ist. */
    
    fahreAufPlattform(wagen);
    
    oeffneTueren(wagen);
    
    /* Besucher einsteigen lassen. */
    sem_post(sitze_lock);
    sem_post(sitze_lock);
    sem_wait(besucher_lock);
    sem_wait(besucher_lock);
    /* Türen schliessen wenn 2 Besucher eingestiegen sind und losfahren. */
    schliesseTueren(wagen);
    
    fahreVonPlattform(wagen);
    sem_post(wagen_lock);
}



void betrete_wagen(int besucherNummer) {
    fprintf(stdout, "Besucher %d steigt in Wagen\n", besucherNummer);
}

uint32_t* besucher_nummer;

void AnkunftBesucher() {
    uint32_t besucher;
    
    
    besucher = (*besucher_nummer)++;
    fprintf(stdout, "Besucher %d kommt an\n", besucher);
    
    
    /* Vor dem Einstieg auf freien Sitzplatz warten. */
    sem_wait(sitze_lock);
    betrete_wagen(besucher);
    sem_post(besucher_lock);
}




int shouldEnd = 0;

/* SIGINT signal handler */
void signal_handler(int signalNum) {
    if(signalNum == SIGINT) {
        printf("Ok, schalte die Achterbahn ab!\n");
        shouldEnd = 1;
    }
}


int main(void) {
        
    // zahlen die mit 0 starten werden octal interpretiert :)
    sitze_lock = sem_open("/sitze_lock", O_CREAT|O_EXCL, 0644, 0); /* Note O_EXCL */
    sem_unlink("/sitze_lock");
    wagen_lock = sem_open("/wagen_lock", O_CREAT|O_EXCL, 0644, 1); /* Note O_EXCL */
    sem_unlink("/wagen_lock");
    besucher_lock = sem_open("/besucher_lock", O_CREAT|O_EXCL, 0644, 0); /* Note O_EXCL */
    sem_unlink("/besucher_lock");
    // Geteilter Speicher um Besuchernummern zu vergeben
    besucher_nummer = mmap(NULL, sizeof(uint32_t), PROT_READ | PROT_WRITE,
                           MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    // Der 1. Besucher ist Nummer 0
    *besucher_nummer = 0;
    /* catch interrupts */
    signal(SIGINT, signal_handler);
    srand(time(NULL));
    // main loop
    while (!shouldEnd) {
        // Wagen und Besucher erzeugen
        int car_or_user = rand();
        pid_t f_pid = fork();
        if (f_pid == 0) {
            
            /* Kinder sollen nicht auf SIG_INT hören */
            signal(SIGINT, SIG_DFL);
            
            /* Zufällige Entscheidung, ob wir ein Kind oder einen Besucher ankommen lassen. */
            if(car_or_user < (RAND_MAX/2)) {
                // Definiere diesen Fall als neuen Wagen
                AnkunftWagen();
            }else {
                // Definiere diesen Fall als neuen Besucher
                AnkunftBesucher();
            }
            exit(0);
        }else {
            // Vaterprozess muss nichts tun
            sleep(1);
        }
    }
    sem_close(besucher_lock);
    sem_close(wagen_lock);
    sem_close(sitze_lock);
}
