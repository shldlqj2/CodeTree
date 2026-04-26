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

    melt=set()

    for row in range(n):
        for col in range(m):
            if a[row][col]==0:
                queue.append((row,col))

    while queue:
        cr,cc=queue.popleft()
        cnt=0
        ice=[]

        for i in range(4):
            nr,nc=cr+dr[i],cc+dc[i]
            if 0<=nr<n and 0<=nc<m and a[nr][nc]==1:
                cnt+=1
                ice.append((nr,nc))

        if cnt==4:
            continue
        else:
            for i in ice:
                melt.add(i)
    
    if len(melt)==0:
        break
    
    for row,col in melt:
        a[row][col]=0

    time += 1
    lastmelt=len(melt)
    

print(time, lastmelt)
    

