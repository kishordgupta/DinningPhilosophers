# DinningPhilosophers
Dinning Philosophers without using shared memory, for concurrent server using only socket message passing

Logic:

Main program 
creating  5 process for Chopsticks . These process create Server using portid 5000 to 5004
After that 
Crating 5 process for Philosopher’s . 

Chopsticks server bind their respective port and wait for philosophers

Every philosopher send  “”ACQUIRE” message to server. If Chopstick server busy they return N else Y.
If Philosopher’s got  two Y from server he send DONE message to both server . which release the chopsticks and increase a counter by 1. 
If one of the server send N then chopsticks send ‘RELEASE’ message to another server. And wait a random time.


