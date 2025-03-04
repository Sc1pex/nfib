package config

import (
	"errors"
	"net/url"
	"os"
	"strconv"
)

type Config struct {
	ServerUrl string
	RunnerUrl string

	ListenPort uint32

	Name  string
	Impls []string
}

func ParseConfig() (*Config, error) {
	var config Config

	parse_functions := map[string]func(*Config, []string) ([]string, error){}
	parse_functions["--server-url"] = parse_server_url
	parse_functions["-s"] = parse_server_url
	parse_functions["--runner-url"] = parse_runner_url
	parse_functions["-r"] = parse_runner_url
	parse_functions["--listen-port"] = parse_listen_port
	parse_functions["-p"] = parse_listen_port
	parse_functions["--impl"] = parse_impl
	parse_functions["-i"] = parse_impl
	parse_functions["--name"] = parse_name
	parse_functions["-n"] = parse_name

	args := os.Args[1:]
	for len(args) > 0 {
		arg := args[0]

		if parse_function, ok := parse_functions[arg]; ok {
			var err error
			args, err = parse_function(&config, args[1:])
			if err != nil {
				return nil, err
			}
		}
	}

	if config.ServerUrl == "" {
		return nil, errors.New("Server URL is required")
	}
	if config.RunnerUrl == "" {
		return nil, errors.New("Runner URL is required")
	}
	if config.ListenPort == 0 {
		return nil, errors.New("Listen port is required")
	}
	if config.Name == "" {
		return nil, errors.New("Name is required")
	}
	if len(config.Impls) == 0 {
		return nil, errors.New("At least one implementation is required")
	}

	return &config, nil
}

func parse_server_url(config *Config, args []string) ([]string, error) {
	if _, err := url.ParseRequestURI(args[0]); err != nil {
		return args, errors.New("Invalid server URL")
	}
	config.ServerUrl = args[0]
	return args[1:], nil
}

func parse_runner_url(config *Config, args []string) ([]string, error) {
	if _, err := url.ParseRequestURI(args[0]); err != nil {
		return args, errors.New("Invalid runner URL")
	}
	config.RunnerUrl = args[0]
	return args[1:], nil
}

func parse_listen_port(config *Config, args []string) ([]string, error) {
	num, err := strconv.ParseInt(args[0], 10, 32)
	if err != nil {
		return args, errors.New("Invalid listen port")
	}
	config.ListenPort = uint32(num)
	return args[1:], nil
}

func parse_impl(config *Config, args []string) ([]string, error) {
	config.Impls = append(config.Impls, args[0])
	return args[1:], nil
}

func parse_name(config *Config, args []string) ([]string, error) {
	config.Name = args[0]
	return args[1:], nil
}
