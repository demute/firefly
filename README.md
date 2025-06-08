    [MacBook-Pro] firefly > make test
    gcc -g -ggdb -Wall -O3 -D_THREAD_SAFE -std=c2x -D_BSD_SOURCE -ferror-limit=5 -I/opt/homebrew/include -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -I. -fPIC -o firefly_test.o -c firefly_test.c
    gcc -g -ggdb -Wall -O3 -D_THREAD_SAFE -std=c2x -D_BSD_SOURCE -ferror-limit=5 -I/opt/homebrew/include -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -I. -fPIC -o firefly.o -c firefly.c
    gcc -g -ggdb -Wall -O3 -D_THREAD_SAFE -std=c2x -D_BSD_SOURCE -ferror-limit=5 -I/opt/homebrew/include -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -I. -o firefly_test firefly_test.o firefly.o -lpthread
    ./firefly_test
    iter:      0 alpha:1.00e-01 obj:3.96102e+00 pos: -2.4953,-11.5452
                                obj:6.54955e-05 pos: -6.1302, -6.1916 highscore!
    iter:  10000 alpha:1.44e-02 obj:1.61767e-01 pos: -2.0123, -6.1272
                                obj:2.17967e-07 pos: -6.1352, -6.1884 highscore!
    iter:  20000 alpha:2.33e-03 obj:3.26131e-03 pos: -6.0927, -6.1917
                                obj:4.13629e-09 pos: -6.1353, -6.1887 highscore!
    iter:  30000 alpha:3.29e-04 obj:3.63397e-06 pos: -6.1340, -6.1882
                                obj:7.56609e-11 pos: -6.1353, -6.1887 highscore!
    iter:  40000 alpha:4.63e-05 obj:7.88589e-06 pos: -6.1336, -6.1876
                                obj:2.54391e-12 pos: -6.1353, -6.1887 highscore!
    iter:  50000 alpha:6.27e-06 obj:2.68500e-09 pos: -6.1353, -6.1887
                                obj:8.95730e-14 pos: -6.1353, -6.1887 highscore!
    iter:  60000 alpha:9.02e-07 obj:7.12567e-11 pos: -6.1353, -6.1887
                                obj:3.03728e-16 pos: -6.1353, -6.1887 highscore!
    iter:  70000 alpha:1.30e-07 obj:1.23141e-11 pos: -6.1353, -6.1887
                                obj:5.01297e-18 pos: -6.1353, -6.1887 highscore!
    iter:  80000 alpha:1.79e-08 obj:5.13279e-15 pos: -6.1353, -6.1887
                                obj:1.03431e-19 pos: -6.1353, -6.1887 highscore!
    iter:  90000 alpha:2.47e-09 obj:8.29244e-16 pos: -6.1353, -6.1887
                                obj:9.25903e-21 pos: -6.1353, -6.1887 highscore!
    iter: 100000 alpha:3.48e-10 obj:7.27892e-19 pos: -6.1353, -6.1887
                                obj:1.36599e-22 pos: -6.1353, -6.1887 highscore!
    iter: 110000 alpha:5.01e-11 obj:5.97561e-20 pos: -6.1353, -6.1887
                                obj:1.15785e-24 pos: -6.1353, -6.1887 highscore!
    iter: 120000 alpha:6.92e-12 obj:1.20326e-20 pos: -6.1353, -6.1887
                                obj:2.14407e-26 pos: -6.1353, -6.1887 highscore!
    iter: 130000 alpha:1.01e-12 obj:1.42013e-22 pos: -6.1353, -6.1887
                                obj:3.07064e-28 pos: -6.1353, -6.1887 highscore!
    iter: 140000 alpha:1.55e-13 obj:1.98890e-24 pos: -6.1353, -6.1887
                                obj:2.21867e-30 pos: -6.1353, -6.1887 highscore!
    iter: 150000 alpha:2.10e-14 obj:8.31601e-26 pos: -6.1353, -6.1887
                                obj:1.97215e-31 pos: -6.1353, -6.1887 highscore!
    iter: 160000 alpha:2.84e-15 obj:5.74389e-29 pos: -6.1353, -6.1887
    iter: 170000 alpha:3.76e-16 obj:2.17923e-28 pos: -6.1353, -6.1887
    iter: 180000 alpha:4.99e-17 obj:1.97215e-31 pos: -6.1353, -6.1887
    iter: 190000 alpha:6.62e-18 obj:1.97215e-31 pos: -6.1353, -6.1887
    optimisation done: lowest value 0.000000 found at x=-6.135294 y=-6.188697%
    [MacBook-Pro] firefly >
