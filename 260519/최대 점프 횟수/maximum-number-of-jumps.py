n = int(input())
arr = list(map(int, input().split()))

# Please write your code here.
dp=[float('-inf')]*n
dp[0]=0
for i in range(1,n):
    for j in range(i):
        if dp[j]==float('-inf'):
            continue
        if arr[j]+j>=i:
            dp[i]=max(dp[i],dp[j]+1)
    
print(max(dp))