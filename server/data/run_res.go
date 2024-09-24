package data

type RunResult struct {
	Input  uint64
	Output string

	AvgTime float64
	MinTime float64
	MaxTime float64

	NumRuns int
}
