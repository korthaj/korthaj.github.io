/*
	Monte carlo simulations

	Monte Carlo simulations are a broad class of algorithms that
	use repeated random sampling to obtain numerical results.

	Source: https://yourbasic.org/algorithms/las-vegas/
*/
package main

import (
	"fmt"
	"image"
	"image/color"
	"image/png"
	"math/rand"
	"os"
)

func main() {
	const n = 100000
	const r = 400
	gray := color.Gray16{0xcccc}
	im := image.NewRGBA(image.Rectangle{image.Point{-r, -r}, image.Point{r, r}})

	count := 0
	for i := 0; i < n; i++ {
		x := 2*rand.Float64() - 1
		y := 2*rand.Float64() - 1
		if x*x+y*y < 1 {
			count++
			im.Set(int(r*x), int(r*y), color.Black)
		} else {
			im.Set(int(r*x), int(r*y), gray)
		}
	}
	fmt.Println(4 * float64(count) / float64(n))

	f, _ := os.Create("circle.png")
	png.Encode(f, im)
}
