/**
* @file data.c
 * @brief Implémentation des fonctions de gestion du type générique Data.
 */


#include "data.h"


/*******************************************
    Fonction allocation mémoire
********************************************/
/**
 * @brief Alloue la mémoire pour une Data et son contenu interne selon le type.
 * @note  Alloue d'abord la structure Data, puis le pointeur interne
 *        (Processus* ou Resultat*) via allocMemProcessus() ou allocMemResultat().
 *        En cas d'échec de l'allocation interne, libère Data et retourne NULL.
 * @param type - Le type de donnée à allouer (TYPE_PROCESSUS ou TYPE_RESULTAT).
 * @return Data* - Pointeur vers la Data allouée, NULL en cas d'échec.
 */
Data* allocMemData(TypeDonnee type) {
    Data *data = (Data *) malloc(sizeof(Data));

    if ( data == NULL ) {
        fprintf(stderr, "Erreur : echec de l'allocation memoire\n");
        return NULL;
    }

    switch (type) {

        case TYPE_PROCESSUS :
            data->content.processus = allocMemProcessus();

            if ( data->content.processus == NULL ) {
                fprintf(stderr, "Erreur : echec de l'allocation memoire\n");
                free(data);
                return NULL;
            }
            break;

        case TYPE_RESULTAT:
            data->content.resultat = allocMemResultat();

            if ( data->content.resultat == NULL ) {
                fprintf(stderr, "Erreur : echec de l'allocation memoire\n");
                free(data);
                return NULL;
            }
            break;

        default:
            fprintf(stderr, "Erreur : type de donnee inconnu\n");
            free(data);
            return NULL;
    }

    data->type = type;
    return data;

}



/*******************************************
    Fonction libération mémoire
********************************************/
/**
 * @brief Libère la mémoire du contenu interne d'une Data selon son type.
 * @note  Ne libère pas la structure Data elle-même — uniquement son contenu interne.
 *        Pour libérer complètement une Data allouée dynamiquement,
 *        appelez free (data) après cette fonction.
 * @param data - Pointeur vers la Data dont le contenu est à libérer.
 */
void libMemData(Data *data) {
    if (data == NULL) return;

    switch (data->type) {
        case TYPE_PROCESSUS :
            libMemProcessus(&data->content.processus);
            break;
        case TYPE_RESULTAT :
            libMemResultat(&data->content.resultat);
            break;
    }
}



/******************************************
    Fonction d'initialisation
******************************************/
/**
 * @brief Initialise le contenu interne d'une Data à ses valeurs par défaut.
 * @note  Délègue à initProcessus() ou initResultat() selon le type.
 * @param data - Pointeur vers la Data à initialiser.
 */
void initData(const Data *data) {
    if (data == NULL) return;
    switch (data->type) {
        case TYPE_PROCESSUS :
            initProcessus(data->content.processus);
            break;
        case TYPE_RESULTAT :
            initResultat(data->content.resultat);
            break;
    }
}



/******************************************
    Fonction de création de donnée
******************************************/
/**
 * @brief Alloue et initialise une Data à partir d'arguments variadiques.
 * @note  Pour TYPE_PROCESSUS, attend : (const char* name, int arrival, int quantum).
 *        Pour TYPE_RESULTAT,  attend : (int donnée).
 * @param type - Le type de donnée à créer.
 * @param ...  - Arguments variadiques selon le type.
 * @return Data* - Pointeur vers la Data créée, NULL en cas d'échec.
 */
Data* buildData(TypeDonnee type, ...) {
    va_list args;
    va_start(args, type);
    Data* d = allocMemData(type); // Alloue la structure ET le content interne
    if (!d) { va_end(args); return NULL; }

    if (type == TYPE_PROCESSUS) {
        char* name = va_arg(args, char*);
        int arrival = va_arg(args, int);
        int quantum = va_arg(args, int);

        // Sécurité : Vérifie que d->content.processus n'est pas NULL
        if (d->content.processus) {
            strncpy(d->content.processus->name, name, NBMAXCHAR - 1);
            d->content.processus->time_arrival = arrival;
            d->content.processus->nb_quantum = quantum;
        }
    }
    if (type == TYPE_RESULTAT) {
        int val = va_arg(args, int);
        if (d->content.resultat) {
            d->content.resultat->donnee = val;
        }
    }

    va_end(args);
    return d;
}

/**
 * @brief Crée une Data de type TYPE_PROCESSUS en copiant le contenu d'un Processus source.
 * @note  allocMemData alloue la structure Data ET le pointeur interne processus.
 *        Le contenu de src est copié dans l'espace alloué via déréférencement.
 * @param src - Pointeur vers le Processus source à copier.
 * @return Data* - Pointeur vers la Data créée, NULL en cas d'échec.
 */
Data* createDataProcessus(const Processus *src) {
    if (src == NULL) return NULL;

    // allocMemData(TYPE_PROCESSUS) alloue la structure Data
    // ET le pointeur interne data->content.processus
    Data* data = allocMemData(TYPE_PROCESSUS);
    if (data == NULL) return NULL;

    // On copie le contenu de src vers l'espace alloué dans Data
    *(data->content.processus) = *src;

    return data;
}


/**
 * @brief Crée une Data de type TYPE_RESULTAT en copiant le contenu d'un Resultat source.
 * @note  allocMemData alloue la structure Data ET le pointeur interne resultat.
 *        Le contenu de src est copié dans l'espace alloué via déréférencement.
 * @param src - Pointeur vers le Resultat source à copier.
 * @return Data* - Pointeur vers la Data créée, NULL en cas d'échec.
 */
Data* createDataResultat(const Resultat *src) {
    if (src == NULL) return NULL;

    Data* data = allocMemData(TYPE_RESULTAT);
    if (data == NULL) return NULL;

    *(data->content.resultat) = *src;

    return data;
}


/**
 * @brief Crée une copie d'une structure Data.
 * @note  Alloue une nouvelle Data et duplique son contenu selon le type
 *        (TYPE_PROCESSUS ou TYPE_RESULTAT).
 * @param src - Pointeur vers la Data source à copier.
 * @return Pointeur vers la nouvelle Data copiée, ou NULL en cas d'erreur
 *         d'allocation ou si src est NULL.
 */
Data* deepCopyData(const Data *src) {
    if (!src) return NULL;
    Data *dst = (Data*) malloc(sizeof(Data));
    if (!dst) return NULL;
    dst->type = src->type;
    switch (src->type) {

        case TYPE_PROCESSUS:
            dst->content.processus = allocMemProcessus();
            if (!dst->content.processus) {
                free(dst);
                return NULL;
            }
            *(dst->content.processus) = *(src->content.processus); // copie struct
            break;

        case TYPE_RESULTAT: dst->content.resultat = allocMemResultat();
            if (!dst->content.resultat) {
                free(dst);
                return NULL;
            }
            *(dst->content.resultat) = *(src->content.resultat); // copie struct
            break;

    }
    return dst;
}



/******************************************
    Fonction primitive
*******************************************/
/**
 * @brief Affiche le contenu d'une Data dans la sortie standard.
 * @note  Délègue à traiterProcessus() ou traiterResultat() selon le type.
 * @param data - Pointeur vers la Data à afficher.
 */
void traiterData(const Data *data) {
    if ( data == NULL ) {
        printf("Le Data vide est vide\n");
        return;
    }

    switch (data->type) {
        case TYPE_PROCESSUS :
            traiterProcessus(data->content.processus);
            break;
        case TYPE_RESULTAT :
            traiterResultat(data->content.resultat);
            break;
    }
}