package main

import (
	"log"
	"net/http"
	"nfib/api"
	"nfib/livereload"
)

func main() {
	handler := http.NewServeMux()

	handler.Handle("/api/", http.StripPrefix("/api", api.ApiHandler()))
	handler.Handle("/", http.FileServer(http.Dir("ui-dist")))
	handler.HandleFunc("GET /livereload", livereload.Handler)

	log.Fatal(http.ListenAndServe(":8080", handler))
}
