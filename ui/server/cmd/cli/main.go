package main

import (
	"log"
	"nfib/runner"
)

func main() {
	for i := range 100 {
		err := runner.Run(uint64(i), "CNaive")
		if err != nil {
			log.Fatal(err)
		}
	}
}
