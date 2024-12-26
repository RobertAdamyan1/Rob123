
#include "shared-array.h"
#include <iostream>
#include <unistd.h>

int main(void) {
    shared_array array("shared_array_name", 100);

    while (true) {
        for (size_t i = 0; i < 100; ++i) {
            array[i] = i * 2;
            std::cout << "[First] Set array[" << i << "] = " << array[i] << std::endl;
        }
        sleep(1);
    }

    return 0;
}
