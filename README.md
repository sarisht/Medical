Given a Graph file (alarm.bif) the startup code runs to create a network with all nodes, their parents and children.  We read the data file (records.dat) into a vector of vectors. All input is read each vector at a time. First the missing data is imputed for that vector (a line) and then all the corresponding value of counter for each relevant counters is updated. Imputation is done by passing the counts of the required vector, the start and number of values to be tried (only those considered where given values are satisfied). This is continued till time runs out in which in our case is set to be 500 seconds (safety margin from 10 minutes) (Convergence happens in less than a minute). Then the counts are converted into probability by dividing by the total according to the given values to get the required probability.
Features:
1.	Smoothing with 0.1 frequency added to each counter (experimentally best score).
2.	Probabilistic pick of value to impute.(No maximum probability bias)
3.	Store the index of parents so as to reduce time taken (to reduce the use of find() function)
4.	Changed all lists with vectors to get access time of O(1) instead of O(n) as achieved in list with iterator.
Problems:
1.	Some samples still not encountered, i.e. C(_|A,B) = 0 (or very low), that is A and B never occur together in our sample space this leads to all values being equally likely 
Alternate Solution could have been:
Instead of assigning the ? value randomly, we can make 100 samples out of it with each being in ratio of their probability. This could answer the above problem. (Not tried this yet)
