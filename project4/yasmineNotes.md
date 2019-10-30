# Project #4 ( Localization) - notes

## Part 1: 
- goal is to get from point a to point b, relying on the return of perfect review of position 
- Should use proportion control, figure out speed and turn rate
- inverse relationship between speed and area
- use square-root of area to force a linear relationship 


## Part 2: 
*real-local: you do not have the benefit of perfect review of position*
- need to rely on laser to localize
- your task is to finish particle filtering
- particle filtering unleashes 100 or 1000 particles and allowing them to evolve. Particle filters return a bunch of possible positions.
- must modify readposition function, because it currently gives bogus position 
- analyze read position function which has array of hypotheses. Element 0 in array is least likely position and last element has the highest probability. The function currently returns the lowest probability element in the array. 
- Localization is almost finished – when the number of hypotheses shrinks, the position get crystallized. You should wait until the number of elements in the array is 2. Then return the hypothesis with the highest probability
- once the particle filter has finished its work, you cheat by comparing to the original position ( -6, -6)
- to keep successful run direct output into a file – when you run real-local > log.txt (this will allow you to override the previous log) , you can then submit the log file. Make sure you use cout to print message succeeding. Log.txt is proof that you have successfully completed project.
Log.txt should have “success localization” and “success navigation”
