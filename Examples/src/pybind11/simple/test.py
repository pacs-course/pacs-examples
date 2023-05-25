import mymodule as my
print("give me two integer numbers:")
a, b=map(int, input().split())
res=my.sumInt(n=a,m=b)
print("the result is: ",res)
