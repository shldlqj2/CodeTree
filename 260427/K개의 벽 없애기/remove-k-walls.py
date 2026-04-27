n, k = map(int, input().split())
grid = [list(map(int, input().split())) for _ in range(n)]
r1, c1 = map(int, input().split())
r2, c2 = map(int, input().split())

r1 -= 1
c1 -= 1
r2 -= 1
c2 -= 1

# Please write your code here.
from collections import deque
from itertools import combinations

wallRCS=[]
dr=[1,-1,0,0]
dc=[0,0,1,-1]
mintime=float('inf')
cnt=0

for row in range(n):
    for col in range(n):
        if grid[row][col]==1:
            wallRCS.append((row,col))

wallRCS=list(combinations(wallRCS,k))


for removeWall in wallRCS:
    tempgrid=[row[:] for row in grid]

    for row,col in removeWall:
        tempgrid[row][col]=0


    queue=deque()
    queue.append((r1,c1))
    visited=[[-1]*n for _ in range(n)]
    visited[r1][c1]=0

    while queue:
        cr,cc=queue.popleft()
        currdist=visited[cr][cc]

        for i in range(4):
            nr,nc = cr+dr[i], cc+dc[i]
            
            if 0<=nr<n and 0<=nc< n and tempgrid[nr][nc]==0 and visited[nr][nc] == -1:
                visited[nr][nc]=currdist+1
                queue.append((nr,nc))

    if visited[r2][c2]==-1:
        cnt+=1
    else:
        mintime=min(mintime,visited[r2][c2])

if cnt == len(wallRCS):
    print(-1)
else:
    print(mintime)