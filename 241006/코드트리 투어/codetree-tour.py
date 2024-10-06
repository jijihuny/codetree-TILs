from heapq import heappush as push, heappop as pop, heapify
graph = {}
cost = {}
revenue_queue = []
product = {}
# O(VlogV + ElogV)
def shortest_path(start):
    global graph, cost
    cost = {i: float('inf') for i in graph.keys()}
    cost[start] = 0
    heap = []
    if start not in graph:
        graph[start] = []
    for adjacent, distance in graph[start]:
        cost[adjacent] = distance
        push(heap, (distance, adjacent))
    visited = set()
    while heap:
        shortest, node = pop(heap)
        if node in visited:
            continue
        visited |= {node}
        for adjacent, distance in graph[node]:
            cost[adjacent] = min(cost[adjacent], shortest + distance)
            push(heap, (cost[adjacent], adjacent))

def construct(edges):
    for v, u, w in edges:
        if v not in graph:
            graph[v] = []
        if u not in graph:
            graph[u] = []

        graph[v] += [(u, w)]
        graph[u] += [(v, w)]

    shortest_path(0)

def create_revenue(id, revenue, dest):
    global cost, product
    product[id] = (revenue, dest)
    push(revenue_queue, ((cost[dest] if dest in cost else float('inf')) - revenue, id))

def delete_revenue(id):
    if id in product:
        del product[id]

def selling():
    global revenue_queue, product
    id = -1
    while revenue_queue:
        income, cid = pop(revenue_queue)
        if cid not in product:
            continue
        if income > 0:
            continue
        id = cid
        break
    if id in product:
        del product[id]
    return id

def change_start(start):
    global revenue_queue, cost, product
    revenue_queue = []
    shortest_path(start)

    for id, (revenue, dest) in product.items():
        revenue_queue += [(cost[dest] - revenue, id)]
    
    heapify(revenue_queue)


Q = int(input().replace(' ', ''))

for i in range(Q):
    t, *command = input().split(' ')

    if t == '100':
        n, m, *edges = command
        arr = []
        while edges:
            u, v, w, *edges = edges
            arr += [(int(u), int(v), int(w))]
        construct(arr)
    elif t == '200':
        id, revenue, dest = (int(j) for j in command)
        create_revenue(id, revenue, dest)
    elif t == '300':
        id = int(command[0])
        delete_revenue(id)
    elif t == '400':
        print(selling())
    elif t == '500':
        start = int(command[0])
        change_start(start)