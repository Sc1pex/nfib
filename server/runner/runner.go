package runner

import (
	"errors"
	"log"
	"os/exec"
	"strconv"
	"strings"
)

type impl struct {
	cmd  string
	args []string
}

var impls = map[string]impl{
	"CNaive":  {cmd: "../impls/bin/nfib_c", args: []string{"naive"}},
	"CMatrix": {cmd: "../impls/bin/nfib_c", args: []string{"matrix"}},
}

var ErrInvalidImpl = errors.New("invalid impl")

type RunResult struct {
	Num string
	Dur float64
}

func Run(n uint64, impl string) (RunResult, error) {
	i, exists := impls[impl]
	if !exists {
		return RunResult{}, ErrInvalidImpl
	}
	i.args = append(i.args, strconv.FormatUint(n, 10))

	cmd := exec.Command(i.cmd, i.args...)
	out, err := cmd.CombinedOutput()

	if err != nil {
		return RunResult{}, err
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

	return r, nil
}
