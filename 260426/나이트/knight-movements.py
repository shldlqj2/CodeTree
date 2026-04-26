n = int(input())
r1, c1, r2, c2 = map(int, input().split())

# Please write your code here.
from collections import deque

queue=deque()

dr=[-2,-2,-1,-1,1,1,2,2]
dc=[-1,1,-2,2,-2,2,-1,1]

queue.append((r1-1,c1-1))

visited=[[-1]*n for _ in range(n)]
visited[r1-1][c1-1]=0

while queue:
    cr,cc=queue.popleft()
    currdist=visited[cr][cc]

    for i in range(8):
        nr,nc=cr+dr[i],cc+dc[i]
        if 0<=nr<n and 0<=nc<n and visited[nr][nc]==-1:
            visited[nr][nc]=currdist+1
            queue.append((nr,nc))

print(visited[r2-1][c2-1])