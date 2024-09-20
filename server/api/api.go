package api

import (
	"encoding/json"
	"errors"
	"fmt"
	"log/slog"
	"net/http"
	"nfib/runner"
	"strconv"
)

func ApiHandler() http.Handler {
	handler := http.NewServeMux()

	handler.HandleFunc("GET /calculate/{impl}/{num}", makeHandler(calculate))

	return handler
}

func calculate(w http.ResponseWriter, r *http.Request) error {
	impl := r.PathValue("impl")
	num_s := r.PathValue("num")

	num, err := strconv.ParseUint(num_s, 10, 64)

	if err != nil {
		return invalidData("num should be a 64bit unsigned integer")
	}

	res, err := runner.Run(num, impl)

	if errors.Is(err, runner.ErrInvalidImpl) {
		return invalidData(fmt.Sprintf("unknown impl %s", impl))
	}

	return writeJson(w, http.StatusOK, map[string]any{"num": res.Num, "ms": res.Dur})
}

type apiHandlerFunc func(http.ResponseWriter, *http.Request) error

func makeHandler(h apiHandlerFunc) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		if err := h(w, r); err != nil {
			var apiError apiError
			if errors.As(err, &apiError) {
				writeJson(w, apiError.StatusCode, apiError)
			} else {
				writeJson(w, http.StatusInternalServerError, map[string]string{"err": "internal server error"})
			}
			slog.Error("API Error", "err: ", err.Error(), "path: ", r.URL.Path)
		}
	}
}

func writeJson(w http.ResponseWriter, status int, v any) error {
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(status)
	return json.NewEncoder(w).Encode(v)
}

type apiError struct {
	StatusCode int `json:"statusCode"`
	Err        any `json:"err"`
}

func (e apiError) Error() string {
	return fmt.Sprintf("api error: %d", e.StatusCode)
}

func invalidData(err any) apiError {
	return apiError{
		StatusCode: http.StatusBadRequest,
		Err:        err,
	}
}
