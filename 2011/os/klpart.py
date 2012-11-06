import sys
import copy
import itertools
import random

from collections import OrderedDict

class Edge(object):
    def __init__(self, src, dst, weight):
        self.src, self.dst, self.weight = src, dst, float(weight)
        self.tuple = tuple(sorted((self.src, self.dst)))

def cost_difference(edges, left, right, current_vertex):
    current_subset = left if current_vertex in left else right
    internal, external = 0, 0
    for edge in edges:
        for vertex in edge.tuple:
            if vertex != current_vertex and current_vertex in edge.tuple:
                if vertex in current_subset:
                    internal += edge.weight
                else:
                    external += edge.weight
    return external - internal

def partition(edges, weights, vertices, initial):
    A, B = [set(subgraph) for subgraph in initial]

    marked = set()
    calcdiff = lambda L, R: { v: cost_difference(edges, L, R, v) for v in vertices }

    first_pass = True
    while first_pass or maxgain > 0:
        print(partition_cost(weights, (A, B)))

        D = calcdiff(A, B)
        gain = lambda a, b: D[a] + D[b] - 2 * weights.get(tuple(sorted((a, b))), 0)

        marked = set()
        gains = OrderedDict()
        SA, SB = copy.deepcopy((A, B))

        there_are_marked = False
        for _ in range(len(vertices) // 2):
            maxgain, maxa, maxb = 0, 0, 0
            found = False

            for a, b in itertools.product(A, B):
                if a not in marked and b not in marked and a != b:
                    curgain = gain(a, b)
                    if curgain > maxgain:
                        found = True
                        there_are_marked = True
                        maxa, maxb, maxgain = a, b, curgain

            if found:
                marked.add(maxa)
                marked.add(maxb)
                SA.remove(maxa)
                SB.remove(maxb)
                SA.add(maxb)
                SB.add(maxa)
                D = calcdiff(SA, SB)
                gains[(maxa, maxb)] = maxgain

        maxgain = 0
        fromAtoB, fromBtoA = set(), set()
        for (a, b), gain in gains.items():
            if maxgain + gain > maxgain:
                maxgain += gain
                fromAtoB.add(a)
                fromBtoA.add(b)

        if maxgain > 0:
            oldA, oldB = copy.deepcopy((A, B))
            A = (A - fromAtoB) | fromBtoA
            B = (B - fromBtoA) | fromAtoB
            #if partition_cost(weights, (oldA, oldB)) <= partition_cost(weights, (A, B)):
            #    A, B = oldA, oldB
            #    break

        first_pass = False

    return A, B

def splitline(line):
    return [int(x) for x in line.split()]

def partition_cost(weights, partition):
    A, B = partition
    return sum(weights.get(tuple(sorted((a, b))), 0) for a in A for b in B)

def main(args):
    with open('input.txt') as fin:
        vcount, ecount = splitline(fin.readline())
        edges = [Edge(*splitline(line)) for line in fin.readlines()]

    vertices = range(1, vcount + 1)

    initial = [[1, 2], [3, 4]]
    #initial = [list(), list()]
    #for vertex in vertices:
    #    initial[random.randint(0, 1)].append(vertex)

    weights = { e.tuple: e.weight for e in edges }

    print('Initial: %.2f' % partition_cost(weights, initial))
    result = partition(edges, weights, vertices, initial)
    print('Result: %.2f' % partition_cost(weights, result))

    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
