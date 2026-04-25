n, m = map(int, input().split())
a = [list(map(int, input().split())) for _ in range(n)]

# Please write your code here.
from collections import deque
queue=deque()
queue.append((0,0))

dc=[1,-1,0,0]
dr=[0,0,1,-1]

visited=[[False]*m for _ in range(n)]
visited[0][0]=True

while queue:
    cr,cc=queue.popleft()


    if cr==n-1 and cc==m-1:
        print(1)
        exit()

    for i in range(4):
        nr=dr[i]+cr
        nc=dc[i]+cc

        if 0<=nr<n and 0<=nc<m and a[nr][nc]==1 and not visited[nr][nc]:
            queue.append((nr,nc))
            visited[nr][nc]=True


print(0)