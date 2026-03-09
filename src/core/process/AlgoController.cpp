#include "AlgoController.h"


AlgoController& AlgoController::getInstance() {
    static AlgoController instance;
    return instance;
}


