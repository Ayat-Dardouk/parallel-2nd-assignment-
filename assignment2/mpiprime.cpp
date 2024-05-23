#include <iostream>
#include <cmath>
#include <vector>
#include <mpi.h>
#include <sys/time.h>
using namespace std;
/*double GetWallTime() {
    struct timeval tp;
    int rtn = gettimeofday(&tp, NULL);
    return ((double) tp.tv_sec + (1.e-6)*tp.tv_usec);
}*/

bool isPrime(long number) {
    if (number <= 1) {
        return false;
    } else if (number <= 3) {
        return true;
    } else if (number % 2 == 0) {
        return false;
    } else {
        for (long i = 3; i <= sqrt(number); i += 2) {
            if (number % i == 0) {
                return false;
            }
        }
    }
    return true;
}

void findPrimesInRange(long start, long end, vector<long>& primes) {
    for (long i = start; i <= end; i++) {
        if (isPrime(i)) {
            primes.push_back(i);
        }
    }
}

int main(int argc, char **argv) {
//double starttime = GetWallTime();
    MPI_Init(&argc, &argv);
    double starttime = MPI_Wtime(); 
    
    int rank;       // Store the rank of the current process
    int size;       // Store the total number of processes
    long n;         // Number up to which to find prime numbers

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        cout << "Enter a number to find prime numbers up to: ";
        cin >> n;
    }

    // Broadcast the number 'n' to all processes
    MPI_Bcast(&n, 1, MPI_LONG, 0, MPI_COMM_WORLD);

    long range = (n + 1) / size;
    long start = rank * range;
    long end = (rank == size - 1) ? n : start + range;

    vector<long> localPrimes;
    findPrimesInRange(start, end, localPrimes);

    // Print each process's prime numbers separately
    cout << "Process " << rank << " primes: ";
    for (long prime : localPrimes) {
        cout << prime << " ";
    }
    cout << endl<<endl;

    if (rank == 0) {
        vector<long> allPrimes;
        allPrimes.insert(allPrimes.end(), localPrimes.begin(), localPrimes.end());

        for (int i = 1; i < size; i++) {
            int recvCount;
            MPI_Recv(&recvCount, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            vector<long> recvPrimes(recvCount);
            MPI_Recv(recvPrimes.data(), recvCount, MPI_LONG, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            allPrimes.insert(allPrimes.end(), recvPrimes.begin(), recvPrimes.end());
            
          
        }

        cout << "Prime numbers up to " << n << ": ";
        for (long prime : allPrimes) {
            cout << prime << " ";
        }
        cout << endl;
    } else {
        int localCount = localPrimes.size();
        MPI_Send(&localCount, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(localPrimes.data(), localCount, MPI_LONG, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
  
    //double finish = GetWallTime();
    //cout <<"process "<<rank <<"  Elapsed time = " << finish-start << endl;
     if (rank == 0) {
        double finish = MPI_Wtime(); // Stop the timer
        cout << "Elapsed time for all processes: " << finish-starttime << " seconds" << endl;
    }
    
    return 0;
}

