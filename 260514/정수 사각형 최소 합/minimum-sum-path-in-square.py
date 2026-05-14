n = int(input())
grid = [list(map(int, input().split())) for _ in range(n)]

# Please write your code here.
from collections import deque
dr,dc=[1,0],[0,-1]

queue=deque()
queue.append((0,n-1))
dp=[[float('inf')]*n for _ in range(n)]
dp[0][n-1]=grid[0][n-1]

while queue:
    cr,cc=queue.popleft()
    for i in range(2):
        nr,nc =dr[i]+cr, dc[i]+cc
        if 0<=nr<n and 0<=nc<n and dp[nr][nc]>dp[cr][cc]+grid[nr][nc]:
            dp[nr][nc]= dp[cr][cc]+grid[nr][nc]
            queue.append((nr,nc))
ans=dp[n-1][0]
print(ans)