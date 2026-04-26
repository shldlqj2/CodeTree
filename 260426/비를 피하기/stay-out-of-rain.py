n, h, m = map(int, input().split())
grid = [list(map(int, input().split())) for _ in range(n)]

# Please write your code here.
from collections import deque
# 0은 이동 가능 칸 1은 벽 2는 사람 3은 비를 피할수 있는 곳
# 사람 상하좌우 움직이는데 1초
# 벽이 아닌곳은 이동 가능


dr=[1,-1,0,0]
dc=[0,0,1,-1]

resultgrid=[[0]*n for _ in range(n)]

for row in range(n):
    for col in range(n):
        if grid[row][col]==2:
            queue=deque()
            queue.append((row,col,0))
            visited=[[False]*n for _ in range(n)]
            
            rainskip=False

            while queue:
                cr,cc,currmove=queue.popleft()
                for i in range(4):
                    nr,nc = cr+dr[i],cc+dc[i]
                    if 0<=nr<n and 0<=nc<n and grid[nr][nc]!=1 and not visited[nr][nc]:
                        if grid[nr][nc]==3:
                            rainskip=True
                            resultgrid[row][col]=currmove+1
                            break
                        else:
                            queue.append((nr,nc,currmove+1))
                            visited[nr][nc]=True
                if rainskip:
                    break
            if not rainskip:
                resultgrid[row][col]=-1
for arr in resultgrid:
    print(*arr)

                

