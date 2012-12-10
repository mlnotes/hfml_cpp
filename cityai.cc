#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::sort;

typedef vector<float>  vectorf;
typedef vector<vector<float> >  datasetf;
typedef vector<vector<int> >  dataseti;

struct score_item{
	float score;
	vectorf sol;
};

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

void print(vectorf &vec){
	for(int i = 0; i < vec.size(); ++i){
		std::cout << vec[i];
		if(i != vec.size()-1)
			std::cout << ',';
	}
	std::cout << '\n';
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
			total += -100.0 / (vdistance(ss, points[j])+1);
		}
	}
	return total;
}

int annealing(datasetf &points, 
				vectorf &sol,
				datasetf &record,
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

		record.push_back(vectorf(3));
		record[record.size()-1][0] = T;
		record[record.size()-1][1] = curcost;
		record[record.size()-1][2] = bestcost;

		T *= cool;
	}
	sol = bestsol;
	return 1;
}

int sort_compare(score_item *a, score_item *b){
	return (a->score < b->score);
}

int genetic(datasetf &points, vectorf &sol, datasetf &record, float step=5,
			int popsize=50, float mutprob=0.2, float elite=0.2,
			int iterations=100){
	
	// random gen solutions
	score_item **scores = new score_item*[popsize];
	scores[0] = new score_item();
	scores[0]->sol = sol;
	scores[0]->score = cost(points, sol);

	for(int i = 1; i < popsize; ++i){
		scores[i] = new score_item();
		for(int j = 0; j < sol.size(); ++j){
			scores[i]->sol.push_back(random()%1000);
		}
		scores[i]->score = cost(points, scores[i]->sol);
	}

	int elitesize = popsize * elite;
	score_item *best = scores[0];	
	for(int i = 0; i < iterations; ++i){
		// sort the solutions according to cost asc
		sort(scores, scores+popsize, sort_compare);

		if(i > 10 && best->score == scores[0]->score)
			break;
		else if(best->score > scores[0]->score)
			best = scores[0];

		record.push_back(vectorf(3));
		record[record.size()-1][0] = i;
		record[record.size()-1][1] = scores[0]->score;
		record[record.size()-1][2] = best->score;
		// get the top elite and generate the rest by muating and crossover
		for(int j = elitesize; j < popsize; ++j){
			if((random()%1000)/1000.0 < mutprob){
				// gen a solution by mutating
				int index = random() % sol.size();
				if(random()%1000 < 500)
					scores[j]->sol[index] += step;
				else
					scores[j]->sol[index] -= step;
				scores[j]->score = cost(points, scores[j]->sol);
			}else{
				// gen a solution by crossover
				int c1 = random() % j;
				int c2 = random() % j;
				int middle = random() % sol.size();
				for(int k = 0; k < middle; ++k)
					scores[j]->sol[k] = scores[c1]->sol[k];
				for(int k = middle; k < sol.size(); ++k)
					scores[j]->sol[k] = scores[c2]->sol[k];

				scores[j]->score = cost(points, scores[j]->sol);
			}
		}
	}

	sol = best->sol;
	// free memory
	for(int i = 0; i < popsize; ++i){
		delete scores[i];
	}
	delete [] scores;

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

	datasetf record;
//	annealing(points, sol, record);
	genetic(points, sol, record);

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

	std::cout << record.size() << '\n';
	print(record);
}
