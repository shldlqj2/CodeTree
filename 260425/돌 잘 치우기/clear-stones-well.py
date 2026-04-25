n, k, m = map(int, input().split())

grid = [list(map(int, input().split())) for _ in range(n)]

r = []
c = []
for _ in range(k):
    ri, ci = map(int, input().split())
    r.append(ri - 1)
    c.append(ci - 1)

# Please write your code here.
rcs=list(zip(r,c))

from collections import deque
from itertools import combinations


dr=[1,-1,0,0]
dc=[0,0,1,-1]


cnt=0

rocks=[]

for row in range(n):
    for col in range(n):
        if grid[row][col]==1:
            rocks.append((row,col))

removelist=list(combinations(rocks,m))


for list1 in removelist:
    tempgrid=[row[:] for row in grid]
    for row,col in list1:
        tempgrid[row][col]=0
    
    currcnt=0

    visited=[[False]*n for _ in range(n)]

    for row,col in rcs:
        queue=deque()
        queue.append((row,col))

        

        if not visited[row][col]:
            visited[row][col]=True
            currcnt+=1


        while queue:
            cr,cc=queue.popleft()
            for i in range(4):
                nr,nc=cr+dr[i],cc+dc[i]
                if 0<=nr<n and 0<=nc<n and tempgrid[nr][nc]==0 and not visited[nr][nc]:
                    visited[nr][nc]=True
                    queue.append((nr,nc))
                    currcnt+=1

    cnt=max(cnt,currcnt)

print(cnt)
                            


                    
            
