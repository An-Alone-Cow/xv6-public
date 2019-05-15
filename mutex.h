struct mutex {
    uint locked;        // Is the lock held?
    struct spinlock lk; // spinlock protecting this mutex

    int mid;            // Mutex ID
    int pid;            // Process holding mutex
    int ownerpid;       // Process that created the mutex
};
