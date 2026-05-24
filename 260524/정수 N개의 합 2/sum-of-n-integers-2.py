n, k = map(int, input().split())
arr = list(map(int, input().split()))

# Please write your code here.
s=[0]*(n)
for i in range(1,n): #O(N)
    s[i]=s[i-1]+arr[i]
sk=[s[j]-s[j-k] for j in range(k,len(s)) ] #O(N-k)-> O(1)
print(max(sk))
