package data

import (
	"maps"
	"slices"
)

type Impl struct {
	Cmd  string
	Args []string
}

var Impls = map[string]Impl{
	"CNaive":  {Cmd: "../impls/bin/nfib_c", Args: []string{"naive"}},
	"CMatrix": {Cmd: "../impls/bin/nfib_c", Args: []string{"matrix"}},
}

func IsValidImpl(i string) bool {
	_, valid := Impls[i]
	return valid
}

func ImplNames() []string {
	return slices.Collect(maps.Keys(Impls))
}
