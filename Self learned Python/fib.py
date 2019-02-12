def fib(n):
    seq = ['1' for x in range(n)]
    seq[0] = 1
    if n > 1:
        seq[1] = 1
    if n==1 or n==2:
        return 1
    for i  in range (2,n):
        seq[i] = seq[i-1] + seq[i-2]
    return seq[n-1]

while True:
    
    n  = int(input("Please enter the number: "))
    print(("The fib number of {} is {}").format(n,fib(n)))
    print()


