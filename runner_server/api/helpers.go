package api

import (
	"encoding/json"
	"errors"
	"fmt"
	"log/slog"
	"net/http"
)

type apiHandlerFunc func(http.ResponseWriter, *http.Request) error

func makeHandler(h apiHandlerFunc) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		if err := h(w, r); err != nil {
			var apiError apiError
			if errors.As(err, &apiError) {
				writeJson(w, apiError.StatusCode, map[string]string{"err": apiError.Err.(string)})
			} else {
				writeJson(w, http.StatusInternalServerError, map[string]string{"err": "internal server error"})
			}
			slog.Error("API Error", "err", err.Error(), "path", r.URL.Path)
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
	return fmt.Sprintf("api error (%d): %s", e.StatusCode, e.Err)
}

func invalidData(err any) apiError {
	return apiError{
		StatusCode: http.StatusBadRequest,
		Err:        err,
	}
}
