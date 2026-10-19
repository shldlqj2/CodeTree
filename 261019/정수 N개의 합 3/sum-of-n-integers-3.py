import sys

input = sys.stdin.readline

n, k = map(int, input().split())
arr =[[0]*(n+1)] + [[0] + list(map(int, input().split())) for _ in range(n)]

# Please write your code here.
prefixSum=[[0] * (n+1) for _ in range(n+1)]

def getSum(x1,y1, x2,y2):
    return prefixSum[x2][y2] - prefixSum[x1-1][y2] - prefixSum[x2][y1-1] + prefixSum[x1-1][y1-1]

for i in range(1,n+1):
    for j in range(1,n+1):
        prefixSum[i][j]=prefixSum[i][j-1] + prefixSum[i-1][j] - prefixSum[i-1][j-1] + arr[i][j]

ans=-float('inf')
for i in range(1,n-k+2):
    for j in range(1,n-k+2):
        ans = max(ans,getSum(i,j,i+k-1,j+k-1))

print(ans)