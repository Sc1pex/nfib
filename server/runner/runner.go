package runner

import (
	"log"
	"nfib/data"
	"os/exec"
	"strconv"
	"strings"
)

func Run(n uint64, impl string, num_runs int) data.RunResult {
	var r data.RunResult
	r.Input = n
	r.NumRuns = num_runs

	for i := range num_runs {
		output, time := runSingle(n, impl)

		if i == 0 {
			r.Output = output
			r.MinTime = time
			r.MaxTime = time
			r.AvgTime = time
		} else {
			if r.Output != output {
				// TODO: Return error
			}

			r.MinTime = min(r.MinTime, time)
			r.MaxTime = max(r.MaxTime, time)
			r.AvgTime += time
		}
	}

	r.AvgTime /= float64(r.NumRuns)

	return r
}

func runSingle(n uint64, impl string) (string, float64) {
	i, exists := data.Impls[impl]
	if !exists {
		log.Fatal("Called run with invalid impl: ", impl)
	}
	i.Args = append(i.Args, strconv.FormatUint(n, 10))

	cmd := exec.Command(i.Cmd, i.Args...)
	out, err := cmd.CombinedOutput()

	if err != nil {
		log.Fatal("Error running impl: ", impl, " with num", n, ": ", err)
	}

	split := strings.Split(string(out), " ")
	if len(split) != 2 {
		log.Fatal("Invalid output from ", impl, ": ", len(split), "\n", string(out))
	}

	output := strings.TrimSpace(split[0])
	time, err := strconv.ParseFloat(strings.TrimSpace(split[1]), 64)
	if err != nil {
		log.Fatal("Invalid output from", impl, ". Duration is not a float64: ", split[1])
	}
	return output, time
}
