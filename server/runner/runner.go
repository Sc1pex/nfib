package runner

import (
	"log"
	"nfib/data"
	"os/exec"
	"strconv"
	"strings"
)

type RunResult struct {
	Num string
	Dur float64
}

func Run(n uint64, impl string) RunResult {
	i, exists := data.Impls[impl]
	if !exists {
		log.Fatal("Called run with invalid impl: ", impl)
	}
	i.Args = append(i.Args, strconv.FormatUint(n, 10))

	cmd := exec.Command(i.Cmd, i.Args...)
	out, err := cmd.CombinedOutput()

	if err != nil {
		return RunResult{}
	}

	split := strings.Split(string(out), " ")
	if len(split) != 2 {
		log.Fatal("Invalid output from ", impl, ": ", len(split), "\n", string(out))
	}

	var r RunResult
	r.Num = strings.TrimSpace(split[0])
	r.Dur, err = strconv.ParseFloat(strings.TrimSpace(split[1]), 64)
	if err != nil {
		log.Fatal("Invalid output from", impl, ". Duration is not a float64: ", split[1])
	}

	return r
}
