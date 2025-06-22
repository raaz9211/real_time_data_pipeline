This is the easiest way to get max memory used (RSS):


/usr/bin/time -v ./your_executable

        User time (seconds): 0.73
        System time (seconds): 0.01
        Percent of CPU this job got: 110%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:00.68
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 15784
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 1066
        Voluntary context switches: 712
        Involuntary context switches: 20
        Swaps: 0
        File system inputs: 0
        File system outputs: 72
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
----------------------------------------------------------------------------
valgrind --leak-check=full --show-leak-kinds=all ./RealTimeDataPipeline --config ../config/config.json

==33062== Memcheck, a memory error detector
==33062== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==33062== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==33062== Command: ./RealTimeDataPipeline --config=../config/config.json
==33062== 
Real-Time Data Pipeline started.
Processing took 160715ms
Throughput: 0.622219 records/second
Data processing complete. Output written to ../data/output.txt
==33062== 
==33062== HEAP SUMMARY:
==33062==     in use at exit: 0 bytes in 0 blocks
==33062==   total heap usage: 151,282 allocs, 151,282 frees, 7,583,096 bytes allocated
==33062== 
==33062== All heap blocks were freed -- no leaks are possible
==33062== 
==33062== For lists of detected and suppressed errors, rerun with: -s
==33062== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)


sudo apt install libsqlite3-dev
sudo apt install sqlite3

sudo apt install libssl-dev

websocat ws-l:0.0.0.0:3000 -

npm install ws

node echo-server.js