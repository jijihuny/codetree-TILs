from heapq import heappush as push, heappop as pop, heapify
import sys
input = sys.stdin.readline
INF = float('inf')
graph = []
cost = []
revenue_queue = []
product = {}
# O(VlogV + ElogV)
def shortest_path(start):
    global graph, cost
    cost = [INF] * len(graph)
    cost[start] = 0
    heap = []
    for adjacent, distance in graph[start].items():
        cost[adjacent] = min(cost[adjacent], distance)
        push(heap, (distance, adjacent))
    visited = [False] * len(graph)
    visited[start] = True
    while heap:
        shortest, node = pop(heap)
        if visited[node]:
            continue
        visited[node] = True
        for adjacent, distance in graph[node].items():
            cost[adjacent] = min(cost[adjacent], shortest + distance)
            push(heap, (cost[adjacent], adjacent))

def construct(n, m, edges):
    global graph
    graph = [{} for i in range(n+1)]
    for v, u, w in edges:
        graph[v][u] = min(w, graph[v][u] if u in graph[v] else INF)
        graph[u][v] = min(w, graph[u][v] if v in graph[u] else INF)

    shortest_path(0)

# O(logC)
def create_revenue(id, revenue, dest):
    global cost, product, delete_queue
    product[id] = (revenue, dest)
    if revenue < cost[dest]:
        push(revenue_queue, (cost[dest] - revenue, id))

# O(logP)
def delete_revenue(id):
    global product
    if id in product:
        del product[id]

# O(logC)
def selling():
    global revenue_queue, product
    id = -1
    while revenue_queue:
        income, cid = pop(revenue_queue)
        if cid not in product:
            continue
        id = cid
        break
    if id in product:
        del product[id]
    return id

# O(VlogV + ElogE + ClogC)
def change_start(start):
    global revenue_queue, cost, product
    revenue_queue = []
    shortest_path(start)

    for id, (revenue, dest) in product.items():
        if revenue < cost[dest]:
            revenue_queue += [(cost[dest]  - revenue, id)]
    
    heapify(revenue_queue)


Q = int(input().replace(' ', ''))
from collections import deque

for i in range(Q):
    t, *command = input().split(' ')

    if t == '100':
        n, m, *edges = command
        arr = []
        edges = deque(edges)
        while edges:
            u, v, w = edges.popleft(), edges.popleft(), edges.popleft()
            arr += [(int(u), int(v), int(w))]
        construct(int(n), int(m), arr)
        # print(graph)
        # print(cost)
    elif t == '200':
        id, revenue, dest = (int(j) for j in command)
        create_revenue(id, revenue, dest)
    elif t == '300':
        id = int(command[0])
        delete_revenue(id)
    elif t == '400':
        # print(cost)
        # print(revenue_queue)
        print(selling())
    elif t == '500':
        start = int(command[0])
        change_start(start)