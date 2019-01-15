package main

import (
	"fmt"
)

type result struct {
	joy      int
	pickLeft bool
}

// JoyTab returns a table containing the joys memo[i,j] that
// you get if you eat the chocolates choco[i:j] in an optimal
// order starting at day 1 + len(choco) - (j - i).
func JoyTab(choco []int) (memo [][]result) {
	n := len(choco)
	memo = make([][]result, n)
	for i := range memo {
		memo[i] = make([]result, n+1)
	}
	for i := range memo { // base cases
		memo[i][i+1].joy = n * choco[i]
	}
	for i := n - 1; i >= 0; i-- {
		for j := i + 2; j <= n; j++ {
			day := 1 + n - (j - i)
			left := day*choco[i] + memo[i+1][j].joy
			right := day*choco[j-1] + memo[i][j-1].joy
			memo[i][j].joy = max(left, right)
			memo[i][j].pickLeft = left > right
		}
	}
	return
}

func max(x, y int) int {
	if x > y {
		return x
	}
	return y
}

func main() {
	choco := []int{4, 3, 2, 1, 5}
	memo := JoyTab(choco)
	fmt.Println(memo[0][len(choco)].joy)
	for i, j := 0, len(choco); i < j; {
		if memo[i][j].pickLeft {
			fmt.Print("left ")
			i++
		} else {
			fmt.Print("right ")
			j--
		}
	}
	fmt.Println()
}
