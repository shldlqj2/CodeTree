n, m = map(int, input().split())
a = [list(map(int, input().split())) for _ in range(n)]

# Please write your code here.
from collections import deque

queue=deque()

dr=[1,-1,0,0]
dc=[0,0,1,-1]

queue.append((0,0))
visited=[[-1]*m for _ in range(n)]
visited[0][0]=0

while queue:
    cr,cc= queue.popleft()
    currdist=visited[cr][cc]
    for i in range(4):
        nr,nc=cr+dr[i],cc+dc[i]
        if 0<=nr<n and 0<=nc<m and a[nr][nc]==1 and visited[nr][nc]==-1:
            visited[nr][nc]=currdist+1
            queue.append((nr,nc))

print(visited[n-1][m-1])