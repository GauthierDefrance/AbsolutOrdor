/**
* @file processus.c
 * @brief Implémentation des fonctions de gestion des processus.
 */


#include "processus.h"


/**
 * @brief Alloue un bloc mémoire pour un processus.
 *
 * Sécurise l'allocation en vérifiant le retour du système.
 */
Processus* allocMemProcessus() {
    Processus *p = malloc(sizeof(Processus));
    if (!p) {
        fprintf(stderr, "Erreur : echec allocation Processus\n");
        return NULL;
    }
    return p;
}


/**
 * @brief Libération en cascade (Deep Free).
 *
 * Ne se contente pas de libérer le pointeur Processus*, mais parcourt
 * toute la ListeTQ pour libérer chaque structure Quantum individuellement.
 */
void libMemProcessus(Processus *p) {
    if (p == NULL) return;
    destroyLTQ(p->listeTQ, free);
    free(p);
}


/**
 * @brief Initialisation par défaut.
 *
 * Prépare un processus "propre" avec une chaîne vide et une liste
 * de quanta allouée et prête à recevoir des données.
 */
void initProcessus(Processus *processus) {
    if (processus==NULL) return;

    processus->name[0] = '\0';
    processus->timeArrival = 0;

    processus->listeTQ = allocMemLTQ();
    initLTQ(processus->listeTQ);
}


/**
 * @brief Accesseur sécurisé pour le nom du processus.
 *
 * Permet de récupérer l'identifiant textuel du processus sans exposer
 * directement la structure interne. Retourne NULL si le pointeur est invalide.
 *
 * @param p Pointeur vers le processus.
 * @return const char* Chaîne de caractères représentant le nom.
 */
const char *processusName(const Processus *p) {
    if (p==NULL) return NULL;
    return p->name;
}


/**
 * @brief Récupère l'horloge d'arrivée du processus dans le système.
 *
 *  Cette valeur est utilisée par les ordonnanceurs pour savoir à partir de quel
 * "tick" le processus doit être injecté dans la Ready Queue.
 *
 * @param p Pointeur vers le processus.
 * @return int Le temps d'arrivée (ou -1 si le pointeur est NULL).
 */
int getTimeArrival(const Processus *p) {
    if (p==NULL) return -1;
    return p->timeArrival;
}


/**
 * @brief Accesseur pour la liste des cycles de vie (Quanta) du processus.
 *
 * Fournit l'accès à la ListeTQ contenant la séquence alternée des phases
 * de calcul (UC) et d'entrées/sorties (ES).
 *
 * @param processus Pointeur vers le processus.
 * @return ListeTQ La liste chaînée des quanta associée.
 */
ListeTQ getListeTQProcessus(Processus *processus) {
    if (!processus) return NULL;
    return processus->listeTQ;
}


/**
 * @brief Mécanisme de copie profonde (Deep Copy).
 *
 * C'est une fonction essentielle pour la robustesse : elle alloue de nouveaux
 * blocs mémoire pour chaque quantum de la source. Ainsi, si l'on modifie
 * la copie (ex: ajout de temps d'attente), l'original reste intact.
 */
Processus *deepCopyProcessus(Processus *src) {
    if (!src) return NULL;

    Processus *copy = allocMemProcessus();
    if (!copy) return NULL;
    initProcessus(copy);

    snprintf(copy->name, NBMAXCHAR, "%s", src->name);
    copy->timeArrival = src->timeArrival;

    for (Liste c = src->listeTQ->tete; c != NULL; c = c->suivant) {
        Quantum *qSrc = (Quantum *)c->data;

        Quantum *qCopy = malloc(sizeof(Quantum));
        if (!qCopy) {
            libMemProcessus(copy);
            return NULL;
        }

        qCopy->type      = qSrc->type;
        qCopy->nbQuantum = qSrc->nbQuantum;

        inserQueueLTQ(copy->listeTQ, qCopy);
    }

    return copy;
}


/**
 * @brief Crée une copie partielle du processus (identité uniquement).
 *
 * Contrairement à la copie profonde intégrale, cette fonction duplique le nom
 * et la date d'arrivée mais initialise une liste de quanta vide. Elle est
 * particulièrement utile pour créer les structures de résultats dans la
 * chronologie d'exécution (ExecutionTimeline).
 *
 * @param src Processus source à copier.
 * @return Processus* Nouveau processus alloué (sans quanta).
 */
Processus *deepCopyProcessusWithoutQuantums(Processus *src) {
    if (!src) return NULL;

    Processus *copy = allocMemProcessus();
    if (!copy) return NULL;
    initProcessus(copy);

    snprintf(copy->name, NBMAXCHAR, "%s", src->name);
    copy->timeArrival = src->timeArrival;

    return copy;
}


/**
 * @brief Affiche une représentation textuelle du processus dans la console.
 *
 * Utilise un formatage clair pour visualiser le nom, la date d'arrivée et
 * l'intégralité de la séquence de quanta. Cette fonction est essentielle pour
 * le débogage et la vérification du bon chargement des données CSV.
 *
 * @param p Pointeur vers le processus à afficher.
 */
void afficherProcessus(Processus *p) {
    if (!p) {
        printf("Processus NULL\n");
        return;
    }

    printf("[PROCESSUS] Nom : %s | Arrivee : %d\n",
           p->name,
           p->timeArrival);

    printf("  Quantum : ");

    Liste courant = p->listeTQ->tete;
    while (courant) {
        Quantum *q = courant->data;
        printf("[%s:%d] ",
               q->type == UC ? "UC" :
               q->type == ES ? "ES" : "W",
               q->nbQuantum);

        courant = courant->suivant;
    }

    printf("\n");
}


/**
 * @brief Alloue et initialise dynamiquement un bloc Quantum.
 *
 * Encapsule l'allocation mémoire brute et l'assignation des valeurs de base.
 * Utilisé principalement lors du parsing CSV ou lors de la génération de la timeline.
 *
 * @param type Nature de l'opération (UC, ES ou W).
 * @param nbQuantum Durée de l'opération en unités de temps.
 * @return Quantum* Pointeur vers le quantum alloué.
 */
Quantum *allocQuantum(enum OperationProcessus type, int nbQuantum) {
    Quantum *q = malloc(sizeof(Quantum));
    if (!q) {
        fprintf(stderr, "Erreur : echec allocation Quantum\n");
        return NULL;
    }
    q->type      = type;
    q->nbQuantum = nbQuantum;
    return q;
}


/**
 * @brief Stratégie d'insertion intelligente (Merge).
 *
 * Cette fonction optimise la mémoire. Au lieu d'avoir [UC:1, UC:1, UC:1],
 * elle détecte que le type est identique et transforme la séquence en [UC:3].
 * Cela rend la Timeline d'exécution beaucoup plus lisible.
 */
void pushOrMergeOperationProcessus(ListeTQ liste, enum OperationProcessus type, int n) {

    if (!liste || !liste->tete) {
        inserQueueLTQ(liste, allocQuantum(type, n));
        return;
    }

    Liste last = liste->tete;
    while (last->suivant)
        last = last->suivant;

    Quantum *q = last->data;

    if (q->type == type) {
        q->nbQuantum += n;
    } else {
        inserQueueLTQ(liste, allocQuantum(type, n));
    }
}