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


dr=[1,-1,0,0]
dc=[0,0,1,-1]

visited=[[False]* n for _ in range(n)]

cnt=0


#일단 기본적으로 탐사 가능 한 곳 전부 확인

for row,col in rcs:
    queue=deque()
    queue.append((row,col))
    if not visited[row][col]:
        
        visited[row][col]=True
        cnt+=1

    while queue:
        cr,cc=queue.popleft()
        for i in range(4):
            nr,nc=cr+dr[i],cc+dc[i]
            if 0<=nr<n and 0<=nc<n and grid[nr][nc]==0 and not visited[nr][nc]:
                visited[nr][nc]=True
                queue.append((nr,nc))
                cnt+=1
                

tempcnt=0
for row in range(n):
    for col in range(n):
        if grid[row][col]==0 and visited[row][col]:
            queue=deque()
            queue.append((row,col,0,0))#r,c,wallmove,currcnt
            
            newvisited=[row[:] for row in visited]


            while queue:
                cr,cc,wallmove,currcnt=queue.popleft()
                for i in range(4):
                    nr,nc=cr+dr[i],cc+dc[i]

                    if 0<=nr<n and 0<=nc<n and not newvisited[nr][nc]:
                        if grid[nr][nc]==0:
                            queue.append((nr,nc,wallmove,currcnt+1))
                            newvisited[nr][nc]=True
                            
                        if grid[nr][nc]==1:
                            if wallmove<m:
                                queue.append((nr,nc,wallmove+1,currcnt))
                                newvisited[nr][nc]=True
                tempcnt=max(tempcnt,currcnt)

            

print(cnt+tempcnt+m)
                            


                    
            
