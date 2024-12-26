
#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H

#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

class shared_array {
public:
    shared_array(const std::string &name, size_t size);
    ~shared_array();

    int& operator[](size_t index);
    const int& operator[](size_t index) const;

private:
    std::string name_;
    size_t size_;
    int* array_;
    sem_t* semaphore_;
};

#endif
