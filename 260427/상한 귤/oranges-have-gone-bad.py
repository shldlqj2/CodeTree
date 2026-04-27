n, k = map(int, input().split())
grid = [list(map(int, input().split())) for _ in range(n)]

# Please write your code here.
from collections import deque
#0은 아무것도 놓여있지 않음, 1은 해당칸에 귤, 2는 상한귤

dr=[1,-1,0,0]
dc=[0,0,1,-1]

queue=deque()
finalarr=[[0]*n for _ in range(n)]

for row in range(n):
    for col in range(n):
        if grid[row][col]==2:
            queue.append((row,col))#행,렬,초
            finalarr[row][col]=0
        elif grid[row][col]==0:
            finalarr[row][col]=-1
        elif grid[row][col]==1:
            finalarr[row][col]=-2

        



while queue:
    cr,cc=queue.popleft()
    time=finalarr[cr][cc]

    for i in range(4):
        nr,nc=cr+dr[i],cc+dc[i]
        if 0<=nr<n and 0<=nc<n and finalarr[nr][nc]==-2:
            finalarr[nr][nc]=time+1
            queue.append((nr,nc))

for row in finalarr:
    print(*row)
    



