#pragma once
#include "data/struct/liste_tq.h"
#include "data/struct/ProcessusIterator.h"
#include "data/struct/file.h"
#include "data/struct/ExecutionTimeline.h"
/**
 * @file algo_commun.h
 * @brief Fonctions utilitaires partagées pour les algorithmes d'ordonnancement.
 */


/**
 * @brief Prépare la liste des processus en les triant par temps d'arrivée.
 *
 * @param ltq Liste originale des processus.
 * @return ListeTQ Nouvelle liste triée (les processus ne sont pas dupliqués, seulement les pointeurs).
 */
ListeTQ trieListe(ListeTQ ltq);


/**
 * @brief Recherche et extrait le processus le plus court de la file.
 *
 * Sélectionne le processus ayant le temps de calcul (UC) restant le plus faible.
 * En cas d'égalité, le processus arrivé le plus tôt est privilégié (FIFO break-tie).
 *
 * @param f Pointeur vers la file de processus.
 * @return ProcessusIterator* Le processus sélectionné (retiré de la file).
 */
ProcessusIterator* retirerMinTempsUC(const File* f);


/**
 * @brief Simule l'exécution d'un cycle CPU pour le processus actif.
 *
 * Gère la transition vers l'état suivant (E/S ou Terminé) si le bloc de calcul est fini.
 *
 * @param[in,out] surLeCPU Pointeur vers le processus sur le CPU (mis à NULL si libéré).
 * @param[in,out] fileES File où envoyer le processus s'il entame une E/S.
 * @param[in] resultat Chronologie pour l'enregistrement de l'activité.
 * @param[in,out] nbProcessusTraiter Compteur global de processus terminés.
 */
void traiterUC(ProcessusIterator** surLeCPU, const File *fileES, const ExecutionTimeline *resultat, int *nbProcessusTraiter);