package server_api

import (
	"bytes"
	"errors"
	"net/http"
	"runner_server/config"
)

func RegisterRunner(config *config.Config) error {
	url := config.ServerUrl + "/runnerapi/add"

	impls_json := ""
	for i, impl := range config.Impls {
		impls_json += `"` + impl + `"`
		if i < len(config.Impls)-1 {
			impls_json += ","
		}
	}
	body := []byte(
		`{
			"runner": { 
				"name": "` + config.Name + `",
				"address": "` + config.RunnerUrl + `"
			},
			"impls": [ ` + impls_json + `]
		}`,
	)

	res, err := http.Post(url, "application/json", bytes.NewBuffer(body))
	if err != nil {
		return err
	}

	if res.StatusCode != http.StatusOK {
		return errors.New("Failed to register runner")
	}

	return nil
}
