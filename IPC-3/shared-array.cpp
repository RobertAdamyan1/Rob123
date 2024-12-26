
#include "shared-array.h"

shared_array::shared_array(const std::string &name, size_t size)
    : name_(name), size_(size) {
    int shm_fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    if (ftruncate(shm_fd, size * sizeof(int)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    array_ = static_cast<int*>(mmap(nullptr, size * sizeof(int), PROT_READ | PROT_WRITE,
                                     MAP_SHARED, shm_fd, 0));
    if (array_ == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    semaphore_ = sem_open((name + "_sem").c_str(), O_CREAT, 0666, 1);
    if (semaphore_ == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
}

shared_array::~shared_array() {
    munmap(array_, size_ * sizeof(int));

    shm_unlink(name_.c_str());

    sem_close(semaphore_);
    sem_unlink((name_ + "_sem").c_str());
}

int& shared_array::operator[](size_t index) {
    sem_wait(semaphore_);
    int& value = array_[index];
    sem_post(semaphore_);
    return value;
}

const int& shared_array::operator[](size_t index) const {
    sem_wait(semaphore_);
    const int& value = array_[index];
    sem_post(semaphore_);
    return value;
}
