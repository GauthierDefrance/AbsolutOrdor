#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * @file liste.h
 * @brief Interface d'une liste chaînée simple générique.
 *
 * Cette structure constitue la brique de base de tout le simulateur.
 * Grâce à l'utilisation de pointeurs génériques (void *), elle permet de stocker
 * n'importe quel type de donnée (Processus, Quantum, etc.) tout en réutilisant
 * la même logique de manipulation de liste.
 */


/*******************************************
    Structure de données
********************************************/


/**
 * @struct Cellule
 * @brief Maillon d'une liste chaînée.
 *
 * Liste générique chaînée.
 * Chaque cellule contient un pointeur générique `void *data`.
 * La gestion de l'allocation/libération de `data` est à la charge de l'appelant.
 */
typedef struct Cellule {
    void *data;
    struct Cellule *suivant;
} *Liste;


/*******************************************
    Fonction allocation mémoire
********************************************/


/**
 * @brief Alloue une cellule de liste en mémoire dynamique.
 *
 * @return Liste Le pointeur vers la cellule allouée, ou NULL en cas d'échec du malloc.
 *
 * @note La cellule retournée n'est pas initialisée (data et suivant sont indéfinis).
 */
Liste allocListe();


/******************************************
    Fonction de destruction
*******************************************/

/**
 * @brief Détruit récursivement l'intégralité d'une liste.
 *
 * @param liste La tête de la liste à détruire.
 * @param freeData Callback permettant de libérer la mémoire de 'data'.
 */
void destroyListe(Liste liste, void (*freeData)(void *));

/**
 * @brief Supprime l'élément de tête et met à jour le pointeur de liste.
 *
 * @param l Double pointeur vers la tête (pour modification directe).
 * @param freeData Callback de libération de la donnée.
 */
void suppTete(Liste *l, void (*freeData)(void *));

/**
 * @brief Supprime l'élément de queue (O(n)).
 *
 * @param l Double pointeur vers la tête.
 * @param freeData Callback de libération de la donnée.
 */
void suppQueue(Liste *l, void (*freeData)(void *));


/******************************************
    Fonction d'initialisation
******************************************/

/**
 * @brief Initialise une cellule de liste (met le champ suivant à NULL).
 *
 * @param cellule La cellule à initialiser.
 */
void initListe(Liste cellule);

/**
 * @brief Alloue une cellule et y injecte immédiatement une donnée.
 *
 * @param data Pointeur vers la donnée à stocker.
 * @return Liste La cellule prête à l'emploi.
 */
Liste createListe(void *data);

/**
 * @brief Modifie la donnée stockée dans une cellule.
 *
 * @param cellule - cellule cible.
 * @param data - nouveau pointeur à stocker.
 */
void setCelluleData(Liste cellule, void *data);


/******************************************
    Fonction primitive
*******************************************/

/**
 * @brief Accesseur vers le maillon suivant.
 *
 * @param cellule   Le maillon actuel.
 * @return Liste    Le pointeur vers le maillon suivant.
 */
Liste suivantListe(Liste cellule);

/**
 * @brief Localise la dernière cellule de la liste.
 *
 * @param l         La tête de la liste.
 * @return Liste    Le pointeur vers le dernier maillon, ou NULL si la liste est vide.
 *
 * @warning Complexité O(n).
 */
Liste queueListe(Liste l);

/**
 * @brief Vérifie si la liste est vide.
 *
 * @param l La tête de liste à tester.
 * @return true Si la liste est NULL.
 * @return false Si la liste contient au moins un élément.
 */
bool estVideListe(Liste l);

/**
 * @brief Accesseur vers la donnée stockée dans une cellule.
 *
 * @param l La cellule cible.
 * @return void* Le pointeur vers la donnée.
 */
void *donneeListe(Liste l);

/**
 * @brief Calcule le nombre d'éléments présents dans la liste.
 *
 * @param l La tête de la liste.
 * @return int Le nombre de maillons (0 si la liste est vide).
 * @note Complexité O(n).
 */
int tailleListe(Liste l);

/******************************************
    Fonction d'insertion
*******************************************/

/**
 * @brief Insère une donnée en tête de la liste.
 * @param l - adresse du pointeur vers la tête.
 * @param data - pointeur à stocker.
 */
void inserTete(Liste *l, void *data);

/**
 * @brief Insère une donnée en queue de la liste.
 * @param l - adresse du pointeur vers la tête.
 * @param data - pointeur à stocker.
 */
void inserQueue(Liste *l, void *data);


/******************************************
    Fonction d'affichage console
*******************************************/


/**
 * @brief Affiche la liste en utilisant un formatage personnalisé pour la donnée.
 * @param l La liste à parcourir.
 * @param printData Fonction de rappel pour l'affichage de la donnée.
 */
void printListe(Liste l, void (*printData)(void *));