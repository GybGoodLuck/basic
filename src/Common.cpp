#include "Common.h"

void printGLMMatrix(const glm::mat4 matrix) {

    std::cout << "****************" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}