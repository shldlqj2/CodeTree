n, m = map(int, input().split())
grid = [list(map(int, input().split())) for _ in range(n)]

# Please write your code here.
dp=[[float('-inf')]*m for _ in range(n)]
dp[0][0]=0

maxNum=0

for cr in range(1,n):
    for cc in range(1,m):

        for br in range(cr):
            for bc in range(cc):
                if grid[br][bc] < grid[cr][cc]:
                    dp[cr][cc]=max(dp[cr][cc],dp[br][bc]+1)

                
        maxNum=max(maxNum,dp[cr][cc])

print(maxNum+1) #시작지점 포함