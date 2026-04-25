n, k = map(int, input().split())
grid = [list(map(int, input().split())) for _ in range(n)]
points = [tuple(map(int, input().split())) for _ in range(k)]

# Please write your code here.
from collections import deque

dr=[1,-1,0,0]
dc=[0,0,1,-1]

totmove=0

visited=[[False]*n for _ in range(n)]

for sr,sc in points:
    queue=deque()
    queue.append((sr-1,sc-1))
    if not visited[sr-1][sc-1]:
        visited[sr-1][sc-1]=True
        totmove+=1

    while queue:
        cr,cc=queue.popleft()
        for i in range(4):
            nr,nc = cr+dr[i], cc+dc[i]
            if 0<=nr<n and 0<=nc<n and grid[nr][nc]==0 and not visited[nr][nc]:
                totmove+=1
                queue.append((nr,nc))
                visited[nr][nc]=True

print(totmove)        
