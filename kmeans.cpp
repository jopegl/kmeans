#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

#define N 150 //amount of starting data
#define D 4 //dimensions


void loadData(double points[][D]) {
    ifstream file("iris.data"); //loading file
    string line; //variable to read lines
    int i = 0; //inc

    while (getline(file, line) && (i < N)) {
        stringstream ss(line); //variable that contains each line
        string value; //value in type string

        for (int j = 0; j < D; j++) {
            getline(ss, value, ',');//"cutting" values with "," paramether
            if (!value.empty()) {
                points[i][j] = stod(value); //converting str to double
            }
        }
        i++;
    }
    
    file.close(); //closing file after using it
}

void k_means(double points[][D], int k, int nIterations){
    //initializing variables
    double centroids[100][D];
    int labels[N];
    double newCentroids[100][D] = {};
    int counts[100] = {};
    double currentDist = 0, lowerDist = 0;
    int iLowerDist;
    bool changed;

    //defining first centroids
    for(int i = 0; i < k; i++) {
        int randomIndex = rand() % N;
        for(int j = 0; j < D; j++)
            centroids[i][j] = points[randomIndex][j];
    }

    for (int a = 0; a < nIterations; a++){
        changed = false;
        //defining the centroids related to each point
        for(int i = 0; i < N; i++) {
            iLowerDist = 0;
            lowerDist = 0;
            for(int x = 0; x < D; x++){
                lowerDist += pow(centroids[0][x] - points[i][x], 2); //adding first values to the lower distance
            }
            lowerDist = sqrt(lowerDist);
            for(int j = 1; j < k; j++){
                currentDist = 0;
                for(int x = 0; x < D; x++) {
                    currentDist += pow(centroids[j][x] - points[i][x], 2); //adding values to compare with the lower distance
                }
                currentDist = sqrt(currentDist);
                if(currentDist < lowerDist){ //comparing and changing in case of "true"
                    lowerDist = currentDist;
                    iLowerDist = j;
                }
                currentDist = 0;
            }
            //updating counts and labels with the number of points and index
            counts[iLowerDist]++;

            //checking if label of any point has changed
            if (labels[i] != iLowerDist){
                labels[i] = iLowerDist;
                changed = true;
            }
        }

        //if nothing has changed, breaks the loop
        if(!changed){
            cout << "\033[33mConvergence achieved after " << a + 1 << " iterations.\033[0m" << endl;
            break;
        } 

        //calculating new centroids
        for(int i = 0; i < N; i++)
            for(int x = 0; x < D; x++)
                newCentroids[labels[i]][x] += points[i][x];

        for(int i = 0; i < k; i++)
            for(int x = 0; x < D; x++){
                if (counts[i] != 0)
                    newCentroids[i][x] /= counts[i];
            }

        //copying new centroids to main centroids
        for(int i = 0; i < k; i++)
            for(int x = 0; x < D; x++)
                centroids[i][x] = newCentroids[i][x];
        
        //turning to 0 new centroids and counts
        for(int i = 0; i < k; i++){
            counts[i] = 0;       
            for(int x = 0; x < D; x++)
                newCentroids[i][x] = 0;
        }

    }
    
    //results
    for(int i = 0; i < N; i++) {
        cout << "Modelo " << i << " : " << labels[i] << endl;
    }
    for(int i = 0; i < k; i++) {
        float cont = 0;
        for(int j = 0; j < N; j++){
            if(labels[j] == i) cont++;
        }
        cout << "Procentagem Cluster " << i << ": "<< (cont/N)* 100 << "%" << endl;
    }

}

void runForAllComb(double points[][D], int k, int nIterations) {
    //loop that uses all combinations of data dimensions to use kmeans adapted to two dimensions, and in the end does it with all of them
    for(int i = 0; i < D; i++) {
        for(int j = i+1; j < D; j++) {
            double temp[N][4] = {}; //temp matrix used store temp values of current combination
            for(int x = 0; x < N; x++){
                temp[x][0] = points[x][i];
                temp[x][1] = points[x][j];
            }
            cin.get();
            cout << "\033[32mRunning kmeans for combination " << i << " " << j << ": \033[0m" << endl;
            k_means(temp, k, nIterations);
        }
    }
    cin.get();
    cout << "\033[32mRunning kmeans with all dimensions: \033[0m" << endl;
    k_means(points, k, nIterations);
}

int main()
{
    double points[N][D];
    loadData(points);
    int k, nIterations;
    cout << "Digite o número de clusters: " << endl;
    cin >> k;
    cout << "Enter the number of iterations: " << endl;
    cin >> nIterations;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < D; j++)
            cout << points[i][j] << ' ';
        cout << endl;
    }
            
    runForAllComb(points, k, nIterations);
    cout << "\033[0m";
    
    return 0;
}