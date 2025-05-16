# Regex Cheat Sheet

| **Token**   | **Meaning**                             | **Example**              |
|-------------|-----------------------------------------|--------------------------|
| `a`         | Literal “a”                             | matches “a”             |
| `.`         | Any character except newline           | `t.t` → “tat”, “tot”     |
| `\d`        | Digit (0–9)                             | `\d+` → “2025”           |
| `\w`        | Word character (letter, digit, underscore) | `\w{3}` → “Ab9”      |
| `\s`        | Whitespace (space, tab, newline)        | `\s+` → “ ” or “\n”      |
| `[abc]`     | Any one of a, b, or c                   | `[aeiou]`               |
| `[^0-9]`    | Any character _except_ a digit          | matches “A”, “_”         |
| `^` / `$`   | Start / end of line                     | `^Hello` , `world$`      |
| `*`         | 0 or more of preceding token            | `a*` matches “”, “aaaa”  |
| `+`         | 1 or more of preceding token            | `b+` matches “b”, “bbb”  |
| `?`         | 0 or 1 of preceding token               | `colou?r` → “color”, “colour” |
| `{n}`       | Exactly _n_ of preceding token          | `\d{4}` → “2025”         |
| `{n,}`      | _n_ or more of preceding token          | `\w{2,}` → “Ab9_”        |
| `{n,m}`     | Between _n_ and _m_ of preceding token  | `\d{2,4}` → “12”, “2025” |
| `()`        | Grouping / capture                      | `(foo|bar)`             |
| `(?:…)`     | Non-capturing group                     | `(?:Mr|Ms)\.`           |
| `\1`, `\2`  | Backreference to 1st, 2nd capture group | `(a)(b)\1\2` → “abba”    |
| `|`         | Alternation (“or”)                      | `cat|dog`               |
| `\`         | Escape next metacharacter               | `\.` → “.”              |

> **Tip**:  
> - Test incrementally in a live regex tool (e.g. [regex101.com](https://regex101.com)).  
> - Watch out for **greedy** (`.*`) vs **lazy** (`.*?`) quantifiers.  
> - Start small, then build up: match one component at a time.

[HOME](README.md) | [Exercices](exercices.md) 
