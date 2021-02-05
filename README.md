# fork-sieve
**C program that uses the fork() system call to run the Sieve of Eratosthenes algorithm.**\
CS 470 - Lab 2\
\
Input is given in a text file where each line contains a number from 2 to N.\
Each process is responsible for deleting the multiples in the text file of a different prime number.\
The input file is written to in each process.

# Example of output
Using the file `/src/input.txt`.\
\
**Console Output**\
Primes:\
2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 2, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, \
\
**File (input.txt) after Running**\
2\
3\
5\
7\
11\
13\
17\
19\
23\
29\
31\
37\
41\
43\
47\
53\
59\
61\
67\
71\
73\
79\
83\
89\
97\
101\
103\
107\
109\
113\
127\
131\
137\
139\
149\
151\
157\
163\
167\
173\
179\
181\
191\
193\
197\
199\
211\
223\
227\
229\
233\
239\
241\
251\
257\
263\
269\
271\
277\
281\
283
