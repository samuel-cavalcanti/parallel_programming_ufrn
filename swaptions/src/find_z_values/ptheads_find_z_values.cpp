
#ifdef ENABLE_PTHREADS
#include "find_z_values.h"
#include "../HJM.h"
// Foi tentado paralelizar o calculo do pdf, porem sem sucesso.

// #include <pthread.h>
// #include "runners/block_range/block_range.h"

// typedef struct WorkerFindZParam
// {
//     FTYPE **pdZ;
//     FTYPE **randZ;
//     int BLOCKSIZE;
//     int iN;
//     int l;
//     BlockRange r;
// } WorkerFindZParam;

// void *worker_find_z(void *arg)
// {

//     auto worker_arg = *((WorkerFindZParam *)arg);

//     auto r = worker_arg.r;
//     auto iN = worker_arg.iN;
//     auto BLOCKSIZE = worker_arg.BLOCKSIZE;
//     auto l = worker_arg.l;
//     auto pdZ = worker_arg.pdZ;
//     auto randZ = worker_arg.randZ;

//     for (int b = r.begin; b < r.end; b++)
//     {
//         for (int j = 1; j < iN; j++)
//         {
//             pdZ[l][BLOCKSIZE * j + b] = CumNormalInv(randZ[l][BLOCKSIZE * j + b]); /* 18% of the total executition time */
//         }
//     }

//     return NULL;
// }

// void find_z_values(FTYPE **pdZ, FTYPE **randZ, int BLOCKSIZE, int iN, int iFactors)
// {

//     auto number_threads = 8;

//     auto threads = new pthread_t[number_threads];

//     pthread_attr_t pthread_custom_attr;
//     pthread_attr_init(&pthread_custom_attr);

//     for (int l = 0; l < iFactors; l++)
//     {

//         for (auto thread_index = 0; thread_index < number_threads; thread_index++)
//         {
//             auto range = find_blocked_range(thread_index, BLOCKSIZE, number_threads);
//             auto workerParams = new WorkerFindZParam{
//                 pdZ,
//                 randZ,
//                 BLOCKSIZE,
//                 iN,
//                 l,
//                 range,
//             };

//             pthread_create(&threads[thread_index], &pthread_custom_attr, worker_find_z, workerParams);
//         }
//         for (auto thread_index = 0; thread_index < number_threads; thread_index++)
//         {
//             pthread_join(threads[thread_index], NULL);
//         }
//     }
// }

// void find_z_values(FTYPE **pdZ, FTYPE **randZ, int BLOCKSIZE, int iN, int iFactors)
// {

//     auto number_threads = 8;

//     auto threads = new pthread_t[number_threads];

//     pthread_attr_t pthread_custom_attr;
//     pthread_attr_init(&pthread_custom_attr);

//     for (int l = 0; l < iFactors; l++)
//     {

//         for (auto thread_index = 0; thread_index < number_threads; thread_index++)
//         {
//             auto range = find_blocked_range(thread_index, BLOCKSIZE, number_threads);
//             auto workerParams = new WorkerFindZParam{
//                 pdZ,
//                 randZ,
//                 BLOCKSIZE,
//                 iN,
//                 l,
//                 range,
//             };

//             pthread_create(&threads[thread_index], &pthread_custom_attr, worker_find_z, workerParams);
//         }
//         for (auto thread_index = 0; thread_index < number_threads; thread_index++)
//         {
//             pthread_join(threads[thread_index], NULL);
//         }
//     }
// }

// serial version
void find_z_values(FTYPE **pdZ, FTYPE **randZ, int BLOCKSIZE, int iN, int iFactors)
{

    for (int l = 0; l < iFactors ; ++l)
    {
        for (int b = 0; b < BLOCKSIZE; b++)
        {
            for (int j = 1; j < iN ; ++j)
            {
                pdZ[l][BLOCKSIZE * j + b] = CumNormalInv(randZ[l][BLOCKSIZE * j + b]); /* 18% of the total executition time */
            }
        }
    }
}

#endif // ENABLE_PTHREADS