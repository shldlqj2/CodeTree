n = int(input())
x1, x2 = [], []

for _ in range(n):
    a, b = map(int, input().split())
    x1.append(a)
    x2.append(b)

# Please write your code here.

lines=list(zip(x1,x2))
linescnt=len(lines)
subline=[]


result=0

def selectLines(cnt,sellines):
    global result
    result=max(result,sellines)
    if cnt==linescnt:
        return

    for p1,p2 in lines:
        skipflag=False
        for xp1,xp2 in subline:
            if xp1<=p1<=xp2 or xp1<=p2<=xp2:
                skipflag=True
                break
        if skipflag:
            continue

        subline.append([p1,p2])
        selectLines(cnt+1,sellines+1)
        subline.pop()

selectLines(0,0)
print(result)