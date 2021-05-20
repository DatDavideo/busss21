#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stud_type {
    int matnum;
    char vorname[20];
    char nachname[20];
    struct stud_type *next;
} stud_type;

typedef struct sort_type {
    stud_type *student;
    struct sort_type *next;
} sort_type;


/* Ist die Datenbank leer?      */
bool is_empty(stud_type const* liste) {
    return !liste;
}


/* Einfuegen eines Elementes
 * 
 * Bekommt einen Zeiger auf einen Zeiger der auf das 1. Element der Liste zeigt
 * Bekommt MatNr, Vorname und Nachname des Studenten der eingefügt werden soll
 *
 * Gibt true für Erfolg zurück
 * Gibt false für einen Fehler zurück
 */
bool enqueue(stud_type** studenten_liste, int matnum, char const vorname[20], char const nachname[20]) {
    /* Hole dynamischen Speicher für den neuen Listen Eintrag */
    stud_type *newElement = (stud_type *)malloc(sizeof(stud_type));
    if(newElement == NULL) {
	perror("failed malloc in enqueue");
	return false;
    }
    /* Befülle den Speicher (vorsicht vor buffer overflows!) */
    newElement->matnum = matnum;
    newElement->next = NULL;
    if(strlcpy(newElement->vorname, vorname, 20) >= 20 || strlcpy(newElement->nachname, nachname, 20) >= 20){
	perror("Error copying strings to new Element");								// shouldn't happen ever but better safe than sorry
	free(newElement);
	return false;
    }
    /* Füge den neuen Eintrag in die Liste ein */
    if(is_empty(*studenten_liste)){
        /* Ist die Liste leer ? */
	newElement->next = NULL;
	*studenten_liste = newElement;
	return true;
    } else {
	/* Sortier den Studenten aufsteigend nach Matrikelnummer ein (matrikelnummern sind einzigartig) */
	stud_type *prev = NULL;
	stud_type *iter = *studenten_liste;
	while(iter->next != NULL){
	    if(iter->matnum < matnum) {
		prev = iter;
		iter = iter->next;
	    } else if(iter->matnum == matnum){
		free(newElement);										// not specified what to do in this case -> fail
		return false;
	    } else {
		newElement->next = iter;
		if(prev == NULL) {
		    *studenten_liste = newElement;
		} else {
		    prev->next = newElement;
		}
		return true;
	    }
	}
	iter->next = newElement;
	return true;
    }
}

/* Löschen eines Elementes. 
 * 
 * Bekommt einen Zeiger auf einen Zeiger der auf das 1. Element der Liste zeigt
 * Bekommt die MatNr des Studenten der zu löschen ist
 *
 * Gibt true für Erfolg zurück
 * Gibt false für einen Fehler zurück
 */
bool dequeue(stud_type** studenten_liste, int matnum) {
    /* Prüfe Randbedingungen */
    if(is_empty(*studenten_liste)) {
	return false;
    }
    /* Was muss passieren wenn das 1. Element gelöscht wird? */
    stud_type *iter = *studenten_liste;
    if((*studenten_liste)->matnum == matnum) {
	stud_type *tmp = iter->next;
	free(iter);
	*studenten_liste = tmp;
	return true;
    }
    /* Finde den Studenten */
    while(iter->next != NULL) {
	/* Lösche den Studenten und gibt den Speicher frei */
	if(iter->next->matnum == matnum) {
	    stud_type *tmp = iter->next->next;
	    free(iter->next);
	    iter->next = tmp;
	    return true;
	} else {
	    iter = iter->next;
	}
    }
    /* Was ist wenn es nicht in der Liste ist? */
    return false;
}

/* Auslesen eines Elementes 
 *
 * Bekommt pointer auf den Listenstart
 * Bekommt MatNr des Studenten der ausgelesen werden soll
 *
 * Schreibt Vorname und Nachname in vorname und nachname
 */
bool get_student(stud_type const* studenten_liste, int matnum, char vorname[20], char nachname[20]) {
    /* Durchsuchen der DB */
    stud_type const* curr = studenten_liste;
    while (curr && curr->matnum < matnum) {
        curr = curr->next;
    }

    if (!curr || curr->matnum != matnum) {
        /* Rückgabewert: Fehler */
        return false;
    } else {
        strncpy(vorname, curr->vorname, 19);
        vorname[19] = '\0';
        strncpy(nachname, curr->nachname, 19);
        nachname[19] = '\0';
        /* Rückgabewert: OK */
        return true;
    }
}

static void test_empty(stud_type const* liste) {
    printf(">>> Test ob die Studentenliste leer ist ...\n");

    if(is_empty(liste)) {
        printf("    Die Studentenliste ist leer \n");
    } else {
        printf("    Die Studentenliste ist nicht leer \n");
    }
}

