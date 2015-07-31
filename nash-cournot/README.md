# Cournot-Nash quantity setting firms

This example reproduces the typical Cournot structure with many firms, where
each firm make quantity adjustments based on how previous quantity adjustments
affected profits.

Demand is linear: P(Q) = a - bQ.

Firms are identical, with fixed costs of zero and marginal costs of c (another
option).

a, b, and c are current 50, 2, and 1, respectively.

## Analytical solution

See https://en.wikipedia.org/wiki/Cournot_competition

Individual firm quantities are:

q = (a - c) / (b (N+1))

Aggregate quantity is:

Q = nq = N (a-c) / (b (N+1))

and price is:

P = (a + Nc) / (N+1)

## ABM model

The ABM model selects one or more firms to manipulate quantity by a small fixed
amount.  At the end of the period, the firm considers whether the shift
increased or decreased profits: if it increased, it keeps the change for the
upcoming period, if it decreased, it reverts the change.

