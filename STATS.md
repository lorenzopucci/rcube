# Cfop solver

These are the results of a session of 100 solves using 20-moves scrambles:

| Name                | Value         |
|---------------------|---------------|
| Average length      | $67.04$ moves |
| Standard deviation  | $6.27$        |
| Shortest solution   | $47$ moves    |
| Longest solution    | $83$ moves    |

# Kociemba solver

These are the results of a session of 100 solves using 30-moves scrambles.
Each scramble has been solved both in quick mode and in slow mode (with
`threads=5` and `timeout=15`).

### Quick mode

| Name                | Value         |
|---------------------|---------------|
| Average length      | $23.12$ moves |
| Standard deviation  | $1.11$        |
| Shortest solution   | $21$ moves    |
| Longest solution    | $25$ moves    |

### Slow mode

| Name                | Value         |
|---------------------|---------------|
| Average length      | $22.17$ moves |
| Standard deviation  | $1.07$        |
| Shortest solution   | $19$ moves    |
| Longest solution    | $24$ moves    |

### Comparison

| Name                      | Value              |
|---------------------------|--------------------|
| Average difference        | $0.95$ moves       |
| Standard deviation        | $1.14$             |
| Biggest difference        | $5$ moves          |
| Same length in both modes | $40$% of solutions |
