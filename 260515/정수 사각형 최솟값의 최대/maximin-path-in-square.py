n = int(input())
grid = [list(map(int, input().split())) for _ in range(n)]

# Please write your code here.
from collections import deque
dr,dc=[1,0],[0,1]

queue=deque()
queue.append((0,0,grid[0][0])) #row, col, min
dp=[[float('-inf')]*n for _ in range(n)]
dp[0][0]=grid[0][0]

while queue:
    cr,cc,minNum=queue.popleft()
    for i in range(2):
        nr,nc =dr[i]+cr, dc[i]+cc
        if 0<=nr<n and 0<=nc<n:
            currminNum=min(minNum,grid[nr][nc])
            if dp[nr][nc]<currminNum:
                dp[nr][nc]=currminNum
                queue.append((nr,nc,currminNum))
ans=dp[n-1][n-1]
print(ans)