// #define ENABLE_PTHREADS

#ifdef ENABLE_PTHREADS
#include "find_z_values.h"
#include "HJM/HJM.h"
// // Foi tentado paralelizar o calculo do pdf, porem sem sucesso.

// #include <pthread.h>
// #include "runners/block_range/block_range.h"
// #include <sys/sysinfo.h>

// typedef struct WorkerFindZParam
// {
//     FTYPE **pdZ;
//     FTYPE **randZ;
//     int BLOCKSIZE;
//     int iN;
//     BlockRange r;
// } WorkerFindZParam;

// void *worker_find_z(void *arg)
// {

//     auto worker_arg = *((WorkerFindZParam *)arg);

//     auto r = worker_arg.r;
//     auto iN = worker_arg.iN;
//     auto BLOCKSIZE = worker_arg.BLOCKSIZE;
//     auto pdZ = worker_arg.pdZ;
//     auto randZ = worker_arg.randZ;

//     for (int i = r.begin; i < r.end; i++)
//     {
//         auto l = i / BLOCKSIZE;
//         auto b = i % BLOCK_SIZE;

//         for (int j = 1; j < iN; ++j)
//         {
//             pdZ[l][BLOCKSIZE * j + b] = CumNormalInv(randZ[l][BLOCKSIZE * j + b]); /* 18% of the total executition time */
//         }
//     }

//     return NULL;
// }

// void find_z_values(FTYPE **pdZ, FTYPE **randZ, int BLOCKSIZE, int iN, int iFactors)
// {

//     auto number_threads = get_nprocs();

//     auto threads = new pthread_t[number_threads];

//     pthread_attr_t pthread_custom_attr;
//     pthread_attr_init(&pthread_custom_attr); //

    

//     auto input_size = iFactors * BLOCKSIZE;

//     for (auto thread_index = 0; thread_index < number_threads; thread_index++)
//     {
//         auto range = find_blocked_range(thread_index, input_size, number_threads);

//         auto workerParams = new WorkerFindZParam{
//             pdZ,
//             randZ,
//             BLOCKSIZE,
//             iN,
//             range,
//         };
//         pthread_create(&threads[thread_index], &pthread_custom_attr, worker_find_z, workerParams);
//     }

//        for (auto thread_index = 0; thread_index < number_threads; thread_index++)
//     {
//         pthread_join(threads[thread_index], NULL);
//     }
// }

// serial version
void find_z_values(FTYPE **pdZ, FTYPE **randZ, int BLOCKSIZE, int iN, int iFactors)
{

    for (int l = 0; l < iFactors; ++l)
    {
        for (int b = 0; b < BLOCKSIZE; b++)
        {
            for (int j = 1; j < iN; ++j)
            {
                pdZ[l][BLOCKSIZE * j + b] = CumNormalInv(randZ[l][BLOCKSIZE * j + b]); /* 18% of the total executition time */
            }
        }
    }
}

#endif // ENABLE_PTHREADS