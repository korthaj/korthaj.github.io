/*
	Convert byte size to human readable format

	These utility functions convert 1000 to "1 kB", 1000000 to "1 MB", etc.

	Source: https://yourbasic.org/golang/formatting-byte-size-to-human-readable-format/
*/
package main

import (
	"fmt"
	"math"
)

func ByteCountDecimal(b int64) string {
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

func ByteCountBinary(b int64) string {
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
	fmt.Printf("%20s %16s %16s\n", "Input", "Decimal (SI)", "Binary (IEC)")
	for _, b := range []int64{
		0, 27, 999, 1000, 1023, 1024,
		1728, 1855425871872, math.MaxInt64,
	} {
		fmt.Printf("%20d %16q %16q\n",
			b,
			ByteCountDecimal(b),
			ByteCountBinary(b))
	}
}
