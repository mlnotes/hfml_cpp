#!/usr/bin/python
# -*- coding: UTF-8 -*-

import matplotlib.pyplot as plt

def read_file(filename):
	f = open(filename)
	iterations = int(f.readline().strip())

	clusters = []
	centers = []
	sol = []
	anneal = []

	# read clusters
	k = int(f.readline().strip())
	for i in range(k):
		num = int(f.readline().strip())
		clusters.append([])
		for j in range(num):
			parts = f.readline().strip().split(',')
			clusters[i].append((float(parts[0]), float(parts[1])))
	
	# read centers
	k = int(f.readline().strip())
	for i in range(k):
		parts = f.readline().strip().split(',')
		centers.append((float(parts[0]), float(parts[1])))

	# read sol
	k = int(f.readline().strip())
	for i in range(k):
		parts = f.readline().strip().split(',')
		sol.append((float(parts[0]), float(parts[1])))
	
	# read anneal
	k = int(f.readline().strip())
	for i in range(k):
		parts = f.readline().strip().split(',')
		anneal.append((float(parts[0]), float(parts[1]), float(parts[2])))

	return clusters, centers, sol, anneal

def draw(clusters, centers, sol, anneal):
	plt.figure(figsize=(15, 15))
	for points in clusters:
		plt.plot([p[0] for p in points],
					[p[1] for p in points],
					'x--', ms=5.0)

	plt.plot([p[0] for p in centers],
				[p[1] for p in centers],
				'go', ms=10.0)

	plt.plot([p[0] for p in sol],
				[p[1] for p in sol],
				'ro', ms=10.0)
	
	plt.savefig('cpp.png')

	plt.cla()
	plt.figure(figsize=(20, 8))
	plt.plot([i for i in range(len(anneal))],
			[p[1] for p in anneal],"gx-")
	
	plt.plot([i for i in range(len(anneal))],
			[p[2] for p in anneal],"ro-")

	plt.savefig('cpp_anneal.png')

def main():
	clusters, centers, sol, anneal = read_file('hello')
	draw(clusters, centers, sol, anneal)

main()
