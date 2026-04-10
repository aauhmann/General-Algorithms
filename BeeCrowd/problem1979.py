# Luiz is a very dedicated coordinator of the computer science course. He knows the importance of encouraging their students to study algorithms, so decided to hold a individual programming contest between them, with awards.

# Luiz would like to avoid as much as possible the possibility of fraud in the competition, and so wouldn't want two candidates had friends in the same room during the proof.

# Unfortunately Luiz has only two classrooms for realize the proof, but fortunately he know very well their students, and can tell easily who is friends with whom. However, he needs to know if it is possible, with only two classrooms available, combining the students so that friends don't stay together, for this, he called to you, dedicated student of the course, to create a program to help him.

# Input
# The input consists of several test cases. The first line of each test case consists of an integer N (2 ≤ N ≤ 100) indicating the number of students who will perform the proof.
# Each N couple of these lines describes the friendly relations of each participant, so that the first line consists of the participant's ID, and the next line consists of a list describing an M number of students (1 ≤ M < N) with which one participant has a relationship of friendship.
# You can consider that it not relevant the number of people willing in each room, and that if there is a friendship between the students x and y there is a friendly relationship between y and x. The entry ends when N = 0, and should not be processed.

# Output
# For each test case you should print a line containing the "SIM" answer, if possible have the students so that there are no two friends doing the proof in the same classroom, and "NAO" otherwise.

from collections import deque

def bipartite(g):
    visited = {i : False for i in g.keys()}
    color = visited.copy()  # Mapeia cada nodo para uma cor (0 ou 1)
    for i in g.keys():
        queue = deque([i])
        while len(queue) > 0:
            n = queue.popleft()
            for adjacent in g[n]:
                if not visited[adjacent]:
                    queue.append(adjacent)
                    visited[adjacent] = True
                    color[adjacent] = 1 - color[n]  # Cada adjacent recebe cor oposta
                else:
                    if color[n] == color[adjacent]:  # Se um adjacent já foi visitado e tem mesma cor, forma um ciclo ímpar
                        return False

    return True


n = int(input())

while n != 0:
    graph = {i : [] for i in range(1, n+1)}
    for _ in range(n):
        id = int(input())
        adjacents = [int(x) for x in input().split()]
        for v in adjacents:
            if v not in graph[id]:
                graph[id].append(v)
            if id not in graph[v]:
                graph[v].append(id)

       
    if (bipartite(graph)):
        print("SIM")
    else:
        print("NAO")

    n = int(input())