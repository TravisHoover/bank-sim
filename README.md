The professor is  pettey <br>
The code is   4330L3 <br>
Due March 1, 2017
 
For this lab you will write a pthreads program to simulate a very small bank.  This bank has three ATM machines located some distance from each other.  Using the ATM’s, you can make deposits to your account or make withdrawals from your account.  Of course your significant other may try to deposit/withdraw at the same time you do because they happen to be out running errands at the same time you are, but on the other side of town.  And your teenage child can also make withdrawals (it would never occur to them to make deposits) because you loaned them your ATM card to go shopping (we won’t discuss the intelligence of this decision!).  If you go below $0, an ATM will print a message saying you are overdrawn and will charge you an extra $10. 
Your job is to simulate this situation for four customer accounts for one day.  <br> <br>
At the beginning of the day the bank will start your program running.  <br>
*	Your program should read in the customer account number and balance for four accounts from the files cust0.dat, cust1.dat, cust2.dat, and cust3.dat.  <br>
*	Then reopen the files for writing and place the beginning balance in the files.  <br>
*	Then it should create three threads that represent the three ATMs.  <br>
*	The threads will then read in the transactions from three files atm0.dat, atm1.dat, and atm2.dat.  A transaction will consist of an account number, a ‘d’, or a ‘w’ (for deposit or withdrawal), an amount, and an integer number of seconds to sleep between transactions.  When a thread gets a transaction it will update the proper account accordingly, print the current account and it’s current balance and print an overdrawn message if necessary.  It will then put the transaction into the main program’s work queue. Then sleep the required amount of time before reading a new transaction (this is a system call to sleep).   <br>
*	Main’s work queue is an array of structs.  There are three rows (structs) in this array.  Each struct has an integer atm number, an integer account number, a char transaction type, a float transaction amount, and a float current balance.  Main should wait until there are three transactions available (that is, the third item of work placed in the queue will cause main to “wake up”, and note all three transactions could come from the same thread).  When main wakes up, it should print transactions (one per line, but leave off the customer number) to the appropriate customer files, thus emptying the work queue, and then wait for the queue to fill up again.  <br>
*	When a thread has finished reading a file, it should tell main it is finished by incrementing a global variable done.  <br>
*	When all threads are finished, main should print the account numbers and current balances of the four customers.  <br>
*	Note:  cust0.dat – cust3.dat will contain lists of each account’s transactions for the day when the program is done. <br>
 
For example, if cust0.dat started with:
00023453 1234.56
 
And ATM0 had the following file: <br>
00023453 w 500.00 1 <br>
00023453 w 25.00 1 <br>
00023453 d 100.00 2 <br>
10023678 d 10.00 1 <br>
10023678 w 100.00 1 <br>
02034588 w 500.00 1 <br>
02034588 d 100.00 2 <br>
00004444 w 60.00 1 <br>
00004444 d 10.00 2 <br>
 
And ATM1 had the following file: <br>
10023678 d 10.00 1 <br>
10023678 w 90.00 1 <br>
02034588 w 300.00 1 <br>
00004444 w 10.00 1 <br>
 
And ATM2 had the following file: <br>
00023453 w 25.00 1 <br>
10023678 w 100.00 1 <br>
02034588 w 500.00 1 <br>
00004444 w 60.00 1 <br>
 
Then at the end of the day, cust0.dat might have the following entries (in some order dependent on which atm went when). <br>
00023453   1234.56 <br>
0   w  500.00  734.56 <br>
0   w  25.00  709.56 <br>
2   w  25.00  684.56 <br>
0   d  100.00  784.56 <br>
 
Then after the cust0.dat file was written, main would print the: <br>
The ending balance for 00023453 is $784.56
 
Remember, there should be a cust file for each of the four customers, and a print
of the ending balance for each of the four customers.
 
Sample customer and atm files can be found in the other files in the Lab 3 support files subsection.
 