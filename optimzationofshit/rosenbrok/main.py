a1 = 320.45
a2 = -237.6
a3 = 48.2
b1 = -9.125
b2 = b1

def alph(z1, z2, y1, y2):
    res = -(
        2 * (a1 * z1 * y1 + a3 * z2 * y2) +
            a2 * y2 * z1 +
            a2 * y1 * z2 +
            b1 * z1 +
            b2 * z2
    ) / (
            2 * (a1 * z1*z1 + a2 * z2 * z1 + z2*z2 * a3)
    )
    return res

x10=0.01423779
x20=0.12974998

z10=1.01423779
z20=0.12974998

z11=-z20
z21=z10

for i in range(0,3):
    t1 = x10
    t2 = x20
    print("z vec1","{0:0.5f}".format(z10),",","{0:0.5f}".format(z20))
    print("z vec2","{0:0.5f}".format(z11),",","{0:0.5f}".format(z21))
    print("y vec","{0:0.5f}".format(x10),",","{0:0.5f}".format(x20)) 
    a = alph(z10, z20, x10, x20)
    x11=x10+a*z10
    x21=x20+a*z20

    z11=-z20
    z21=z10
    print("{0:0.5f}".format(x11),",","{0:0.5f}".format(x21)) 

    # print("{0:0.5f}".format(x11),",","{0:0.5f}".format(x21))
    a = alph(z11, z21, x11, x21)
    x10=x11+a*z11
    x20=x21+a*z21
    print("{0:0.5f}".format(x10),",","{0:0.5f}".format(x20))

    z10=x10-t1
    z20=x20-t2
