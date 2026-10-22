n, m = map(int, input().split())
edges = [tuple(map(int, input().split())) for _ in range(m)]
A, B = map(int, input().split())

# Please write your code here.
import heapq

graphE={}
for s,e,d in edges:
    if s not in graphE:
        graphE[s]=[]
    if e not in graphE:
        graphE[e]=[]
    graphE[s].append([d,e])
    graphE[e].append([d,s])


distmap=[float('inf')]*(n+1)

pq=[]
heapq.heappush(pq,[0,A])
distmap[A]=0

while pq:
    currdist, currnode = heapq.heappop(pq)

    if currdist > distmap[currnode]:
        continue
    
    if currnode == B:
        break

    for w,nextnode in graphE[currnode]:
        nextdist = w+currdist
        if nextdist < distmap[nextnode]:
            distmap[nextnode]=nextdist
            heapq.heappush(pq,[nextdist,nextnode])


print(distmap[B])

