/*
	Las Vegas algorithms

	A Las Vegas algorithm is a randomized algorithm that always gives
	the correct result but gambles with resources. 

	Source: https://yourbasic.org/algorithms/las-vegas/
*/
package main

import (
	"image"
	"image/color"
	"image/png"
	"math/rand"
	"os"
)

// Point returns a random point in the unit circle.
func Point() (x, y float64) {
	for { // This loop runs 4/π times on average.
		x = 2*rand.Float64() - 1
		y = 2*rand.Float64() - 1
		if x*x+y*y < 1 {
			return
		}
	}
}

func main() {
	const n = 2000
	const r = 80

	im := image.NewRGBA(image.Rectangle{image.Point{-r, -r}, image.Point{r, r}})
	for i := 0; i < n; i++ {
		x, y := Point()
		im.Set(int(r*x), int(r*y), color.Black)
	}
	f, _ := os.Create("circle.png")
	png.Encode(f, im)
}
