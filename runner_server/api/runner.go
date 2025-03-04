package api

import (
	"net/http"
	"runner_server/config"
)

type runnerApi struct {
	mux    *http.ServeMux
	config *config.Config
}

func RunnerServer(config *config.Config) http.Handler {
	api := runnerApi{
		mux:    http.NewServeMux(),
		config: config,
	}

	api.mux.Handle("GET /health", makeHandler(health))

	return api.mux
}

func health(w http.ResponseWriter, r *http.Request) error {
	return writeJson(w, 200, "alive")
}