static void test_get(stud_type const* liste, int matnum) {
    printf(">>> Test, ob die Matrikelnummer %4i bereits erfasst wurde ...\n", matnum);

    char vorname[20];
    char nachname[20];
    if(get_student(liste, matnum, vorname, nachname)) {
        printf("    Matrikelnummer %4i: %s %s\n", matnum, vorname, nachname);
    } else {
        printf("    Matrikelnummer %4i ist unbekannt\n", matnum);
    }
}

static void test_enqueue(stud_type** liste, int matnum, char const* vorname, char const* nachname) {
    printf(">>> Fuege neuen Studenten in die Liste ein: %s %s [%4i] ...\n", vorname, nachname, matnum);
    if(enqueue(liste, matnum, vorname, nachname)) {
        printf("    Matrikelnummer %4i eingefügt\n", matnum);
    } else {
        printf("    Matrikelnummer %4i konnte nicht eingefügt werden\n", matnum);
    }
}

static void test_dequeue(stud_type** liste, int matnum) {
    printf(">>> Loesche die Matrikelnummer %4i ...\n", matnum);

    if(dequeue(liste, matnum)) {
        printf("    Matrikelnummer %4i geloescht\n", matnum);
    } else {
        printf("    Matrikelnummer %4i war nicht erfasst\n", matnum);
    }
}

static void test_dump(stud_type const* liste) {
    printf(">>> Gebe alle erfassten Studenten aus ...\n");
    for(stud_type const* curr = liste; curr; curr = curr->next) {
        printf("    Matrikelnummer %4i: %s %s\n", curr->matnum, curr->vorname, curr->nachname);
    }
}

int cmpVorname(const stud_type *e1, const stud_type *e2) {
    stud_type **a = (stud_type **)e1;
    stud_type **b = (stud_type **)e2;
    return strcmp((*a)->vorname, (*b)->vorname);
}

int cmpNachname(const stud_type *e1, const stud_type *e2) {
    stud_type **a = (stud_type **)e1;
    stud_type **b = (stud_type **)e2;
    return strcmp((*a)->nachname, (*b)->nachname);
}


sort_type **sort_students(stud_type **liste, int cmp(const stud_type *e1, const stud_type *e2)) {
    int listSize = 0;
    for(stud_type *iter = *liste; iter != NULL; iter=iter->next){
	listSize++;
    }
    stud_type *listArray[listSize];
    int i = 0;
    for(stud_type *iter = *liste; iter != NULL; iter=iter->next){
        listArray[i] = iter;
	i++;
    }
    // put all liste entries into array, sort array with qsort, then generate new list from that array
    mergesort(listArray, listSize, sizeof(stud_type *), (int (*)(const void *, const void *))cmp);
    sort_type *prev = NULL;
    for(i = listSize-1; i >= 0; i--) {
	sort_type *elem = (sort_type *)malloc(sizeof(sort_type));
	// memset(elem, 0, sizeof(sort_type));
	elem->student = listArray[i];
	elem->next = prev;
	prev = elem;
	printf("    Matrikelnummer %4i: %s %s\n", elem->student->matnum, elem->student->vorname, elem->student->nachname);     // Doesn't work without this print.
    }
    sort_type **result = &prev;
    return result;
}

/* Test der Listenfunktionen  */
int main(void) {
    /* Initialisierung der Datenbank */
    stud_type* studenten_liste = NULL;

    test_empty(studenten_liste);
    test_enqueue(&studenten_liste, 1234, "Eddard", "Stark");
    test_get(studenten_liste, 1234);
    test_enqueue(&studenten_liste, 5678, "Jon", "Snow");
    test_get(studenten_liste, 1234);
    test_enqueue(&studenten_liste, 9999, "Tyrion", "Lannister");
    test_get(studenten_liste, 1235);
    test_enqueue(&studenten_liste, 1289, "Daenerys", "Targaryen");
    test_dequeue(&studenten_liste, 1234);
    test_empty(studenten_liste);
    test_get(studenten_liste, 5678);
    test_dequeue(&studenten_liste, 9998);
    test_enqueue(&studenten_liste, 1289, "Viserys", "Targaryen");
    test_dequeue(&studenten_liste, 5678);
    test_enqueue(&studenten_liste, 1, "Tywin", "Lannister");
    test_dump(studenten_liste);

    {
        /* Erzeuge sortierte Liste nach Vorname */
	sort_type **sorted = sort_students(&studenten_liste, cmpVorname);
        /* Gebe Liste aus */
	test_dump(studenten_liste);
	printf("Gebe alle sortierten Studenten aus ...\n");
	for(sort_type *iter = *sorted; iter != NULL; iter = iter->next){
	    printf("    Matrikelnummer %4i: %s %s\n", iter->student->matnum,  iter->student->vorname, iter->student->nachname);
	}
        /* Räume erzeugte Liste auf */
	
    }

    {
        /* Erzeuge sortierte Liste nach Nachname */
        /* Gebe Liste aus */
        /* Räume erzeugte Liste auf */
    }

    /* Räume studenten_liste auf */

    return 0;
}
