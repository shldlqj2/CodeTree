A, B, N = map(int, input().split())

bus_fare = []
stop_count = []
bus_stops = {}
buses={}

for i in range(N):
    fare, count = map(int, input().split())
    bus_fare.append(fare)
    stop_count.append(count)
    tempBusStop=list(map(int, input().split()))
    #bus_stops.append(list(map(int, input().split())))
    for stop in tempBusStop:
        if stop not in bus_stops:
            bus_stops[stop]=[]
        bus_stops[stop].append(i+1)
    buses[i+1] = tempBusStop

# Please write your code here.
# 버스를 노드로 잡아야 하나? 버스를 노드로 잡으면 이동 가능 정류장??? 버스를 노드로 잡는다면 양수 정방향 음수 역방향으로 하면 되고?
import heapq
#print(list(i for i in dir(heapq) if not i.startswith('_')))
faremap=[float('inf')]*(1001)
distmap=[float('inf')]*(1001)
pq=[]
heapq.heappush(pq,[0,0,A,0])#요금,거리,현재정류장,버스번호
faremap[A]=0
distmap[A]=0

while pq:
    currFare, currDist, currStop, befBus = heapq.heappop(pq)

    # print(currFare, currDist, currStop, befBus)
    if currFare > faremap[currStop]:
        continue
    if currStop not in bus_stops:
        break
    for ableBus in bus_stops[currStop]:
        nextFare=currFare
        if ableBus != befBus:
            nextFare += bus_fare[ableBus-1]

        skipflag=True
        distCnt=0
        for ableStop in buses[ableBus]:
            if skipflag:
                if ableStop != currStop:
                    continue
                skipflag=False
                
                continue
            distCnt+=1
            nextDist = currDist+distCnt

            if nextFare < faremap[ableStop]:
                distmap[ableStop] = nextDist
                faremap[ableStop] = nextFare
                heapq.heappush(pq,[nextFare,nextDist,ableStop,ableBus])
            elif nextFare == faremap[ableStop]:
                if distmap[ableStop] > nextDist:
                    distmap[ableStop]= nextDist
                    heapq.heappush(pq,[nextFare,nextDist,ableStop,ableBus])
                
if faremap[B] == float('inf'):
    print(-1,-1)
else:
    print(faremap[B],distmap[B])

