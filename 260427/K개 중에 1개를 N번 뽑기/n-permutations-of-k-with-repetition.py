K, N = map(int, input().split())

# Please write your code here.

subarr=[]

def print_permutation():
    for num in subarr:
        print(num, end=" ")
    print()

def find_per(cnt):
    if cnt==N:
        print_permutation()
        return
    for i in range(1,K+1):
        subarr.append(i)
        find_per(cnt+1)
        subarr.pop()
find_per(0)


"""
from itertools import product

arr=[i for i in range(1,K+1)]



arr=list(product(arr,repeat=N))

for subarr in arr:
    print(*subarr)
    """