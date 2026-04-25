n, k = map(int, input().split())
grid = [list(map(int, input().split())) for _ in range(n)]
r, c = map(int, input().split())

# Please write your code here.
from collections import deque



dr=[1,-1,0,0]
dc=[0,0,1,-1]

newr,newc=r-1,c-1

for i in range(k):
    queue = deque()
    queue.append((newr,newc)) #row,col
    visited=[[False]*n for _ in range(n)]
    visited[newr][newc]=True
    currnum=grid[newr][newc]
    moveableRCS=[]
    while queue:
        cr,cc=queue.popleft()
        

        for j in range(4):
            nr,nc=cr+dr[j],cc+dc[j]
            if 0<=nr<n and 0<=nc<n and grid[nr][nc] < currnum and not visited[nr][nc]:
                queue.append((nr,nc))
                moveableRCS.append((nr,nc,grid[nr][nc]))
                visited[nr][nc]=True

    if len(moveableRCS)==0:
        break
    moveableRCS.sort(key=lambda x:(-x[2],x[0],x[1]))

    newr,newc = moveableRCS[0][0],moveableRCS[0][1]

    
print(newr+1,newc+1)
