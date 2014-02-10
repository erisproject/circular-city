# "Perfectly competitive" model

This example shows a simple model from an undergraduate IO assignment.  The
question:

Identical firms with cost functions C = 25 + q^2 may enter a perfectly
competitive market (that is, they take market price as given) with demand Qd(p)
= 50 - 2p.  Determine the equilibrium price, quantity per firm, market
quantity, and the number of firms.

## Analytical solution

Solution steps:

1. The firm's profit function is Pq - 25 - q^2.  The FOC wrt q yields an
   individual firm's supply function of q = P/2.
2. The no-entry condition says profit is 0.  (This example is cooked: more
   technically, we'd have to find an integer n such as n induces non-negative
   profit and n+1 induces negative profit).  Combining this with a firm's
   supply function yields P^2 / 4 - 25 = 0 and thus P = 10.
3. Aggregating supply functions across n firms we get market supply: Q = nP/2
4. Market supply = market demand gives P = 100 / (n+4), and so combining with
   the previous step gives n=6.
5. P=10 and n=6 yields firm and equilibrium quantities: q=5, Q=30

In summary: P=10, q=6, Q=30, n=6.
