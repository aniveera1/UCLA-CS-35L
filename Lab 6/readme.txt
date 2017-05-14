After Action Report
Name: Anirudh Veeraragavan

For this homework I had to implement multithreading functionality for the SRT
program in order to increase its efficiency. The pthread library and the
functions pthread_create and pthread_join made thread creation and completion a
rather straightforward process. The first step was to analyze main.c to see
where code could be parallelized, and it becomes obvious that the printing of
each pixel is ripe for multithreading. I abstracted this entire part out to a
separate function, which I then called within pthread_create. However, this lead
to multiple errors since the function relied on other variables within main, so
I had to make these variables global. This solved the problem and the function
each thread would execute was complete.

Next I had to decide how each thread would divide up work, and I used a simple
approach where each thread numbered from 0 to n-1, where n is the total number
of threads, would start at the pixel indicated by its number and compute each
pixel offset by the total number of threads. Thus if there were 3 threads,
thread 1 would compute 0,3,6,9; thread 2 would compute 1,4,7,10; thread 3 would
compute 2,5,8,11. This process works well, and evenly divides up the work.

After fixing up some minor synatical errors I ran make clean check, but to my
disappointment the files 2-test.ppm, 4-test.ppm, and 8-test.ppm were radically
different from baseline.ppm. I then realized that each thread was printing
independentally from the others and so the pixels were not printing in the
right order. Thus I went back to main.c and created a global 3D array to store
the values that had to be printed. Now each thread was not printing but rather
storing values in this array. Then back in main after the threads finished I
printed out the contents of this array. When I ran make clean check this time
everything worked as expected.

Make clean check outputs the time each file took to run, and we can analyze
these timestamps to see how performance improved. There is a clear trend of
more threads leading to better performance, and so my implementation of SRT
radically improved its performance. Thus we can conclude that on a multicore
machine running multithreaded applications is significantly faster than any
single threaded application.
