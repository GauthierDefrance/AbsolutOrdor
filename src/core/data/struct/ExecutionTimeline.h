#pragma once
#include "processus.h"
#include "liste_tq.h"
#include "data/struct/ProcessusIterator.h"
/**
 * @file ExecutionTimeline.h
 * @brief Interface du journal d'exécution (Timeline).
 *
 * @details Cette structure sert de base de données post-simulation. Elle stocke
 * l'historique complet de chaque processus sous forme de séquences de quanta
 * (UC, ES, Attente) afin de permettre l'analyse des performances et l'affichage
 * chronologique des tâches.
 */


/**
 *Forward declaration pour éviter l'include circulaire
 */
typedef struct ProcessusIterator ProcessusIterator;


/**
 * @struct ExecutionTimeline
 * @brief Conteneur principal de l'historique de simulation.
 *
 * Une Liste Tête-Queue (ListeTQ) contenant des pointeurs vers des structures
 * 'Processus'. Chaque processus ici est une copie dédiée à l'enregistrement
 * de la chronologie.
 */
typedef struct {
    ListeTQ processus; // ListeTQ de Processus*
} ExecutionTimeline;


/*******************************************
    Allocation / Libération
********************************************/


/**
 * @brief Alloue la mémoire pour une nouvelle Timeline.
 *
 * Crée la structure de contrôle et initialise la liste interne des processus.
 *
 * @return ExecutionTimeline* Pointeur vers la timeline vierge, ou NULL en cas d'échec.
 */
ExecutionTimeline *allocTimeline();

/**
 * @brief Libère la structure Timeline et son conteneur de liste.
 *
 * @param tl La timeline à détruire.
 *
 * @warning Par défaut, cette fonction ne libère pas les structures
 * 'Processus' contenues à l'intérieur pour éviter les doubles libérations si
 * l'ordonnanceur les manipule encore.
 */
void destroyTimeline(ExecutionTimeline *tl);


/*******************************************
    Construction
********************************************/


/**
 * @brief Prépare la timeline en créant des copies "vierges" des processus.
 *
 * Parcourt la liste source et utilise `deepCopyProcessusWithoutQuantums`
 * pour chaque processus. Cela permet d'avoir une structure prête à recevoir
 * les nouveaux quanta générés par l'algorithme d'ordonnancement.
 *
 * @param timeline La timeline cible à initialiser.
 * @param liste_tq La liste des processus originaux (souvent issue du parsing CSV).
 */
void initTimelineProcessus(const ExecutionTimeline *timeline, ListeTQ liste_tq);

/**
 * @brief Ajoute un processus (avec son historique complet) à la timeline.
 *
 * @param tl La timeline cible.
 * @param p Pointeur vers le processus à archiver.
 */
void ajouterProcessusTimeline(ExecutionTimeline *tl, Processus *p);


/*******************************************
    Recherche
********************************************/


/**
 * @brief Recherche un processus spécifique dans la timeline.
 *
 * Effectue une recherche par comparaison de chaînes de caractères (nom).
 * Utile pour retrouver la "fiche d'exécution" d'un processus pendant que
 * l'ordonnanceur travaille sur l'original.
 *
 * @param timeline La timeline de recherche.
 * @param it L'itérateur pointant vers le processus de référence.
 * @return Processus* Le pointeur vers le processus archivé, ou NULL si non trouvé.
 */
Processus *getTimelineProcessus(const ExecutionTimeline *timeline, const ProcessusIterator *it);