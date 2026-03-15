#include "ConsoleStructPrinter.h"

/* =========================
 * Affichage brut du CSV chargé
 * ========================= */
void afficherCSVCharge(ListeTQ liste) {
    if (!liste) return;

    std::cout << "\n===== PROCESSUS CHARGES (CSV) =====\n";

    std::cout << std::left
              << std::setw(14) << "Nom"
              << std::setw(10) << "Arrivee"
              << "Quantums\n";

    std::cout << std::string(50, '-') << "\n";

    for (Liste c = liste->tete; c; c = c->suivant) {
        auto* p = static_cast<Processus*>(c->data);

        std::cout << std::setw(14) << p->name
                  << std::setw(10) << p->timeArrival;

        for (Liste qc = p->listeTQ->tete; qc; qc = qc->suivant) {
            auto *q = static_cast<Quantum*>(qc->data);

            std::cout << (q->type == UC ? "UC" :
                         (q->type == ES ? "ES" : "W"))
                      << "(" << q->nbQuantum << ") ";
        }

        std::cout << "\n";
    }

    std::cout << std::string(50, '=') << "\n\n";
}


/* =========================
 * Affichage timeline simple
 * ========================= */
void afficherTimeline(const ExecutionTimeline *tl) {
    if (!tl) return;

    std::cout << "\n===== EXECUTION TIMELINE =====\n";

    for (Liste cell = tl->processus->tete; cell; cell = cell->suivant) {

        auto *p = static_cast<Processus*>(cell->data);

        std::cout << std::left << std::setw(12) << p->name
                  << " (t=" << p->timeArrival << ") : ";

        for (Liste qc = p->listeTQ->tete; qc; qc = qc->suivant) {

            auto *q = static_cast<Quantum*>(qc->data);


            std::cout << (q->type == UC ? "UC" :
                         (q->type == ES ? "ES" : "W"))
                      << "(" << q->nbQuantum << ") ";
        }

        std::cout << "\n";
    }

    std::cout << "===============================\n\n";
}


/* =========================
 * Affichage console avec décalage temporel
 * ========================= */
void afficherTimelineAvecDecalage(const ExecutionTimeline *tl) {

    if (!tl) return;

    constexpr int LABEL_WIDTH = 12;
    constexpr int TICK_WIDTH  = 2;

    int tMax = 0;

    for (Liste cell = tl->processus->tete; cell; cell = cell->suivant) {

        auto *p = static_cast<Processus*>(cell->data);

        int t = p->timeArrival;

        for (Liste qc = p->listeTQ->tete; qc; qc = qc->suivant)
            t += static_cast<Quantum*>(qc->data)->nbQuantum;

        if (t > tMax)
            tMax = t;
    }

    std::cout << "\n";

    std::cout << std::string(LABEL_WIDTH + 2, ' ') << "t=";

    for (int t = 0; t <= tMax; t += 5) {

        int pos = t * TICK_WIDTH;

        std::string marker = std::to_string(t);

        std::cout << marker;

        int nextPos = (t + 5) * TICK_WIDTH;
        int printed = marker.size();
        int spaces  = nextPos - pos - printed;

        if (spaces > 0)
            std::cout << std::string(spaces, ' ');
    }

    std::cout << "\n";

    std::cout << std::string(LABEL_WIDTH + 4 + tMax * TICK_WIDTH, '-') << "\n";

    for (Liste cell = tl->processus->tete; cell; cell = cell->suivant) {

        auto *p = static_cast<Processus*>(cell->data);

        std::string nom(p->name);

        if ( nom.size() > static_cast<std::size_t>(LABEL_WIDTH) )
            nom = nom.substr(0, LABEL_WIDTH);

        std::cout << nom
                  << std::string(LABEL_WIDTH - nom.size(), ' ')
                  << ": ";

        std::cout << std::string(p->timeArrival * TICK_WIDTH, ' ');

        for (Liste qc = p->listeTQ->tete; qc; qc = qc->suivant) {

            auto *q = static_cast<Quantum*>(qc->data);


            char sym = (q->type == UC) ? 'C' :
                       (q->type == ES) ? 'E' : 'W';

            for (int i = 0; i < q->nbQuantum; i++) {

                std::cout << sym;

                if constexpr (TICK_WIDTH > 1)
                    std::cout << std::string(TICK_WIDTH - 1, ' ');
            }
        }

        std::cout << "\n";
    }

    std::cout << std::string(LABEL_WIDTH + 4 + tMax * TICK_WIDTH, '-') << "\n";

    std::cout << "  Legende : C=UC  E=ES  W=Wait\n\n";
}