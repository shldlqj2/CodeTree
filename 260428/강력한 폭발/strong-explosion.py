n = int(input())
grid = [list(map(int, input().split())) for _ in range(n)]

# Please write your code here.
bombs=[]

for row in range(n):
    for col in range(n):
        if grid[row][col]==1:
            bombs.append((row,col))

howmanybombs=len(bombs)
type1=[(-2,0),(-1,0),(1,0),(2,0)]
type2=[(-1,0),(0,-1),(1,0),(0,1)]
type3=[(-1,-1),(-1,1),(1,1),(1,-1)]

result=0


def findmax(cnt,tempgrid,bombcnt):
    global result
    if cnt == howmanybombs:
        result=max(result,bombcnt)
        return

    
    

    tempcnt=bombcnt
    
    r,c=bombs[cnt]
    
    bombgrid=[row[:] for row in tempgrid]

    for dr,dc in type1:
        nr=r+dr
        nc=c+dc
        if 0<=nr<n and 0<=nc<n and bombgrid[nr][nc]==0:
            bombgrid[nr][nc]=1
            bombcnt+=1
    findmax(cnt+1,bombgrid,bombcnt)

    bombcnt=tempcnt
    bombgrid=[row[:] for row in tempgrid]

    for dr,dc in type2:
        nr=r+dr
        nc=c+dc
        if 0<=nr<n and 0<=nc<n and bombgrid[nr][nc]==0:
            bombgrid[nr][nc]=1
            bombcnt+=1
    
    findmax(cnt+1,bombgrid,bombcnt)
    
    bombcnt=tempcnt
    bombgrid=[row[:] for row in tempgrid]

    for dr,dc in type3:
        nr=r+dr
        nc=c+dc
        if 0<=nr<n and 0<=nc<n and bombgrid[nr][nc]==0:
            bombgrid[nr][nc]=1
            bombcnt+=1
    
    findmax(cnt+1,bombgrid,bombcnt)

    
findmax(0,grid,howmanybombs)

print(result)