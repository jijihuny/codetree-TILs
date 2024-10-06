# 추가 mid pid color max_depth
# O(1) * 20k
# 수정 mid color
# O(V) * 50k
# 조회 mid
# O(1) * 20k
# 점수 가치 제곱의 합 가치 = 서브트리 서로 다른 색깔의 수
# O(V) * 100

root = []
node = {}
parent = {}
childs = {}

def create(m_id, p_id, color, max_depth):
    global root, node, parent, childs
    if p_id == -1:
        parent[m_id] = m_id
        root += [m_id]
    else:
        parent[m_id] = p_id
        childs[p_id] += [m_id]
    childs[m_id] = []
    node[m_id] = [color, max_depth]

def modify(m_id, color):
    global root, node, parent, childs
    def modify_subtree(depth, t_id, color):
        node[t_id][0] = color
        depth = min(node[t_id][1], depth)
        if depth == 1:
            return
        else:
            for child in childs[t_id]:
                modify_subtree(depth - 1, child, color)
    
    modify_subtree(node[m_id][1], m_id, color)

def get_color(m_id):
    global root, node, parent, childs
    return node[m_id][0]

def get_score():
    global root, node, parent, childs
    scores = []
    def get_tree_score(depth, m_id):
        nonlocal scores
        depth = min(node[m_id][1], depth)
        if not childs[m_id] or depth == 1:
            scores += [1]
            return {node[m_id][0]}
        
        colors = {node[m_id][0]}
        for child in childs[m_id]:
            colors |= get_tree_score(depth - 1, child)
        scores += [len(colors)]
        return colors

    for r_id in root:
        get_tree_score(node[r_id][1], r_id)

    return sum(map(lambda x: x * x, scores))
        

Q = int(input())

for i in range(Q):
    t, *command = (i for i in input().split())
    # print(node, parent, childs, root)
    if t == '100':
        m_id, p_id, color, max_depth = (int(c) for c in command)
        create(m_id, p_id, color, max_depth)
    elif t == '200':
        m_id, color = (int(c) for c in command)
        modify(m_id, color)
    elif t == '300':
        m_id = int(command[0])
        print(get_color(m_id))
    elif t == '400':
        print(get_score())