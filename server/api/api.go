package api

import (
	"fmt"
	"net/http"
	"nfib/data"
	"nfib/db"
	"nfib/runner"
	"strconv"
)

type api struct {
	mux *http.ServeMux
	db  *db.Db
}

func ApiHandler() http.Handler {
	api := api{
		mux: http.NewServeMux(),
		db:  db.Open(),
	}
	api.db.Setup(data.ImplNames())

	api.mux.HandleFunc("GET /calculate/{impl}/{num}", makeHandler(api.calculate))
	api.mux.HandleFunc("GET /getall/{impl}", makeHandler(api.getAll))

	return api.mux
}

type apiResponse struct {
	Index   uint64  `json:"idx"`
	Result  string  `json:"num"`
	AvgTime float64 `json:"ms"`
}

func (api *api) calculate(w http.ResponseWriter, r *http.Request) error {
	impl := r.PathValue("impl")
	if !data.IsValidImpl(impl) {
		return invalidData(fmt.Sprintf("unknown impl %s", impl))
	}

	num_s := r.PathValue("num")
	num, err := strconv.ParseUint(num_s, 10, 64)
	if err != nil {
		return invalidData("num should be a 64bit unsigned integer")
	}

	if db_entry := api.db.GetIdx(num, impl); db_entry != nil {
		return writeJson(w,
			http.StatusOK,
			apiResponse{Result: db_entry.Result, AvgTime: db_entry.Avg_time, Index: num},
		)
	}

	res := runner.Run(num, impl)
	api.db.Add(db.Entry{
		Idx:      num,
		Avg_time: res.Dur,
		Min_time: res.Dur,
		Max_time: res.Dur,
		Result:   res.Num,
	}, impl)
	return writeJson(w,
		http.StatusOK,
		apiResponse{Result: res.Num, AvgTime: res.Dur, Index: num},
	)
}

func (api *api) getAll(w http.ResponseWriter, r *http.Request) error {
	impl := r.PathValue("impl")
	if !data.IsValidImpl(impl) {
		return invalidData(fmt.Sprintf("unknown impl %s", impl))
	}

	entries := api.db.GetAll(impl)
	resp := make([]apiResponse, len(entries))

	for i, e := range entries {
		resp[i] = apiResponse{
			Index:   e.Idx,
			Result:  e.Result,
			AvgTime: e.Avg_time,
		}
	}

	return writeJson(w, http.StatusOK, resp)
}
