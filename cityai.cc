#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <vector>

using std::vector;

typedef vector<float>  vectorf;
typedef vector<vector<float> >  datasetf;
typedef vector<vector<int> >  dataseti;

void print(datasetf &data){
	for(int i = 0; i < data.size(); ++i){
		for(int j = 0; j < data[i].size(); ++j){
			std::cout << data[i][j];
			if(j != data[i].size()-1)
				std::cout << ',';
		}
		std::cout << '\n';
	}
}

void print(datasetf &data, dataseti &index){
	for(int i = 0; i < index.size(); ++i){
		std::cout << index[i].size() << '\n';
		for(int j = 0; j < index[i].size(); ++j){
			for(int k = 0; k < data[index[i][j]].size(); ++k){
				std::cout << data[index[i][j]][k];
				if(k != data[index[i][j]].size()-1)
					std::cout << ',';
			}
			std::cout << '\n';
		}
	}
}

float vdistance(vectorf &v1, vectorf &v2){
	int length = v1.size();
	if(length > v2.size())
		length = v2.size();

	float total = 0;
	for(int i = 0; i < length; ++i)
		total += pow(v1[i]-v2[i], 2);

	return sqrt(total);

}

int equal(datasetf &a, datasetf &b){
	if(a.size() != b.size())
		return 0;
	else if(a.size() == 0 && b.size() == 0)
		return 1;

	int size = a.size();
	int ds = a[0].size();
	
	for(int i = 0; i < size; ++i){
		for(int j = 0; j < ds; ++j){
			if(a[i][j] != b[i][j])
				return 0;
		}
	}
	return 1;
}

datasetf random_points(int num){
	datasetf points(num);
	for(int i = 0; i < num; ++i){
		for(int j = 0; j < 2; ++j)
			points[i].push_back(rand()%1000);
	
	}
	return points;
}

int kmeans(datasetf &points, 
			datasetf &centers, 
			dataseti &clusters,
			int k = 8, int iterations = 100){

	// get range of each dimension
	int ds = points[0].size();
	datasetf ranges(ds);
	for(int i = 0; i < ds; ++i){
		float max = points[0][i];
		float min = points[0][i];
		for(int j = 1; j < points.size(); ++j){
			if(points[j][i] > max)
				max = points[j][i];
			else if(points[j][i] < min)
				min = points[j][i];
		}
		ranges[i].push_back(min);
		ranges[i].push_back(max);
	}
	
	// random a center
	centers.resize(k);
	for(int i = 0; i < k; ++i){
		centers[i].clear();
		for(int j = 0; j < ds; ++j){
			centers[i].push_back(
				(rand()%100)/1000.0 * 
					(ranges[j][1]-ranges[j][0])+
					ranges[j][0]);
		}
	}

	datasetf newcenters(k);
	while(iterations > 0){
		iterations -= 1;
		
		clusters.clear();
		newcenters.clear();
		clusters.resize(k);
		newcenters.resize(k);
		// init each dimension of new centers to 0
		for(int i = 0; i < k; ++i){
			for(int j = 0; j < ds; ++j){
				newcenters[i].push_back(0);
			}
		}
	
		
		for(int i = 0; i < points.size(); ++i){
			float minVal = vdistance(points[i], centers[0]);
			int minIndex = 0;
			for(int j = 1; j < k; ++j){
				float curVal = vdistance(points[i], centers[j]);
				if(curVal < minVal){
					minVal = curVal;
					minIndex = j;
				}
			}
			clusters[minIndex].push_back(i);
			for(int j = 0; j < ds; ++j)
				newcenters[minIndex][j] += points[i][j];
		}

		for(int i = 0; i < k; ++i){
			int length = clusters[i].size();
			if(length == 0)
				length = 1;

			for(int j = 0; j < ds; ++j)
				newcenters[i][j] /= length;
		}

		if(equal(centers, newcenters))
			break;
		else
			centers = newcenters;
	}
	std::cout << iterations << '\n';
	return 0;
}

float cost(datasetf &points, vectorf &sol){
	float total = 0.0;
	for(int i = 0; i < sol.size()/2; ++i){
		vectorf ss;
		ss.push_back(sol[2*i]);
		ss.push_back(sol[2*i+1]);
		for(int j = 0; j < points.size(); ++j){
			total += -1000.0 / vdistance(ss, points[j]);
		}
	}
	return total;
}

int annealing(datasetf &points, 
				vectorf &sol,
				datasetf &annel,
				float step = 5.0,
				float T=10000.0, 
				float cool=0.95){

	float bestcost = cost(points, sol);
	vectorf bestsol = sol;

	while(T > 0.1){
		int index = rand() % sol.size();
		float change = step;
		if(rand()%10 < 5)
			change = -step;
	
		bestsol[index] += change;
		float curcost = cost(points, bestsol);
		if(curcost < bestcost || (rand()%1000)/1000.0 < exp((bestcost-curcost)/T))
			bestcost = curcost;
		else
			bestsol[index] -= change;

		annel.push_back(vectorf(3));
		annel[annel.size()-1][0] = T;
		annel[annel.size()-1][1] = curcost;
		annel[annel.size()-1][2] = bestcost;

		T *= cool;
	}
	sol = bestsol;
	return 1;
}

int genetic(datasetf &points, vectorf &sol){

	return 1;
}

int main(){
	srand(time(0));
	datasetf points = random_points(10000);
	datasetf centers;
	dataseti clusters;

	kmeans(points, centers, clusters, 8, 100);
	
	vectorf sol;
	for(int i = 0; i < centers.size(); ++i){
		for(int j = 0; j < centers[i].size(); ++j){
			sol.push_back(centers[i][j]);
		}
	}
	datasetf annel;
	annealing(points, sol, annel);

	datasetf bestsol(sol.size()/2);
	for(int i = 0; i < sol.size()/2; ++i){
		bestsol[i].push_back(sol[2*i]);
		bestsol[i].push_back(sol[2*i+1]);
	}

	std::cout << clusters.size() << '\n';
	print(points, clusters);

	std::cout << centers.size() << '\n';
	print(centers);

	std::cout << bestsol.size() << '\n';
	print(bestsol);

	std::cout << annel.size() << '\n';
	print(annel);
}
