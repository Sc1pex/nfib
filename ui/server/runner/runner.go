package runner

import (
	"errors"
	"fmt"
	"os/exec"
	"strconv"
)

type Impl struct {
	cmd  string
	args []string
}

var impls = map[string]Impl{
	"CNaive":  {cmd: "../../impls/bin/nfib_c", args: []string{"naive"}},
	"CMatrix": {cmd: "../../impls/bin/nfib_c", args: []string{"matrix"}},
}

var ErrInvalidImpl = errors.New("invalid impl")

func Run(n uint64, impl string) error {
	i, exists := impls[impl]
	if !exists {
		return ErrInvalidImpl
	}
	i.args = append(i.args, strconv.FormatUint(n, 10))

	cmd := exec.Command(i.cmd, i.args...)
	out, err := cmd.CombinedOutput()

	if err != nil {
		return err
	}

	fmt.Println(string(out))

	return nil
}
