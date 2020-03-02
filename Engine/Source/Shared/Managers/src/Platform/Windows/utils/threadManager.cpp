#include "mngafx.h"
#include "threadManager.h"

/*
 Each thread consists of,
 * Initialization
 * Main loop
 * Shutdown
*/

namespace Dynamik {
    void ThreadManager::add(Thread* myThread)
    {
        myThreadContainer.push_back(myThread);
    }

    void ThreadManager::run(UI32 index)
    {
        runThread(myThreadContainer[index]);
    }

    void ThreadManager::runThread(Thread* thread)
    {
        auto future = std::async(std::launch::async, [](Thread* thr) {
            thr->init();
            while (!thr->loopEndCommand()) thr->mainLoop();
            thr->shutdown();
            }, thread);
    }
}