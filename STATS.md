# Cfop solver

These are the results of a session of 100 solves using 20-moves scrambles:

| Name                | Value         |
|---------------------|---------------|
| Average length      | $67.04$ moves |
| Standard deviation  | $6.27$ moves  |
| Shortest solution   | $47$ moves    |
| Longest solution    | $83$ moves    |

# Kociemba solver

These are the results of a session of 100 solves using 30-moves scrambles.
Each scramble has been solved both in quick mode and in slow mode (with
`threads=25` and `timeout=10`).

### Quick mode

| Name                | Value         |
|---------------------|---------------|
| Average length      | $22.33$ moves |
| Standard deviation  | $1.55$ moves  |
| Shortest solution   | $18$ moves    |
| Longest solution    | $26$ moves    |

### Slow mode

| Name                | Value         |
|---------------------|---------------|
| Average length      | $20.98$ moves |
| Standard deviation  | $1.18$ moves  |
| Shortest solution   | $18$ moves    |
| Longest solution    | $23$ moves    |

### Comparison

| Name                      | Value              |
|---------------------------|--------------------|
| Average difference        | $1.35$ moves       |
| Standard deviation        | $1.32$ moves       |
| Biggest difference        | $5$ moves          |
| Same length in both modes | $35$% of solutions |
