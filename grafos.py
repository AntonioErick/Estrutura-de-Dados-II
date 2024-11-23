import csv
from collections import deque


def read_nodes(filename):
    nodes = {}
    with open(filename, 'r', encoding='utf-8') as file:
        reader = csv.reader(file)
        next(reader)  # Skip header
        for row in reader:
            if len(row) >= 3:
                id = int(row[0])
                name = row[1].strip('"')
                try:
                    # Assume o último elemento como new_id
                    new_id = int(row[-1])
                    nodes[new_id] = {'id': id, 'name': name}
                except ValueError:
                    print(f"Aviso: Não foi possível converter '{
                          row[-1]}' para inteiro. Linha ignorada.")
    return nodes


def read_edges(filename):
    adj = {}
    with open(filename, 'r') as file:
        for line in file:
            try:
                u, v = map(int, line.strip().split(','))
                adj.setdefault(u, set()).add(v)
                adj.setdefault(v, set()).add(u)
            except ValueError:
                print(f"Aviso: Linha inválida ignorada: {line.strip()}")
    return adj


def bfs(adj, start, end):
    queue = deque([(start, 0)])
    visited = set([start])

    while queue:
        node, distance = queue.popleft()
        if node == end:
            return distance

        for neighbor in adj.get(node, []):
            if neighbor not in visited:
                visited.add(neighbor)
                queue.append((neighbor, distance + 1))

    return -1  # No path found


def find_node_by_name(nodes, name):
    for node_id, node_info in nodes.items():
        if node_info['name'].lower() == name.lower():
            return node_id
    return None


def main():
    nodes = read_nodes('fb_famosos.nodes')
    adj = read_edges('fb_famosos.edges')

    zlatan_id = find_node_by_name(nodes, "Zlatan Ibrahimovic")
    federer_id = find_node_by_name(nodes, "Roger Federer")

    if zlatan_id is None or federer_id is None:
        print("Um ou ambos os jogadores não foram encontrados.")
        return

    steps = bfs(adj, zlatan_id, federer_id)

    if steps != -1:
        print(f"Zlatan Ibrahimovic e Roger Federer estão separados por {
              steps} graus.")
    else:
        print("Não há caminho entre Zlatan Ibrahimovic e Roger Federer.")

    start = 423
    end = 424

    if start not in nodes or end not in nodes:
        print(f"Erro: Um ou ambos os nós ({start}, {
              end}) não foram encontrados.")
        return

    steps = bfs(adj, start, end)

    if steps != -1:
        print(f"Nome do famoso (nó origem - {start}): {nodes[start]['name']}")
        print(f"Nome do famoso (nó de destino - {end}): {nodes[end]['name']}")
        print(f"Quantidade de passos (arestas) que os separam: {steps}")
    else:
        print("Não há caminho entre os nós especificados.")


if __name__ == "__main__":
    main()
