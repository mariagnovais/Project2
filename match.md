# College Matching Algorithm
The goal of the project is to match high school students to the best choice of colleges for them.

# Data Structures and Algorithms used:
For later what we used.

# Sources
Datasets.
- College tuition:
  How to get to table: Choose "SELECT INSTITUTIONS", then "U.S only", and apply. Then, download.
  Website: https://nces.ed.gov/ipeds/SummaryTables/price-trend   
- https://collegescorecard.ed.gov/data/
  TUPLES: 146,000 
  NOTES: Should we clean it to just have 2023 and also delete the ones that don't have 2023 data (usually bought by another)?

# Dataset overview
Each row represents one college or city.

# Factors to match
User Input:
PREFERENCE  | TYPE | EXAMPLES | NOTES 
------------|------|----------|-------
Tuition     |float | 50,000   | per year    
SAT score   | int  | 1500     | test
ACT score   | int  | 30       | test
School Size | str  | big      | in respect to enrollment size
Location    | str  | Florida  | state or city?
Typeofschool| str  | private  | public or private?