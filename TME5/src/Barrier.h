#pragma once
#include <mutex>
#include <condition_variable>

using namespace std;
namespace pr {

class Barrier{
    public:
        recursive_mutex m;
        int compteur;
        int N;
        condition_variable_any cv;

    Barrier(int N) : N(N),compteur(0){};
    
    void done(){
        unique_lock<recursive_mutex> lg(m);
        compteur++;
        if(compteur == N){
            cv.notify_all();
        }
    }

    void waitFor(){
        unique_lock<recursive_mutex> lg(m);
        while(compteur < N){
            cv.wait(lg);
        }
    }

};

}