n, m = map(int, input().split())
a = [list(map(int, input().split())) for _ in range(n)]

# Please write your code here.
from collections import deque

dr=[1,-1,0,0]
dc=[0,0,1,-1]

time=0
lastmelt=0

while True:
    

    queue=deque()

    queue.append((0,0))

    melt=set()

    visited=[[False]*m for _ in range(n)]
    visited[0][0]=True

    while queue:
        cr,cc=queue.popleft()
        ice=[]

        for i in range(4):
            nr,nc=cr+dr[i],cc+dc[i]
            if 0<=nr<n and 0<=nc<m and not visited[nr][nc]:
                if a[nr][nc]==0:
                    queue.append((nr,nc))
                else:
                    melt.add((nr,nc))
                visited[nr][nc]=True

        
    
    if len(melt)==0:
        break
    
    for row,col in melt:
        a[row][col]=0

    time += 1
    lastmelt=len(melt)
    

print(time, lastmelt)
    

