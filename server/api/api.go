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
			apiResponse{Result: db_entry.Output, AvgTime: db_entry.AvgTime, Index: num},
		)
	}

	res := runner.Run(num, impl)
	api.db.Add(res, impl)
	return writeJson(w,
		http.StatusOK,
		apiResponse{Result: res.Output, AvgTime: res.AvgTime, Index: num},
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
			Index:   e.Input,
			Result:  e.Output,
			AvgTime: e.AvgTime,
		}
	}

	return writeJson(w, http.StatusOK, resp)
}
