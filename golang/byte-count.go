/*
	Convert byte size to bytes, kilobytes, megabytes, gigabytes, ...

	These utility functions convert a byte size to a human-readable string
	in either SI (decimal) or IEC (binary) format.


	Source: https://yourbasic.org/golang/formatting-byte-size-to-human-readable-format/
*/
package main

import (
	"fmt"
	"math"
)

func ByteCountSI(b int64) string {
	const unit = 1000
	if b < unit {
		return fmt.Sprintf("%d B", b)
	}
	div, exp := int64(unit), 0
	for n := b / unit; n >= unit; n /= unit {
		div *= unit
		exp++
	}
	return fmt.Sprintf("%.1f %cB",
		float64(b)/float64(div), "kMGTPE"[exp])
}

func ByteCountIEC(b int64) string {
	const unit = 1024
	if b < unit {
		return fmt.Sprintf("%d B", b)
	}
	div, exp := int64(unit), 0
	for n := b / unit; n >= unit; n /= unit {
		div *= unit
		exp++
	}
	return fmt.Sprintf("%.1f %ciB",
		float64(b)/float64(div), "KMGTPE"[exp])
}

func main() {
	fmt.Printf("%20s %16s %16s\n", "Input", "BytecountSI", "BytecountIEC")
	for _, b := range []int64{
		999, 1000, 1023, 1024,
		987654321, math.MaxInt64,
	} {
		fmt.Printf("%20d %16q %16q\n",
			b,
			ByteCountSI(b),
			ByteCountIEC(b))
	}
}
