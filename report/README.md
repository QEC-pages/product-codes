## running statistics

- parameters
  - general: `size na, nb=[6,15],k=[1,3]`
  - more frequent: `size na, nb=[7,12],k=[1,2]`
- set-up
  - sysmetric code A and B, where B is obtained by flip X and Z in code A
  - identical code A and B
  - random code A and B
- what is calculated in each test?
  - generate code A B with a filter of distance > 1
  - subsystem product code
  - higher-dimensional hypergraph product code from the product of these two CSS codes.
  - both left (X) and right (Z) are calculated for all those codes.
- a million tests have been running for each set-up
- estimeted total running time: `256 cores x 7 x 24 hours = 43008 cpu hours`

## result
- dozens of cases has been found with the set-up of symmetric code A B or identical code A B
- no cases has been found with random code A and B
- observations
  - symmetry plays a role here, which tends to give smaller distance. This is consistent with the numericas of homological product code in (Bravyi & Hastings 2013)

## files
- each test has a unique `trial number`, the log files is in this folder, and the matrices are saved in `MatrixMarket` format in `/matrices`
- the shell script to generate those file is `../check_random_concatenation.sh`