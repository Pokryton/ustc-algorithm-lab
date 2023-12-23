package main

import (
	"fmt"
	"unicode/utf8"
)

func Lcs(a, b string) string {
	xs, ys := []rune(a), []rune(b)
	n, m := len(xs), len(ys)

	c := make([][]int, n+1)
	for i := range c {
		c[i] = make([]int, m+1)
	}

	for i, x := range xs {
		for j, y := range ys {
			if x == y {
				c[i+1][j+1] = c[i][j] + 1
			} else if c[i][j+1] > c[i+1][j] {
				c[i+1][j+1] = c[i][j+1]
			} else {
				c[i+1][j+1] = c[i+1][j]
			}
		}
	}
	l := c[n][m]

	lcs := make([]rune, l)
	for i, j, k := n-1, m-1, l-1; i >= 0 && j >= 0; {
		if xs[i] == ys[j] {
			lcs[k] = xs[i]
			k--
			i--
			j--
		} else if c[i+1][j] > c[i][j+1] {
			j--
		} else {
			i--
		}
	}

	return string(lcs)
}

func lcsRuneCount(a, b string) int {
	n := utf8.RuneCountInString(a)
	m := utf8.RuneCountInString(b)
	if n < m {
		a, b = b, a
		n, m = m, n
	}

	prev := make([]int, m+1)
	curr := make([]int, m+1)

	for _, x := range a {
		prev, curr = curr, prev
		j := 0
		for _, y := range b {
			if x == y {
				curr[j+1] = prev[j] + 1
			} else if prev[j+1] > curr[j] {
				curr[j+1] = prev[j+1]
			} else {
				curr[j+1] = curr[j]
			}
			j++
		}
	}

	return curr[m]
}

func LcsRuneCount(a, b string) int {
	n := utf8.RuneCountInString(a)
	m := utf8.RuneCountInString(b)
	if n < m {
		a, b = b, a
		n, m = m, n
	}

	c := make([]int, m+1)

	for _, x := range a {
		prev, curr := 0, 0
		j := 0
		for _, y := range b {
			prev, curr = curr, c[j+1]
			if x == y {
				c[j+1] = prev + 1
			} else if c[j] > c[j+1] {
				c[j+1] = c[j]
			}
			j++
		}
	}

	return c[m]
}

func main() {
	var a, b string
	fmt.Scan(&a, &b)

	lcs := Lcs(a, b)
	fmt.Printf("lcs = %#v, len = %d\n", lcs, utf8.RuneCountInString(lcs))
}
