n, m = map(int, input().split())
A = list(map(int, input().split()))

# Please write your code here.
dp=[float('inf')]*(m+1)
dp[0]=0
for i in range(n):
    for j in range(m,0,-1):
        if j>=A[i]:
            if dp[j-A[i]]==float('inf'):
                continue
            dp[j]=min(dp[j],dp[j-A[i]]+1)
if dp[m]==float('inf'):
    print("No")
else:
    print("Yes")