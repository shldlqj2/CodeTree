n, k, u, d = map(int, input().split())
grid = [list(map(int, input().split())) for _ in range(n)]

# Please write your code here.
from collections import deque
from itertools import combinations

dr=[1,-1,0,0]
dc=[0,0,1,-1]

arr=[(i,j) for i in range(n) for j in range(n)]

choose=list(combinations(arr,k))

maxvisit=0

for karr in choose:
    queue=deque()

    visited=[[False]*n for _ in range(n)]

    canvisit=0

    for subarr in karr:
        row,col=subarr
        queue.append((row,col))
        visited[row][col]=True
        canvisit+=1
        
    
    while queue:
        cr,cc=queue.popleft()
        currheight=grid[cr][cc]
        for i in range(4):
            nr,nc= cr+dr[i],cc+dc[i]
            if 0<=nr<n and 0<=nc<n and not visited[nr][nc]:
                if u <= abs(currheight - grid[nr][nc]) <=d:
                    canvisit+=1
                    queue.append((nr,nc))
                    visited[nr][nc]=True
    

    maxvisit=max(maxvisit,canvisit)

print(maxvisit)
                    
