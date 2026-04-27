K, N = map(int, input().split())

# Please write your code here.
from itertools import product

arr=[i for i in range(1,K+1)]



arr=list(product(arr,repeat=N))

for subarr in arr:
    print(*subarr)