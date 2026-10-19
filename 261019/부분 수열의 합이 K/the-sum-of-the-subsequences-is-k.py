import sys

input = sys.stdin.readline

n, k = map(int, input().split())
arr = [0] + list(map(int, input().split()))

# Please write your code here.

prefixSum = [0] * (n+1)
for i in range(1,len(arr)):
    prefixSum[i] = prefixSum[i-1] + arr[i]



cnt = 0
for i in range(1,n+1):
    for j in range(0,i):

        if prefixSum[i]-prefixSum[j] == k:
            cnt+=1
print(cnt)