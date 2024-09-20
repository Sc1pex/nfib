package main

import (
	"log"
	"net/http"
	"nfib/api"
)

func main() {
	handler := http.NewServeMux()

	handler.Handle("/api/", http.StripPrefix("/api", api.ApiHandler()))
	handler.Handle("/", http.FileServer(http.Dir("../ui/dist")))

	log.Fatal(http.ListenAndServe(":8080", handler))
}
