package main

import (
	"fmt"
	"log"
	"nfib/runner"
)

func main() {
	res, err := runner.Run(200000, "CNaive")
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(res.Num, res.Dur)
}
