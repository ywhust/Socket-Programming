// Example code for limiting the number of "active processes" by fork()
// shared memory part is copied from https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c
// 

#include <iostream>
#include <unistd.h>
#include <sys/mman.h>

void* create_shared_memory(size_t size) {
    // Our memory buffer will be readable and writable:
    int protection = PROT_READ | PROT_WRITE;

    // The buffer will be shared (meaning other processes can access it), but
    // anonymous (meaning third-party processes cannot obtain an address for it),
    // so only this process and its children will be able to use it:
    int visibility = MAP_ANONYMOUS | MAP_SHARED;

    // The remaining parameters to `mmap()` are not important for this use case,
    // but the manpage for `mmap` explains their purpose.
    return mmap(NULL, size, protection, visibility, 0, 0);
}

int *num_process;

void child(int id) {
    std::cout << "Current number of active processes: " << *num_process << std::endl;

    std::cout << "I am process " << id << ", I will terminate after 30s" << std::endl;
    sleep(30);
    std::cout << "I (process " << id << ") terminates" << std::endl;
    -- (*num_process);
}

void child_(int id) {
    std::cout << "Current number of active processes: " << *num_process << std::endl;
    std::cout << "Potential process " << id <<
              ": Sorry! Number of active processes exceeds, cannot create more" << std::endl;
    -- (*num_process);
}

void run()
{

    const int MAX_NUM_PROC = 3;

    num_process = (int *)create_shared_memory(sizeof(int));

    while (1) {
        int id = 0;
        std::cout << "Enter an id: " << std::endl;
        std::cin >> id;
        if (id < 0) {
            perror("accept");
        } else {
            int pid = fork();
            if (pid == 0) {// child process
                if (*num_process > MAX_NUM_PROC)
                    child_(id);
                else
                    child(id);
                exit(0);
            }
            ++ (*num_process);
        }
    }
}

int
main(int c, char **v)
{
    run();
    return 0;
}