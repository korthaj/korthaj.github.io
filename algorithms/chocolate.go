package main

import (
	"fmt"
)

type result struct {
	joy      int
	pickLeft bool
}

// Joy returns the total pleasure if you eat the chocolates
// in an optimal order starting at the given day.
func Joy(choco []int, day int) int {
	n := len(choco)
	if n == 1 {
		return day * choco[0]
	}
	left := day*choco[0] + Joy(choco[1:], day+1)
	right := day*choco[n-1] + Joy(choco[:n-1], day+1)
	return max(left, right)
}

// JoyMemo returns the joy if you eat the chocolates choco[i:j]
// in an optimal order starting at day 1 + len(choco) - (j - i).
func JoyMemo(choco []int, i, j int, memo [][]result) int {
	// Reuse previous result, if available.
	if res := memo[i][j].joy; res != 0 {
		return res
	}

	day := 1 + len(choco) - (j - i)
	if j-i == 1 {
		return day * choco[i]
	}
	left := day*choco[i] + JoyMemo(choco, i+1, j, memo)
	right := day*choco[j-1] + JoyMemo(choco, i, j-1, memo)
	res := max(left, right)

	// Save the result.
	memo[i][j].joy = res
	memo[i][j].pickLeft = left > right
	return res
}

func max(x, y int) int {
	if x > y {
		return x
	}
	return y
}

func main() {
	choco := []int{4, 3, 2, 1, 5}
	fmt.Println(Joy(choco, 1))
	n := len(choco)
	memo := make([][]result, n)
	for i := range memo {
		memo[i] = make([]result, n+1)
	}
	JoyMemo(choco, 0, n, memo)
	for i, j := 0, n; i < j; {
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
