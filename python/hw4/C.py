#pylint: disable = no-value-for-parameter
def accfunc(f):
    acc = f
    def ret(x=None):
        nonlocal acc
        if x is None:
            return acc
        acc = lambda y, prev=acc, func=x: func(prev(y))
        return ret
    return ret

def f1(x):
    return x + 1
def f2(x):
    return x * x
def f3(x):
    return x + x
def f4(x):
    return x*3
def f5(x):
    return x-4

while True:
    try:
        s = input()
        n = int(input())
        s = s.split()
        k = accfunc
        for x in s:
            k = k(eval(x))
        print(k()(n))
    except:  #读到 eof产生异常
        break