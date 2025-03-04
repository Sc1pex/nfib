package main

import (
	"fmt"
	"log"
	"net/http"
	"runner_server/api"
	"runner_server/config"
	"runner_server/server_api"
)

func main() {
	config, err := config.ParseConfig()
	if err != nil {
		log.Fatal(err)
	}

	err = server_api.RegisterRunner(config)
	if err != nil {
		log.Fatal(err)
	}
	log.Printf("Registered runner with server at %s", config.ServerUrl)

	server := api.RunnerServer(config)
	log.Printf("Listening on port %d", config.ListenPort)
	err = http.ListenAndServe(fmt.Sprintf(":%d", config.ListenPort), server)
	if err != nil {
		log.Fatal(err)
	}
}
